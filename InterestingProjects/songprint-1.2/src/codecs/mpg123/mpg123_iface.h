/*
 *  mpg123_iface.h - interface for reading in MP3 audio files for the song
 *                   identification library
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

#ifndef MPG123_IFACE_H
#define MPG123_IFACE_H

#include "mp3.h"
#include "mpglib.h"
#include "codecs.h"

#include <glib.h>
#include <songprint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct
  {
    struct mpstr mp;		/* Internal data structure used by mpg123 */
    gchar *filename;		/* The name of the MP3 */
    FILE *fp;			/* The MP3's file pointer */
    mp3_params_t *params;      	/* Other parameters of the MP3, taken
				   from the header */

    /* The pcm_buf is where mpg123 puts the decoded pcm data. 
     * pcm_buf_size is the current size of it (after that point,
     * it's just random garbage) 
     * pcm_offset is the difference between the beginning of
     * the pcm_buf and the start of the actual pcm data
     */
    gchar pcm_buf[MP3_MAX_DECODED_FRAME_LENGTH];
    guint32 pcm_buf_size;
    guint32 pcm_offset;

    /* These are used to calculate the average bitrate */
    guint32 bitrate_avg;
    guint32 bitrate_count;
  }
    mpg123_state_t;
    
    codec_state_t * 
    mpg123_open (const gchar * filename);

  void
    mpg123_get_format (codec_state_t * state, sp_audio_info_t * pcm_format);

  void mpg123_get_song_info (codec_state_t * state, sp_id_t * song_info);

    guint
    mpg123_get_pcm_data (guint start_offset, guint32 num_samples_to_read,
			 guchar * output_buf, void *state);

  void mpg123_close (codec_state_t * state);

#ifdef __cplusplus
}
#endif

#endif				/* MPG123_IFACE_H */
