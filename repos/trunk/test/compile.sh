#gcc -o translate `curl-config --cflags` `curl-config --libs` translate.c 
gcc -o read-config-xml `xml2-config --cflags` read-config-xml.c `xml2-config --libs`  -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lglib-2.0

