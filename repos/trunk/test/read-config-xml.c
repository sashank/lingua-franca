#include <libxml/xmlreader.h>
#include <glib.h>
#include <string.h>


typedef struct _LangPairStruct {
  char *from;
  char *to;
  char *lp;
}LangPair;

GList *langpair_list =NULL;

typedef struct _PostOptionStruct {
  char *name;
  char *value;
}PostOption;

GList *postoptions_list;

void add_lang_pair(char *from,char *to,char *lp)
{
  LangPair *langpair;
  g_return_if_fail(from != NULL);
  g_return_if_fail(to != NULL);
  g_return_if_fail(lp != NULL);

  langpair = g_new0(LangPair,1);
  langpair->from = g_strdup(from);
  langpair->to = g_strdup(to);
  langpair->lp = g_strdup(lp);

  langpair_list = g_list_append(langpair_list,langpair);
}


void add_post_option(char *name ,char *value)
{
 
  PostOption *po;
  g_return_if_fail(name !=NULL);
  g_return_if_fail(value !=NULL);

  po = g_new0(PostOption , 1);
  po->name = g_strdup(name);
  po->value = g_strdup(value);
 
 /*po = strdup(g_strjoin("=",name,value));*/
 postoptions_list = g_list_append(postoptions_list,po);

}

char *get_lang_pref(char *from ,char *to)
{
  LangPair *lpair;
  int i = 0;
  guint size = g_list_length(langpair_list);

  printf("Inside get lang pref %d \n",size);
  for ( i =0 ; i < size ; i++)
   {
        lpair = g_list_nth_data(langpair_list,i);
      /*  printf("Name = %s , Value = %s \n",lpair->from,lpair->to); */
        if ((strcmp(lpair->from,from) == 0) && (strcmp(lpair->to,to) == 0))
          return lpair->lp ;
   }

  return "NA";
}

GString *get_post_option(char *mesg,char *from,char *to)
{
    int i;
    char *lp ;
    lp = get_lang_pref(from,to);
    PostOption *po;
    guint size = g_list_length(postoptions_list);
    printf("Inside get post options %d \n",size);
    GString *post_this = g_string_new(NULL);
    for ( i =0 ; i < size ; i++)
   {
       char *pair;
        po = g_list_nth_data(postoptions_list,i);
        if (strcmp(po->value,"STRING_TO_BE_TRANSLATED")==0)
         po->value = strdup(mesg);
        if (strcmp(po->value,"LANG_PAIR")==0)
         po->value = strdup(lp); 
         post_this = g_string_append(post_this,po->name);
         post_this = g_string_append(post_this,"=");
         post_this = g_string_append(post_this,po->value);
         post_this = g_string_append(post_this,"&");
   }
    
   return post_this;
}

void processNode(xmlTextReaderPtr reader) {
  xmlChar *name, *value;

    name = xmlTextReaderName(reader);
    if (name == NULL)
        name = xmlStrdup(BAD_CAST "--");
    value = xmlTextReaderValue(reader);

     if (strcmp(name,"post_option")==0)
    {
        char *post_name = xmlTextReaderGetAttribute(reader,(xmlChar *)"name");
        char *post_value = xmlTextReaderGetAttribute(reader,(xmlChar *)"value");
        add_post_option(post_name,post_value);
    }



  /*  printf("%d %d %s %d",
            xmlTextReaderDepth(reader),
            xmlTextReaderNodeType(reader),
            name,
            xmlTextReaderIsEmptyElement(reader));  
 */

    if (strcmp(name,"lang_pair")==0)
    {
	char *from = xmlTextReaderGetAttribute(reader,(xmlChar *)"from");
	char *to = xmlTextReaderGetAttribute(reader,(xmlChar *)"to");
	char *lp = xmlTextReaderGetAttribute(reader,(xmlChar *)"lp");
        add_lang_pair(from,to,lp);
    }
/*
    if (strcmp(name,"prefix")==0)
    {   
        if( xmlTextReaderNodeType(reader) == 1 ) 
                xmlTextReaderRead(reader);

        if(xmlTextReaderNodeType(reader) == 3)
        {
                value = xmlTextReaderValue(reader);
                printf("Response Prefix is %s \n",value);
                xmlTextReaderRead(reader);
        }

        if( xmlTextReaderNodeType(reader) == 2 ) 
                xmlTextReaderRead(reader);
    }   
    if (strcmp(name,"suffix")==0)
    {   
        if( xmlTextReaderNodeType(reader) == 1 ) 
                xmlTextReaderRead(reader);

        if(xmlTextReaderNodeType(reader) == 3)
        {
                value = xmlTextReaderValue(reader);
                printf("Response Suffix is %s \n",value);
                xmlTextReaderRead(reader);
        }

        if( xmlTextReaderNodeType(reader) == 2 )
                xmlTextReaderRead(reader);
    }
*/
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

/*    printf("LP = %s",get_lang_pref(argv[2],argv[3]));*/
      printf("Post String  %s" , get_post_option(argv[2],argv[3],argv[4])->str);
}
