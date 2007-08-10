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


#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)


gboolean on_pref_window_delete_event(GtkWidget       *widget, GdkEvent        *event, gpointer         user_data);

gboolean on_pref_window_destroy_event(GtkWidget       *widget, GdkEvent        *event, gpointer         user_data);

void on_save_button_clicked(GtkButton       *button, gpointer         user_data) ;

void on_incoming_combo_changed (GtkComboBox     *combo, gpointer         user_data);

void on_outgoing_combo_changed(GtkComboBox     *combo, gpointer         user_data);

void on_buddy_combo_changed    (GtkComboBox     *combo, gpointer         user_data);

void on_view_trans_toggle_toggled(GtkToggleButton       *button, gpointer         user_data);

void on_buddy_toggle_toggled (GtkToggleButton       *button, gpointer         user_data);

void on_trans_server_combo_changed          (GtkComboBox     *combo, gpointer         user_data);

void on_time_out_spin_value_changed               (GtkSpinButton     *spin, gpointer         user_data);
