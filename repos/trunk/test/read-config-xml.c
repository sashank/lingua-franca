#include <libxml/xmlreader.h>
#include <glib.h>

struct LangPairStruct {
  GString *from ;
  GString *to;
  GString *lp;
};

GList *langpair_list =NULL;

void *myrealloc(void *ptr, size_t size)
{
  /* There might be a realloc() out there that doesn't like reallocing
     NULL pointers, so we take care of it here */
  if(ptr)
    return realloc(ptr, size);
  else
    return malloc(size);
}


char *get_lang_pref(char *from ,char *to)
{
  struct LangPairStruct *lpair;
  int i = 0;
  guint size = g_list_length(langpair_list);

  printf("Inside get lang pref %d \n",size);
  for ( i =0 ; i < size ; i++)
   {
        lpair = g_list_nth_data(langpair_list,i);
        printf("Name = %s , Value = %s \n",lpair->from->str,lpair->to->str);
        if ((strcmp(lpair->from,from) == 0) && (strcmp(lpair->to,to) == 0))
          return lpair->lp->str ;
   }

  return "NA";
}


void processNode(xmlTextReaderPtr reader) {
  xmlChar *name, *value;

    name = xmlTextReaderName(reader);
    if (name == NULL)
        name = xmlStrdup(BAD_CAST "--");
    value = xmlTextReaderValue(reader);

  /*  printf("%d %d %s %d",
            xmlTextReaderDepth(reader),
            xmlTextReaderNodeType(reader),
            name,
            xmlTextReaderIsEmptyElement(reader));  */

  /* attributes */
    if (strcmp(name,"lang_pair")==0)
    {
	struct LangPairStruct *langpair ;
	langpair->from = NULL;
	langpair->to = NULL;
	langpair->lp = NULL;

	char *from = xmlTextReaderGetAttribute(reader,(xmlChar *)"from");
	char *to = xmlTextReaderGetAttribute(reader,(xmlChar *)"to");
	char *lp = xmlTextReaderGetAttribute(reader,(xmlChar *)"lp");
	
	printf("Attribute Count %d \n", xmlTextReaderAttributeCount(reader));
	printf("From Name %s ,length %d\n",from,strlen(from));
	printf("To Value %s ,length %d\n",to,strlen(to));
	printf("LP Value %s ,length %d\n",lp,strlen(lp)); 

	langpair->from =(char *)myrealloc(from ,(size_t)strlen(xmlTextReaderGetAttribute(reader,(xmlChar *)"from")));
        langpair->to =(char *)myrealloc(to, (size_t)strlen(xmlTextReaderGetAttribute(reader,(xmlChar *)"to")));
        langpair->lp =(char *)myrealloc(lp,(size_t)strlen(xmlTextReaderGetAttribute(reader,(xmlChar *)"lp")));

	strcpy(langpair->from,xmlTextReaderGetAttribute(reader,(xmlChar *)"from") );
	strcpy(langpair->to,xmlTextReaderGetAttribute(reader,(xmlChar *)"to"));
	strcpy(langpair->lp,xmlTextReaderGetAttribute(reader,(xmlChar *)"lp"));

	langpair_list = g_list_append(langpair_list,langpair);
    }
	
    xmlFree(name);
    xmlFree(value);
}

int main(int argc, char **argv) {
    const char *filename = argv[1];
    xmlTextReaderPtr reader;
    int ret;

    reader = xmlNewTextReaderFilename(filename);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            processNode(reader);
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            printf("%s : failed to parse\n", filename);
        }
    } else {
        printf("Unable to open %s\n", filename);
    }

   /* printf("LP = %s",get_lang_pref(argv[2],argv[3]));*/
}
