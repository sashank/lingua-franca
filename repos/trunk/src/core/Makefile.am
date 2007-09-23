## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = lingua-franca

lingua-franca_SOURCES = \
	main.c \
	xml-ui.c xml-ui.h \
	xml-translate.c xml-translate.h \
	translate.c translate.h \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h 
	lingua-franca.c lingua-franca.h

lingua-franca_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

