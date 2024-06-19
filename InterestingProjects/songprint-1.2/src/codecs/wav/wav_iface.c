/*
 *  wav_iface.c
 *
 *  This file manages the details of decoding a WAV file and returning
 *  the PCM data to the "codec manager" layer above.  It can open and close
 *  WAV files, find out information about the song, find out the audio
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

#include <string.h>

#include "wav_iface.h"

static wav_state_t *_wav_new_state ();

static void _wav_destroy_state (wav_state_t * state);

/**
 * wav_open:
 *
 * @filename: the filename to open
 *
 * Opens @filename and initializes a new wav_state_t 
 * 
 * Returns a pointer to the new state, or NULL if there 
 * is a problem
 */
codec_state_t *
wav_open (const gchar * filename)
{
  guint8 header_raw[WAV_HEADER_SIZE];

  wav_state_t *state;

  g_return_val_if_fail(filename != NULL, NULL);

  state = _wav_new_state ();

  state->fp = fopen (filename, "rb");

  state->filename = g_strdup (filename);

  if (state->fp == NULL)
    {
      g_free (state->filename);
      g_free (state);
      return NULL;
    }
      if (fread (header_raw, WAV_HEADER_SIZE, 1, state->fp) != 1)
        {
          fclose (state->fp);
          g_free (state->filename);
          g_free (state);
          return NULL;
        }
      state->header = wav_make_header (header_raw);

      if (state->header == NULL)
        {
          fclose (state->fp);
          g_free (state->filename);
          g_free (state);
          return NULL;
        }

  return (codec_state_t *) state;
}

/**
 * wav_get_format:
 *
 * @state: the state
 * @pcm_format: where the info goes
 *
 * Takes an existing #sp_audio_info_t structure and fills
 * in data about the audio format, such as sample rate or
 * number of channels.  This function may only be called 
 * between calls to wav_open and wav_close.
 */
void
wav_get_format (codec_state_t * state, sp_audio_info_t * pcm_format)
{
  wav_state_t *_state = (wav_state_t *) state;

  g_return_if_fail(state != NULL);
  g_return_if_fail(pcm_format != NULL);

  pcm_format->sample_rate = _state->header->sample_rate;
  pcm_format->num_channels = _state->header->num_channels;
  pcm_format->avg_bitrate = _state->header->bits_per_sample
    * _state->header->sample_rate * _state->header->num_channels;
  pcm_format->bits_per_sample = _state->header->bits_per_sample;
  pcm_format->codec_type = CODEC_WAV;
}

/**
 * wav_get_song_info:
 *
 * @state: the state
 * @song_info: where the info goes
 *
 * Stores the filename (since wavs don't have any ID3-like 
 * information, this is all that can be provided) in 
 * @song_info. This function may only be called between 
 * calls to wav_open and wav_close.
 */
void
wav_get_song_info (codec_state_t * state, sp_id_t * song_info)
{
  wav_state_t *_state = (wav_state_t *) state;
  g_return_if_fail(state != NULL);
  sp_id_set_filename (song_info, _state->filename);
}

/**
 * wav_get_pcm_data:
 *
 * @sample_offset: the number of samples to skip, 
 *                  independant of word size or number of channels
 * @num_samples: the number of samples we are to decode, 
 *                  independant of word size or number of channels
 * @output_buf: the buffer to write data to
 * @state: the state
 *
 * Stores the requested number of samples in the output_buf variable.
 * The samples are obviously independant of the bitrate and other codec 
 * details.  We are returning a specific amount of raw PCM data, in a 
 * signed, little-endian, 16-bit format.
 *
 * Returns the number of bytes written to the output_buf
 */
guint
wav_get_pcm_data (guint sample_offset, guint32 num_samples_to_read,
                  guchar * output_buf, void *state)
{
  wav_state_t *_state = (wav_state_t *) state;
  glong rv;
  guint32 num_bytes_read;
  const guint32 num_bytes_to_read = 
      state != NULL 
      ? num_samples_to_read * _state->header->block_align
      : 0;  

  g_return_val_if_fail(state != NULL, 0);

  rv = fseek (_state->fp, _state->header->block_align
              * sample_offset + WAV_HEADER_SIZE, SEEK_SET);
  if (rv != 0)
    return 0;

  rv = 1;
  num_bytes_read = 0;
  while (rv && (num_bytes_read < num_bytes_to_read))
    {
      rv = fread (output_buf + num_bytes_read,
                  1, num_bytes_to_read - num_bytes_read, _state->fp);
      
      if(ferror(_state->fp))
          return 0;

      num_bytes_read += rv;
    }

  return num_bytes_read;
}

/**
 * wav_close:
 *   This function closes the currently open wav stream associated with the
 *   state variable that is passed in
 *
 * @state: the state
 */
void
wav_close (codec_state_t * state)
{
  wav_state_t *_state = (wav_state_t *) state;

  g_return_if_fail(state);

  fclose (_state->fp);
  _wav_destroy_state (_state);
}

/**
 * _wav_new_state: 
 *
 * Creates a new wav_state_t and initializes it
 *
 * Returns a pointer to that struct
 */
static wav_state_t *
_wav_new_state ()
{
  wav_state_t *state = g_malloc (sizeof (wav_state_t));

  state->fp = NULL;
  state->filename = NULL;
  state->header = NULL;

  return state;
}

/**
 * _wav_destroy_state:
 *
 * Cleans up and frees @state
 */
static void
_wav_destroy_state (wav_state_t * state)
{
  g_free (state->filename);
  g_free (state->header);
  g_free (state);
}
