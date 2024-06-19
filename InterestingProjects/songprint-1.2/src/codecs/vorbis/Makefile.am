## Process this file with automake to produce Makefile.in

INCLUDES = @GLIB_CFLAGS@ -I$(top_srcdir)/src -I$(top_srcdir)/src/codecs/vorbis/vorbis/include

vorbissrcs = vorbis_iface.c \
vorbis_iface.h

if HAVE_OGGVORBIS

subdirs=vorbis

vorbis_ltlibraries = libvorbis_iface.la 
vorbis_sources = $(vorbissrcs)
vorbis_libadd = \
$(top_builddir)/src/codecs/vorbis/vorbis/lib/libvorbisfile.la \
$(top_builddir)/src/codecs/vorbis/vorbis/lib/libvorbis.la 

vorbis_ldflags = 

else

subdirs=

vorbis_ltlibraries = 
vorbis_sources = 
vorbis_libadd = 
vorbis_ldflags = 

endif

noinst_LTLIBRARIES = $(vorbis_ltlibraries)
libvorbis_iface_la_SOURCES = $(vorbis_sources)
libvorbis_iface_la_LIBADD = $(vorbis_libadd)
libvorbis_iface_la_LDFLAGS = $(vorbis_ldflags)
SUBDIRS = $(subdirs)

EXTRA_DIST = $(vorbissrcs)
EXTRA_LTLIBRARIES = 

