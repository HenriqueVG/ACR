/*  sp_id_t.c - Routines for manipulating the sp_id_t structure 
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

#include "songprint.h"

/**
 *  _stringv_append:
 *  @stringv:  Pointer to a string array 
 *  @string:   String to append
 * 
 *  Appends a string to the string array.  The array pointed to
 *  by stringv will grow by one slot and string will be copied
 *  into the new slot.
 */
static void
_stringv_append (gchar *** stringv, const gchar * string)
{
  gint array_count;

  /* Count the elements in *stringv */
  for (array_count = 0; (*stringv)[array_count] != 0; array_count++);

  /* Add the null array terminator and the new list element */
  array_count += 2;
  *stringv = g_realloc (*stringv, array_count * sizeof (gchar *));
  (*stringv)[array_count - 2] = g_strdup (string);
  (*stringv)[array_count - 1] = NULL;
}

/**
 *  _stringv_clear:
 *  @stringv  String array
 *
 *  Frees all allocated strings in the string array and sets the
 *  first pointer to NULL.
 */
static void
_stringv_clear (gchar ** stringv)
{
  while (*stringv)
    {
      g_free (*stringv);
      *stringv = NULL;
      stringv++;
    }
}

/**
 *  sp_id_new:
 *
 *  Create a new sp_id_t.
 *
 *  Returns a newly allocated, cleared sp_id_t.
 */
sp_id_t *
sp_id_new ()
{
  sp_id_t *new_id;

  new_id = g_malloc0 (sizeof (sp_id_t));

  sp_id_set_filename (new_id, "");
  sp_id_set_title (new_id, "");
  sp_id_set_moreinfo (new_id, "");
  sp_id_set_media_id (new_id, 0);
  new_id->artists = g_malloc0 (sizeof (gchar *));
  new_id->albums = g_malloc0 (sizeof (gchar *));
  new_id->genres = g_malloc0 (sizeof (gchar *));
  return (new_id);
}

/**
 *  sp_id_destroy:
 *  @info   A previously allocated sp_id_t.
 *
 *  Frees all memory associated with @info.
 */
void
sp_id_destroy (sp_id_t * info)
{
  g_return_if_fail(info != NULL);

  sp_id_clear_filename (info);
  sp_id_clear_title (info);
  sp_id_clear_artists (info);
  sp_id_clear_albums (info);
  sp_id_clear_genres (info);
  sp_id_clear_moreinfo (info);

  /* g_free handles checks for NULL */
  g_free (info->filename);
  g_free (info->title);
  g_free (info->artists);
  g_free (info->albums);
  g_free (info->genres);
  g_free (info->moreinfo);

  g_free (info);
}

/**
 *  sp_id_set_filename:
 *  @info      A previously allocated sp_id_t
 *  @filename  New filename to store in @info
 *
 *  Copies the string @filename into the filename field of @info.
 */
void
sp_id_set_filename (sp_id_t * info, const gchar * filename)
{
  g_return_if_fail(info != NULL);
  g_return_if_fail(filename != NULL);

  g_free (info->filename);
  info->filename = g_strdup (filename);
}

/**
 *  sp_id_set_title:
 *  @info   A previously allocated sp_id_t
 *  @title  New title to store in @info
 *
 *  Copies the string @title into the title field of @info.
 */
void
sp_id_set_title (sp_id_t * info, const gchar * title)
{
  g_return_if_fail(info != NULL);
  g_return_if_fail(title != NULL);

  g_free (info->title);
  info->title = g_strdup (title);
}

/**
 *  sp_id_append_artist:
 *  @info      A previously allocated sp_id_t
 *  @artist    New artist to store in @info
 *
 *  Appends the string @artist to the string array in @info.
 */
void
sp_id_append_artist (sp_id_t * info, const gchar * artist)
{
  g_return_if_fail(info != NULL);
  g_return_if_fail(artist != NULL);

  _stringv_append (&info->artists, artist);
}

/**
 *  sp_id_append_album:
 *  @info     A previously allocated sp_id_t
 *  @album    New album to store in @info
 *
 *  Appends the string @album to the string array in @info.
 */
void
sp_id_append_album (sp_id_t * info, const gchar * album)
{
  g_return_if_fail(info != NULL);
  g_return_if_fail(album != NULL);

  _stringv_append (&info->albums, album);
}

/**
 *  sp_id_append_genre:
 *  @info     A previously allocated sp_id_t
 *  @genre    New genre to store in @info
 *
 *  Appends the string @genre to the string array in @info.
 */
void
sp_id_append_genre (sp_id_t * info, const gchar * genre)
{
  g_return_if_fail(info != NULL);
  g_return_if_fail(genre != NULL);

  _stringv_append (&info->genres, genre);
}

/**
 *  sp_id_set_moreinfo:
 *  @info   A previously allocated sp_id_t
 *  @moreinfo  New url to store in @info
 *
 *  Copies the string @moreinfo into the moreinfo field of @info.
 *  Moreinfo is a URL that should provide data about the song.
 */
void
sp_id_set_moreinfo (sp_id_t * info, const gchar * moreinfo)
{
  g_return_if_fail(info != NULL);
  g_return_if_fail(moreinfo != NULL);

  g_free (info->moreinfo);
  info->moreinfo = g_strdup (moreinfo);
}

/**
 *  sp_id_set_media_id:
 *  @info   A previously allocated sp_id_t
 *  @media_id  New media id to store in @info
 *
 *  Sets the media_id field in @info to the value of @media_id.
 *  Media id is a unique number that identifies a specific recording.
 */
void
sp_id_set_media_id (sp_id_t * info, const guint32 media_id)
{
  g_return_if_fail(info != NULL);

  info->media_id = media_id;
}

/**
 *  sp_id_clear_filename:
 *  @info      A previously allocated sp_id_t
 *
 *  Clears the filename field in @info.
 */
void
sp_id_clear_filename (sp_id_t * info)
{
  g_return_if_fail(info != NULL);
  sp_id_set_filename (info, "");
}

/**
 *  sp_id_clear_title:
 *  @info      A previously allocated sp_id_t
 *
 *  Clears the title field in @info.
 */
void
sp_id_clear_title (sp_id_t * info)
{
  g_return_if_fail(info != NULL);
  sp_id_set_title (info, "");
}

/**
 *  sp_id_clear_artists:
 *  @info      A previously allocated sp_id_t
 *
 *  Clears the artist array in @info.
 */
void
sp_id_clear_artists (sp_id_t * info)
{
  g_return_if_fail(info != NULL);
  _stringv_clear (info->artists);
}

/**
 *  sp_id_clear_albums:
 *  @info      A previously allocated sp_id_t
 *
 *  Clears the album array in @info.
 */
void
sp_id_clear_albums (sp_id_t * info)
{
  g_return_if_fail(info != NULL);
  _stringv_clear (info->albums);
}

/**
 *  sp_id_clear_genres:
 *  @info      A previously allocated sp_id_t
 *
 *  Clears the genre array in @info.
 */
void
sp_id_clear_genres (sp_id_t * info)
{
  g_return_if_fail(info != NULL);
  _stringv_clear (info->genres);
}

/**
 *  sp_id_clear_moreinfo:
 *  @info      A previously allocated sp_id_t
 *
 *  Clears the moreinfo field in @info.
 */
void
sp_id_clear_moreinfo (sp_id_t * info)
{
  g_return_if_fail(info != NULL);
  sp_id_set_moreinfo (info, "");
}

/**
 *  sp_id_get_filename:
 *  @info      A previously allocated sp_id_t
 *
 *  Get the filename field from @info. 
 *   
 *  Returns the filename string.  Callers should copy this
 *  string before modifying it.
 */
gchar *
sp_id_get_filename (sp_id_t * info)
{
  g_return_val_if_fail(info != NULL, NULL);
  return (info->filename);
}

/**
 *  sp_id_get_title:
 *  @info      A previously allocated sp_id_t
 *
 *  Get the title field from @info. 
 *   
 *  Returns the title string.  Callers should copy this
 *  string before modifying it.
 */
gchar *
sp_id_get_title (sp_id_t * info)
{
  g_return_val_if_fail(info != NULL, NULL);
  return (info->title);
}

/**
 *  sp_id_get_artists:
 *  @info      A previously allocated sp_id_t
 *
 *  Get the artist array from @info. 
 *   
 *  Returns the artist string array. 
 */
gchar **
sp_id_get_artists (sp_id_t * info)
{
  g_return_val_if_fail(info != NULL, NULL);
  return (info->artists);
}

/**
 *  sp_id_get_albums:
 *  @info      A previously allocated sp_id_t
 *
 *  Get the album array from @info. 
 *   
 *  Returns the album string array. 
 */
gchar **
sp_id_get_albums (sp_id_t * info)
{
  g_return_val_if_fail(info != NULL, NULL);
  return (info->albums);
}

/**
 *  sp_id_get_genres:
 *  @info      A previously allocated sp_id_t
 *
 *  Get the genre array from @info. 
 *   
 *  Returns the genre string array. 
 */
gchar **
sp_id_get_genres (sp_id_t * info)
{
  g_return_val_if_fail(info != NULL, NULL);
  return (info->genres);
}

/**
 *  sp_id_get_moreinfo:
 *  @info      A previously allocated sp_id_t
 *
 *  Get the moreinfo field from @info. 
 *   
 *  Returns the moreinfo URL.  Callers should copy this
 *  string before modifying it.
 */
gchar *
sp_id_get_moreinfo (sp_id_t * info)
{
  g_return_val_if_fail(info != NULL, NULL);
  return (info->moreinfo);
}

/**
 *  sp_id_get_media_id:
 *  @info      A previously allocated sp_id_t
 *
 *  Get the media_id field from @info. 
 *   
 *  Returns the unique media id.  
 */
guint32
sp_id_get_media_id (sp_id_t * info)
{
  g_return_val_if_fail(info != NULL, 0);
  return (info->media_id);
}

/**
 *  sp_id_count_artists:
 *  @info      A previously allocated sp_id_t
 *
 *  Count the size of the artist array from @info. 
 *   
 *  Returns the size of the array. 
 */
guint 
sp_id_count_artists (sp_id_t * info)
{
  guint i;
  gchar **artists;

  g_return_val_if_fail(info != NULL, 0);

  artists = sp_id_get_artists (info);
  for (i = 0; artists[i] != NULL; i++);
  return i;
}

/**
 *  sp_id_count_albums:
 *  @info      A previously allocated sp_id_t
 *
 *  Count the size of the albums array from @info. 
 *   
 *  Returns the size of the array. 
 */
guint 
sp_id_count_albums (sp_id_t * info)
{
  guint i;
  gchar **albums;

  g_return_val_if_fail(info != NULL, 0);

  albums = sp_id_get_albums (info);
  for (i = 0; albums[i] != NULL; i++);
  return i;
}

/**
 *  sp_id_count_genres:
 *  @info      A previously allocated sp_id_t
 *
 *  Count the size of the genres array from @info. 
 *   
 *  Returns the size of the array. 
 */
guint 
sp_id_count_genres (sp_id_t * info)
{
  guint i;
  gchar **genres;

  g_return_val_if_fail(info != NULL, 0);

  genres = sp_id_get_genres (info);
  for (i = 0; genres[i] != NULL; i++);
  return i;
}
