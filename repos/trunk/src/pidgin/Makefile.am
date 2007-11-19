## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" 

# lingua-franca plugin for pidgin 

lib_LTLIBRARIES = liblinguafranca.la

liblinguafranca_la_SOURCES = plugin.c \
                        ../ui/xml-ui.c \
                        ../ui/xml-ui.h \
                        ../lib/xml-translate.c \
                        ../lib/xml-translate.h \
                        ../lib/lf.c \
                        ../lib/lf.h \
                        ../ui/support.c \
                        ../ui/support.h \
                        ../ui/interface.c \
                        ../ui/interface.h \
                        ../ui/callbacks.c \
                        ../ui/callbacks.h \
                        ../ui/lingua-franca.c \
                        ../ui/lingua-franca.h

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
 
