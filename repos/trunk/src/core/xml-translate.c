/**
 * @file xml-translate.c
 * @ingroup core
 *
 * lingua-franca
 *
 * lingua-franca is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include <string.h>
#include "xml-translate.h"

typedef struct _LangPairStruct {
  char *from;
  char *to;
  char *lp;
}LangPair;

GList *langpairs_list;
GList *langs;

typedef struct _PostOptionStruct {
  char *name;
  char *value;
}PostOption;

char *resp_prefix ;
char *resp_suffix ;
char *host_url;
char *post_string;
char *server_name;

GList *postoptions_list=NULL;


char *get_host_url()
{
  return host_url;
}

void add_avail_langs( char *lang)
{
  char *l ;
  int i ;
  gboolean exists = FALSE;
  int count  = g_list_length(langs);
  for(i = 0 ;i < count ; i++)
  {
    l = g_list_nth_data(langs,i); 
     if(strcmp(lang,l)== 0)
         exists = TRUE;
  }
  if( exists == FALSE || count == 0)
    langs = g_list_append(langs,lang);
}
  
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

  langpairs_list = g_list_append(langpairs_list,langpair);

  /* Add into Avail Langs list also*/
   add_avail_langs(from);
   add_avail_langs(to);
}

void add_post_option(char *name ,char *value)
{
  PostOption *po; 
  g_return_if_fail(name !=NULL);
  g_return_if_fail(value !=NULL);

  po = g_new0(PostOption , 1);
  po->name = g_strdup(name);
  po->value = g_strdup(value);

 postoptions_list = g_list_append(postoptions_list,po); 
 
}
char *get_lang_pref(char *from ,char *to)
{
  LangPair *lpair;
  int i = 0;
  guint size = g_list_length(langpairs_list);

 /* printf("Inside get lang pref %d \n",size);*/
  for ( i =0 ; i < size ; i++)
   {
        lpair = g_list_nth_data(langpairs_list,i);
/*        printf("Name = %s , Value = %s \n",lpair->from,lpair->to); */
        if ((strcmp(lpair->from,from) == 0) && (strcmp(lpair->to,to) == 0))
          return lpair->lp ;
   }

  return "NA";
}

GString *get_post_string(char *mesg,char *from,char *to)
{
    int i;
    char *lp ;
    lp = get_lang_pref(from,to);
    PostOption *po;
    guint size = g_list_length(postoptions_list);
/*    printf("Inside get post string %d \n",size);
    printf("get post string called with  %s,%s,%s \n",mesg,from,to);  */
    GString *post_this = g_string_new(NULL) ;
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
         if (i < (size-1))
         post_this = g_string_append(post_this,"&");
   }

/*    printf("Exiting get post options %d \n",size); 
    printf("Exiting get post options %s \n",post_this->str);  */
   return post_this;
}
char *get_server_name()
{
 return server_name;
}
void processNode(xmlTextReaderPtr reader)
{
  xmlChar *name, *value;

    name = xmlTextReaderName(reader);
    if (name == NULL)
        name = xmlStrdup(BAD_CAST "--");

    if(strcmp(name,"server_name") == 0 )
     {
        if( xmlTextReaderNodeType(reader) == 1 )
                xmlTextReaderRead(reader);

        if(xmlTextReaderNodeType(reader) == 3)
        {
                value = xmlTextReaderValue(reader);
              /*  printf("Server Name is %s \n",value); */
		server_name = g_strdup(value);
                xmlTextReaderRead(reader);
        }

        if( xmlTextReaderNodeType(reader) == 2 )
                xmlTextReaderRead(reader);
     }

    if(strcmp(name,"host_url") == 0 )
     {
        if( xmlTextReaderNodeType(reader) == 1 )
                xmlTextReaderRead(reader);

        if(xmlTextReaderNodeType(reader) == 3)
        {
                value = xmlTextReaderValue(reader);
             /*   printf("host Name is %s \n",value); */
		host_url = g_strdup(value);
                xmlTextReaderRead(reader);
        }

        if( xmlTextReaderNodeType(reader) == 2 )
                xmlTextReaderRead(reader);
     }

    if (strcmp(name,"post_option")==0)
    {
	char *post_name = xmlTextReaderGetAttribute(reader,(xmlChar *)"name");
        char *post_value = xmlTextReaderGetAttribute(reader,(xmlChar *)"value");
        add_post_option(post_name,post_value);
    }

    if (strcmp(name,"lang_pair")==0)
    {
	char *from = xmlTextReaderGetAttribute(reader,(xmlChar *)"from");
        char *to = xmlTextReaderGetAttribute(reader,(xmlChar *)"to");
        char *lp = xmlTextReaderGetAttribute(reader,(xmlChar *)"lp");
	add_lang_pair(from,to,lp);
    }
    if (strcmp(name,"prefix")==0)
    {
        if( xmlTextReaderNodeType(reader) == 1 )
                xmlTextReaderRead(reader);

        if(xmlTextReaderNodeType(reader) == 3)
        {
                value = xmlTextReaderValue(reader);
              /*  printf("Response Prefix is %s \n",value); */
		resp_prefix = g_strdup(value);
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
              /*  printf("Response Suffix is %s \n",value); */
		resp_suffix = g_strdup(value);
                xmlTextReaderRead(reader);
        }

        if( xmlTextReaderNodeType(reader) == 2 )
                xmlTextReaderRead(reader);
    }

}

char *get_response_prefix()
{
  return resp_prefix;
}
char *get_response_suffix()
{
  return resp_suffix;
}
GList *get_avail_languages()
{
 return langs;
}
void xml_translate_init(char *serverfilename)
{
    xmlTextReaderPtr xml_reader;
    int ret;

    xml_reader = xmlNewTextReaderFilename(serverfilename);
    if (xml_reader != NULL) {
        ret = xmlTextReaderRead(xml_reader);
        while (ret == 1) {
            processNode(xml_reader);
            ret = xmlTextReaderRead(xml_reader);
        }
        xmlFreeTextReader(xml_reader);
        if (ret != 0) {
            printf("%s : failed to parse\n", serverfilename);
        }
    } else {
        printf("Unable to open %s\n", serverfilename);
    }

}

void xml_translate_unload()
{
  g_list_free(langs); 
  g_list_free(postoptions_list); 
  g_list_free(langpairs_list); 
  g_free(host_url);
  g_free(server_name);
  g_free(post_string);
  g_free(resp_suffix);
  g_free(resp_prefix);
}
