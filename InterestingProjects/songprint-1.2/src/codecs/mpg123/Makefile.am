## Process this file with automake to produce Makefile.in

INCLUDES = @GLIB_CFLAGS@ -I $(top_srcdir)/src 

mpg123srcs = \
common.c common.h \
dct64_i386.c \
decode_i386.c \
interface.c interface.h \
layer1.c layer1.h \
layer2.c layer2.h \
layer3.c \
tabinit.c \
l2tables.h \
huffman.h \
mpg123.h \
mpglib.h \
mp3.c mp3.h \
mpg123_iface.c mpg123_iface.h 

if HAVE_MPG123

mpg123_ltlibraries = libmpg123_iface.la
mpg123_sources = $(mpg123srcs)
mpg123_ldflags =

else

mpg123_ltlibraries =
mpg123_sources =
mpg123_ldflags =

endif 

EXTRA_DIST = $(mpg123srcs)
noinst_LTLIBRARIES = $(mpg123_ltlibraries)
libmpg123_iface_la_SOURCES = $(mpg123_sources)
libmpg123_iface_la_LDFLAGS = $(mpg123_ldflags)
