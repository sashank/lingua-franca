/**
 * @file lingua-franca.c
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

#include "lingua-franca.h"

GtkWidget *lf_ui()
{
  printf("lingua-franca:lf-ui  entered \n");
  GtkWidget *ui = get_interface();
  printf("lingua-franca:lf-ui  exiting \n");
  return ui;
}

char *lf_translate_incoming(char *mesg)
{
  printf("lingua-franca:lf_translate_incoming  entered \n");
  printf("lingua-franca:lf_translate_incoming  Mesg is %s \n",mesg);
  char *incoming_lang_pref = get_incoming_lang_pref();
  char *translated_mesg ;
  printf("lingua-franca:lf_translate_incoming  incoming_lang_pref is %s \n",incoming_lang_pref);

  translated_mesg = translate(mesg,incoming_lang_pref);

  printf("lingua-franca:lf_translate_incoming  translated mesg is %s \n",translated_mesg);
  printf("lingua-franca:lf_translate_outgoing  exiting \n");
  return translated_mesg;
}

char *lf_translate_outgoing(char *mesg, char *buddy)
{
  printf("lingua-franca:lf-translate_outgoing entered \n");
  printf("lingua-franca: Mesg is %s \n",mesg);
  printf("lingua-franca: buddy is %s \n",buddy);
  char *outgoing_lang = get_outgoing_lang_pref(buddy);
  char *translated_mesg ;

  printf("lingua-franca: Outgoing Lang Pref is %s \n",outgoing_lang);
  translated_mesg = translate(mesg,outgoing_lang);

  printf("lingua-franca: Translated Mesg is %s \n",translated_mesg);
  printf("lingua-franca:lf-translate_outgoing exited \n");
  return translated_mesg;
}

void lf_init(GList *buddies,char *dir)
{
 printf("lingua-franca:lf-init entered \n");
 #ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");

  interface_init(buddies,dir);
 
 printf("lingua-franca:lf-init exited \n");
}

void lf_unload()
{
 printf("lingua-franca:lf-unload entered \n");
  interface_unload();
 printf("lingua-franca:lf-unload exited \n");
}
