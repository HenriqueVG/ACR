/*  sp_signature.h - defines data types necessary to generate and store a signature
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

#ifndef _SP_SIGNATURE_H
#define _SP_SIGNATURE_H

#include <rfftw.h>

#include "songprint.h"

#define NUM_FFT_SAMPLES 2048
#define NUM_FFT_FINAL_BANDS 16
#define NUM_FFT_SAMPLES_SQ (NUM_FFT_SAMPLES * NUM_FFT_SAMPLES)

typedef struct
{
    guchar *raw_pcm_data;
    guint total_bytes_decoded;
    guint sample_size;
    guint buffer_size;
    guint sample_offset;
    gboolean found_content;
} _sig_state_decode_data_t;

typedef struct
{
    rfftw_plan plan;
    sp_audio_info_t *format;
    guint num_blocks;
    guint blocks_allocated;
    gulong *freq_blocks[NUM_FFT_FINAL_BANDS];
    _sig_state_decode_data_t decode_data;
} _sig_state_t;

#endif /* _SP_SIGNATURE_H */
