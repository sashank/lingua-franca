## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" 

# test the core translation library
bin_PROGRAMS = translate_test linguafranca_test

translate_test_SOURCES = translate-test.c \
			../src/lib/xml-translate.c \
			../src/lib/xml-translate.h \
			../src/lib/lf.c \
			../src/lib/lf.h 
translate_test_CFLAGS = \
		      $(GLIB_CFLAGS) \
                      $(AM_CFLAGS) \
                      $(LIBXML_CFLAGS)\
                      $(CURL_CFLAGS)\
		      $(GTK_CFLAGS)

translate_test_LDFLAGS = $(GLIB_LIBS) \
                      $(CURL_LIBS) \
                      $(LIBXML_LIBS)\
		      $(GTK_LIBS) \
		      $(INTLLIBS)
# lingua-franca, test purposes

linguafranca_test_SOURCES = linguafranca_test.c \
                        ../src/core/xml-ui.c \
                        ../src/core/xml-ui.h \
                        ../src/lib/xml-translate.c \
                        ../src/lib/xml-translate.h \
                        ../src/lib/lf.c \
                        ../src/lib/lf.h \
                        ../src/core/support.c \
                        ../src/core/support.h \
                        ../src/core/interface.c \
                        ../src/core/interface.h \
                        ../src/core/callbacks.c \
                        ../src/core/callbacks.h \
                        ../src/core/lingua-franca.c \
                        ../src/core/lingua-franca.h

linguafranca_test_CFLAGS = \
                      $(GLIB_CFLAGS) \
                      $(AM_CFLAGS) \
                      $(LIBXML_CFLAGS)\
                      $(CURL_CFLAGS)\
                      $(GTK_CFLAGS)

linguafranca_test_LDFLAGS = $(GLIB_LIBS) \
                      $(CURL_LIBS) \
                      $(LIBXML_LIBS)\
                      $(GTK_LIBS) \
                      $(INTLLIBS)
 
