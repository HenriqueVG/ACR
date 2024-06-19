/*
 *  codecs.h - interface between main.c and all supported codecs
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

#ifndef CODECS_H
#define CODECS_H

#include <songprint.h>
#include "config.h"

typedef void codec_state_t; 

typedef struct
{
    codec_state_t * (*open) (const gchar *filename);
    void (*get_format) (codec_state_t *state, sp_audio_info_t *pcm_format);
    void (*get_song_info) (codec_state_t *state, sp_id_t *song_info); 
    guint (*get_pcm_data) (guint sample_offset, guint num_samples,
                           guchar *output_buf, void *user_data); 
    void (*close) (codec_state_t *state);
} codec_vtable_t;

codec_type_t codecs_classify(const gchar *filename, 
                             codec_vtable_t *functions);

#endif
