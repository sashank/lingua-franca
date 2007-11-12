/**
 * @file interface.c
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

#include "interface.h"
#include "callbacks.h"
#include "../lib/lf.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkTooltips *tooltips;
GList     *buddies = NULL;
GList     *trans_servers = NULL;
GList     *languages = NULL;
int        buddycount = 0 ;

void *add_element(gpointer data,gpointer userdata)
{
  /*  printf("interface.c: add_element  entered \n");
    g_return_if_fail(data != NULL);
  printf("interface.c: lang is %s \n",(gchar *)data); */
   gtk_combo_box_append_text((GtkComboBox *)userdata, (gchar *)data);
  /* printf("interface.c: add_element  exiting \n"); */
}
void add_buddies_table(char *buddy,GtkWidget *buddy_table)
{
  printf("interface.c: add_buddies_table entered \n");
  printf("interface.c: buddy is %s \n",buddy);
   GtkWidget *buddy_label;
   GtkWidget *buddy_combo;
   GtkWidget *buddy_toggle;
   int default_pref ;

  buddy_label = gtk_label_new (buddy);
  gtk_widget_show (buddy_label);
  gtk_table_attach (GTK_TABLE (buddy_table), buddy_label, 0, 1, buddycount+1, buddycount+2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (buddy_label, 70, 25);
  gtk_misc_set_alignment (GTK_MISC (buddy_label), 0.35, 0.5);

  buddy_combo = gtk_combo_box_new_text ();
  g_list_foreach(languages,(GFunc)add_element, (gpointer)buddy_combo);
  printf("interface.c: before index adding langs \n");
  gtk_widget_show (buddy_combo);
  gtk_table_attach (GTK_TABLE (buddy_table), buddy_combo, 1, 2, buddycount+1,buddycount+ 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_widget_set_size_request (buddy_combo, 35, 25);
  default_pref = g_list_index(languages,get_outgoing_lang_pref(buddy));
  printf("interface.c: default pref %d \n",default_pref);
  gtk_combo_box_set_active(buddy_combo,default_pref);

  buddy_toggle = gtk_toggle_button_new_with_mnemonic ("gtk-yes");
  gtk_button_set_use_stock (GTK_BUTTON (buddy_toggle), TRUE);
  gtk_widget_show (buddy_toggle);
  gtk_table_attach (GTK_TABLE (buddy_table), buddy_toggle, 2, 3, buddycount+1, buddycount+2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (buddy_toggle, -1, 35);
  gtk_tooltips_set_tip (tooltips, buddy_toggle, ("Outgoing chat messages to this buddy would be translated to the language set here"), NULL);

  g_signal_connect ((gpointer)buddy_combo, "changed",
                    G_CALLBACK (on_buddy_combo_changed),
                    (gchar *)buddy);

  g_signal_connect ((gpointer)buddy_toggle, "clicked",
                    G_CALLBACK (on_buddy_toggle_toggled),
                    (gchar *)buddy);
  buddycount++;

  printf("interface.c: add_buddies_table exited \n");
}


void interface_unload()
{
  xml_ui_unload();
  lf_translate_unload();
}
GtkWidget *create_ui (void)
{
  printf("interface.c: create_ui entered \n");
  GtkWidget *lingua_franca_win;
  GtkWidget *notebook;
  GtkWidget *general_frame;
  GtkWidget *alignment1;
  GtkWidget *scrolledwindow1;
  GtkWidget *general_layout;
  GtkWidget *incoming_label;
  GtkWidget *save_button;
  GtkWidget *save_align;
  GtkWidget *hbox4;
  GtkWidget *image4;
  GtkWidget *save_label1;
  GtkWidget *incoming_combo;
  GtkWidget *outgoing_label;
  GtkWidget *outgoing_combo;
  GtkWidget *general_label;
  GtkWidget *general_label2;
  GtkWidget *buddy_frame;
  GtkWidget *alignment3;
  GtkWidget *scrolledwindow2;
  GtkWidget *viewport1;
  GtkWidget *buddy_table;
  GtkWidget *bname_label;
  GtkWidget *lp_label;
  GtkWidget *enable_label;
  GtkWidget *buddy_lp_label;
  GtkWidget *buddyconfig_label;
  GtkWidget *misc_frame;
  GtkWidget *alignment4;
  GtkWidget *scrolledwindow4;
  GtkWidget *misc_layout;
  GtkWidget *trans_server_label;
  GtkWidget *trans_server_combo;
  GtkWidget *view_trans_label;
  GtkWidget *view_trans_toggle;
  GtkWidget *time_out_label;
  GtkObject *time_out_spin_adj;
  GtkWidget *time_out_spin;
  GtkWidget *save2_button;
  GtkWidget *alignment6;
  GtkWidget *hbox3;
  GtkWidget *image3;
  GtkWidget *label21;
  GtkWidget *misc_label2;
  GtkWidget *misc_label;
  GtkWidget *ret,*vbox;

  tooltips = gtk_tooltips_new ();
  ret = gtk_vbox_new(FALSE, 18);
        gtk_container_set_border_width (GTK_CONTAINER(ret), 12);

        vbox = pidgin_make_frame(ret, _("Lingua Franca Preferences"));
        gtk_container_set_border_width(GTK_CONTAINER(vbox), 4);
        gtk_widget_show(vbox);

        lingua_franca_win = gtk_scrolled_window_new(0, 0);
        gtk_box_pack_start(GTK_BOX(vbox), lingua_franca_win, TRUE, TRUE, 0);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(lingua_franca_win),
                                                                                GTK_SHADOW_IN);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(lingua_franca_win),
                        GTK_POLICY_NEVER,
                        GTK_POLICY_ALWAYS);
        gtk_widget_show(lingua_franca_win);

  lingua_franca_win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (lingua_franca_win), ("Lingua Franca Preferences"));
  gtk_window_set_position (GTK_WINDOW (lingua_franca_win), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_set_default_size (GTK_WINDOW (lingua_franca_win), 390, 425);

  notebook = gtk_notebook_new ();
  gtk_widget_show (notebook);
  gtk_container_add (GTK_CONTAINER (lingua_franca_win), notebook);

  general_frame = gtk_frame_new (NULL);
  gtk_widget_show (general_frame);
  gtk_container_add (GTK_CONTAINER (notebook), general_frame);
  gtk_frame_set_shadow_type (GTK_FRAME (general_frame), GTK_SHADOW_NONE);

  alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (general_frame), alignment1);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment1), 0, 0, 12, 0);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow1);
  gtk_container_add (GTK_CONTAINER (alignment1), scrolledwindow1);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_SHADOW_IN);

  general_layout = gtk_layout_new (NULL, NULL);
  gtk_widget_show (general_layout);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), general_layout);
  gtk_tooltips_set_tip (tooltips, general_layout, ("Set the Preferred Incoming and Outgoing Languages"), NULL);
  gtk_layout_set_size (GTK_LAYOUT (general_layout), 350, 370);
  GTK_ADJUSTMENT (GTK_LAYOUT (general_layout)->hadjustment)->step_increment = 10;
  GTK_ADJUSTMENT (GTK_LAYOUT (general_layout)->vadjustment)->step_increment = 10;

  incoming_label = gtk_label_new (("Incoming Language"));
  gtk_widget_show (incoming_label);
  gtk_layout_put (GTK_LAYOUT (general_layout), incoming_label, 8, 32);
  gtk_widget_set_size_request (incoming_label, 137, 25);
  gtk_misc_set_alignment (GTK_MISC (incoming_label), 0.42, 0.62);

  save_button = gtk_button_new ();
  gtk_widget_show (save_button);
  gtk_layout_put (GTK_LAYOUT (general_layout), save_button, 136, 216);
  gtk_widget_set_size_request (save_button, 85, 30);

  save_align = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (save_align);
  gtk_container_add (GTK_CONTAINER (save_button), save_align);

  hbox4 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox4);
  gtk_container_add (GTK_CONTAINER (save_align), hbox4);

  image4 = gtk_image_new_from_stock ("gtk-yes", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image4);
  gtk_box_pack_start (GTK_BOX (hbox4), image4, FALSE, FALSE, 0);

  save_label1 = gtk_label_new_with_mnemonic ("Save");
  gtk_widget_show (save_label1);
  gtk_box_pack_start (GTK_BOX (hbox4), save_label1, FALSE, FALSE, 0);

  incoming_combo = gtk_combo_box_new_text();
  gtk_widget_show (incoming_combo);
  gtk_layout_put (GTK_LAYOUT (general_layout), incoming_combo, 168, 32);
  gtk_widget_set_size_request (incoming_combo, 134, 31);
  g_list_foreach(languages,(GFunc)add_element, (gpointer)incoming_combo);

  outgoing_label = gtk_label_new ("Outgoing Language");
  gtk_widget_show (outgoing_label);
  gtk_layout_put (GTK_LAYOUT (general_layout), outgoing_label, 16, 112);
  gtk_widget_set_size_request (outgoing_label, 128, 17);
  gtk_misc_set_alignment (GTK_MISC (outgoing_label), 0.38, 0.5);

  outgoing_combo = gtk_combo_box_new_text();
  gtk_widget_show (outgoing_combo);
  gtk_layout_put (GTK_LAYOUT (general_layout), outgoing_combo, 168, 104);
  gtk_widget_set_size_request (outgoing_combo, 128, 31);
  g_list_foreach(languages,(GFunc)add_element, (gpointer)outgoing_combo);

  general_label = gtk_label_new ("<b>Preferred Languages</b>");
  gtk_widget_show (general_label);
  gtk_frame_set_label_widget (GTK_FRAME (general_frame), general_label);
  gtk_label_set_use_markup (GTK_LABEL (general_label), TRUE);

  general_label2 = gtk_label_new (("General"));
  gtk_widget_show (general_label2);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 0), general_label2);

  buddy_frame = gtk_frame_new (NULL);
  gtk_widget_show (buddy_frame);
  gtk_container_add (GTK_CONTAINER (notebook), buddy_frame);
  gtk_frame_set_shadow_type (GTK_FRAME (buddy_frame), GTK_SHADOW_NONE);

  alignment3 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment3);
  gtk_container_add (GTK_CONTAINER (buddy_frame), alignment3);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment3), 0, 0, 12, 0);

  scrolledwindow2 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow2);
  gtk_container_add (GTK_CONTAINER (alignment3), scrolledwindow2);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow2), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  viewport1 = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (viewport1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow2), viewport1);

  buddy_table = gtk_table_new (10, 3, FALSE);
  gtk_widget_show (buddy_table);
  gtk_container_add (GTK_CONTAINER (viewport1), buddy_table);
  gtk_widget_set_size_request (buddy_table, 350, -1);

  bname_label = gtk_label_new ("Buddy Name");
  gtk_widget_show (bname_label);
  gtk_table_attach (GTK_TABLE (buddy_table), bname_label, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (bname_label, 100, 25);
  gtk_label_set_justify (GTK_LABEL (bname_label), GTK_JUSTIFY_CENTER);

  lp_label = gtk_label_new ("Language Preference");
  gtk_widget_show (lp_label);
  gtk_table_attach (GTK_TABLE (buddy_table), lp_label, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (lp_label, 158, 25);
  gtk_label_set_justify (GTK_LABEL (lp_label), GTK_JUSTIFY_CENTER);

  enable_label = gtk_label_new ("Enable");
  gtk_widget_show (enable_label);
  gtk_table_attach (GTK_TABLE (buddy_table), enable_label, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (enable_label, 88, -1);

  /* Add Buddies to table */
  int buddy_cnt = g_list_length(buddies);
    printf(" buddy count is %d \n",buddy_cnt);
  int i ;
  gchar *buddy_name ;
  for ( i =0 ; i < buddy_cnt ; i++)
  {
    buddy_name = (gchar *)g_list_nth_data(buddies,i);
    printf(" buddy is %s \n",buddy_name);
    add_buddies_table(buddy_name,buddy_table);
  }

  buddy_lp_label = gtk_label_new ("<b>Buddy  Lang Preference</b>");
  gtk_widget_show (buddy_lp_label);
  gtk_frame_set_label_widget (GTK_FRAME (buddy_frame), buddy_lp_label);
  gtk_label_set_use_markup (GTK_LABEL (buddy_lp_label), TRUE);

  buddyconfig_label = gtk_label_new ("Buddy Config");
  gtk_widget_show (buddyconfig_label);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 1), buddyconfig_label);

  misc_frame = gtk_frame_new (NULL);
  gtk_widget_show (misc_frame);
  gtk_container_add (GTK_CONTAINER (notebook), misc_frame);
  gtk_frame_set_shadow_type (GTK_FRAME (misc_frame), GTK_SHADOW_NONE);

  alignment4 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment4);
  gtk_container_add (GTK_CONTAINER (misc_frame), alignment4);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment4), 0, 0, 12, 0);

  scrolledwindow4 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow4);
  gtk_container_add (GTK_CONTAINER (alignment4), scrolledwindow4);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow4), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow4), GTK_SHADOW_IN);

  misc_layout = gtk_layout_new (NULL, NULL);
  gtk_widget_show (misc_layout);
  gtk_container_add (GTK_CONTAINER (scrolledwindow4), misc_layout);
  gtk_layout_set_size (GTK_LAYOUT (misc_layout), 350, 370);
  GTK_ADJUSTMENT (GTK_LAYOUT (misc_layout)->hadjustment)->step_increment = 10;
  GTK_ADJUSTMENT (GTK_LAYOUT (misc_layout)->vadjustment)->step_increment = 10;

  trans_server_label = gtk_label_new ("Translate Server");
  gtk_widget_show (trans_server_label);
  gtk_layout_put (GTK_LAYOUT (misc_layout), trans_server_label, 16, 48);
  gtk_widget_set_size_request (trans_server_label, 120, 25);

  trans_server_combo = gtk_combo_box_new_text ();
  gtk_widget_show (trans_server_combo);
  gtk_layout_put (GTK_LAYOUT (misc_layout), trans_server_combo, 160, 48);
  gtk_widget_set_size_request (trans_server_combo, 100, 31);
  g_list_foreach(trans_servers,(GFunc)add_element, (gpointer)trans_server_combo);

  view_trans_label = gtk_label_new ("View Translation");
  gtk_widget_show (view_trans_label);
  gtk_layout_put (GTK_LAYOUT (misc_layout), view_trans_label, 16, 128);
  gtk_widget_set_size_request (view_trans_label, 120, 25);

  view_trans_toggle = gtk_toggle_button_new_with_mnemonic ("gtk-yes");
  gtk_button_set_use_stock (GTK_BUTTON (view_trans_toggle), TRUE);
  gtk_widget_show (view_trans_toggle);
  gtk_layout_put (GTK_LAYOUT (misc_layout), view_trans_toggle, 160, 128);
  gtk_widget_set_size_request (view_trans_toggle, 91, 29);

  time_out_label = gtk_label_new ("Time out (Secs)");
  gtk_widget_show (time_out_label);
  gtk_layout_put (GTK_LAYOUT (misc_layout), time_out_label, 22, 200);
  gtk_widget_set_size_request (time_out_label, 120, 25);
  gtk_misc_set_alignment (GTK_MISC (time_out_label), 0, 0.5);

  time_out_spin_adj = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
  time_out_spin = gtk_spin_button_new (GTK_ADJUSTMENT (time_out_spin_adj), 1, 0);
  gtk_widget_show (time_out_spin);
  gtk_layout_put (GTK_LAYOUT (misc_layout), time_out_spin, 160, 200);
  gtk_widget_set_size_request (time_out_spin, 40, 24);

  save2_button = gtk_button_new ();
  gtk_widget_show (save2_button);
  gtk_layout_put (GTK_LAYOUT (misc_layout), save2_button, 200, 256);
  gtk_widget_set_size_request (save2_button, 85, 30);

  alignment6 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment6);
  gtk_container_add (GTK_CONTAINER (save2_button), alignment6);

  hbox3 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox3);
  gtk_container_add (GTK_CONTAINER (alignment6), hbox3);

  image3 = gtk_image_new_from_stock ("gtk-yes", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image3);
  gtk_box_pack_start (GTK_BOX (hbox3), image3, FALSE, FALSE, 0);

  label21 = gtk_label_new_with_mnemonic ("Save");
  gtk_widget_show (label21);
  gtk_box_pack_start (GTK_BOX (hbox3), label21, FALSE, FALSE, 0);

  misc_label2 = gtk_label_new ("<b>Miscelleanous Options</b>");
  gtk_widget_show (misc_label2);
  gtk_frame_set_label_widget (GTK_FRAME (misc_frame), misc_label2);
  gtk_label_set_use_markup (GTK_LABEL (misc_label2), TRUE);

  misc_label = gtk_label_new ("Miscelleanous");
  gtk_widget_show (misc_label);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 2), misc_label);

  g_signal_connect ((gpointer) lingua_franca_win, "delete_event",
                    G_CALLBACK (on_pref_window_delete_event),
                    NULL);
  g_signal_connect ((gpointer) lingua_franca_win, "destroy_event",
                    G_CALLBACK (on_pref_window_destroy_event),
                    NULL);
  g_signal_connect ((gpointer) save_button, "clicked",
                    G_CALLBACK (on_save_button_clicked),
                    (guint *)1);
  g_signal_connect_after ((gpointer) incoming_combo, "changed",
                          G_CALLBACK (on_incoming_combo_changed),
                        (guint *)1);
  g_signal_connect_after ((gpointer) outgoing_combo, "changed",
                          G_CALLBACK (on_outgoing_combo_changed),
                         (guint *)2);
  g_signal_connect_after ((gpointer)trans_server_combo, "changed",
                          G_CALLBACK (on_trans_server_combo_changed),
                         (guint *) 3);
  g_signal_connect ((gpointer) time_out_spin, "value-changed",
                    G_CALLBACK (on_time_out_spin_value_changed),
                    NULL);
  g_signal_connect ((gpointer) save2_button, "clicked",
                    G_CALLBACK (on_save_button_clicked),
                    (guint *)2);

  g_signal_connect ((gpointer)view_trans_toggle, "toggled",
                    G_CALLBACK (on_view_trans_toggle_toggled),
                    (guint *)10000);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (ret, ret, "lingua_franca");
  GLADE_HOOKUP_OBJECT (ret, notebook, "notebook");
  GLADE_HOOKUP_OBJECT (ret, general_frame, "general_frame");
  GLADE_HOOKUP_OBJECT (ret, alignment1, "alignment1");
  GLADE_HOOKUP_OBJECT (ret, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (ret, general_layout, "general_layout");
  GLADE_HOOKUP_OBJECT (ret, incoming_label, "incoming_label");
  GLADE_HOOKUP_OBJECT (ret, save_button, "save_button");
  GLADE_HOOKUP_OBJECT (ret, save_align, "save_align");
  GLADE_HOOKUP_OBJECT (ret, hbox4, "hbox4");
  GLADE_HOOKUP_OBJECT (ret, image4, "image4");
  GLADE_HOOKUP_OBJECT (ret, save_label1, "save_label1");
  GLADE_HOOKUP_OBJECT (ret, incoming_combo, "incoming_combo");
  GLADE_HOOKUP_OBJECT (ret, outgoing_label, "outgoing_label");
  GLADE_HOOKUP_OBJECT (ret, outgoing_combo, "outgoing_combo");
  GLADE_HOOKUP_OBJECT (ret, general_label, "general_label");
  GLADE_HOOKUP_OBJECT (ret, general_label2, "general_label2");
  GLADE_HOOKUP_OBJECT (ret, buddy_frame, "buddy_frame");
  GLADE_HOOKUP_OBJECT (ret, alignment3, "alignment3");
  GLADE_HOOKUP_OBJECT (ret, scrolledwindow2, "scrolledwindow2");
  GLADE_HOOKUP_OBJECT (ret, viewport1, "viewport1");
  GLADE_HOOKUP_OBJECT (ret, buddy_table, "buddy_table");
  GLADE_HOOKUP_OBJECT (ret, bname_label, "bname_label");
  GLADE_HOOKUP_OBJECT (ret, lp_label, "lp_label");
  GLADE_HOOKUP_OBJECT (ret, enable_label, "enable_label");
  GLADE_HOOKUP_OBJECT (ret, buddyconfig_label, "buddyconfig_label");
  GLADE_HOOKUP_OBJECT (ret, misc_frame, "misc_frame");
  GLADE_HOOKUP_OBJECT (ret, alignment4, "alignment4");
  GLADE_HOOKUP_OBJECT (ret, scrolledwindow4, "scrolledwindow4");
  GLADE_HOOKUP_OBJECT (ret, misc_layout, "misc_layout");
  GLADE_HOOKUP_OBJECT (ret, trans_server_label, "trans_server_label");
  GLADE_HOOKUP_OBJECT (ret, trans_server_combo, "trans_server_combo");
  GLADE_HOOKUP_OBJECT (ret, view_trans_label, "view_trans_label");
  GLADE_HOOKUP_OBJECT (ret, view_trans_toggle, "view_trans_toggle");
  GLADE_HOOKUP_OBJECT (ret, time_out_label, "time_out_label");
  GLADE_HOOKUP_OBJECT (ret, time_out_spin, "time_out_spin");
  GLADE_HOOKUP_OBJECT (ret, save2_button, "save2_button");
  GLADE_HOOKUP_OBJECT (ret, alignment6, "alignment6");
  GLADE_HOOKUP_OBJECT (ret, hbox3, "hbox3");
  GLADE_HOOKUP_OBJECT (ret, image3, "image3");
  GLADE_HOOKUP_OBJECT (ret, label21, "label21");
  GLADE_HOOKUP_OBJECT (ret, misc_label2, "misc_label2");
  GLADE_HOOKUP_OBJECT (ret, misc_label, "misc_label");
  GLADE_HOOKUP_OBJECT_NO_REF (ret, tooltips, "tooltips");

  printf("interface.c: create_ui exiting \n");
  gtk_widget_show_all(ret);
  return ret;
}

void interface_init(GList *buddies_list,GList *servers,GList *langs)
{
   printf("interface.c: interface_init entered \n");

   buddies = buddies_list ;
   trans_servers = servers ;
   languages  = langs ;

   xml_ui_init();

   printf("interface.c: interface_init exiting \n");
}

