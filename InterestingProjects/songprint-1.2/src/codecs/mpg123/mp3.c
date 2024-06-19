/*
 *  mp3.c - information and functions related to decoding MP3s
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
#include "mp3.h"

#include <string.h>
#include <stdlib.h>

const guint16 MP3_KILOBIT_RATES[3][2][16] = {
/* Layer 3: 2, 1 */
  {{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0},
   {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0}},
/* Layer 2: 2, 1 */
  {{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0},
   {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, 0}},
/* Layer 1: 2, 1 */
  {{0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 0},
   {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0}}
};

const guint16 MP3_SAMPLE_RATES[4][4] = {
  {11025, 12000, 8000, 0},	/* Version: 2.5          */
  {0, 0, 0, 0},			/*          Reserved     */
  {22050, 24000, 16000, 0},	/*          2.0          */
  {44100, 48000, 32000, 0},	/*          1.0          */
};

const gchar *mp3_genres[MP3_NUM_GENRES] = {
  "Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk",
  "Grunge", "Hip-Hop", "Jazz", "Metal", "New Age", "Oldies", "Other",
  "Pop", "R&B", "Rap", "Reggae", "Rock", "Techno", "Industrial",
  "Alternative", "Ska", "Death Metal", "Pranks", "Soundtrack",
  "Euro-Techno", "Ambient", "Trip-Hop", "Vocal", "Jazz+Funk", "Fusion",
  "Trance", "Classical", "Instrumental", "Acid", "House", "Game",
  "Sound Clip", "Gospel", "Noise", "Alt", "Bass", "Soul", "Punk",
  "Space", "Meditative", "Instrumental Pop", "Instrumental Rock",
  "Ethnic", "Gothic", "Darkwave", "Techno-Industrial", "Electronic",
  "Pop-Folk", "Eurodance", "Dream", "Southern Rock", "Comedy", "Cult",
  "Gangsta Rap", "Top 40", "Christian Rap", "Pop/Funk", "Jungle",
  "Native American", "Cabaret", "New Wave", "Psychedelic", "Rave",
  "Showtunes", "Trailer", "Lo-Fi", "Tribal", "Acid Punk", "Acid Jazz",
  "Polka", "Retro", "Musical", "Rock & Roll", "Hard Rock", "Folk",
  "Folk/Rock", "National Folk", "Swing", "Fast-Fusion", "Bebob",
  "Latin", "Revival", "Celtic", "Bluegrass", "Avantgarde",
  "Gothic Rock", "Progressive Rock", "Psychedelic Rock",
  "Symphonic Rock", "Slow Rock", "Big Band", "Chorus",
  "Easy Listening", "Acoustic", "Humour", "Speech", "Chanson",
  "Opera", "Chamber Music", "Sonata", "Symphony", "Booty Bass",
  "Primus", "Porn Groove", "Satire", "Slow Jam", "Club", "Tango",
  "Samba", "Folklore", "Ballad", "Power Ballad", "Rhythmic Soul",
  "Freestyle", "Duet", "Punk Rock", "Drum Solo", "A Cappella",
  "Euro-House", "Dance Hall", "Goa", "Drum & Bass", "Club-House",
  "Hardcore", "Terror", "Indie", "BritPop", "Negerpunk", "Polsk Punk",
  "Beat", "Christian Gangsta Rap", "Heavy Metal", "Black Metal",
  "Crossover", "Contemporary Christian", "Christian Rock", "Merengue",
  "Salsa", "Thrash Metal", "Anime", "JPop", "Synthpop"
};

/**
 * mp3_get_framesize:
 *
 * @params: the mp3's parameters to calculate the framesize from
 *
 * Calculates the framesize of an mp3
 *
 * Returns the framesize
 */
guint32 mp3_get_framesize (mp3_params_t * params)
{
  g_return_val_if_fail (params != NULL, 0);

  if ((params->version & 1) == 1)
    {
      /* version 1 */
      if (params->layer == MP3_MPEG_LAYER_1)
	return ((12 * params->bitrate) / params->freq + params->padding) * 4;

      return (144 * params->bitrate) / params->freq + params->padding;
    }
  else
    {
      /* version 2 or 2.5 */
      if (params->layer == MP3_MPEG_LAYER_1)
	return ((24 * params->bitrate) / params->freq + params->padding) * 4;

      return (72 * params->bitrate) / params->freq + params->padding;
    }
}

/** 
 * mp3_get_params:
 *
 * @header: the bytes to analyze
 *
 * Checks an mp3 header and determines the parameters therein
 *
 * Returns a pointer to a new mp3_params struct containing the
 * parameters encoded in this header, or NULL if it's not valid
 */

mp3_params_t *
mp3_get_params (const guint32 header)
{
  mp3_params_t *params;

  guint16 bitrate_index;
  gint16 freq_index;
  guint16 channel_code;

  gboolean failed = FALSE;

  params = g_malloc (sizeof (mp3_params_t));

  bitrate_index = (header & 0x0000F000) >> 12;
  freq_index = (header & 0x00000C00) >> 10;
  channel_code = (header & 0x000000C0) >> 6;

  params->framesync = (header & 0xFFE00000) >> 21;
  params->version = (header & 0x00180000) >> 19;
  params->layer = (header & 0x00060000) >> 17;
  params->crc = (header & 0x00010000) >> 16;
  params->padding = (header & 0x00000200) >> 9;
  params->priv = (header & 0x00000100) >> 8;
  params->modeext = (header & 0x00000030) >> 4;
  params->copyright = (header & 0x00000008) >> 3;
  params->original = (header & 0x00000004) >> 2;
  params->emphasis = (header & 0x00000003);

  if ((params->framesync != MP3_FRAME_SYNC)
      || (params->layer == MP3_MPEG_LAYER_RESERVED)
      || (params->version == MP3_VER_RESERVED)
      || (bitrate_index == 0x0f) || (freq_index < 0) || (freq_index > 4))
    {
      failed = TRUE;
    }

  if (!failed)
    {
      params->bitrate = MP3_KILOBIT_RATES[params->layer - 1]
	[params->version & 1][bitrate_index] * 1000;
      params->freq = MP3_SAMPLE_RATES[params->version][freq_index];
    }

  if ((params->bitrate == 0) || (params->freq == 0))
    failed = TRUE;

  if (!failed)
    params->framesize = mp3_get_framesize (params);

  if (params->framesize > MP3_MAX_FRAME_LENGTH)
    failed = TRUE;

  if (channel_code == 0x3)
    params->channels = 1;
  else
    params->channels = 2;

  if (failed)
    {
      g_free (params);
      return NULL;
    }
  else
    return params;
}

/**
 * mp3_sync_file: 
 *
 * @file: the file to be synced
 *
 * Uses mp3_sync() to find the start of the next frame and seeks @file
 * to that point
 *
 * Returns the new seek position of the file, or -1 if there are no more
 * frames, or -2 in the event of an error
 */
#define MAXSYNCBUF 8192

glong mp3_sync_file (FILE * inf)
{
/*        const guint16 maxsyncbuf = 8192;*//* Try to get two consecutive frames */

  guchar buffer[MAXSYNCBUF];
  guint8 buf_index = 0;
  glong result;
  glong num_read = 0;
  glong total_read = 0;

  while (!feof (inf))
    {
      num_read = fread (&buffer[buf_index], 1, MAXSYNCBUF - buf_index, inf);
      total_read += num_read;

      if (num_read + buf_index < 4)
	{
	  break;
	}

      result = mp3_sync (buffer, num_read + buf_index, 0);

      if (result == -1)
	{
	  /* At most the last 3 bytes may include a sync */
	  memmove (buffer, &buffer[(num_read + buf_index) - 3], 3);
	  buf_index = 3;
	}
      else
	{
	  /* Sync found */
	  fseek (inf, -((num_read + buf_index) - result), SEEK_CUR);
	  return (ftell (inf));
	}
      if (ferror (inf))
	return -2;
    }

  fseek (inf, -total_read, SEEK_CUR);
  return -1;
}

/**
 * mp3_sync:
 * 
 * @buf: the buffer to read through
 * @buf_size: its length
 * @index: where to start reading
 *
 * Proceeds through a buffer byte by byte, looking for
 * a valid frame. It checks a number of parameters and 
 * even makes sure the next frame looks legit
 *
 * Returns: Index of sync on success, -1 on error.
 */
glong mp3_sync (const guchar * buf, glong buf_size, glong index)
{
  mp3_params_t *params;

  guint32 header;

  guint32 next_frame_index;

  for (; index < buf_size - 3; index++)
    {
      /* Look for eleven 1 bits in a row - it might be the start */
      if ((buf[index] == 0xff) && ((buf[index + 1] & 0xe0) == 0xe0))
	{
	  /* do a bunch of sanity checks */
	  header = (buf[index] << 24) | (buf[index + 1] << 16) |
	    (buf[index + 2] << 8) | buf[index + 3];

	  params = mp3_get_params (header);

	  if (params == NULL)
	    /* invalid header */
	    continue;

	  next_frame_index = params->framesize + index;
	  g_free (params);

	  if (next_frame_index >= (buf_size - 3))
	    /* there aren't any more frames coming up to check - we're at
	       the end of the buffer */
	    return index;

	  /* get the next frame */
	  header = (buf[next_frame_index + 0] << 24) |
	    (buf[next_frame_index + 1] << 16) |
	    (buf[next_frame_index + 2] << 8) | (buf[next_frame_index + 3]);

	  if ((header & 0xffffff00) ==
	      (('T' << 24) | ('A' << 16) | ('G' << 8)))
	    /* the next frame appears to be a tag */
	    return index;

	  params = mp3_get_params (header);

	  if (params != NULL)
	    {
	      /* the next frame is valid, too */
	      g_free (params);
	      return index;
	    }

	  /* the next frame didn't pass any of our tests. 
	     we'll keep looking */
	}
    }
  return -1;
}

/**
 * mp3_read_tag:
 *
 * @fp: the file pointer to read the tag from
 * @info: where the info from the tag gets stored
 *
 * Reads the ID3v1 tag from @fp, decodes it, and stores
 * the information in @info
 */
void
mp3_read_tag (FILE * fp, sp_id_t * info)
{
  guchar tag[128];
  gchar tmp_str[31];
  gchar *tmp_str2;
  glong seek_to = ftell (fp);

  g_return_if_fail (info);

  sp_id_clear_title (info);
  sp_id_clear_artists (info);
  sp_id_clear_albums (info);
  sp_id_clear_genres (info);

  /* Not a g_return_if_fail() because it
     doesn't indicate an internal error */
  if (fseek (fp, -128, SEEK_END) < 0)
    return;

  g_return_if_fail (fread (tag, 128, 1, fp) == 1);

  if (!memcmp (tag, "TAG", 3))
    {

      strncpy (tmp_str, &(tag[3]), 30);
      tmp_str[30] = 0;
      g_strstrip (tmp_str);
      sp_id_set_title (info, tmp_str);

      strncpy (tmp_str, &(tag[33]), 30);
      tmp_str[30] = 0;
      g_strstrip (tmp_str);
      sp_id_append_artist (info, tmp_str);

      strncpy (tmp_str, &(tag[63]), 30);
      tmp_str[30] = 0;
      g_strstrip (tmp_str);
      sp_id_append_album (info, tmp_str);

      if (tag[127] < MP3_NUM_GENRES)
	tmp_str2 = g_strdup (mp3_genres[tag[127]]);
      else
	tmp_str2 = g_strdup ("Unknown");

      sp_id_append_genre (info, tmp_str2);
      g_free (tmp_str2);
    }

  g_return_if_fail (fseek (fp, seek_to, SEEK_SET) >= 0);
}
