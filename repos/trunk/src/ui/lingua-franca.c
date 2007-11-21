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
  GtkWidget *ui = create_ui();
  return ui;
}

/* Determines whther translation engine is on */
gboolean translation = TRUE ;

char *lf_translate_incoming(char *mesg,char *buddy)
{
  printf("lingua-franca:lf_translate_incoming  Mesg is %s \n",mesg);

  char *translated_mesg ;
  char *my_lang =  get_lang_pref("MY_LANG");
  char *buddy_lang = get_lang_pref(buddy);

  /* Do translation only when 
   * 1 . Translation engine is ON
   * 2 . My language is not None  
   * 3 . Buddy language is not None
   * 4 . My lang and buddy lang are not same
   * 5 . Language translation is available
   */
  if ( translation == TRUE 
		&& !(strcmp(my_lang,"None") == 0)
		&& !(strcmp(buddy_lang,"None")==0)
		&& !(strcmp(my_lang,buddy_lang)==0)
		&& lf_is_translation_avail(buddy_lang,my_lang) == TRUE)
  {
     translated_mesg = lf_translate_from_to(mesg,buddy_lang,my_lang); 
  }
  else
  {
   printf("lingua-franca:lf_translate_incoming  No Translation %s \n",translated_mesg);
   translated_mesg = mesg;
  }

  return translated_mesg;
}

char *lf_translate_outgoing(char *mesg,char *buddy)
{

  char *translated_mesg ;
  char *my_lang =  get_lang_pref("MY_LANG");
  char *buddy_lang = get_lang_pref(buddy);

  /* If language is not set for this buddy
   * check whther any preference is set for 
   * all the buddies 
   */ 
  if(strcmp(buddy_lang,"None")==0)
     buddy_lang = get_lang_pref("ALL");
    

  /* Do translation only when 
   * 1 . Translation engine is ON
   * 2 . My language is not None  
   * 3 . Buddy language is not None
   * 4 . My lang and buddy lang are not same
   * 5 . Language translation available
   */
  if ( translation == TRUE 
		&& !(strcmp(my_lang,"None") == 0)
		&& !(strcmp(buddy_lang,"None")==0)
		&& !(strcmp(my_lang,buddy_lang)==0)
/*		&& lf_is_translation_avail(my_lang,buddy_lang) == TRUE*/)
  {
    printf("lingua-franca:lf_translate_outgoing  Mesg is %s,%s,%s \n",mesg,my_lang,buddy_lang);
     translated_mesg = lf_translate_from_to(mesg,my_lang,buddy_lang); 
  }
  else
  {
   printf("lingua-franca:lf_translate_outgoing  No Translation %s \n",translated_mesg);
   translated_mesg = mesg;
  }

  return translated_mesg;
}

void lf_init(GList *buddies)
{
 GList *trans_servers,*languages;
 #ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");

  
  /* Init Translate engine */
   translation = lf_translate_init();
   trans_servers = lf_get_trans_servers();
   languages = lf_get_avail_languages();
  
  interface_init(buddies,trans_servers,languages);
 
}

void lf_unload()
{
 printf("lingua-franca:lf-unload entered \n");
  interface_unload();
 printf("lingua-franca:lf-unload exited \n");
}
