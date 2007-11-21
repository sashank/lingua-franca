/* @file lf.c Language Translate Library
 * 
 * Author : Sashank Dara
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
/* XML Processor */
#include "xml-translate.h"
#include "lf.h"

struct ResponseStruct {
  char *response;
  size_t size;
};

struct ResponseStruct chunk;
int i= 0;

char *serverfilename;
CURL *curl;
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
  i++;
  return realsize;
}

char *parse_response(char *response)
{
   gchar **splits;
   char *parsed_response,*prefix,*suffix;
   int res_size;
/*   printf(" Parse response Size %d\n ",strlen(response)); 
   printf(" Prefix  %s\n ",get_response_prefix()); 
   printf(" Suffix  %s\n ",get_response_suffix()); 
   printf(" parse response %s",response); */
   

/*
   prefix = strstr(response,get_response_prefix());
   printf(" parse_response Prefix %s\n ",prefix); 
   prefix += strlen(get_response_prefix());
    

   suffix = strstr(prefix,get_response_suffix());

   res_size = strlen(prefix)-strlen(suffix);
      
*/
   splits = g_strsplit(response ,get_response_prefix(),-1); 
   response = strdup(splits[1]);


   g_strfreev(splits);

   splits = g_strsplit(response,get_response_suffix(),-1);
   parsed_response = strdup(splits[0]);

   g_strfreev(splits); 

   printf(" parse_response Exiting \n ");
  return parsed_response;
}

char *lf_determine_lang(char *mesg)
{
 return "English";
}

char *lf_translate_from_to(char *message , char *from , char *to)
{
  char *translated_mesg;
  CURLcode ret;
   printf("lib: translate_message Entered \n "); 
/*A*/   printf("lib: translate_message %s,%s,%s \n ",message,from,to); 
  
  if ( strcmp(from,to) == 0)
  {
    printf ( "Both languages seem to be same ");
    return message ;
  }
  if ( lf_is_translation_avail(from,to) == FALSE )
  {
    printf ( "Language Translation does not exist ");
    return message ;
  }

     curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
     curl = curl_easy_init();

    /* specify URL to get */
     curl_easy_setopt(curl, CURLOPT_URL, get_host_url());

    /* we pass our 'chunk' struct to the callback function */
     curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
     curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
     chunk.response=NULL; /* we expect realloc(NULL, size) to work */
     chunk.size = 0;    /* no data at this point */

  GString *post;
  post = get_post_string(message,from,to);

  printf(" Post String is %s\n ",post->str); 
  printf(" host url is %s\n ",get_host_url()); 

  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post->str); 

  /* send all data to this function  */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseCallback);


  /* get it! */
  ret = curl_easy_perform(curl);
  if ( ret != CURLE_OK)
  {
    printf(" Some Network error \n");
    curl_easy_cleanup(curl);
    return message;
  }
     


  /* cleanup curl stuff */
  curl_easy_cleanup(curl);
  /*
   * Now, our chunk.memory points to a memory block that is chunk.size
   * bytes big and contains the remote file.
   *
   * Do something nice with it!
   *
   * You should be aware of the fact that at this point we might have an
    
     * allocated data block, and nothing has yet deallocated that data. So when
   * you're done with it, you should free() it as a nice application.
   */

/*     printf("It seems we have got response %s",chunk.response);  */
   if(chunk.response != NULL)
     translated_mesg = parse_response(chunk.response);

    /* You should be aware of the fact that at this point we might have an
     * allocated data block, and nothing has yet deallocated that data. So when
     * you're done with it, you should free() it as a nice application.
     */
     if(chunk.response)
       free(chunk.response);

   printf(" translate_message Exited \n ");

 return translated_mesg;
}

GList *lf_get_trans_servers()
{
   printf("translate.c: get_trans_servers entered \n");
 GList *server_list = NULL;
  /* For now hardcoded , to be modified later */
  server_list = g_list_append(server_list , "google"); 
  server_list = g_list_append(server_list , "altavista"); 
   printf("translate.c: get_trans_servers exited \n");
 return server_list;
}
gboolean lf_is_translation_avail(char *lang1,char *lang2)
{
 gboolean exists = TRUE ;
 char *lp = get_lp(lang1,lang2);
  printf("is_avail ..%s",lp);
  if ( strcmp(lp,"NA")==0)
   exists = FALSE ;
 return exists ;
}
void lf_set_translate_server(char *server)
{
    /* this may be deprecated soon*/
	
       serverfilename = strcat(server,".xml") ;
      /*  //Free the existing XML
        xml_translate_unload();

 	//Initialise XML
  	xml_translate_init(serverfilename); */

}
gboolean lf_translate_init()
{
   printf("translate.c: translate_init entered \n");
    gboolean xml_ret ;
    xml_ret = xml_translate_init();
    
    if(xml_ret == FALSE)
     {
	printf("Boo Hoo .. ! No Translation \n");
	printf("Could not find any server xmls \n");
        return FALSE;
     }
 

   printf("translate.c: translate_init exiting \n");
 return TRUE ;
}

char *lf_translate_to(char *mesg,char *to)
{
   char *from = lf_determine_lang(mesg);  
   return lf_translate_from_to(mesg,from,to);
}

GList *lf_get_avail_languages()
{
 return get_avail_languages();
}
void lf_translate_unload()
{
  
}
