/**
 * @file callbacks.c
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
#include "callbacks.h"

gboolean
on_pref_window_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  save_preferences();
  return FALSE;
}


gboolean
on_pref_window_destroy_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  save_preferences();
  return FALSE;
}


void
on_save_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  g_print(" %d save button is clicked \n",(guint *)user_data);
  save_preferences();
}


void
on_my_combo_changed              (GtkComboBox     *combo,
                                        gpointer         user_data)
{
  set_lang_pref("MY_LANG",gtk_combo_box_get_active_text(combo));
}


void
on_outgoing_combo_changed              (GtkComboBox     *combo,
                                        gpointer         user_data)
{
  g_print("Outgoing Lang %s is selected \n", gtk_combo_box_get_active_text(combo)) ;
  set_lang_pref("ALL",gtk_combo_box_get_active_text(combo));
}


void
on_buddy_combo_changed                 (GtkComboBox     *combo,
                                        gpointer         user_data)
{
  g_print("Buddy %s Lang %s is selected \n", (char *)user_data,gtk_combo_box_get_active_text(combo)) ;
  set_lang_pref((char *)user_data,gtk_combo_box_get_active_text(combo));
}


void
on_view_trans_toggle_toggled                (GtkToggleButton       *button,
                                        gpointer         user_data)
{
  g_print("Toggle button  %d is selected \n",(guint *)user_data ) ;
  set_view_translation(1);
}

void
on_buddy_toggle_toggled                (GtkToggleButton       *button,
                                        gpointer         user_data)
{
  g_print("Toggle button  %s is selected \n",(gchar *)user_data ) ;
  set_buddy_toggle((gchar *)user_data);
}

void
on_trans_server_combo_changed          (GtkComboBox     *combo,
                                        gpointer         user_data)
{
  g_print("Translate Server %s is selected \n", gtk_combo_box_get_active_text(combo)) ;
  set_trans_server(gtk_combo_box_get_active_text(combo));
}


void
on_time_out_spin_value_changed               (GtkSpinButton     *spin,
                                        gpointer         user_data)
{
  g_print("Timeout Value %d is selected \n", gtk_spin_button_get_value_as_int(spin)) ;
  set_time_out( gtk_spin_button_get_value_as_int(spin));
}

