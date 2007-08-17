/* @file translate.c Lang Translate API
 * 
 * Translate API is developed by sashank (krishna.sashank@gmail.com)
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
#include "translate.h"
struct PostOptionStruct {
  char *name;
  char *value;
};

GList *postoptions_list=NULL;

void *myrealloc(void *ptr, size_t size)
{
  /* There might be a realloc() out there that doesn't like reallocing
     NULL pointers, so we take care of it here */
  if(ptr)
    return realloc(ptr, size);
  else
    return malloc(size);
}

size_t
ResponseCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  struct ResponseStruct *resp = (struct ResponseStruct *)data;

  resp->response = (char *)myrealloc(resp->response, resp->size + realsize + 1);
  if (resp->response) {
    memcpy(&(resp->response[resp->size]), ptr, realsize);
    resp->size += realsize;
    resp->response[resp->size] = 0;
  }
  return realsize;
}

char *parse_response(char *response)
{
	printf("%s",response);	

  return "hello";
}

char *get_lang_pref(char *from ,char *to)
{

  guint size = g_list_length(langpairs_list);
  struct LangPairStruct *lpair;
  int i = 0;
   for ( i =0 ; i < size ; i++)
   {
	lpair = g_list_nth_data(langpairs_list,i);
	if ((strcmp(lpair->from,from) == 0) && (strcmp(lpair->to,to) == 0))
	  return lpair->lp ;
   }
  
  return "NA";
}

void set_post_options(char *message,char *lp)
{
  sprintf(post,"doit=done&intl=1&tt=urltext&trtext=%s&lp=%s&btnTrTxt=Translate",message,lp);
}
char* translate_message(char* message , char *from , char *to)
{
  char postthis[512];
  char *translated_mesg;
  char *lp = get_lang_pref(from ,to );

  if(curl) {
    
	/* set the post options */
  	set_post_options(message,lp); 

	/* set the post options */
  	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);

	/* if we don't provide POSTFIELDSIZE, libcurl will strlen() by itself */
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(postthis));

 	/* get it! */
  	curl_easy_perform(curl);

  	/* cleanup curl stuff */
  	curl_easy_cleanup(curl);
  
     }

    /* 
     * Now, our chunk.response points to a response block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Let us parse to get the translated String
     */
     translated_mesg = parse_response(chunk.response);

    /* You should be aware of the fact that at this point we might have an
     * allocated data block, and nothing has yet deallocated that data. So when
     * you're done with it, you should free() it as a nice application.
     */
     if(chunk.response)
       free(chunk.response);


 return translated_mesg;
}

void curl_init()
{
	chunk.response=NULL; /* we expect realloc(NULL, size) to work */
	chunk.size = 0;    /* no data at this point */

	/*Initialise curl */
  	curl_global_init(CURL_GLOBAL_ALL);

  	curl = curl_easy_init();

  	/* send all data to this function  */
  	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseCallback);

  	/* we pass our 'chunk' struct to the callback function */
  	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  	/* some servers don't like requests that are made without a user-agent
     		field, so we provide one */
  	curl_easy_setopt(curl, CURLOPT_USERAGENT, "lingua-franca");
}
void add_post_options(char *name,char *value)
{
   struct PostOptionStruct *postoption;
	postoption->name = malloc(strlen(name));
	postoption->value = malloc(strlen(value));
	
	strcpy(postoption->name,name);
	strcpy(postoption->value,value);

	postoptions_list = g_list_append(postoptions_list,postoption);
}
void add_lang_pair(char *from , char *to,char *lp)
{
   	struct LangPairStruct *langpair ;
 	langpair->from =malloc((size_t)strlen(from));
        langpair->to =malloc((size_t)strlen(to));
        langpair->lp =malloc((size_t)strlen(lp));

 	strcpy(langpair->from, from);
        strcpy(langpair->to,to);
        strcpy(langpair->lp, lp);

        printf("From Name %s \n",langpair->from);
        printf("To Value %s \n",langpair->to);
        printf("LP Value %s \n",langpair->lp);

	langpairs_list = g_list_append(langpairs_list,langpair);
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
                printf("Server Name is %s \n",value);
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
                printf("host Name is %s \n",value);
  		/* specify URL to get */
  		curl_easy_setopt(curl, CURLOPT_URL, value);
                xmlTextReaderRead(reader);
        }

        if( xmlTextReaderNodeType(reader) == 2 )
                xmlTextReaderRead(reader);
     }

    if (strcmp(name,"post_option")==0)
    {
        printf("Attribute Count %d \n", xmlTextReaderAttributeCount(reader));
        printf("Post Name %s \n",xmlTextReaderGetAttribute(reader,(xmlChar *)"name"));
        printf("Post Value %s \n",xmlTextReaderGetAttribute(reader,(xmlChar *)"value"));
	char *post_name = xmlTextReaderGetAttribute(reader,(xmlChar *)"name");
        char *post_value = xmlTextReaderGetAttribute(reader,(xmlChar *)"value");
	add_post_options(post_name,post_value);
    }

    if (strcmp(name,"lang_pair")==0)
    {
	char *from = xmlTextReaderGetAttribute(reader,(xmlChar *)"from");
        char *to = xmlTextReaderGetAttribute(reader,(xmlChar *)"to");
        char *lp = xmlTextReaderGetAttribute(reader,(xmlChar *)"lp");
	add_lang_pair(from,to,lp);
    }

     
}
void xml_init(char *server)
{
    xmlTextReaderPtr xml_reader;
    int ret;

    xml_reader = xmlNewTextReaderFilename(server);
    if (xml_reader != NULL) {
        ret = xmlTextReaderRead(xml_reader);
        while (ret == 1) {
            processNode(xml_reader);
            ret = xmlTextReaderRead(xml_reader);
        }
        xmlFreeTextReader(xml_reader);
        if (ret != 0) {
            printf("%s : failed to parse\n", server);
        }
    } else {
        printf("Unable to open %s\n", server);
    }

}
int translate_init(char *serverfilename)
{
 
 //Initialise CURL
  curl_init();

 //Initialise XML
  xml_init(serverfilename);

}
