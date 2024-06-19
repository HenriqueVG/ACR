/*
 *  vorbis_iface.c
 *
 *  This file manages the details of decoding an oggVorbis file and returning
 *  the PCM data to the "codec manager" layer above.  It can open and close
 *  Vorbis files, find out information about the song, find out the audio
 *  format, and read the PCM data. 
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


#include <glib.h>
#include <stdlib.h>

#include "config.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif

/* Vorbis includes */
#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>

/* our headers */
#include "vorbis_iface.h"

/* some Vorbis defines.  These shouldn't change for different architectures, 
 * b/c we're just requesting the audio data to be in this format 
 */
#define VORBIS_ENDIANNESS  0        /* little endian */
#define VORBIS_WORD_SIZE   2        /* 16 bit */
#define VORBIS_SIGNED      1        /* signed samples */

/* "private" function prototypes */
static vorbis_iface_state_t *_vorbis_iface_new_state ();
/* end "private" function prototypes */

/**
 * vorbis_open:
 *
 * @filename: the filename to open
 *
 * Opens @filename
 * 
 * returns: a newly initialzed vorbis_iface_state_t pointer (that 
 * struct contains fp, the file pointer)
 *
 */
codec_state_t *
vorbis_open (const gchar * filename)
{
  /* create a new state struct to pass around for this session */
  vorbis_iface_state_t *state = _vorbis_iface_new_state ();

  if (filename == NULL)
    {
      g_free (state);
      state = NULL;
      return NULL;
    }
    
  /* save the filename in our state structure */
  state->filename = g_strdup (filename);

  /* zero out the OggVorbis_File structure */
  memset (&state->vf, 0, sizeof (state->vf));

  /* try to fopen the file */
  state->stream = fopen (filename, "rb");
  if (state->stream == NULL)
    {
      ov_clear (&state->vf);   /* ov_clear closes the stream if it is open. */
      g_free (state);
      state = NULL;
      return NULL;
    }
  else
    {
      /* The open function performs full stream detection and machine
         initialization.  None of the rest of ov_xx() works without it */

      if (ov_open (state->stream, &state->vf, NULL, 0) < 0)
        {
          fclose (state->stream);
          g_free (state);
          state = NULL;
          return NULL;
        }
    }

  if (state != NULL)
    {
      state->vi = ov_info (&state->vf, -1);
      if (state->vi == NULL) { /* see if state->vi is valid */
          ov_clear (&state->vf); /* ov_clear closes the stream if it is open. */
          g_free (state);
          state = NULL;
          return NULL;
      }

      state->current_sample_offset = 0;
    }

  return state;
}

/**
 * vorbis_close:
 *   This function closes the currently open Vorbis stream associated with the
 *   state variable that is passed in
 *
 * @state: the vorbis state variable we pass around
 *
 */
void
vorbis_close (codec_state_t * codec_state)
{
  vorbis_iface_state_t *state = (vorbis_iface_state_t *) codec_state;
  if (state == NULL)
      return;

  ov_clear (&state->vf);        /* ov_clear closes the stream if it is open. */
  g_free (state->filename);
  g_free (state);
}

/**
 * vorbis_get_pcm_data:
 *
 * @sample_offset: the number of samples to skip, 
 *                  independant of word size or number of channels
 *
 * @num_samples: the number of samples we are to decode, 
 *                  independant of word size or number of channels
 *
 * @output_buf: the buffer to write data to.  We assume it is valid and large
 *                  enough to hold all the data
 *
 * @state: the vorbis state variable we pass around
 *
 * Decodes a requested number of samples and returns them in the output_buf variable.
 * The samples are obviously independant of the bitrate and other codec details.  We 
 * are returning a specific amount of raw PCM data, in a signed, little-endian, 
 * 16-bit format.
 *
 */
guint
vorbis_get_pcm_data (guint sample_offset, guint num_samples,
                     guchar * output_buf, void *user_data)
{
  guint32 bytes_decoded = 0;
  guint32 output_size;
  gint result = 1;
  void *bufptr = output_buf;
  vorbis_iface_state_t *state = (vorbis_iface_state_t *) user_data;
  if (state == NULL)
      return 0;

  /* seek to the offset in the PCM data, NOT the compressed data */

  if (sample_offset != state->current_sample_offset)
    {
      ov_pcm_seek (&state->vf, sample_offset);
      state->current_sample_offset = sample_offset;
    }

  output_size = state->vi->channels * VORBIS_WORD_SIZE * num_samples;

  /* loop until we have an error or we decode the requested amount of data */
  while (result > 0 && bytes_decoded < output_size)
    {

      /* VORBIS_ENDIANNESS, VORBIS_WORD_SIZE, and VORBIS_SIGNED are 
       * all #defined at the top of this file
       */
      result = ov_read (&state->vf, bufptr, output_size - bytes_decoded,
                        VORBIS_ENDIANNESS, VORBIS_WORD_SIZE, VORBIS_SIGNED,
                        &state->current_section);

      if (result > 0)
        {

          /* we didn't have an error code return, so we can assume
           * that we read the number of bytes stored in 'result'
           */

          bytes_decoded += result;
          (guchar *) bufptr += result;
        }
    }

  /* increment our offset counter to where we now are in the file */
  if (result > 0)
    {
      state->current_sample_offset += num_samples;
    }

  /* neither of these error codes mean anything to us, but are left in
   * the code for completeness.  They could be used in the future.
   */

  switch (result)
    {
    case 0:
      /* EOF */
      break;
    case -1:
      /* error in the stream.  Not a problem, just reporting it in
         case we (the app) cares.  In this case, we don't. */
      break;
    }

  return bytes_decoded;
}

/**
 * vorbis_get_format
 *
 * @state: the vorbis state variable we pass around
 * @audio_info: a structure that holds details about the audio format
 *
 * This function takes an existing #sing_audio_info_t structure and fills
 * in data about the audio format.  This function may only be called between
 * calls to vorbis_open and vorbis_close.
 *
 */
void
vorbis_get_format (codec_state_t * codec_state, sp_audio_info_t * audio_info)
{
  vorbis_iface_state_t *state = (vorbis_iface_state_t *) codec_state;
  if ((state == NULL) || (audio_info == NULL))
      return;

  audio_info->sample_rate = state->vi->rate;
  audio_info->num_channels = state->vi->channels;
  audio_info->avg_bitrate = (gint) ov_bitrate (&state->vf, -1);
  audio_info->bits_per_sample = 8 * VORBIS_WORD_SIZE;
  audio_info->codec_type = CODEC_OGGVORBIS;
}

/**
 * vorbis_get_song_info
 *
 * @state: the vorbis state variable we pass around
 * @song_info: the #sp_id_t structure to hold data about the song
 */
void
vorbis_get_song_info (codec_state_t * codec_state, sp_id_t * song_info)
{
  vorbis_comment *comment;
  gchar *tag;
  gint i;

  vorbis_iface_state_t *state = (vorbis_iface_state_t *) codec_state;
  if ((state == NULL) || (song_info == NULL))
      return;

  comment = ov_comment (&state->vf, -1);
  if (comment == NULL)
      return;

  /* save the filename in the sp_id_t struct */
  sp_id_set_filename (song_info, state->filename);

  /****************** get the title tag *********************/
  tag = vorbis_comment_query (comment, "title", 0);
  if (tag != NULL) 
  {
      sp_id_set_title (song_info, tag);
  }

  /****************** get all the artist tags *********************/
  i = 0;
  while ((tag = vorbis_comment_query (comment, "artist", i)) != NULL)
    {
      sp_id_append_artist (song_info, tag);
      i++;
    }

  /****************** get all the album tags *********************/
  i = 0;
  while ((tag = vorbis_comment_query (comment, "album", i)) != NULL)
    {
      sp_id_append_album (song_info, tag);
      i++;
    }

  /****************** get all the genre tags *********************/
  i = 0;
  while ((tag = vorbis_comment_query (comment, "genre", i)) != NULL)
    {
      sp_id_append_genre (song_info, tag);
      i++;
    }
}

/************** "private" function definitions *******************/

/**
 * _vorbis_iface_new_state: 
 *
 * Creates a new vorbis_iface_state_t and initializes it
 *
 * Returns: a pointer to the state structure
 */
static vorbis_iface_state_t *
_vorbis_iface_new_state ()
{
  vorbis_iface_state_t *state = g_malloc (sizeof (vorbis_iface_state_t));
  return state;
}
