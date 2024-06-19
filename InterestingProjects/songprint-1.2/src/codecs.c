/*
 *  codecs.c - interface between main.c and all supported codecs
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

#include "codecs.h"
#include "config.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_MPG123
#include "mpg123_iface.h"
#endif

#ifdef HAVE_WAV
#include "wav_iface.h"
#endif

#ifdef HAVE_OGGVORBIS
#include "vorbis_iface.h"
#endif

/* These macros remove some redundancy for assigning function pointers */

/* Add one function to vtable.  Uses '##' preprocessor operator for 
   concatenation */
#define _CODEC_ADD_FUNC(vtable, fn, codec) ( vtable->fn = codec ## _ ## fn )

/* Add a host of functions to the vtable */
#define _CODEC_ADD_FUNCS(vtable, codec) \
{ \
    _CODEC_ADD_FUNC(vtable, open, codec); \
    _CODEC_ADD_FUNC(vtable, get_format, codec); \
    _CODEC_ADD_FUNC(vtable, get_song_info, codec); \
    _CODEC_ADD_FUNC(vtable, get_pcm_data, codec); \
    _CODEC_ADD_FUNC(vtable, close, codec); \
}

/**
 *  codecs_classify:
 *  @filename:   File to classify
 *  @functions:  Table of function pointers to fill in
 *
 *  Determines the codec to use based upon filename.  If
 *  the file has a supported extension, the function pointers
 *  in @functions are filled in appropriately.
 *
 *  Returns: the codec type on success, %CODEC_UNKNOWN if the
 *  file isn't supported.      
 */
codec_type_t
codecs_classify (const gchar * filename, codec_vtable_t * functions)
{
  codec_type_t return_codec_type = CODEC_UNKNOWN;
  gchar *ext;

  g_return_val_if_fail(functions != NULL, return_codec_type);
  g_return_val_if_fail(filename != NULL, return_codec_type);

  ext = strrchr (filename, '.');

  if (!ext || !*(ext + 1))
    return CODEC_UNKNOWN;

  ext++;
  if (g_strcasecmp (ext, "ogg") == 0)
    {
#ifdef HAVE_OGGVORBIS
      _CODEC_ADD_FUNCS (functions, vorbis);
      return_codec_type = CODEC_OGGVORBIS;
#endif
    }
  else if (g_strcasecmp (ext, "mp3") == 0)
    {
#ifdef HAVE_MPG123
      _CODEC_ADD_FUNCS (functions, mpg123);
      return_codec_type = CODEC_MP3_MPG123;
#endif
    }
  else if (g_strcasecmp (ext, "wav") == 0)
    {
#ifdef HAVE_WAV
      _CODEC_ADD_FUNCS (functions, wav);
      return_codec_type = CODEC_WAV;
#endif
    }

  return return_codec_type;
}
