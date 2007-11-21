## Process this file with automake to produce Makefile.in
SUBDIRS = src/lib src/pidgin test

EXTRA_DIST = \
	autogen.sh 
	src/config/google.xml 
	src/config/altavista.xml 
	src/lib/lf.pc
	src/lib/lf-uninstalled.pc

DIST_SUBDIRS = test src/lib src/pidgin

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/pixmaps; \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(pkgdatadir)/pixmaps; \
	    fi \
	  done \
	fi

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi
	rm -rf `find $(top_distdir) -name .svn`

