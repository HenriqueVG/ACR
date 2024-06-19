/*
 *  wav_iface.h - interface for reading in WAV audio files for the song
 *                identification library
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

#ifndef WAV_IFACE_H
#define WAV_IFACE_H

#include <glib.h>
#include <stdio.h>

#include "wav.h"
#include "codecs.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct
  {
    FILE *fp;
    gchar *filename;
    wav_header_t *header;
  }
  wav_state_t;

  codec_state_t 
*wav_open (const gchar * filename);

  void 
wav_get_format (codec_state_t * state, sp_audio_info_t * pcm_format);

  void 
wav_get_song_info (codec_state_t * state, sp_id_t * song_info);

    guint
    wav_get_pcm_data (guint start_offset, guint32 num_samples_to_read,
                      guchar * output_buf, void *state);

  void 
wav_close (codec_state_t * state);

#ifdef __cplusplus
}
#endif

#endif                                /* WAV_IFACE_H */
