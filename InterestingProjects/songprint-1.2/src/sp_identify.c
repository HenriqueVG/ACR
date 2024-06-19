/*  sp_identify.c:
 *
 *  This code provides functions to perform a signature lookup on the
 *  eTantrum servers. 
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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#ifdef HAVE_WINSOCK2
#include <winsock2.h>
#elif HAVE_WINSOCK
#include <winsock.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#include "songprint.h"
#include "sp_types.h"
#include "sp_identify.h"

#define SP_SERVER          "feedback.etantrum.com"
#define SP_PORT            40000

/**  _sp_signal:
 *   @signo:   The signal number to change
 *   @func:    Pointer to the new signal handler function
 *
 *   Provides a generic Unix interface to change a signal handler to
 *   a specified function.
 *
 *   Return values: 
 *      none.
 */
static void
_sp_signal (gint signo, void (*func) (int))
{
#ifdef HAVE_SIGACTION
  struct sigaction act, oact;

  act.sa_handler = func;
  sigemptyset (&act.sa_mask);
  act.sa_flags = 0;

  sigaction (signo, &act, &oact);
#elif HAVE_SIGNAL
  signal (signo, func);
#endif
}

/**  _disable_sigpipe:
 * 
 *   Disables the SIGPIPE signal.
 *   
 *   Return values:
 *      none.
 */
static void
_disable_sigpipe ()
{
#if defined (HAVE_SIGACTION) || defined (HAVE_SIGNAL)
  _sp_signal (SIGPIPE, SIG_IGN);
#endif
}

/**
 *   _create_tag:
 *   @label:       Pointer to the buffer to contain the label information
 *   @tagname:     The tag name to put the current tag information under 
 *                 in the label
 *   @tag:         The data to store under the current tag name
 *   @pos:         Current length of label
 *   @need_header: Does this iteration of create_tag need the header
 *   @last_tag:    Is this the last tag in the list (need termination)
 *
 *   Creates and/or appends data to a current label in order to prepare it 
 *   for transmission to the server.
 *
 *   Return values:
 *      the current length of the label
 */
static guint16
_create_tag (gchar ** label, gchar * tagname, gchar * tag,
             guint16 pos, guint16 need_header, gint last_tag)
{
  guint16 tagname_len = 0, tag_len = 0, extra_len = 0;

  if (!tagname || !tag)
    return pos;

  /* figure out how much space we're going to need for tags, delimiters, etc */
  if (need_header)
    tagname_len = strlen (tagname);
  tag_len = strlen (tag);
  if (last_tag)
    extra_len = 1;

  if (need_header)
    {
      /* malloc enough memory for the tagname, the tag, the end padding, 
       * as well as the 3 bytes of delimeters */
      *label =
        (gchar *) g_realloc (*label,
                             pos + tagname_len + tag_len + extra_len + 3);

      if (last_tag)
        {
          sprintf (*label + pos, "%s\03%s\02\01", tagname, tag);
        }
      else
        {
          sprintf (*label + pos, "%s\03%s\02", tagname, tag);
        }

      /* increase the length of the label by the length of the tagname, the tag, 
       * the end padding, as well as the delimiters */
      pos += tagname_len + tag_len + extra_len + 2;
    }
  else
    {
      /* malloc enough memory for the tag, the end padding, and the 2 bytes
       * of delimeters */
      *label = (gchar *) g_realloc (*label, pos + tag_len + extra_len + 2);

      if (last_tag)
        {
          sprintf (*label + pos, "%s\02\01", tag);
        }
      else
        {
          sprintf (*label + pos, "%s\02", tag);
        }

      /* increase the length of the label by the length of the tag, the end padding,
       * as well as the delimiters */
      pos += tag_len + extra_len + 1;
    }

  return pos;
}

/**
 *   _create_label:
 *   @artists:      Pointer to a list of artists to include in the label
 *   @title:        Pointer to the title to include in the label
 *   @genres:       Pointer to a list of genres to include in the label
 *   @albums:       Pointer to a list of albums to include in the label
 *   @filename:     Pointer to the filename to include in the label
 *   @moreinfo:     Pointer to the moreinfo url for this song (if any)
 *   @media_id:     Pointer to the 32 bit media ID for this song (if any)
 *   @label_length: Length of the created label
 *
 *   Creates the actual label with the specified data.  Returns a char
 *   array to the new label, along with the length of the label.
 *
 *   This label needs to be freed when completed with g_free()
 *
 *   Return values:
 *      Pointer to the label data that was constructed.
 */
static gchar *
_create_label (gchar ** artists, gchar * title, gchar ** genres,
               gchar ** albums, gchar * filename, gchar * moreinfo,
               guint32 * media_id, guint16 * label_length)
{
  gchar *label = NULL;
  gchar *tmpbuf;
  guint16 pos = 0;
  guint16 i;

  if (artists && artists[0])
    {
      pos =
        _create_tag (&label, "artists", artists[0], pos, 1, artists[1] == 0);
      for (i = 1; artists[i]; i++)
        {
          pos =
            _create_tag (&label, "artists", artists[i], pos, 0,
                         artists[i + 1] == 0);
        }
    }
  if (genres && genres[0])
    {
      pos = _create_tag (&label, "genres", genres[0], pos, 1, genres[1] == 0);
      for (i = 1; genres[i]; i++)
        {
          pos =
            _create_tag (&label, "genres", genres[i], pos, 0,
                         genres[i + 1] == 0);
        }
    }
  if (albums && albums[0])
    {
      pos = _create_tag (&label, "albums", albums[0], pos, 1, albums[1] == 0);
      for (i = 1; albums[i]; i++)
        {
          pos =
            _create_tag (&label, "albums", albums[i], pos, 0,
                         albums[i + 1] == 0);
        }
    }
  if (title)
    {
      pos = _create_tag (&label, "title", title, pos, 1, 1);
    }
  if (filename)
    {
      pos = _create_tag (&label, "filename", filename, pos, 1, 1);
    }
  if (moreinfo)
    {
      pos = _create_tag (&label, "moreinfo", moreinfo, pos, 1, 1);
    }
  if (media_id)
    {
      tmpbuf = g_strdup_printf ("%d", *media_id);
      pos = _create_tag (&label, "mediaid", tmpbuf, pos, 1, 1);
      g_free (tmpbuf);
    }

  pos++;

  *label_length = pos;

  return (label);
}

/**
 *   _init_socket:
 *   @s:  Socket handle to be initialized
 *
 *   Initializes the socket layer and creates a valid socket handle,
 *   also setting the socket to be non-blocking.
 *
 *   Return values:
 *      SP_SUCCESS: on success
 *      SP_FAILURE: on failure
 */
static sp_rv
_init_socket (guint * s)
{
  gint flags = 0;

#ifdef HAVE_WINSOCK
  WSADATA data;
  guint32 timeout = 5;

  if (WSAStartup (MAKEWORD (1, 1), &data))
    {
      return SP_FAILURE;
    }
#endif

  /* init socket */
  *s = socket (AF_INET, SOCK_STREAM, 0);

  /* set nonblocking */
#ifdef HAVE_WINSOCK
  ioctlsocket (*s, FIONBIO, &timeout);
#else
  flags = fcntl (*s, F_GETFL, 0);
  flags |= O_NONBLOCK;
  if (fcntl (*s, F_SETFL, flags) < 0)
    {
      return SP_FAILURE;
    }
#endif

  return SP_SUCCESS;
}

/**
 *   _close_socket:
 *   @s:  Socket handle to be closed
 *
 *   Closes and deinitializes the socket and socket layer.
 *
 *   Return values:
 *      SP_SUCCESS: on success
 */
static sp_rv
_close_socket (guint s)
{
#ifdef HAVE_WINSOCK
  closesocket (s);
  WSACleanup ();
#else
  close (s);
#endif

  return SP_SUCCESS;
}

/**
 *   _send_request:
 *   @s:       Valid socket handle
 *   @buf:     Data to be sent to the socket
 *   @buf_len: Length of data to be sent to the socket
 * 
 *   Opens the socket to the server, and transmits the data stored in buf
 *
 *   Return values:
 *      SP_SUCCESS: on success
 *      SP_FAILURE: on failure (any socket related error)
 */
static sp_rv
_send_request (guint s, gchar * buf, gint buf_len)
{
  const gint timeout = 10;        /* 10 second timeout on connect */

  struct hostent *he;
  struct sockaddr_in sin;
  fd_set wset;
  struct timeval tval;

  he = gethostbyname (SP_SERVER);

  if (!he)
    {
#ifdef HAVE_WINSOCK
      WSACleanup ();
#endif
      return SP_FAILURE;
    }

  sin.sin_family = AF_INET;
  sin.sin_port = htons (SP_PORT);

  memcpy (&sin.sin_addr, he->h_addr, he->h_length);

  if (connect (s, (struct sockaddr *) &sin, sizeof (sin)) < 0)
    {
#ifdef HAVE_WINSOCK
      if (WSAGetLastError () != WSAEWOULDBLOCK)
        {
          WSACleanup ();
          return SP_FAILURE;
        }
#elif HAVE_ERRNO_H
      if (errno != EINPROGRESS)
        {
          return SP_FAILURE;
        }
#else
      return SP_FAILURE;
#endif
    }

  FD_ZERO (&wset);
  FD_SET (s, &wset);

  tval.tv_sec = timeout;
  tval.tv_usec = 0;

  /* wait for the connect to finish - 10 second timeout */
  if (select (s + 1, NULL, &wset, NULL, &tval) == 0)
    {
      _close_socket (s);
      return SP_FAILURE;
    }

  /* should be connected */
  if (send (s, buf, buf_len, 0) == SOCKET_ERROR)
    {
      _close_socket (s);
      return SP_FAILURE;
    }

  return SP_SUCCESS;
}

/**
 *   _recv_response:
 *   @s:             Valid socket handle
 *   @buf:           Pointer to buffer to receive data into
 *   @bytes_to_read: Number of bytes to read
 *
 *   Listens to the socket and receives the response from the server,
 *   copying the received data into the buffer.  This function will
 *   timeout after 15 seconds if bytes_to_read bytes haven't been
 *   read.
 *
 *   Return values:
 *      number of bytes received by the server (-1 if socket error)
 */

#define RECV_BLOCK_SIZE 1024        /* 1k buffer length */

#define WAIT_LENGTH     1        /* time between read attempts is 1 second */
#define TIMEOUT_LENGTH  15 * WAIT_LENGTH        /* total of 15 seconds before the read fails */

static gint
_recv_response (guint s, gchar * buf, gint bytes_to_read)
{
  gchar tmpbuf[RECV_BLOCK_SIZE];
  gint recv_bytes = -1;
  gint total_bytes = 0;
  fd_set rset, eset;
  struct timeval tval;
  gint ret;
  gint count = 0;

  tval.tv_sec = WAIT_LENGTH;
  tval.tv_usec = 0;

  while ((recv_bytes != 0) && (total_bytes < bytes_to_read))
    {
      FD_ZERO (&rset);
      FD_SET (s, &rset);
      eset = rset;

      /* if select returns 0, that means we've timed out.  increment the counter */
      ret = select (s + 1, &rset, NULL, &eset, &tval);
      if (ret == -1)
        {
          _close_socket (s);
          return -1;
        }
      else if (ret == 0)
        {
          count++;
        }

      /* we have data to read */
      if (FD_ISSET (s, &rset))
        {

          /* receive the data */
          if (bytes_to_read < RECV_BLOCK_SIZE)
            {
              recv_bytes = recv (s, tmpbuf, bytes_to_read, 0);
            }
          else
            {
              recv_bytes = recv (s, tmpbuf, RECV_BLOCK_SIZE, 0);
            }

          if (recv_bytes == SOCKET_ERROR)
            {
              _close_socket (s);
              return -1;
            }

          /* if we have room in our return buffer ... */
          if (total_bytes + recv_bytes <= bytes_to_read)
            {
              /* copy the data to the receive buffer */
              memcpy (buf + total_bytes, tmpbuf, recv_bytes);

              total_bytes += recv_bytes;
            }
        }

      /* if we've waited 15 seconds and still haven't received
         bytes_to_read bytes, bail out */
      if (count >= TIMEOUT_LENGTH)
        {
          return total_bytes;
        }

      /* if an exception has been raised... */
      if (FD_ISSET (s, &eset))
        {
          return total_bytes;
        }
    }

  return total_bytes;
}

/**
 *   _parse_label:
 *   @buf:         Pointer to the buffer containing the label to be parsed
 *   @num_labels:  Number of labels in the structure to be parsed
 *
 *   Parses the given label into a list of sp_id_recv_t structures, creating
 *   space for them as required.  The final sp_id_recv_t pointer will
 *   be set to NULL.
 *
 *   The data returned by this function MUST be freed with sp_id_delete_recv()
 *   when the client is through with the data.
 *
 *   Return values:
 *      Pointer to the sp_id_recv_t buffer that was filled with the data
 *      from the label or NULL if an error occured.
 */

/* state variables used for parsing the label */
#define STATE_LOOP         0        /* inside the loop state */
#define STATE_OUT_MEMBER   1        /* curword is a member of the current label */
#define STATE_OUT_LABEL    2        /* we are through with this label */

static sp_id_recv_t **
_parse_label (gchar * buf, gint num_labels)
{
  sp_id_recv_t **outbuf;
  gchar *ptr = buf;
  gint i, cur = 0, out = STATE_LOOP, word_len = 0, curlabel = 0;
  gchar *curword = NULL;
  guint32 media_id;

  /* reserve memory for the recv array, plus one for the null terminator */
  outbuf =
    (sp_id_recv_t **) g_malloc (sizeof (sp_id_recv_t *) * (num_labels + 1));

  /* memory sanity check */
  if (outbuf == NULL)
    {
      return NULL;
    }

  for (i = 0; i < num_labels; i++)
    {
      outbuf[i] = sp_id_new ();
    }
  outbuf[num_labels] = '\0';

  while (cur < num_labels)
    {
      /* grab the string to the next delimiter */
      word_len = 0;
      out = STATE_LOOP;

      while (out == STATE_LOOP)
        {
          /* if we are not at a delimiter, make the buffer bigger for the string */
          if (*ptr >= '\03')
            {
              /* reallocate curword for room for the new character */
              curword = (gchar *) g_realloc (curword, word_len + 1);

              /* memory sanity check */
              if (curword == NULL)
                {
                  return NULL;
                }

              curword[word_len] = *ptr;
              ptr++;
              word_len++;
            }

          /* if we are at a delimiter now, parse the string we've read */
          if (*ptr >= '\00' && *ptr <= '\03')
            {
              switch (*ptr)
                {
                case '\03':        /* end of a header */
                  if (memcmp (curword, "filename", 8) == 0)
                    {
                      curlabel = LABEL_FILENAME;
                    }
                  else if (memcmp (curword, "title", 5) == 0)
                    {
                      curlabel = LABEL_TITLE;
                    }
                  else if (memcmp (curword, "artists", 7) == 0)
                    {
                      curlabel = LABEL_ARTISTS;
                    }
                  else if (memcmp (curword, "genres", 6) == 0)
                    {
                      curlabel = LABEL_GENRES;
                    }
                  else if (memcmp (curword, "albums", 6) == 0)
                    {
                      curlabel = LABEL_ALBUMS;
                    }
                  else if (memcmp (curword, "moreinfo", 8) == 0)
                    {
                      curlabel = LABEL_MOREINFO;
                    }
                  else if (memcmp (curword, "mediaid", 7) == 0)
                    {
                      curlabel = LABEL_MEDIAID;
                    }
                  else
                    {
                      curlabel = LABEL_UNKNOWN;
                    }

                  g_free (curword);
                  curword = NULL;
                  word_len = 0;

                  break;
                case '\02':        /* end of a member of a header */
                  /* reallocate curword for room for a null terminator */
                  curword = (gchar *) g_realloc (curword, word_len + 1);

                  /* memory sanity check */
                  if (curword == NULL)
                    {
                      return NULL;
                    }

                  curword[word_len] = '\0';
                  out = STATE_OUT_MEMBER;
                  break;
                case '\00':        /* end of this label */
                  cur++;
                  out = STATE_OUT_LABEL;
                  break;
                }
              ptr++;
            }
        }

      /* if this string is a member of the current label, add it to the current recv struct */
      if (out == STATE_OUT_MEMBER)
        {
          switch (curlabel)
            {
            case LABEL_FILENAME:
              sp_id_set_filename (outbuf[cur], curword);
              break;
            case LABEL_TITLE:
              sp_id_set_title (outbuf[cur], curword);
              break;
            case LABEL_ARTISTS:
              sp_id_append_artist (outbuf[cur], curword);
              break;
            case LABEL_GENRES:
              sp_id_append_genre (outbuf[cur], curword);
              break;
            case LABEL_ALBUMS:
              sp_id_append_album (outbuf[cur], curword);
              break;
            case LABEL_MOREINFO:
              sp_id_set_moreinfo (outbuf[cur], curword);
              break;
            case LABEL_MEDIAID:
              media_id = atoi (curword);
              sp_id_set_media_id (outbuf[cur], media_id);
              break;
            }

          g_free (curword);
          curword = NULL;
        }
    }

  return (outbuf);
}

/** 
 *   _nbocopy:
 *   @buf:      Pointer to the memory to copy the data
 *   @data:     The int to be converted and copied
 *   @size:     The size of the int
 *
 *   Converts the data to network byte order (big-endian) and 
 *   copies it into the buffer.
 *
 *   Return values:
 *      none.
 */
static void
_nbocopy (gchar * buf, guint data, guint size)
{
  guint16 tmp16;
  guint32 tmp32;

  if (size == 2)
    {
      tmp16 = g_htons ((guint16) data);
      memcpy (buf, &tmp16, size);
    }
  else if (size == 4)
    {
      tmp32 = g_htonl ((guint32) data);
      memcpy (buf, &tmp32, size);
    }
}

/**
 *   _nbocopy_sp_send_song_id_header_t:
 *   @header:            Pointer to a valid sp_audio_info_t structure with data
 *   @buffer:            Pointer to the pre-allocated buffer to store the data
 *   @buffer_size:       Size of the buffer                                
 *   @num_written:       Number of bytes written to the buffer                  
 *
 *   Will copy a sp_send_song_id_header_t structure into a buffer in network
 *   byte order
 *
 *   Return values:
 *      SP_SUCCESS: on success
 *      SP_FAILURE: on failure
 */
sp_rv
_nbocopy_sp_send_song_id_header_t (const sp_send_song_id_header_t * header,
                                   gchar * buffer, guint buffer_size,
                                   guint * num_written)
{
  if ((header == NULL) || (buffer_size < SP_SEND_SONG_ID_HEADER_T_SIZE)
      || (num_written == NULL))
    {
      return SP_FAILURE;
    }

  _nbocopy (buffer, header->version, 2);
  buffer += 2;

  _nbocopy (buffer, header->sample_rate, 2);
  buffer += 2;

  _nbocopy (buffer, header->num_channels, 2);
  buffer += 2;

  _nbocopy (buffer, header->avg_bitrate, 2);
  buffer += 2;

  _nbocopy (buffer, header->sample_size, 2);
  buffer += 2;

  _nbocopy (buffer, header->codec_type, 2);

  *num_written = SP_SEND_SONG_ID_HEADER_T_SIZE;

  return SP_SUCCESS;
}

/**
 *   _hbocopy_sp_response_packet_t:
 *   @buffer:            Pointer to the valid buffer data to be converted
 *   @buffer_size:       Length of the buffer                          
 *   @header:            Pointer to a valid _hbocopy_sp_response_packet_t
 *                       structure with no data
 *
 *   Will copy a sp_response_packet_t structure from a buffer in network
 *   byte order to host order
 *
 *   Return values:
 *      SP_SUCCESS: on success
 *      SP_FAILURE: on failure
 */
sp_rv
_hbocopy_sp_response_packet_t (const gchar * buffer, guint buffer_size,
                               sp_response_packet_t * header)
{
  if ((header == NULL) || (buffer_size < SP_RESPONSE_PACKET_T_SIZE))
    {
      return SP_FAILURE;
    }

  header->sp_response_type = g_ntohs (*((guint16 *) (&buffer[0])));
  header->sp_packet_length = g_ntohs (*((guint16 *) (&buffer[2])));

  return SP_SUCCESS;
}

/**
 *   _hbocopy_sp_response_song_id_t:
 *   @buffer:            Pointer to the valid buffer data to be converted
 *   @buffer_size:       Length of the buffer                          
 *   @header:            Pointer to a valid _hbocopy_sp_response_song_id_t
 *                       structure with no data
 *
 *   Will copy a sp_response_song_id_t structure from a buffer in network
 *   byte order to host order
 *
 *   Return values:
 *      SP_SUCCESS: on success
 *      SP_FAILURE: on failure
 */
sp_rv
_hbocopy_sp_response_song_id_t (const gchar * buffer, guint buffer_size,
                                sp_response_song_id_t * header)
{
  if ((header == NULL) || (buffer_size < SP_RESPONSE_SONG_ID_T_SIZE))
    {
      return SP_FAILURE;
    }

  header->sp_req_response_type = g_ntohl (*((guint16 *) (&buffer[0])));
  header->sp_label_type = buffer[2];
  header->sp_num_labels = buffer[3];

  return SP_SUCCESS;
}

/**
 *   _build_send_packet:
 *   @buf:                Buffer created that stores the packet to be sent
 *   @packet_length:      Length of buffer that was allocated
 *   @song_audio_format:  Pointer to a valid sp_audio_info_t structure with data
 *   @send_info:          Pointer to any known label data to be transmitted
 *   @signature:          Pointer to the valid signature data
 *
 *   This function builds the send packet to be transmitted to the server.  It
 *   will not fail, and assumes the parameters passed are valid.
 *
 *   The buffer returned from this function needs to be freed with g_free().
 *
 *   Return Values:
 *      SP_SUCCESS: on success
 */
sp_rv
_build_send_packet (gchar ** buf, guint * packet_length,
                    sp_audio_info_t * song_audio_format,
                    sp_id_send_t * send_info, sp_signature_t * signature)
{
  sp_send_packet_t sp;
  sp_send_song_id_header_t ssih;
  sp_send_song_id_t ssi;

  gchar *sp_label_bytes;
  guint16 label_length;
  guint ofs = 0;
  guint tmp, i;

  /* eTantrum packet header: "etc".  "e" == alphabet letter 5, and so on. */
  sp.sp_header[0] = '\x05';
  sp.sp_header[1] = '\x14';
  sp.sp_header[2] = '\x03';

  sp.sp_request_type = RT_SONG_IDENTIFICATION;

    /** build the packet **/

  /* the header */
  ssih.version = SP_HEADER_VERSION;
  ssih.sample_rate = song_audio_format->sample_rate;
  ssih.num_channels = song_audio_format->num_channels;
  ssih.avg_bitrate = song_audio_format->avg_bitrate;
  ssih.codec_type = song_audio_format->codec_type;

  ssi.sp_header_type = SP_HEADER_STANDARD;
  ssi.sp_label_type = SP_LABEL_STANDARD;

  /* create the label structure */
  sp_label_bytes = _create_label (send_info->artists, send_info->title,
                                  send_info->genres, send_info->albums,
                                  send_info->filename, send_info->moreinfo,
                                  &send_info->media_id, &label_length);

  /* calc size of packet */
  sp.sp_packet_length =
    (SP_SEND_SONG_ID_T_SIZE + SP_SEND_SONG_ID_HEADER_T_SIZE +
     SP_SIGNATURE_T_SIZE + label_length);

  /* allocate memory for the buffer to be sent */
  *buf =
    (gchar *) g_malloc (sizeof (gchar) *
                        (sp.sp_packet_length + SP_SEND_PACKET_T_SIZE));

  /* dump the packet data into the buffer */
  memcpy (*buf, &sp.sp_header, 3);        /* the sp_header is 3 bytes long */
  ofs += 3;
  _nbocopy (*buf + ofs, sp.sp_request_type, 2);        /* the sp_request_type is 2 bytes long */
  ofs += 2;
  _nbocopy (*buf + ofs, sp.sp_packet_length, 2);        /* the sp_packet_length is 2 bytes long */
  ofs += 2;
  memcpy (*buf + ofs, &ssi.sp_header_type, 1);        /* the sp_header_type is 1 byte long */
  ofs += 1;
  memcpy (*buf + ofs, &ssi.sp_label_type, 1);        /* the sp_label_type is 1 byte long */
  ofs += 1;

  _nbocopy_sp_send_song_id_header_t (&ssih, *buf + ofs,
                                     SP_SEND_SONG_ID_HEADER_T_SIZE, &tmp);
  ofs += tmp;

  /* convert the signature to network byte order */
  for (i = 0; i < SP_SIG_LENGTH; i++)
    {
      _nbocopy (*buf + ofs, signature->data[i], 2);        /* the signature data is 2 bytes long (each) */
      ofs += 2;
    }

  /* if we have a label to append to the end of the packet, do it */
  if (sp_label_bytes != NULL)
    {
      memcpy (*buf + ofs, sp_label_bytes, label_length);
      g_free (sp_label_bytes);
    }

  *packet_length = sp.sp_packet_length + SP_SEND_PACKET_T_SIZE;

  return SP_SUCCESS;
}

/**
 *   sp_id_signature:
 *   @song_audio_format: Pointer to a valid sp_audio_info_t structure with data
 *   @signature:         Pointer to the valid signature data
 *   @send_info:         Pointer to any known label data to be transmitted
 *   @recv_info:         Pointer to the label data received back from the server
 *   @num_matches:       Number of matches received from the server
 *   @error:             Pointer to a valid sp_error_t structure for error data
 *
 *   Forms the packet to be transmitted to the server for the signature
 *   lookup.  Transmits the packet, and receives the information back
 *   from the server, parsing it and storing it into sp_id_recv_t buffers
 *   to be handled by the calling procedure.
 *
 *   sp_id_recv_t needs to be deallocated by the client when it is through
 *   with the data with a call to sp_id_delete_recv().
 *   
 *   If error is NULL, no extended error information will be passed
 *   back (if an error occurs server-side).
 *
 *   error needs to be deallocated by the client if an extended error is passed
 *   by calling sp_id_delete_error()
 *
 *   Return values: 
 *      SP_SUCCESS: on success
 *      SP_FAILURE: on failure (any socket related issues)
 */
sp_rv
sp_id_signature (sp_audio_info_t * song_audio_format,
                 sp_signature_t * signature,
                 sp_id_send_t * send_info,
                 sp_id_recv_t *** recv_info,
                 gint * num_matches, sp_error_t * error)
{
  sp_response_packet_t srp;
  sp_response_song_id_t srsi;

  guint packet_length, s;

  gchar *buf;

  gchar recv_buf[4];

  if (!song_audio_format || !signature || !send_info || !recv_info)
    {
      return SP_FAILURE;
    }

  /* set up to a default value in case of an error */
  if (num_matches)
    {
      *num_matches = -1;
    }

  /* set the recv_info array to be null in case of an error before it is allocated */
  *recv_info = NULL;

  _disable_sigpipe ();

  /* build the send packet from the data we have had passed in */
  if (_build_send_packet (&buf, &packet_length, song_audio_format, send_info,
                          signature) != SP_SUCCESS)
    {
      return SP_FAILURE;
    }

  /* initialize the socket */
  if (_init_socket (&s) != SP_SUCCESS)
    {
      return SP_FAILURE;
    }

  /* send the request */
  if (_send_request (s, buf, packet_length) != SP_SUCCESS)
    {
      return SP_FAILURE;
    }

  /* free the send packet */
  g_free (buf);
  buf = NULL;

  /* read the header (4 bytes) */
  if (_recv_response (s, &recv_buf[0], 4) != 4)
    {
      return SP_FAILURE;
    }

  /* we now know the length of the packet, read the rest */
  _hbocopy_sp_response_packet_t (recv_buf, SP_RESPONSE_PACKET_T_SIZE, &srp);

  /* if we have more of the packet to read, do it now */
  if (srp.sp_packet_length > 0)
    {
      /* create the buffer for the response */
      buf = (gchar *) g_malloc (sizeof (gchar) * srp.sp_packet_length);

      if (_recv_response (s, buf, srp.sp_packet_length) !=
          srp.sp_packet_length)
        {
          return SP_FAILURE;
        }
    }

  if (_close_socket (s) != SP_SUCCESS)
    {
      return SP_FAILURE;
    }

  switch (srp.sp_response_type)
    {
    case SP_RESPONSE_ERROR:
      /* if the client wants extended errors... */
      if (error != NULL)
        {
          error->error_number = g_ntohs ((guint16) * buf);
          error->error = (gchar *) g_malloc (srp.sp_packet_length - 1);

          /* memory sanity check */
          if (error->error != NULL)
            {
              strncpy (error->error, (const char *) (buf + 2),
                       srp.sp_packet_length - 2);
            }
        }

      return SP_FAILURE_EXTENDED;
    case SP_RESPONSE_NOTFOUND:
      /* id was not found, set receive buffers to NULL */
      if (num_matches)
        {
          *num_matches = 0;
        }
      *recv_info = NULL;
      if (buf)
        {
          g_free (buf);
        }

      break;
    case SP_RESPONSE_FOUND:
      /* at lease one id was found, parse the packet buffer */
      _hbocopy_sp_response_song_id_t (buf, SP_RESPONSE_SONG_ID_T_SIZE, &srsi);

      switch (srsi.sp_label_type)
        {
        case SP_LABEL_STANDARD:
          /* parse the label information (skipping the song_id_t struct) */
          *recv_info =
            _parse_label (buf + SP_RESPONSE_SONG_ID_T_SIZE,
                          srsi.sp_num_labels);

          if (num_matches)
            {
              *num_matches = srsi.sp_num_labels;
            }
          if (buf)
            {
              g_free (buf);
            }

          break;
        default:
          /* unknown label type type! */
          if (buf)
            {
              g_free (buf);
            }
          return SP_FAILURE;
        }

      break;
    case SP_RESPONSE_POSSIBLE:
      *recv_info = NULL;
      if (num_matches)
        {
          *num_matches = 0;
        }

      break;
    default:
      /* unknown response type! */
      if (buf)
        {
          g_free (buf);
        }
      return SP_FAILURE;
    }

  return SP_SUCCESS;
}

/**
 *   sp_delete_id_recv:
 *   @recv_info:   Pointer to an array of sp_id_recv_t structures
 *   @num_labels:  Number of structures to expect
 *
 *   Frees an array of sp_id_recv_t buffers created by a call to
 *   sp_id_signature, destroying all the data stored therein.
 *
 *   Return values:
 *      SP_SUCCESS: on success
 *      SP_FAILURE: on failure
 */
sp_rv sp_delete_id_recv (sp_id_recv_t ** recv_info, gint num_labels)
{
  gint i;

  if (!recv_info)
    {
      return SP_FAILURE;
    }

  for (i = 0; i < num_labels; i++)
    {
      sp_id_destroy (recv_info[i]);
    }

  g_free (recv_info);

  return SP_SUCCESS;
}

/**
 *   sp_delete_error:  
 *   @error: Pointer to the error structure to free
 *
 *   Frees the character error string created by a call to sp_id_signature.
 *
 *   Return values:
 *      SP_SUCCESS: on success
 *      SP_FAILURE: on failure
 */
sp_rv sp_delete_error (sp_error_t * error)
{
  if (error == NULL)
    {
      return SP_FAILURE;
    }

  if (error->error != NULL)
    {
      g_free (error->error);
    }

  return SP_SUCCESS;
}
