## Process this file with automake to produce Makefile.in

INCLUDES = @GLIB_CFLAGS@ -I $(top_srcdir)/src

wavsrcs = \
wav.c \
wav.h \
wav_iface.c \
wav_iface.h

if HAVE_WAV

wav_ltlibraries = libwav_iface.la
wav_sources = $(wavsrcs)
wav_ldflags = 

else

wav_ltlibraries = 
wav_sources = 
wav_ldflags = 

endif

noinst_LTLIBRARIES = $(wav_ltlibraries)
libwav_iface_la_SOURCES = $(wav_sources)
libwav_iface_la_LDFLAGS = $(wav_ldflags)

EXTRA_LTLIBRARIES = 
EXTRA_DIST = 

