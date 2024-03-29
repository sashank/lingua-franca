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

#include "../src/lib/lf.h"

int main(int argc, char **argv)
{
 char *result ;
 if(lf_translate_init()) 
  result = lf_translate_from_to( "Hello","English","French"); 
 else
   printf(" Translations Not available ");	
 printf("Translated Message %s \n",result);
}
