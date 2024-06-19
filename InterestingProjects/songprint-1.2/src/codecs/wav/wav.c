/*
 *  wav.c - Functions for reading in a WAV audio file
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
#include <string.h>
#include "wav.h"

/* These macros deference x and convert them 
   from little endian to host byte order */
#define _WAV_16_BITS_TO_HOST_ORDER(x) GUINT16_FROM_LE(*((guint16 *)(x)))
#define _WAV_32_BITS_TO_HOST_ORDER(x) GUINT32_FROM_LE(*((guint32 *)(x)))

static gboolean _wav_header_is_valid (const wav_header_t * header);

/**
 * wav_make_header:
 *
 * @header_raw: an array containing a wav header,
 * as read from a file
 *
 * Decodes @header_raw and, if it's a valid wav
 * header, returns a #wav_header_t with the 
 * information from it - or NULL if it's invalid.
 *
 * Returns the header
 */
wav_header_t *
wav_make_header (const guint8 header_raw[])
{
  wav_header_t *header;

  header = g_malloc (sizeof (wav_header_t));

  memcpy (header->_riff, header_raw + 0, 4);
  memcpy (header->_wavefmt, header_raw + 8, 8);
  memcpy (header->_data, header_raw + 36, 4);

  header->length = _WAV_32_BITS_TO_HOST_ORDER (header_raw + 4);
  header->_header_length = _WAV_32_BITS_TO_HOST_ORDER (header_raw + 16);
  header->sample_rate = _WAV_32_BITS_TO_HOST_ORDER (header_raw + 24);
  header->bytes_per_sec = _WAV_32_BITS_TO_HOST_ORDER (header_raw + 28);
  header->data_length = _WAV_32_BITS_TO_HOST_ORDER (header_raw + 40);

  header->_format = _WAV_16_BITS_TO_HOST_ORDER (header_raw + 20);
  header->num_channels = _WAV_16_BITS_TO_HOST_ORDER (header_raw + 22);
  header->block_align = _WAV_16_BITS_TO_HOST_ORDER (header_raw + 32);
  header->bits_per_sample = _WAV_16_BITS_TO_HOST_ORDER (header_raw + 34);

  if (_wav_header_is_valid (header))
    return header;
  else
    {
      g_free (header);
      return NULL;
    }
}

/**
 * _wav_header_is_valid
 * 
 * Returns TRUE is the header is valid, FALSE
 * otherwise (duh)
 */
static gboolean
_wav_header_is_valid (const wav_header_t * header)
{
  if (strncmp (header->_riff, "RIFF", 4))
    return FALSE;
  if (strncmp (header->_wavefmt, "WAVEfmt ", 8))
    return FALSE;
  if (strncmp (header->_data, "data", 4))
    return FALSE;
  if (header->_header_length != 0x00000010)
    return FALSE;
  if (header->_format != 0x0001)
    return FALSE;

  if ((header->num_channels != 0x0001) && (header->num_channels != 0x0002))
    return FALSE;

  if (header->bytes_per_sec != header->sample_rate * header->block_align)
    return FALSE;

  if (header->block_align * 8 != header->num_channels
      * header->bits_per_sample)
    return FALSE;

  return TRUE;
}
