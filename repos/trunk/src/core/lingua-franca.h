/**
 * @file lingua-franca.h
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glib.h>

#include "interface.h"
#include "support.h"
#include "../lib/translate.h"

/* Init method */
void lf_init(GList *buddies,char *dir);

/* User Preferences */
GtkWidget *lf_ui();

/* translate outgoing messages inputs are mesg and buddyname */
char *lf_translate_outgoing(char *mesg,char *buddy);

/* translate outgoing messages inputs are mesg */
char *lf_translate_incoming(char *mesg);

/*Destructor method */
void lf_unload();
