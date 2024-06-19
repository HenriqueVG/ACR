## Process this file with automake to produce Makefile.in

INCLUDES = \
@GLIB_CFLAGS@ \
-I$(top_srcdir)/intl \
-I$(top_srcdir)/src 

noinst_PROGRAMS = sp_example

sp_example_SOURCES = \
sp_example.c 

## We can't do -lsongprintfile because they haven't had a chance to do 'make install' yet.  
## You should use -lsongprintfile instead of statically linking it.
sp_example_LDADD = $(top_builddir)/src/libsongprintfile.la $(top_builddir)/src/libsongprint.la @CODEC_LIBS@ @GLIB_LIBS@
sp_example_LDFLAGS = -static

EXTRA_DIST = 
