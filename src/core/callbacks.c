/**
 * @file callbacks.c  Lingua Franca Core Api
 * @ingroup core
 * @Author Sashank
 * lingua-franca
 *
 * lingua-franca is the legal property of its developers
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
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)


gboolean
on_pref_window_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}


gboolean
on_pref_window_destroy_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}


void
on_save_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  g_print(" %d save button is clicked \n",(guint *)user_data);
}


void
on_incoming_combo_changed              (GtkComboBox     *combo,
                                        gpointer         user_data)
{

  g_print("Incoming Lang %s is selected \n", gtk_combo_box_get_active_text(combo)) ;
}


void
on_outgoing_combo_changed              (GtkComboBox     *combo,
                                        gpointer         user_data)
{
  g_print("Outgoin Lang %s is selected \n", gtk_combo_box_get_active_text(combo)) ;
}


void
on_buddy_combo_changed                 (GtkComboBox     *combo,
                                        gpointer         user_data)
{
  g_print("Buddy %d Lang %s is selected \n", (gint *)user_data,gtk_combo_box_get_active_text(combo)) ;
}


void
on_view_trans_toggle_toggled                (GtkToggleButton       *button,
                                        gpointer         user_data)
{
  g_print("Toggle button  %d is selected \n",(guint *)user_data ) ;
}

void
on_buddy_toggle_toggled                (GtkToggleButton       *button,
                                        gpointer         user_data)
{
  g_print("Toggle button  %d is selected \n",(guint *)user_data ) ;
}

void
on_trans_server_combo_changed          (GtkComboBox     *combo,
                                        gpointer         user_data)
{
  g_print("Translate Server %s is selected \n", gtk_combo_box_get_active_text(combo)) ;
}


void
on_time_out_spin_value_changed               (GtkSpinButton     *spin,
                                        gpointer         user_data)
{
  g_print("Timeout Value %d is selected \n", gtk_spin_button_get_value_as_int(spin)) ;
}

