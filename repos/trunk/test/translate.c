/* @file translate.c Lang Translate API
 * 
 * This program is for unit testing the http requests for translation
 * and parsing the responses
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

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <glib.h>

#define USER_AGENT "lingua-franca" ;

struct ResponseStruct {
  char *response;
  size_t size;
};

struct ResponseStruct chunk;
CURL *curl;
char *post; /*string to be sent for post options */

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
   gchar **strings;

   strings = g_strsplit(response ,"<td bgcolor=white class=s><div style=padding:10px;>",2);
   strings = g_strsplit(strings[1],"</div></td>",2);

   printf("%s \n",strings[0]); 
	
  return string[0];
}

char *get_lang_pref(char *from ,char *to)
{
  return "en_fr";
}
char* translate_message(char* message , char *from , char *to)
{
  char postthis[512];
  char *translated_mesg;
  char *lp = get_lang_pref(from ,to );
  sprintf(postthis,"doit=done&intl=1&tt=urltext&trtext=%s&lp=%s&btnTrTxt=Translate",message,lp);
  
  if(curl) {
    
	/* set the post options */
  	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);

  	/* get it! */
  	curl_easy_perform(curl);

  	/* cleanup curl stuff */
  	curl_easy_cleanup(curl);
  
     }

    /* Bingo ! 
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

  	/* specify URL to get */
  	curl_easy_setopt(curl, CURLOPT_URL, "http://babelfish.altavista.com/tr");

  	/* send all data to this function  */
  	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseCallback);

  	/* we pass our 'chunk' struct to the callback function */
  	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  	/* some servers don't like requests that are made without a user-agent
     		field, so we provide one */
  	curl_easy_setopt(curl, CURLOPT_USERAGENT, "lingua-franca");
}

int main(void)
{
 
 //Initialise CURL
  curl_init();

 translate_message( "Hello How are you?" ,"English","French"); 

}