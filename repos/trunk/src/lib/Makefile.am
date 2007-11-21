## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" 

#pkgconfig INFO
pkgconfigdir = $(libdir)/pkgconfig
dist_pkgconfig_DATA = lf.pc

# the core translation library
liblf_ladir = $(packagedir)

lib_LTLIBRARIES = liblf.la 

dist_pkgdata_DATA = ../config/google.xml ../config/altavista.xml

dist_liblf_la_SOURCES = lf.c \
		   xml-translate.c \
		   xml-translate.h

dist_liblf_la_HEADERS = lf.h

liblf_la_CFLAGS = \
		      $(GLIB_CFLAGS) \
                      $(AM_CFLAGS) \
                      $(LIBXML_CFLAGS)\
                      $(CURL_CFLAGS)\
		      $(GTK_CFLAGS)

liblf_la_LDFLAGS = $(GLIB_LIBS) \
                      $(CURL_LIBS) \
                      $(LIBXML_LIBS)\
		      $(GTK_LIBS) \
		      $(INTLLIBS)
