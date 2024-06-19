/*
 *  songprintfile.c
 *
 *  A layer above Songprint, Songprintfile contains codecs, and
 *  thereby can identify a file given only its filename.
 *
 *  Copyright (C) 2000  eTantrum, Inc.
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <ctype.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "songprintfile.h"
#include "codecs.h"

static gchar *
_songprintfile_url_create (sp_id_t * info, sp_signature_t * sig);
static gchar *
_songprintfile_url_encode_char (const gchar chr);
static gchar *
_songprintfile_url_encode (const gchar * url);
static void
_songprintfile_url_encode_replace (gchar ** url);
static void
_songprintfile_url_array_encode_replace (const guint array_size, gchar ** urls);

/**
 *  sp_identify_file:
 *  @filename:  a full path and filename of the audio file
 *  @matches: a pointer to an array to store matches for that 
 *  song (this array must be freed with sp_id_delete_recv())
 *  @num_matches: a pointer to a gint where the number of 
 *  matches will be stored (you can pass NULL if you don't
 *  care)
 *  @url: a pointer to a place where a string will be 
 *  allocated which will contain a URL where song information
 *  can be submitted (you can pass NULL if you don't care)
 *  Otherwise, this string must be freed with g_free(). 
 *
 *  This function opens the file, generates the signature, 
 *  contacts the server, and returns the possible matches
 *  for that signature.
 * 
 *  Returns SP_FAILURE on error. If this happens, the values
 *  pointed to by @matches and @num_matches may be trashed.
 *  Nothing will need to be freed.
 */
sp_rv
sp_identify_file(const gchar * filename, sp_id_recv_t ***matches,
                 gint *num_matches, gchar ** url)
{
        sp_rv rv;
        sp_error_t error;
        
        codec_type_t file_type;        
        codec_vtable_t codec_funcs;
        
        codec_state_t *codec_state;
        sp_audio_info_t pcm_format;

        sp_id_send_t    *send_info;

        sp_signature_t sig;
        
        sp_id_recv_t    **recv_info;

        gint num_records;

        file_type = codecs_classify(filename, &codec_funcs);                

        if (file_type == CODEC_UNKNOWN) {
                (*matches) = NULL;
                return SP_FAILURE;
        }
        
        codec_state = codec_funcs.open(filename);
        
        codec_funcs.get_format(codec_state, &pcm_format);
        
        send_info = sp_id_new();
        codec_funcs.get_song_info(codec_state, send_info);
        
        rv = sp_generate_signature(&pcm_format, &sig,
                                   codec_funcs.get_pcm_data, 
                                   codec_state);
        
        codec_funcs.close(codec_state);
        
        if(rv == SP_FAILURE) {
                (*matches) = NULL;
                return SP_FAILURE;
        }                
        
        rv = sp_id_signature(&pcm_format, &sig, send_info, &recv_info,
                             &num_records, &error);
        
        if(rv == SP_FAILURE) {
                (*matches) = NULL;
                return SP_FAILURE;
        }
        
        if(num_matches != NULL)
          *num_matches = num_records;

        if(url != NULL)
          *url = _songprintfile_url_create(send_info, &sig);
        
        (*matches) = recv_info;
        return SP_SUCCESS;
}

/**
 * _songprintfile_url_create:
 * @info: song information to submit
 * @sig: signature for the song
 *
 * Creates a url for submitting data to eTantrum
 *
 * There may be more than one of artist, album, and genre. 
 * NOTE: Side effect - url encodes the title, artist, album, and 
 * genre fields in the sig structure!!
 *
 * Returns: a url suitable for unknown song submission
 */
static gchar *
_songprintfile_url_create (sp_id_t * info, sp_signature_t * sig)
{
  gint i;
  gchar *sig_string, *title_string, *artist_string, *album_string,
    *genre_string, *url_string;

  gchar *format_string = "http://sigadd.etantrum.com/"
                         "index.php?sig=%s&title=%s" 
                         "&artist=%s&album=%s&genre=%s";

  /* sig string is 32 hex shorts (max of 4 characters each),
     separated by plus signs */

  sig_string = g_malloc0 ((4 + 1) * SP_SIG_LENGTH);
  for (i = 0; i < SP_SIG_LENGTH; i++)
    {
      sprintf (&sig_string[strlen (sig_string)], "%.4x", sig->data[i]);

      if (i != SP_SIG_LENGTH - 1)
        sig_string[strlen (sig_string)] = ' ';
    }

  _songprintfile_url_array_encode_replace (sp_id_count_artists (info), info->artists);
  _songprintfile_url_array_encode_replace (sp_id_count_albums (info), info->albums);
  _songprintfile_url_array_encode_replace (sp_id_count_genres (info), info->genres);

  title_string = g_strdup (info->title);

  artist_string = g_strjoinv ("&artist=", info->artists);
  album_string = g_strjoinv ("&album=", info->albums);
  genre_string = g_strjoinv ("&genre=", info->genres);

  _songprintfile_url_encode_replace (&sig_string);
  _songprintfile_url_encode_replace (&title_string);

  url_string = g_strdup_printf (format_string, sig_string, title_string,
                                artist_string, album_string, genre_string);

  g_free (sig_string);
  g_free (title_string);
  g_free (artist_string);
  g_free (album_string);
  g_free (genre_string);

  return url_string;
}

/**
 *  _songprintfile_url_encode_char:
 *  @chr:   the char to encode
 *
 *  This will encode the char, if necessary, and return the result as a gchar *
 *
 *  Returns: A new string which contains the encoded char.  
 *  The string needs to be freed with g_free.  This function cannot fail.  
 */
static gchar *
_songprintfile_url_encode_char (const gchar chr)
{
  static gchar reserved_chars[] =
    { ';', '/', '?', ':', '@', '&', '=', '+', '$', ',',
    '<', '>', '%', '#', '\t', '\r', '\n', '\v', '\0'
  };
  /* assume worst-case */
  gchar *encoded_string;
  gint j = 0;

  /* special case for ' ' -> '+' */
  if (chr == ' ')
    {
      return g_strdup_printf ("+");
    }
  else
    {
      /* see if it matches a reserved char first */
      while (reserved_chars[j] != '\0')
        {
          if (reserved_chars[j] == chr)
            {
              encoded_string = g_strdup_printf ("%%%.2X", chr);
              return encoded_string;
            }
          j++;
        }
    }
  /* if it's printable and not a reserved char, it's fine */
  if (isprint (chr))
    return g_strdup_printf ("%c", chr);

  /* if it's not printable, encode it */
  encoded_string = g_strdup_printf ("%%%.2X", chr);
  return encoded_string;
}

/**
 *  _songprintfile_url_encode:
 *  @url:   a string to encode
 *
 *  This will return a new string that is a URL-safe representation of
 *  the string.  All non-printable chars are converted to %xx notation,
 *  ampersands and spaces are converted, etc.
 *
 *  Returns: The encoded string.  The string needs to be freed with g_free.
 *  This function cannot fail.
 */
static gchar *
_songprintfile_url_encode (const gchar * url)
{
  gchar *encoded_str = g_strdup ("");
  gchar *encoded_chr;
  gchar *old_str;
  gint i;

  if (url == NULL)
      return encoded_str; //return the null string

  for (i = 0; i < strlen (url); i++)
    {
      old_str = encoded_str;
      encoded_chr = _songprintfile_url_encode_char (url[i]);
      encoded_str = g_strdup_printf ("%s%s", encoded_str, encoded_chr);
      g_free (encoded_chr);
      g_free (old_str);
    }
  return (encoded_str);
}

/**
 *  _songprintfile_url_encode_replace:
 *  @url:   a string to encode
 *
 *  This will replace the string in *url with a URL encoded represtation
 *  of the string.  It calls g_free on the string in *url after encoding.
 */
static void
_songprintfile_url_encode_replace (gchar ** url)
{
  gchar *temp_url;
  if (url == NULL)
      return;
    
  temp_url = *url;
  if (*url == NULL)
    {
      *url = g_strdup ("");
    }
  else
    {
      *url = _songprintfile_url_encode (*url);
      g_free (temp_url);
   }
}

/**
 *  _songprintfile_url_array_encode_replace:
 *  @array_size:  the number of elements in the array
 *  @urls:   an array of strings to be encoded
 *
 *  This will replace each string in the array with a URL encoded represtation
 *  of the string.  
 */
static void
_songprintfile_url_array_encode_replace (const guint array_size, gchar ** urls)
{
  int i;
  for (i = 0; i < array_size; i++)
    _songprintfile_url_encode_replace (&urls[i]);
}
