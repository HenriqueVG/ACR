/*  songprint.h - Generates a signature based on PCM data and sends it 
 *  to a server for identification
 *
 *  Glossary:
 *
 *  PCM data  - uncompressed waveform data, suitable for 
 *              sending to a sound card
 *
 *  codec     - something which converts between an encoded
 *              sound file and its PCM data
 *
 *  signature - a 512-bit number which summarizes what a
 *              block of PCM data "sounds like"
 *
 *  id        - the information which characterizes a song, such 
 *              as the song's title, the albums it is on, etc.
 *            
 *  This API is subject to revision.
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

#ifndef _SONGPRINT_H
#define _SONGPRINT_H

#include <glib.h>

/* Sometimes used on Win32 systems */
#ifdef NEED_DLLEXPORT
# define SP_SDK_API __declspec(dllexport)
#else
# define SP_SDK_API
#endif /* NEED_DLLEXPORT */

/*
 * All songprint functions return an sp_rv. As of now,
 * only success and two kinds of failure are supported.
 */
typedef enum {
        SP_SUCCESS,             /* Generic success */
        SP_FAILURE,             /* Generic failure */
        SP_FAILURE_EXTENDED     /* Failure with extended error information */
} sp_rv;

/*
 * The songprint servers use information about the codec 
 * which encoded a song in the identification process.
 */
typedef enum 
{
    CODEC_UNKNOWN,
    CODEC_OTHER,
    CODEC_OGGVORBIS, 
    CODEC_MP3_MPG123, 
    CODEC_MP3_XAUDIO, 
    CODEC_MP3_XING, 
    CODEC_MP3_FRAUNHOFER, 
    CODEC_WAV
} codec_type_t;

/*
 * Information needed to make heads or tails of the PCM
 * stream
 */
typedef struct
{
    gint            sample_rate;
    gint            num_channels;
    gint            avg_bitrate;
    gint            bits_per_sample;
    codec_type_t    codec_type;
} sp_audio_info_t;

/*
 * Currently, a signature consists only of a 512-bit
 * number.
 */

#define SP_SIG_LENGTH 32
typedef struct
{
    guint16 data[SP_SIG_LENGTH];
} sp_signature_t;

#define SP_SIGNATURE_T_SIZE SP_SIG_LENGTH * 2

/* "id" information, as described in the glossary */
typedef struct
{
    gchar   *filename;
    gchar   *title;
    gchar   **artists;
    gchar   **genres;
    gchar   **albums;
    gchar   *moreinfo;
    guint32 media_id;
} sp_id_t, sp_id_send_t, sp_id_recv_t;

/* Error data */
typedef struct
{
	guint16	error_number;
	gchar	*error;
} sp_error_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * sp_generate_signature: 
 *
 * Reads an audio stream and generates a signature
 * based on what it "sounds like"
 *
 * @song_audio_format: information needed to make heads
 * or tails of the PCM stream
 *
 * @signature: the location to write the signature to
 *
 * @get_pcm_data: a pointer to your function which fills
 * output_buf with num_samples worth of PCM data starting
 * sample_offset bytes info the PCM stream
 *
 * @user_data: gets passed to the function pointed to by 
 * @get_pcm_data
 *
 * Returns SP_SUCCESS or SP_FAILURE
 */
SP_SDK_API sp_rv
sp_generate_signature(sp_audio_info_t *song_audio_format, 
                      sp_signature_t *signature, 
                      guint (*get_pcm_data)(guint sample_offset, 
                                            guint num_samples, 
                                            guchar *output_buf, 
                                            void *user_data),
                      void *user_data);

/**
 * sp_id_signature:
 * 
 * Sends a signature off to a server for identification
 *
 * @song_audio_format: information needed to make heads
 * or tails of the PCM stream
 *
 * @signature: the signature to be matches against the 
 * server's database
 *
 * @send_info: any information which might be able to help
 * the server make its identification, such as the name
 * of the file being identified, or any other information
 * which is probably true (such as the artist given in an 
 * MP3's ID3 tag)
 *
 * @recv_info: the place to store the id information 
 * returned by the server. The server may return a
 * number of possible matches here, all in order of
 * likelihood. When no longer needed, this value must be 
 * passed to sp_delete_id_recv() to avoid a memory leak.
 *
 * @num_matches: where to store the number of matches made
 *
 * @error: where to store information about an error, if
 * one occurs. When no longer needed, this value must be 
 * passed to sp_delete_error() to avoid a memory leak.
 *
 * Returns SP_SUCCESS or SP_FAILURE
 */
SP_SDK_API sp_rv
sp_id_signature(sp_audio_info_t *song_audio_format,
                sp_signature_t *signature,
                sp_id_send_t *send_info,
                sp_id_recv_t ***recv_info,
                gint *num_matches,
                sp_error_t *error);
        
SP_SDK_API sp_rv
sp_delete_id_recv(sp_id_recv_t **recv_info, gint num_labels);
        
SP_SDK_API sp_rv
sp_delete_error(sp_error_t *error);

/* Functions for manipulating the sp_id_t datatype */

sp_id_t *
sp_id_new();

void 
sp_id_destroy(sp_id_t *info);

void 
sp_id_set_filename(sp_id_t *info, const gchar *filename);
void 
sp_id_set_title(sp_id_t *info, const gchar *title);
void 
sp_id_append_artist(sp_id_t *info, const gchar *artist);
void 
sp_id_append_album(sp_id_t *info, const gchar *album);
void 
sp_id_append_genre(sp_id_t *info, const gchar *genre);
void 
sp_id_set_moreinfo(sp_id_t *info, const gchar *moreinfo);
void 
sp_id_set_media_id(sp_id_t *info, const guint32 media_id);

void 
sp_id_clear_filename(sp_id_t *info);
void 
sp_id_clear_title(sp_id_t *info);
void 
sp_id_clear_artists(sp_id_t *info);
void 
sp_id_clear_albums(sp_id_t *info);
void 
sp_id_clear_genres(sp_id_t *info);
void 
sp_id_clear_moreinfo(sp_id_t *info);

gchar *
sp_id_get_filename(sp_id_t *info);
gchar *
sp_id_get_title(sp_id_t *info);
gchar **
sp_id_get_artists(sp_id_t *info);
gchar **
sp_id_get_albums(sp_id_t *info);
gchar **
sp_id_get_genres(sp_id_t *info);
gchar *
sp_id_get_moreinfo(sp_id_t *info);
guint32
sp_id_get_media_id(sp_id_t *info);

guint 
sp_id_count_artists(sp_id_t *info);
guint 
sp_id_count_albums(sp_id_t *info);
guint 
sp_id_count_genres(sp_id_t *info);

#ifdef __cplusplus
}
#endif 

#endif /* _SONGPRINT_H */
