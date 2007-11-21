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

GList *langpairs_list= NULL;
GList *langs = NULL;

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
 /* printf("xml-translate: add_avail_langs entered \n"); */
  char *l ;
  int i ;
  gboolean exists = FALSE;
  int count  = g_list_length(langs);
 /* printf("xml-translate: count is %d \n ",count); 
  printf("xml-translate: lang  is %s  \n",lang); */
  for(i = 0 ;i < count ; i++)
  {
    l = g_list_nth_data(langs,i); 
     if(strcmp(lang,l)== 0)
         exists = TRUE;
  }
  if( exists == FALSE || count == 0)
    langs = g_list_append(langs,lang);
 /* printf("xml-translate: add_avail_langs exited \n"); */
}
  
void add_lang_pair(char *from,char *to,char *lp)
{
/*  printf("xml-translate: add_lang_pairs entered \n");
  printf("xml-translate:  from %s " ,from);
  printf("to %s " ,to);
   printf("lp %s \n" ,lp); */
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
  /* printf("xml-translate: add_lang_pairs exited"); */
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
char *get_lp(char *from ,char *to)
{
  LangPair *lpair;
  int i = 0;
  guint size = g_list_length(langpairs_list);
  char *lp = "NA";

 /* printf("Inside get lang pref %d \n",size);*/
  for ( i =0 ; i < size ; i++)
   {
        lpair = g_list_nth_data(langpairs_list,i);
        if ((strcmp(lpair->from,from) == 0) && (strcmp(lpair->to,to) == 0))
          lp = lpair->lp ;
   }

  return lp;
}

GString *get_post_string(char *mesg,char *from,char *to)
{
    int i;
    char *lp ;
    lp = get_lp(from,to);
    PostOption *po ;
    guint size = g_list_length(postoptions_list);
/*    printf("Inside get post string %d \n",size);*/
    printf("get post string called with  %s,%s,%s \n",mesg,from,to); 
    GString *post_this = g_string_new(NULL) ;
    for ( i =0 ; i < size ; i++)
   {
       char *pair;
        po = g_list_nth_data(postoptions_list,i);
        post_this = g_string_append(post_this,po->name);
        post_this = g_string_append(post_this,"=");
        if (strcmp(po->value,"STRING_TO_BE_TRANSLATED")==0)
         post_this = g_string_append(post_this,mesg);
        else if (strcmp(po->value,"LANG_PAIR")==0)
         post_this = g_string_append(post_this,lp);
        else
         post_this = g_string_append(post_this,po->value);

         if (i < (size-1))
         post_this = g_string_append(post_this,"&");
   }

/*    printf("Exiting get post options %d \n",size); */
    printf("Exiting get post options %s \n",post_this->str); 
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
  printf("xml-translate: get_avail_languages entered \n");
 return langs;
}
gboolean xml_translate_init()
{
  printf("xml-translate: xml_translate_init entered \n");
    xmlTextReaderPtr xml_reader;
    int ret;
    gboolean xml_init = TRUE;

    gchar *serverfilename = g_build_filename(PACKAGE_DATA_DIR,PACKAGE,"google.xml",NULL);
    /* If File does not exists make dir and return */
   if(!g_file_test(serverfilename,G_FILE_TEST_EXISTS))
   {
      xml_init = FALSE ; 
   }
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
      		xml_init = FALSE ; 
        }
    } else {
        printf("Unable to open %s\n", serverfilename);
      xml_init = FALSE ; 
    }
     
  /* add default */
  add_avail_langs("No Translation");
  printf("xml-translate: xml_translate_init exited \n");

  return xml_init;
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
