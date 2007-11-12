## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" 

# the core translation library
liblf_ladir = $(packagedir)
lib_LTLIBRARIES = liblf.la liblinguafranca.la

liblf_la_SOURCES = lib/lf.c \
		   lib/xml-translate.c \
		   lib/xml-translate.h

liblf_la_HEADERS = lib/lf.h

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
# lingua-franca plugin for pidgin 

liblinguafranca_la_SOURCES = pidgin/plugin.c \
                        core/xml-ui.c \
                        core/xml-ui.h \
                        lib/xml-translate.c \
                        lib/xml-translate.h \
                        lib/lf.c \
                        lib/lf.h \
                        core/support.c \
                        core/support.h \
                        core/interface.c \
                        core/interface.h \
                        core/callbacks.c \
                        core/callbacks.h \
                        core/lingua-franca.c \
                        core/lingua-franca.h

liblinguafranca_la_CFLAGS = \
                      $(GLIB_CFLAGS) \
                      $(AM_CFLAGS) \
                      $(LIBXML_CFLAGS)\
                      $(CURL_CFLAGS)\
                      $(GTK_CFLAGS) \
                      $(PURPLE_CFLAGS)\
		      $(PIDGIN_CFLAGS)

liblinguafranca_la_LDFLAGS = $(GLIB_LIBS) \
                      $(CURL_LIBS) \
                      $(LIBXML_LIBS)\
                      $(GTK_LIBS) \
                      $(INTLLIBS)\
                      $(PURPLE_LIBS)
 
