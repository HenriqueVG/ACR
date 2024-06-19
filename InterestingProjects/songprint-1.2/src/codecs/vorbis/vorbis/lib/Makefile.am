INCLUDES = -I$(top_srcdir)/include 
CFLAGS = @CFLAGS@ -Dvorbis_size32_t='@SIZE32@'

LOCAL_HFILES = \
bitwise.h bookinternal.h envelope.h iir.h lpc.h lsp.h \
masking.h mdct.h misc.h os.h psy.h registry.h scales.h \
sharedbook.h smallft.h spectrum.h window.h 


HFILES =	../include/vorbis/codec.h \
		../include/vorbis/internal.h ../include/vorbis/backends.h \
		../include/vorbis/codebook.h \
        $(LOCAL_HFILES)
LFILES = 	framing.c mdct.c smallft.c block.c envelope.c window.c\
		lsp.c lpc.c analysis.c synthesis.c psy.c info.c bitwise.c\
		time0.c floor0.c res0.c mapping0.c registry.c\
		codebook.c sharedbook.c iir.c
VF_HFILES = ../include/vorbis/vorbisfile.h ../include/vorbis/codec.h \
		../include/vorbis/internal.h ../include/vorbis/codebook.h \
		os.h misc.h 
VF_LFILES = vorbisfile.c

PSY_FILES = mdct.c psy.c lpc.c smallft.c window.c psytune.c floor0.c \
	bitwise.c lsp.c codebook.c sharedbook.c

noinst_LTLIBRARIES = libvorbis.la libvorbisfile.la

libvorbis_la_SOURCES = $(LFILES) $(LOCAL_HFILES)
libvorbis_la_LDFLAGS = 

libvorbisfile_la_SOURCES = $(VF_LFILES)
libvorbisfile_la_LDFLAGS = 

