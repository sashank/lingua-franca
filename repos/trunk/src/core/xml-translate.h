
/* XML Directives */
#include <libxml/xmlreader.h>

/* Glib Directives */
#include <glib.h>

struct LangPairStruct {
  char *from;
  char *to;
  char *lp;
};

GList *langpairs_list;

struct PostOptionStruct {
  char *name;
  char *value;
};

GList *postoptions_list=NULL;

/*Init XML Processing of Translate XML File */
void xml_translate_init(char *filename);

/* Is Translation available from lang1 to lang2 */
char *get_lang_pair(char *lang1 ,char *lang2);

char *get_host_url();

/* get the post string */
char *get_post_string(char *mesg,char *from,char *to);

/* get the translation server name */
char *get_server_name();
