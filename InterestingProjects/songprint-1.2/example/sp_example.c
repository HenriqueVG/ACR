/*  
 *  sp-example.c - Reference implementation for Songprint
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

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <songprintfile.h>

#include "codecs.h"

static void display_song_info (sp_id_t * info);

int
main (int argc, char *argv[])
{
  gint i, j;

  gint num_matches;
  sp_id_recv_t **matches;
  gchar * url;

  if (argc < 2)
    {
      fprintf (stderr, "USAGE: %s file1 ... filen\n", argv[0]);
      return 1;
    }

  for (i = 1; i < argc; i++)
    {
      if(sp_identify_file (argv[i], &matches, &num_matches, &url) !=
         SP_SUCCESS)
        {
          num_matches = -1;      
        }

      if (num_matches < 0)
        {
          printf ("Error identifying %s.\n", argv[i]);
        }
      else if(num_matches == 0) 
        {
          printf ("No matches found for %s.\n", argv[i]);
          printf("You can submit information about this song at the ");
          printf("following URL:\n");
          printf("%s", url);          
        }
      else 
        {
          printf ("Matches found for %s:", argv[i]);      
        }

      if (num_matches > 1)
        printf (" (in order of likelihood)");

      printf ("\n");

      for (j = 0; j < num_matches; j++)
        display_song_info (matches[j]);

      if (num_matches >= 0)
        {
          sp_delete_id_recv (matches, num_matches);
          g_free(url);
        }

      if (i + 1 < argc)
        printf ("\n\n");
    }
  return 0;
}

/**
 * display_song_info:
 * @info: the song information to be displayed
 *
 * Takes the song information, as returned by the
 * Songprint server, and prints it to stdout. If 
 * you want to write a custom app based on Songprint,
 * you can easily do so just by changing this 
 * one function.
 */
static void
display_song_info (sp_id_t * info)
{
  gchar *title, *moreinfo;
  gchar **artists, **albums, **genres;
  guint32 media_id;

  g_return_if_fail (info != NULL);

  title = sp_id_get_title (info);

  g_return_if_fail (title != NULL);

  moreinfo = sp_id_get_moreinfo (info);
  media_id = sp_id_get_media_id (info);

  artists = sp_id_get_artists (info);
  albums = sp_id_get_albums (info);
  genres = sp_id_get_genres (info);

  printf ("\n");

  printf ("Title: %s\n", title);

  while (*artists)
    printf ("Artist: %s\n", *artists++);

  while (*albums)
    printf ("Album: %s\n", *albums++);

  while (*genres)
    printf ("Genre: %s\n", *genres++);

  if (moreinfo != NULL && moreinfo[0] != 0)
    printf ("Moreinfo: %s\n", moreinfo);

  if (media_id != 0)
    printf ("Media ID: %u\n", media_id);
}


