/**
 * @file xml-ui.h
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
#include <libxml/xmlwriter.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <glib.h>


void xml_ui_init(char *dir);

char *get_incoming_lang_pref();

char *get_outgoing_lang_pref(char *buddy);

gboolean get_pref_status(char *buddy);

void set_incoming_lang_pref(char *lang);

void set_outgoing_lang_pref(char *buddy,char *lang);

void set_view_translation(int  state);

void set_buddy_toggle(gchar *buddy);

void set_trans_server(char *server);

void set_time_out(int time_out);

void save_preferences();

void xml_ui_unload();
