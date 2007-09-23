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

/* XML Processor */
#include "xml-translate.h"
struct ResponseStruct {
  char *response;
  size_t size;
};

struct ResponseStruct chunk;
int i= 0;

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
   char *parsed_response;

/*   printf(" parse_response Entered %s\n ",response); 
   printf(" Parse response Size %d\n ",strlen(response)); 
   printf(" Prefix  %s\n ",get_response_prefix()); 
   printf(" Suffix  %s\n ",get_response_suffix());  */

   if(response != NULL)
    return NULL ;
  
    printf(" response  %s\n ",response); 

   splits = g_strsplit(response ,get_response_prefix(),-1); 
   response = strdup(splits[1]);

   g_strfreev(splits);

   splits = g_strsplit(response,get_response_suffix(),-1);
   parsed_response = strdup(splits[0]);

   g_strfreev(splits);

    printf(" parsed_response  %s\n ",parsed_response);
   printf(" parse_response Exiting \n ");
  return response;
}

char *determine_lang(char *mesg)
{

 return "English";
}

char *translate_message(char *message , char *from , char *to)
{
  char *translated_mesg;
  GString *post; 
   printf(" translate_message Entered \n "); 

  chunk.response=NULL; /* we expect realloc(NULL, size) to work */
  chunk.size = 0;    /* no data at this point */

  post = get_post_string(message,from,to);

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl = curl_easy_init();

  /* specify URL to get */
  curl_easy_setopt(curl, CURLOPT_URL, get_host_url());

  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post->str);

  /* send all data to this function  */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  /* get it! */
  curl_easy_perform(curl);

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


     printf("It seems we have got response %s",chunk.response); 
     translated_mesg = strdup(parse_response(chunk.response));

    /* You should be aware of the fact that at this point we might have an
     * allocated data block, and nothing has yet deallocated that data. So when
     * you're done with it, you should free() it as a nice application.
     */
     if(chunk.response)
       free(chunk.response);

   printf(" translate_message Entered \n ");

 return translated_mesg;
}

GList *get_trans_servers()
{
   printf("translate.c: get_trans_servers entered \n");
 GList *server_list = NULL;
  /* For now hardcoded , to be modified later */
  server_list = g_list_append(server_list , "Google"); 
  server_list = g_list_append(server_list , "Altavista"); 
   printf("translate.c: get_trans_servers exited \n");
 return server_list;
}
gboolean is_translation_avail(char *lang1,char *lang2)
{
 return TRUE ;
}
void set_translate_server(char *serverfilename)
{
    /* For now do nothing */

      /*  //Free the existing XML
        xml_translate_unload();

 	//Initialise XML
  	xml_translate_init(serverfilename); */

}
void translate_init(char *dir)
{
   printf("translate.c: translate_init entered \n");
     /* By default load altavista xml */
     char *serverfilename =  g_build_filename(dir,"altavista.xml",NULL);
      if (!g_file_test(serverfilename, G_FILE_TEST_EXISTS))
        {
                printf( "File %s does not exist (this is not "
                                                "necessarily an error)\n", serverfilename);
                g_free(serverfilename);
        }

 	//Initialise XML
  	xml_translate_init(serverfilename);
   printf("translate.c: translate_init exiting \n");
}

char *translate(char *mesg,char *to)
{
   char *from = determine_lang(mesg);  
   return translate_message(mesg,from,to);
}

void translate_unload()
{
  
}
