/**
 * @file xml-translate.h
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

/* XML Directives */
#include <libxml/xmlreader.h>

/* Glib Directives */
#include <glib.h>

#include <config.h>


/*Init XML Processing of Translate XML File */
gboolean xml_translate_init();

/* Is Translation available from lang1 to lang2 */
char *get_lang_pair(char *lang1 ,char *lang2);

/* get the Host Url */
char *get_host_url();

/* get the post string */
GString *get_post_string(char *mesg,char *from,char *to);

/* get the translation server name */
char *get_server_name();

/*get the response prefix */
char *get_response_prefix();

/*get the response suffix */
char *get_response_suffix();

/*get List of available Languages */
GList *get_avail_languages();

/* free up xml */
void xml_translate_unload();
