/*
 *  vorbis_iface.h
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

#ifndef VORBIS_IFACE
#define VORBIS_IFACE

#include "vorbis/vorbisfile.h"

#include "songprint.h"
#include "codecs.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct 
  {
    OggVorbis_File vf;
    FILE *stream;
    vorbis_info *vi;
    gchar *filename;
    gint current_section;
    gint last_section;
    guint current_sample_offset;
  }
  vorbis_iface_state_t;

  codec_state_t *vorbis_open (const gchar * filename);
  void vorbis_close (codec_state_t * state);
  guint vorbis_get_pcm_data (guint sample_offset, guint num_samples,
                             guchar * output_buf, void *user_data);
  void vorbis_get_song_info (codec_state_t * state, sp_id_t * song_info);
  void vorbis_get_format (codec_state_t * state, sp_audio_info_t * format);

#ifdef __cplusplus
}
#endif

#endif /* VORBIS_IFACE */
