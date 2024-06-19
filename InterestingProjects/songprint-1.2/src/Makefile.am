## Process this file with automake to produce Makefile.in

SUBDIRS=codecs

INCLUDES = \
@GLIB_CFLAGS@ \
-I$(top_srcdir)/intl \
-I$(top_srcdir)/src \
-I$(top_srcdir)/src/codecs/mpg123 \
-I$(top_srcdir)/src/codecs/wav \
-I$(top_srcdir)/src/codecs/vorbis \
-I$(top_srcdir)/src/codecs/vorbis/vorbis/include

lib_LTLIBRARIES = libsongprint.la libsongprintfile.la

libsongprint_la_SOURCES = \
sp_id_t.c \
sp_identify.c sp_identify.h \
sp_types.h \
sp_signature.c sp_signature.h \
sig_tables.c sig_tables.h

libsongprintfile_la_SOURCES = \
songprintfile.c songprintfile.h \
codecs.c codecs.h

libsongprintfile_la_LIBADD = @CODEC_LIBS@
libsongprintfile_la_LIBFLAGS = -static -version-info 1:1:1

include_HEADERS = songprint.h songprintfile.h

libsongprint_la_LDFLAGS = -version-info 1:1:1

EXTRA_LTLIBRARIES = 
EXTRA_DIST = 

