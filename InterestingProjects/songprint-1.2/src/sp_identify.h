/*  sp_identify.h - defines data types for doing network communication with
 *                  a song identification server
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

#ifndef _SP_IDENTIFY_H
#define _SP_IDENTIFY_H

#include "sp_types.h"

typedef enum { 
        SP_RESPONSE_ERROR,
        SP_RESPONSE_FOUND,
        SP_RESPONSE_NOTFOUND,
        SP_RESPONSE_POSSIBLE
} sp_response;

/* The generic send packet header */
typedef struct 
{
	gchar	sp_header[3];				/* == \x05\x14\x03 (etc) */
	guint16	sp_request_type;
	guint16	sp_packet_length;
} sp_send_packet_t;

#define SP_SEND_PACKET_T_SIZE 7

/* sp_request_type == 0x00 (RT_SONG_IDENTIFICATION) structures */
typedef struct 
{
	guint16	version;
	guint16	sample_rate;
	guint16	num_channels;
	guint16	avg_bitrate;
	guint16	sample_size;
	guint16	codec_type;
} sp_send_song_id_header_t;

#define SP_SEND_SONG_ID_HEADER_T_SIZE 12

typedef struct
{
	guint8	sp_header_type;
	guint8	sp_label_type;
} sp_send_song_id_t;

#define SP_SEND_SONG_ID_T_SIZE 2

/* response packet structure */
typedef struct
{
	guint16	sp_response_type;
	guint16	sp_packet_length;
} sp_response_packet_t;

#define SP_RESPONSE_PACKET_T_SIZE 4

typedef struct
{
	guint16	sp_req_response_type;
	guint8	sp_label_type;
	guint8	sp_num_labels;
} sp_response_song_id_t;

#define SP_RESPONSE_SONG_ID_T_SIZE 4

#endif /* _SP_IDENTIFY_H */
