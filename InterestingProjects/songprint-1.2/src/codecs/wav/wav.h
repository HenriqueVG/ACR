/*
 *  wav.h - library header for reading in WAV audio files
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

#define WAV_HEADER_SIZE 44

typedef struct
{
  guint8 _riff[4];
  guint32 length;
  guint8 _wavefmt[8];
  guint32 _header_length;
  guint16 _format;
  guint16 num_channels;
  guint32 sample_rate;
  guint32 bytes_per_sec;
  guint16 block_align;
  guint16 bits_per_sample;
  guint8 _data[4];
  guint32 data_length;
}
wav_header_t;

wav_header_t *wav_make_header (const guint8 header_raw[]);
