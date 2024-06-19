/*  sp_types.h - defines the data types and version information for network
 *               communication with a song identification server
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

#ifndef _SP_TYPES_H
#define _SP_TYPES_H

#include <glib.h>

/* Various Version Information */
#define SP_HEADER_VERSION		0;

/* Send/Receive Request Packet Types */
#define RT_SONG_IDENTIFICATION	0x0

/* Header Types */
#define SP_HEADER_STANDARD      0x0

/* Label Types */
#define SP_LABEL_STANDARD       0x0
    
/* Label Types */
typedef enum {
        LABEL_FILENAME,
        LABEL_TITLE,
        LABEL_ARTISTS,
        LABEL_GENRES,
        LABEL_ALBUMS,
        LABEL_MOREINFO,
        LABEL_MEDIAID,
        LABEL_UNKNOWN
} sp_label;

#endif /* _SP_TYPES_H */
