/*
 *  universalConverter, a tool for converting table like text 
 *  files in to other formats, for Linux and Windows.
 *  Copyright (C) 2016  Florian Bärwolf
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  
 */




// GTK Applet Functions
#ifndef GTKSTUFF_HPP
#define GTKSTUFF_HPP

#include <string>
#include <cstring>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
// 
#include <vector>
#include <locale.h>
#include <iostream>

#include "globalvars.hpp"
#include "tools.hpp"
#include "functions.hpp"

// #define DEBUG

using namespace std;



#ifdef __cplusplus
extern "C" {
#endif
  
// static void
// drag_data_received_handl
// (GtkWidget *widget, GdkDragContext *context, gint x, gint y,
//         GtkSelectionData *selection_data, guint target_type, guint time,
//         gpointer data);
  
void DnDreceive (GtkWidget *widget, GdkDragContext *context, gint x, gint y,
GtkSelectionData *data, guint ttype, guint time, gpointer *NA);  
  
G_MODULE_EXPORT void received_clipboard_data();  
  
void dialog_show(GtkWidget *widget, gpointer data);

G_MODULE_EXPORT void info_accept(GtkWidget *widget, gpointer data);

G_MODULE_EXPORT void dialog_accept (GtkWidget *widget, gpointer data);

G_MODULE_EXPORT void dialog_close(GtkWidget *widget, gpointer data);

G_MODULE_EXPORT void toggle_remove_headers_checkbutton(GtkWidget *widget, gpointer data);

bool print_gtkoverlay_params_to_gtkoverlay();

G_MODULE_EXPORT void startup_focus_event(GtkWidget *widget, gpointer data);
  
G_MODULE_EXPORT void trigger();

bool delete_profile();

bool save_profile();
  
G_MODULE_EXPORT void delete_profile_button_clicked(GtkWidget *widget, gpointer data);
  
G_MODULE_EXPORT void load_profile_button_clicked(GtkWidget *widget, gpointer data);
  
G_MODULE_EXPORT void save_profile_button_clicked(GtkWidget *widget, gpointer data);
  
G_MODULE_EXPORT void activate_overwrite_protection(GtkWidget *widget, gpointer data);  

bool load_matrix_to_liststore(GtkTreeView *treeview, vector<vector<string> > *matrix);

bool parse_replacement_liststore();

bool set_gtktextview_content_from_name_as_string(string widget_name,string content);

string get_gtktextview_content_from_name_as_string(string textview_name);

bool set_gtkcheckbutton_active(string widget_name, bool is_active);

bool set_gtkcombobox_pos(string widget_name, int pos);

bool get_gtkcombobox_pos(string widget_name, int *pos);

string get_gtkentry_content_from_name_as_string(string widget_name);  

bool set_gtkentry_content_from_name_as_string(string widget_name,string content);

bool write_profile_list_to_combobox();
  
bool parse_output_format();

bool parse_input_files();

bool parse_output_dir();
  
bool parse_output_filename();  
  
bool parse_input_lines_type();

bool parse_input_columns_type();
  
bool clear_params();
  
bool parse_input_lines_recognition();  

bool parse_input_columns_recognition();  
  
bool parse_profile_overlay();
  
string get_delimiter_from_comboboxtext(string comboboxtext_name);

void show_box(string message,string type);

void show_error_box(string message);

void show_info_box(string message);

G_MODULE_EXPORT void error_accept(GtkWidget *widget, gpointer data);
  
G_MODULE_EXPORT void set_input_name_delimiter_free(GtkWidget *widget, gpointer data);
  
G_MODULE_EXPORT void set_input_delimiter_free(GtkWidget *widget, gpointer data);

G_MODULE_EXPORT void set_output_delimiter_free(GtkWidget *widget, gpointer data);



bool get_all_delimiters();

bool parse_input_output_overlay(); 
  
G_MODULE_EXPORT void on_button_clicked(GtkWidget *widget, gpointer data);

G_MODULE_EXPORT void on_window_destroy (GtkWidget *widget, gpointer data);


bool clear_columns_from_treeview(GtkTreeView *treeview);

// keys

G_MODULE_EXPORT gboolean check_escape(GtkWidget *widget, GdkEventKey *event, gpointer data);

G_MODULE_EXPORT gboolean check_enter(GtkWidget *widget, GdkEventKey *event, gpointer data);

// mouse

G_MODULE_EXPORT bool signal_handler_event(GtkWidget *widget, GdkEventButton *event, gpointer func_data);

#ifdef __cplusplus
}
#endif

#endif // GTKSTUFF_HPP