## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" 

# the core translation library
liblf_ladir = $(packagedir)
lib_LTLIBRARIES = liblf.la 

liblf_la_SOURCES = lf.c \
		   xml-translate.c \
		   xml-translate.h

liblf_la_HEADERS = lf.h

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
