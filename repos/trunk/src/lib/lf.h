/* @file lf.h Language Translate lib from lingua-franca
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

/* Include Directives */
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Curl Directives */
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

/* Glib Directives */
#include <glib.h>

#define USER_AGENT "lingua-franca" ;

/*translate engine*/
gboolean lf_translate_init();

/* This method translates the message "from_lang" to "to_lang" language */
char *lf_translate_from_to(char *message ,char *from_lang, char *to_lang);

/* This method translates the message "to_lang" language
 * this method is used when "from" language is unknown
 * internally the language is determined .. its Magic !  */
char *lf_translate_to(char *message , char *to_lang);

/* Is translation available from lang1 to lang2 */
gboolean lf_is_translation_avail(char *lang1 ,char *lang2);

/* Set the translation server */
void lf_set_translate_server(char *serverfilename);

/* get the available servers */
GList *lf_get_trans_servers();

/* get the available servers */
GList *lf_get_avail_languages();
