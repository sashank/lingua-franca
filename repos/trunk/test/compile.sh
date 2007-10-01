# Can some one please write a Makefile for this 
#gcc -o read-config-xml `xml2-config --cflags` ../src/core/xml-translate.c ../src/core/xml-translate.h read-config-xml.c `xml2-config --libs`  -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lglib-2.0
gcc -g -o translate-test  `curl-config --cflags` `curl-config --libs` `xml2-config --cflags` ../src/core/xml-translate.c ../src/core/xml-translate.h ../src/core/translate.c ../src/core/translate.h translate-test.c `xml2-config --libs`  -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lglib-2.0

