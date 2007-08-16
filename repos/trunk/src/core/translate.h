/* @file translate.h Lang Translate API
 * 
 * Translate API alone is added to purple by sashank (krishna.sashank@gmail.com)
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

/* Include Directives */
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#define USER_AGENT "lingua-franca" ;

struct ResponseStruct {
  char *response;
  size_t size;
};

struct ResponseStruct chunk;
CURL *curl;
char *post; /*string to be sent for post options */

/* This method translates the message */
char* translate_message(char *message , char *from,char *to);
