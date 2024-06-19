/*
 *  mpg123_iface.c 
 *
 *  This file manages the details of decoding an MP3 file and returning
 *  the PCM data to the "codec manager" layer above.  It can open and close
 *  MP3 files, find out information about the song, find out the audio
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

#include <stdlib.h>
#include <string.h>

#include "mpg123.h"
#include "mpglib.h"
#include "mpg123_iface.h"
#include "mp3.h"

static mpg123_state_t *_mpg123_new_state ();

static gboolean
_mpg123_refill_pcm_buffer (mpg123_state_t * state, guint32 new_pcm_offset);

static void _mpg123_reset_avg (mpg123_state_t * state);

static guint32
_mpg123_num_bytes_to_copy (mpg123_state_t * state, guint32 num_bytes_to_read,
			   guint32 num_bytes_read, guint32 pcm_index);

static void
_mpg123_little_endian_memcpy (void *bto, const void *bfrom, gint32 n);

/**
 * mpg123_open:
 *
 * @filename: the filename to open
 *
 * Opens @filename and initializes a new mpg123_state_t 
 * 
 * Returns a pointer to the new state, or NULL if there 
 * is a problem
 */
codec_state_t *
mpg123_open (const gchar * filename)
{
  guchar mp3_data[MP3_HEADER_LENGTH];
  guint32 header;
  mpg123_state_t *state;

  g_return_val_if_fail (filename != NULL, NULL);

  state = _mpg123_new_state ();

  g_assert (state != NULL);

  state->fp = fopen (filename, "rb");

  if (state->fp == NULL)
    {
      g_free (state);
      return NULL;
    }

  state->filename = g_strdup (filename);

  if (mp3_sync_file (state->fp) < 0)
    {
      g_free (state->filename);
      g_free (state);
      return NULL;
    }

  if (fread (mp3_data, MP3_HEADER_LENGTH, 1, state->fp) != 1)
    {
      g_free (state->filename);
      g_free (state);
      return NULL;
    }

  header = MP3_MAKE_HEADER (mp3_data);
  state->params = mp3_get_params (header);

  if (state->params == NULL)
    {
      g_free (state->params);
      g_free (state->filename);
      g_free (state);
      return NULL;
    }

  state->bitrate_avg =
    (state->bitrate_avg * state->bitrate_count
     + state->params->bitrate) / ++state->bitrate_count;

  return (codec_state_t *) state;
}

/**
 * mpg123_get_format:
 *
 * @state: the state
 * @pcm_format: where the info goes
 *
 * Takes an existing #sp_audio_info_t structure and fills
 * in data about the audio format, such as sample rate or
 * number of channels.  This function may only be called 
 * between calls to mpg123_open and mpg123_close.
 */
void
mpg123_get_format (codec_state_t * state, sp_audio_info_t * pcm_format)
{
  const mpg123_state_t *_state = state;

  g_return_if_fail (state != NULL);
  g_return_if_fail (pcm_format != NULL);

  pcm_format->sample_rate = _state->params->freq;
  pcm_format->num_channels = _state->params->channels;
  pcm_format->avg_bitrate = _state->bitrate_avg;
  pcm_format->bits_per_sample = MP3_BITS_PER_SAMPLE;
  pcm_format->codec_type = CODEC_MP3_MPG123;
}

/**
 * mpg123_get_song_info:
 *
 * @state: the state
 * @song_info: where the info goes
 *
 * Stores ID3 information in @song_info. This function may 
 * only be called between calls to mpg123_open and mpg123_close.
 */
void
mpg123_get_song_info (codec_state_t * state, sp_id_t * song_info)
{
  const mpg123_state_t *_state = state;

  g_return_if_fail (song_info != NULL);

  mp3_read_tag (_state->fp, song_info);
}

/**
 * mpg123_get_pcm_data:
 *
 * @start_offset: the number of samples to skip, 
 *                  independent of word size or number of channels
 * @num_samples_to_read: the number of samples we are to decode, 
 *                  independent of word size or number of channels
 * @output_buf: the buffer to write data to
 * @state: the state
 *
 * Decodes a requested number of samples and puts them in the output_buf variable.
 * The samples are obviously independant of the bitrate and other codec details.  We 
 * are returning a specific amount of raw PCM data, in a signed, little-endian, 
 * 16-bit format.
 *
 * Returns the number of bytes written to the output_buf
 */
guint
mpg123_get_pcm_data (guint start_offset, guint32 num_samples_to_read,
		     guchar * output_buf, void *state)
{
  mpg123_state_t *_state = (mpg123_state_t *) state;
  gboolean refill_success;

  const guint32 real_bytes_per_sample =
    MP3_BYTES_PER_SAMPLE * _state->params->channels;
  const guint32 num_bytes_to_read =
    real_bytes_per_sample * num_samples_to_read;

  guint32 num_bytes_read = 0;

  guint32 next_byte_to_read;
  guint32 num_bytes_to_copy;
  guint32 pcm_index;

  while (num_bytes_read < num_bytes_to_read)
    {

      next_byte_to_read =
	real_bytes_per_sample * start_offset + num_bytes_read;

      pcm_index = next_byte_to_read - _state->pcm_offset;

      if ((pcm_index < 0) || (pcm_index >= _state->pcm_buf_size))
	{
	  refill_success =
	    _mpg123_refill_pcm_buffer (_state, next_byte_to_read);

	  if (!refill_success)
	    return num_bytes_read;

	  pcm_index = next_byte_to_read - _state->pcm_offset;
	}

      g_assert (pcm_index >= 0);
      g_assert (pcm_index < _state->pcm_buf_size);

      num_bytes_to_copy =
	_mpg123_num_bytes_to_copy (_state, num_bytes_to_read, num_bytes_read,
				   pcm_index);

      g_assert (next_byte_to_read >= _state->pcm_offset);

      g_assert (next_byte_to_read + num_bytes_to_copy
		<= _state->pcm_offset + _state->pcm_buf_size);

      _mpg123_little_endian_memcpy (output_buf + num_bytes_read,
				    _state->pcm_buf + pcm_index,
				    num_bytes_to_copy);

      num_bytes_read += num_bytes_to_copy;
    }
  return num_bytes_read;
}

/**
 * mpg123_close:
 *   This function closes the currently open mp3 stream associated with the
 *   state variable that is passed in
 *
 * @state: the state
 */
void
mpg123_close (codec_state_t * state)
{
  mpg123_state_t *_state = state;

  ExitMP3 (&_state->mp);
  fclose (_state->fp);
  g_free (_state->filename);
  g_free (_state->params);
  g_free (_state);
}


/**
 * _mpg123_new_state: 
 *
 * Creates a new mpg123_state_t and initializes it
 *
 * returns: a pointer to that struct, or NULL if
 * InitMP3 failed (which it can't do, at least in 
 * the version we're currently using)
 */
static mpg123_state_t *
_mpg123_new_state ()
{
  mpg123_state_t *state = g_malloc (sizeof (mpg123_state_t));

  g_return_val_if_fail (InitMP3 (&(state->mp)), NULL);

  state->fp = NULL;
  state->params = NULL;

  _mpg123_reset_avg (state);

  state->pcm_buf_size = 0;
  state->pcm_offset = -1;

  return state;
}


/**
 * _mpg123_refill_pcm_buffer:
 *
 * Fills up the buffer in @state so that @new_pcm_offset
 * is in the buffer (and as close to the beginning as 
 * possible)
 *
 * @state:
 * @new_pcm_offset: 
 *
 * Returns FALSE on failure
 */
static gboolean
_mpg123_refill_pcm_buffer (mpg123_state_t * state, guint32 new_pcm_offset)
{
  guchar mp3_data[MP3_MAX_FRAME_LENGTH];
  guint32 header;
  mp3_params_t *params;
  int result;
  gint num_bytes_decoded;

  if ((state->pcm_buf_size == 0) || (state->pcm_offset > new_pcm_offset))
    {
      rewind (state->fp);
      state->pcm_offset = 0;
      state->pcm_buf_size = 0;
      _mpg123_reset_avg (state);
    }

  do
    {
      state->pcm_offset += state->pcm_buf_size;
      if (mp3_sync_file (state->fp) == -1)
	return FALSE;

      if (fread (mp3_data, MP3_HEADER_LENGTH, 1, state->fp) != 1)
	return FALSE;

      header = MP3_MAKE_HEADER (mp3_data);

      params = mp3_get_params (header);

      if (params == NULL)
	{
	  g_free (params);
	  return FALSE;
	}

      state->bitrate_avg =
	(state->bitrate_avg * state->bitrate_count
	 + params->bitrate) / ++state->bitrate_count;

      result =
	fread (mp3_data + MP3_HEADER_LENGTH,
	       params->framesize - MP3_HEADER_LENGTH, 1, state->fp);

      if (result != 1)
	{
	  g_free (params);
	  return FALSE;
	}
      result = decodeMP3 (&state->mp, mp3_data, params->framesize,
			  state->pcm_buf, MP3_MAX_DECODED_FRAME_LENGTH,
			  &num_bytes_decoded);

      if (result != MP3_OK)
	{
	  g_free (params);
	  return FALSE;
	}

      state->pcm_buf_size = num_bytes_decoded;

      g_free (params);
    }
  while ((num_bytes_decoded < 1)
	 || (state->pcm_offset + state->pcm_buf_size < new_pcm_offset));
  return TRUE;
}

/**
 * _mpg123_reset_avg
 *
 * @state: the state
 *
 * Restarts calculation of the average bitrate
 */
static void
_mpg123_reset_avg (mpg123_state_t * state)
{
  g_assert (state);
  state->bitrate_count = 0;
  state->bitrate_avg = 0;
}

/**
 * _mpg123_num_bytes_to_copy:
 *
 * Returns the number of bytes that should be copied to the
 * output buffer
 */
static guint32
_mpg123_num_bytes_to_copy (mpg123_state_t * state, guint32 num_bytes_to_read,
			   guint32 num_bytes_read, guint32 pcm_index)
{
  g_assert (state->pcm_buf_size);

  return MIN (num_bytes_to_read - num_bytes_read,
	      state->pcm_buf_size - pcm_index);
}

/**
 * _mpg123_little_endian_memcpy:
 *
 * Only used on a big-endian system. Does a memcpy, but
 * swaps every other byte.
 */
static void
_mpg123_little_endian_memcpy (void *bto, const void *bfrom, gint32 n)
{
#if (G_BYTE_ORDER == G_LITTLE_ENDIAN)
  memcpy (bto, bfrom, n);
#else
  const gchar *from = (const gchar *) bfrom;
  gchar *to = (gchar *) bto;

  if (n % 2)
    n++;

  while (n > 1)
    {
      const gchar b0 = from[--n], b1 = from[--n];
      to[n] = b0;
      to[n + 1] = b1;
    }
#endif
}
