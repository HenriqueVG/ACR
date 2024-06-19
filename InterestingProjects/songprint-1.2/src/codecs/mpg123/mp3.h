/*  
 *  mp3.h - information and functions related to decoding MP3s
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
#ifndef MP3_H
#define MP3_H

#include <stdio.h>

#include <songprint.h>

#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM 1
#endif

#define MP3_NUM_GENRES 148

#define MP3_HEADER_LENGTH      4

#define MP3_FRAME_SYNC         0x7FF

#define MP3_MAX_FRAME_LENGTH   8192
#define MP3_MAX_SAMPLES_PER_FRAME 1152
#define MP3_BITS_PER_SAMPLE    16
#define MP3_BYTES_PER_SAMPLE   ((MP3_BITS_PER_SAMPLE + 7) / 8)
#define MP3_MAX_NUM_CHANNELS   2
#define MP3_MAX_DECODED_FRAME_LENGTH (MP3_MAX_NUM_CHANNELS \
                                      * MP3_MAX_SAMPLES_PER_FRAME \
                                      * MP3_BYTES_PER_SAMPLE)

#define MP3_CRC_ON 0
#define        MP3_PADDING_ON 1

#define MP3_MAKE_HEADER(s) (((s[0]) << 24) | ((s[1]) << 16) | \
                             ((s[2]) << 8) | (s[3]))

typedef enum
{
  MP3_MPEG_LAYER_RESERVED,
  MP3_MPEG_LAYER_3,
  MP3_MPEG_LAYER_2,
  MP3_MPEG_LAYER_1
}
mp3_layer_t;

typedef enum
{
  MP3_VER_2_5,
  MP3_VER_RESERVED,
  MP3_VER_2,
  MP3_VER_1
}
mp3_ver_t;

typedef enum
{
  MP3_CHAN_STEREO,
  MP3_CHAN_JOINTSTEREO,
  MP3_CHAN_DUALSTEREO,
  MP3_CHAN_MONO
}
mp3_chan_t;

typedef enum
{
  MP3_EMPHASIS_NONE,
  MP3_EMPHASIS_50_15MS,
  MP3_EMPHASIS_RESERVED,
  MP3_EMPHASIS_CCITJ_17
}
mp3_emphasis_t;

typedef struct
{
  gint32 framesync;
  gint32 version;
  gint32 layer;
  gint32 crc;
  gint32 bitrate;
  gint32 freq;
  gint32 padding;
  gint32 priv;
  gint32 channel_code;
  gint32 modeext;
  gint32 copyright;
  gint32 original;
  gint32 emphasis;
  gint32 framesize;
  gint32 channels;
}
mp3_params_t;

extern const guint16 MP3_KILOBIT_RATES[3][2][16];
extern const guint16 MP3_SAMPLE_RATES[4][4];
extern const gchar *MP3_GENRES[];

guint32 mp3_get_framesize (mp3_params_t * params);
mp3_params_t *mp3_get_params (const guint32 header);
glong mp3_sync_file (FILE * inf);
glong mp3_sync (const guchar * buf, glong buf_size, glong index);
void mp3_read_tag (FILE * fp, sp_id_t * info);

#endif /* MP3_H */
