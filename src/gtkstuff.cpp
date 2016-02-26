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

#include "gtkstuff.hpp"


parameter params;
map<string, map<string, string> >  gtkoverlay_params;

// teilweise nativer c-code

// #define DEBUG

#ifdef __cplusplus
extern "C" {
#endif
  
G_MODULE_EXPORT void trigger() {
    cout << "trigger" << endl;
}
  
/*gtktreeview stuff*/  

G_MODULE_EXPORT void add_line_to_gtktreeview() {
		
	GtkListStore *liststore=GTK_LIST_STORE ( gtk_builder_get_object (builder, "liststore2")); 
	GtkTreeIter *iter=NULL;
	gtk_list_store_insert_with_values (liststore, iter,-1,0,"changeme",1,"changeme",-1);
	
}

void remove_rows(GtkTreePath *path) {
	GtkTreeView *treeview = GTK_TREE_VIEW ( gtk_builder_get_object (builder, "REPLACEMENT_TREEVIEW"));
// 	GtkListStore *liststore=GTK_LIST_STORE ( gtk_builder_get_object (builder, "liststore2")); 
	GtkTreeModel *model = gtk_tree_view_get_model(treeview);
	GtkTreeIter   iter,child_iter;
	
	if (!gtk_tree_model_get_iter(model, &iter, path))
	      return; /* path describes a non-existing row - should not happen */
// 	      gtk_tree_model_sort_convert_iter_to_child_iter(model,&child_iter,&iter);
	gtk_list_store_remove(GTK_LIST_STORE(model), &iter); 
}

bool remove_lines_from_liststore(GtkTreeView *treeview) {
	GtkListStore *liststore = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));
	GtkTreeModel *model = gtk_tree_view_get_model(treeview);
	GtkTreeIter iter;
	string path=string("0"); // "line"
	if (!gtk_tree_model_get_iter(model, &iter, gtk_tree_path_new_from_string(path.c_str()))) return false;
	while (gtk_list_store_remove(liststore,&iter));
	return true;
}

/*should be just one, else exception*/
G_MODULE_EXPORT void remove_selected_lines_from_REPLACEMENT_TREEVIEW() {
	GtkTreeView *treeview = GTK_TREE_VIEW ( gtk_builder_get_object (builder, "REPLACEMENT_TREEVIEW"));
	GtkListStore *liststore=GTK_LIST_STORE ( gtk_builder_get_object (builder, "liststore2")); 
	GtkTreeModel *model = gtk_tree_view_get_model(treeview);
	GtkTreeIter   iter;
	GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
// 	gtk_tree_selection_selected_foreach(selection, view_selected_foreach_func, NULL);
	GList *rows = gtk_tree_selection_get_selected_rows (selection,&model);   
	      
	g_list_foreach(rows, (GFunc)remove_rows, NULL);
// 	g_list_foreach(rows, (GFunc)g_free, NULL);
	
	g_list_free_full (rows, (GDestroyNotify) gtk_tree_path_free);
}

gint
  get_col_number_from_tree_view_column (GtkTreeViewColumn *col)
  {
    GtkTreeView *treeview = GTK_TREE_VIEW ( gtk_builder_get_object (builder, "REPLACEMENT_TREEVIEW"));
    GList *cols;
    gint   num;

    g_return_val_if_fail ( col != NULL, -1 );
    g_return_val_if_fail ( treeview != NULL, -1 );

    cols = gtk_tree_view_get_columns(treeview);

    num = g_list_index(cols, (gpointer) col);
//     cout << num << endl;

    g_list_free(cols);

    return num;
  }

  
G_MODULE_EXPORT void replacements_edited(GtkCellRendererText *renderer,gchar *path,gchar *new_text,gpointer user_data) {
// 	cout << "text: " << new_text << endl;
// 	cout << "path: " << path << endl;
	GtkTreeView *treeview = GTK_TREE_VIEW ( gtk_builder_get_object (builder, "REPLACEMENT_TREEVIEW"));
	GtkListStore *liststore=GTK_LIST_STORE ( gtk_builder_get_object (builder, "liststore2")); 
	GtkTreeModel *model = gtk_tree_view_get_model(treeview);
	GtkTreeIter   iter;
	int columnd_id=get_col_number_from_tree_view_column((GtkTreeViewColumn*)user_data);
	
	if (!gtk_tree_model_get_iter(model, &iter, gtk_tree_path_new_from_string(path)))
	      return; /* path describes a non-existing row - should not happen */
// 	if (renderer == GTK_CELL_RENDERER ( gtk_builder_get_object (builder, "cellrenderertext3"))  columnd_id=0;
// 	else columnd_id=1;
	    
	GValue dumps = {0,}; // 0 initialisierung, extrem wichtig!!!11
	g_value_init (&dumps, G_TYPE_STRING);
	g_value_set_string(&dumps,new_text);
			
	    
	gtk_list_store_set_value(liststore,&iter,columnd_id, &dumps);

}

G_MODULE_EXPORT void create_replacement_table() {
	
	/*iter*/
	GtkTreeIter *iter=NULL;
	/*treeview*/
	GtkTreeView *treeview = GTK_TREE_VIEW ( gtk_builder_get_object (builder, "REPLACEMENT_TREEVIEW"));
	if (!treeview) {
#ifdef DEBUG
		cout << "treeview ist NULL" << endl;
#endif
	}	
	
	/*columns*/
	GtkTreeViewColumn *col0 = gtk_tree_view_get_column(treeview,0);
	GtkTreeViewColumn *col1 = gtk_tree_view_get_column(treeview,1);
	/*cellrenderer*/
	GtkCellRenderer *renderer0 = GTK_CELL_RENDERER ( gtk_builder_get_object (builder, "cellrenderertext3"));
	GtkCellRenderer *renderer1 = GTK_CELL_RENDERER ( gtk_builder_get_object (builder, "cellrenderertext4"));
	
	/*column titles and types*/
// 	gtk_tree_view_insert_column_with_attributes (treeview,-1,"replace this",renderer0,"text",0,NULL);
// 	gtk_tree_view_insert_column_with_attributes (treeview,-1,"with",renderer1,"text",1,NULL);    
	
	g_object_set(renderer0, "editable", TRUE, NULL); 
	g_object_set(renderer1, "editable", TRUE, NULL);
	
	/*liststore*/
	GtkListStore *liststore = GTK_LIST_STORE( gtk_builder_get_object (builder, "liststore2"));
	
	GtkTreeSortable *sortable = GTK_TREE_SORTABLE(liststore);
	for (int i=0;i<2;i++) {
		gtk_tree_view_column_set_sort_column_id(gtk_tree_view_get_column (treeview,i), i);
		gtk_tree_sortable_set_sort_column_id(sortable, i, GTK_SORT_ASCENDING);
		gtk_tree_view_column_set_resizable (gtk_tree_view_get_column (treeview,i),TRUE);
	}
	
	
// 	gtk_list_store_insert_with_values (liststore, iter,-1,0,"A1",1,"B1",-1);
// 	gtk_list_store_insert_with_values (liststore, iter,-1,0,"A2",1,"B2",-1);
	
	gtk_tree_view_set_model (treeview, GTK_TREE_MODEL(liststore));
	return;
}

// bool print_matrix_in_treeview(vector<vector<string>> *matrix, vector<string> column_types) {
// 	setlocale(LC_ALL, "C"); 
// 	GtkCellRenderer *renderer;
// 	GtkTreeSortable *sortable;
// // 	vector<string> column_types;
// 	GtkTreeIter iter;
// 	GtkTreeView *treeview = GTK_TREE_VIEW ( gtk_builder_get_object (builder, "treeview1"));  // gtk_tree_view_new ()
// 	if (!treeview) {cout << "treeview ist NULL" << endl;}
// 	GtkListStore *liststore;
// 	if (!liststore) {cout << "liststore ist NULL" << endl;}
// 	clear_columns_from_treeview(treeview);
// 	
// 	int size=matrix->size();
// 	if (size<1) return false;
// 	size=(matrix->at(0)).size();
// // 	get_type_vector(&matrix->at(1),&column_types);
// 	GType types [size];
// 	int pos [size];
// 	for (int i=0; i<size;i++) {
// 		if (column_types[i]=="double") types[i] =  G_TYPE_DOUBLE;
// 		else if (column_types[i]=="int") types[i] =  G_TYPE_INT;
// 		else types[i] =  G_TYPE_STRING;
// 		pos[i] = i;
// 		#ifdef DEBUG
// 	cout << "type["<<i<<"]: " << column_types[i] << endl; 
// #endif
// 		renderer = gtk_cell_renderer_text_new ();
// 		gtk_tree_view_insert_column_with_attributes (treeview,-1,(matrix->at(0))[i].c_str(),renderer,"text",(gint) i,NULL); // 		
// 	}
// 	liststore = gtk_list_store_newv (size,const_cast<GType*>(types));
// 	sortable = GTK_TREE_SORTABLE(liststore);
// 	for (int i=0;i<size;i++) {
// 		gtk_tree_view_column_set_sort_column_id(gtk_tree_view_get_column (treeview,i), i);
// 		gtk_tree_sortable_set_sort_column_id(sortable, i, GTK_SORT_ASCENDING);
// 		gtk_tree_view_column_set_resizable (gtk_tree_view_get_column (treeview,i),TRUE);
// 	}
// 	
// 	
// 	double temp_double;
// 	int temp_int;
// 	string temp_string;
// 	GValue *dumps;
// 	for (int i=1; i<matrix->size();i++) {
// 		size = (matrix->at(i)).size();
// 		dumps = (GValue*) alloca (sizeof (GValue) * size);
// 		memset (dumps, 0, sizeof (GValue) * size);
// 		for (int j=0;j<size;j++) {
// 			if (column_types[j]=="double") {
// 				g_value_init (&(dumps [j]), G_TYPE_DOUBLE);
// 				g_value_set_double (&(dumps [j]), str_to_double((matrix->at(i))[j]));
// // 				cout << "str_to_double((matrix->at(i))[j]): " << str_to_double((matrix->at(i))[j]) << endl;
// 			}
// 			else if (column_types[j]=="int") {
// 				g_value_init (&(dumps [j]), G_TYPE_INT);
// 				g_value_set_int (&(dumps [j]), str_to_int((matrix->at(i))[j]));
// 			}
// 			else {
// 				g_value_init (&(dumps [j]), G_TYPE_STRING);
// 				g_value_set_string (&(dumps [j]), ((matrix->at(i))[j]).c_str());
// 		
// 			}	
// 		}
// 		gtk_list_store_insert_with_valuesv (liststore, &iter,-1,const_cast<gint*>(pos),dumps,size);
//   	}
//   	gtk_tree_view_set_model (treeview, GTK_TREE_MODEL(liststore));
// 
// 	return true;
// }

/*******************/





G_MODULE_EXPORT void startup_focus_event(GtkWidget *widget, gpointer data) {
	get_profile_list();
	write_profile_list_to_combobox();
	create_replacement_table();
}  
  
G_MODULE_EXPORT void load_profile_button_clicked(GtkWidget *widget, gpointer data) {
	parse_profile_overlay();
	load_profile(params.profile.name);
}

G_MODULE_EXPORT void received_clipboard_data() {
// 	GdkDisplay *display = gdk_display_get_default();
// 	GtkClipboard *clipboard = gtk_clipboard_get_default (display);
// 	GdkAtom target_type;
// 	target_type = GDK_POINTER_TO_ATOM (widget); /* Choose the best target type */
	GdkAtom atom = gdk_atom_intern("atom_name",TRUE);
	GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	string * text_string;
// 	gchar * text = gtk_clipboard_wait_for_text(clipboard);
	GtkSelectionData *data = gtk_clipboard_wait_for_contents(clipboard,atom);
	if (data==NULL) {cout << "NULL" << endl; return;}
	guchar * text = gtk_selection_data_get_text(data);
	cout << text_string << endl;
}

G_MODULE_EXPORT void received_drag_data(/*GtkWidget *widget, GdkDragContext *context, GtkSelectionData *data, guint info, guint time, gpointer user_data*/) {
// G_MODULE_EXPORT void received_drag_data(GtkWidget *widget, gpointer data) {
// 	if ((data->length >= 0) /*&& (data->format == 8)*/) {
		cout << "get data" << endl;
// 		const guchar * text = gtk_selection_data_get_data(data);
		
// 		cout << *data << endl;
// 		cout << "laenge: " << gtk_selection_data_get_length(data) << endl;
// 		string text_string = (string(reinterpret_cast<const char*>(text)));
// 		cout << sizeof(text) << endl;
// 	}
}

G_MODULE_EXPORT void received_drop_file_event(GtkWidget *widget, GdkDragContext *context, gint x, gint y, GtkSelectionData *data, guint ttype, guint time, gpointer *NA) {
	cout << "files dropped" << endl;
}


bool print_gtkoverlay_params_to_gtkoverlay() {
	for (map<string,map<string,string> >::iterator it=gtkoverlay_params.begin(); it!=gtkoverlay_params.end(); ++it) {
		if (it->second["type"]=="gtktextview") {
			set_gtktextview_content_from_name_as_string(it->first,it->second["content"]);
		} else if (it->second["type"]=="gtkcheckbutton") {
			if (it->second["content"]=="1") set_gtkcheckbutton_active(it->first,true);
			else set_gtkcheckbutton_active(it->first,false);
		} else if (it->second["type"]=="gtkentry") {
			set_gtkentry_content_from_name_as_string(it->first,it->second["content"]);
		} else if (it->second["type"]=="gtkcombobox") {
			set_gtkcombobox_pos(it->first,Tools::str_to_int(it->second["content"]));
		} else if (it->second["type"]=="liststore" || it->second["type"]=="treeview") {  //load_matrix_to_liststore
// 			set_gtkcombobox_pos(it->first,Tools::str_to_int(it->second["content"]));
			vector<vector<string> > liststore_matrix = Tools::format_string_to_matrix(&(it->second["content"]),"\n",config_params["config_delimiter"]);
// 			Print::matrix_string(&liststore_matrix);
// 			remove_lines_from_liststore(GTK_TREE_VIEW ( gtk_builder_get_object (builder, (it->first).c_str())));
			load_matrix_to_liststore(GTK_TREE_VIEW ( gtk_builder_get_object (builder, (it->first).c_str())),&liststore_matrix);
		} 
		
		else {
			show_error_box("unkown gtk type: "+it->second["type"]);
		}
	}
	return true;
}

bool set_gtkcheckbutton_active(string widget_name, bool is_active) {
	GtkToggleButton *togglebutton = GTK_TOGGLE_BUTTON( gtk_builder_get_object (builder, widget_name.c_str()));
	gtk_toggle_button_set_active(togglebutton, is_active);
	return true;
}

bool set_gtktextview_content_from_name_as_string(string widget_name,string content) {
	
	GtkTextView *textview = NULL;
	GtkTextBuffer *textview_buffer = NULL;
  
	textview=GTK_TEXT_VIEW( gtk_builder_get_object (builder, widget_name.c_str()));
	if (!textview) {
	      return false;
	}
	textview_buffer=gtk_text_view_get_buffer(textview);
	
	gtk_text_buffer_set_text (textview_buffer,content.c_str(),content.length());
	return true;
}

bool add_gtktextview_content_from_name_as_string(string widget_name,string content) {
	
	GtkTextView *textview = NULL;
	GtkTextBuffer *textview_buffer = NULL;
  
	textview=GTK_TEXT_VIEW( gtk_builder_get_object (builder, widget_name.c_str()));
	if (!textview) {
	      return false;
	}
	textview_buffer=gtk_text_view_get_buffer(textview);
	
	string old_content = get_gtktextview_content_from_name_as_string(widget_name.c_str());
// 	if (old_content!="") old_content.append(config_params["line_delimiter"]); 
	content = content+config_params["line_delimiter"]+ old_content;
	gtk_text_buffer_set_text (textview_buffer,content.c_str(),content.length());
	return true;
}


bool set_gtkentry_content_from_name_as_string(string widget_name,string content) {
	GtkEntry *gtkentry = GTK_ENTRY( gtk_builder_get_object (builder, widget_name.c_str()));
	if (!gtkentry) {
	  #ifdef DEBUG 
	  cout << "output_path ist NULL" << endl;
	  #endif
	  return false;
	}
	content=Tools::remove_linebreakers_from_string(content);
	gtk_entry_set_text(gtkentry, content.c_str());
	return true;
}

bool save_profile() {
	  // transform overlay_params to xml
    parse_input_output_overlay();
//     parse_profile_overlay();
    string xml;
    transform_gtkoverlay_params_to_xml(&xml);
    Tools::mkpath(config_params["profiles_dir"],0750);
    string file = config_params["profiles_dir"]+params.profile.name+config_params["profile_ending"];
    Tools::write_to_file(file,&xml,false);
    get_profile_list();
    write_profile_list_to_combobox();
    params.profile.action="";
}


G_MODULE_EXPORT void save_profile_button_clicked(GtkWidget *widget, gpointer data) {
    // load all overlay params
//     parse_input_output_overlay();
    bool exec=true;
    parse_profile_overlay();
    /*check wheter overwrite, ask for overwrite*/
    for (int i=0;i<params.profile.list.size();i++) {
	if (params.profile.list[i]==params.profile.name) {
		/*overwrite - sure?*/
		exec=false;
		params.profile.action="save";
		show_info_box("are you sure to overwrite?");
	}
    }
    if (exec) {
		save_profile();
		params.profile.action="";
    }
}  

bool delete_profile() {
	parse_profile_overlay();
	if (!delete_profile(params.profile.name)) {
		show_error_box("could NOT delete profile");
		return false;
	}
	get_profile_list();
	write_profile_list_to_combobox();
	params.profile.action="";
	return true;
}

G_MODULE_EXPORT void delete_profile_button_clicked(GtkWidget *widget, gpointer data) {
	params.profile.action="delete";
	show_info_box("are you sure to delte?");
}

G_MODULE_EXPORT void on_button_clicked(GtkWidget *widget, gpointer data) {
    #ifdef DEBUG
    cout << "on_button_clicked" << endl;
    #endif
    clear_params();
    parse_input_output_overlay();
    
    execution();
}

bool write_profile_list_to_combobox() {
	string widget_name="VORLAGENLISTE_COMBOBOX";
	GtkComboBoxText *comboboxtext = GTK_COMBO_BOX_TEXT( gtk_builder_get_object (builder, widget_name.c_str())); // 
	if (!comboboxtext) {
	  #ifdef DEBUG 
	  cout << "VORLAGENLISTE_COMBOBOX ist NULL" << endl;
	  #endif
	  return false;
	}
	gtk_combo_box_text_remove_all (comboboxtext);
	for (int i=0; i<params.profile.list.size();i++) {
		gtk_combo_box_text_insert(comboboxtext,i,NULL,params.profile.list.at(i).c_str());
	}
	return true;
}

bool parse_output_format() {
	string widget_name = "AUSGABEFORMAT_TEXTVIEW";
	string output_format_string;
	string line;
	vector<string> columns_in_line;
	output_format_string = get_gtktextview_content_from_name_as_string(widget_name.c_str());
	if (output_format_string.size()<1) {
// 		show_error_box("no output format");
	}
	gtkoverlay_params[widget_name]["content"]=output_format_string;
	gtkoverlay_params[widget_name]["type"]="gtktextview";
	
	params.output.format=Tools::format_string_to_matrix(&output_format_string,"\n",params.output.data_delimiter);
	return true;
}
  
  
// vector<vector<string> > get_matrix_from_gtktreemodel(GtkTreeModel *model) {
// 	return vector<vector<string> > matrix;
// // 	cols= gtk_tree_view_get_columns();
// // 	gtk_tree_model_get_value();
// }
//   

vector<string> get_liststore_column_entries(GtkTreeView *treeview, int col_num) {
	vector<string> strings;
	GtkTreeModel *model = gtk_tree_view_get_model(treeview);
	GtkTreeIter iter;
	string path=string("0"); // "line"
	if (!gtk_tree_model_get_iter(model, &iter, gtk_tree_path_new_from_string(path.c_str()))) return strings;
	GValue value ={0,};
	do  { //iterate all lines for this col
		gtk_tree_model_get_value(model,&iter,col_num,&value);
		strings.push_back(g_value_get_string(&value));
	} while (gtk_tree_model_iter_next(model,&iter));
	g_value_unset(&value);
	return strings;
}

bool load_matrix_to_liststore(GtkTreeView *treeview, vector<vector<string> > *matrix) {
	GtkTreeModel *model = gtk_tree_view_get_model(treeview);
// 	cout << "Removelines begin" << endl;
	remove_lines_from_liststore(treeview);
// 	cout << "Removelines end" << endl;
	GtkListStore *liststore = GTK_LIST_STORE(model);
	int size;
	
// 	cout << "pos size set" << endl;
	
	string path=string("0"); // "line"
// 	cout << "path string" << endl;
	
	GValue *dumps;
	for (int i=0;i<(matrix->size());i++) {
// 		cout << "Begin: " << i << endl;
		size = (matrix->at(i)).size();
		if (size<1) {
			cout << "load_matrix_to_liststore FALSE" << endl;
			return false;
		}
		int pos [size];
		dumps = (GValue*) alloca (sizeof (GValue) * size);
		memset (dumps, 0, sizeof (GValue) * size);
		for (int j=0;j<size;j++) {
			pos[j]=j;
			g_value_init (&(dumps[j]), G_TYPE_STRING);
			g_value_set_string (&(dumps[j]), ((matrix->at(i))[j]).c_str());
		}
		gtk_list_store_insert_with_valuesv (liststore, NULL,-1,const_cast<gint*>(pos),dumps,size);
// 		cout << "End: " << i << endl;
	}
	return true;
}

bool parse_replacement_liststore() {
// 	vector<vector<string> > matrix;
	string widget_name="REPLACEMENT_TREEVIEW";
	vector<string> replace_this_strings;
	vector<string> replace_with_strings;
	GtkTreeView *treeview = GTK_TREE_VIEW ( gtk_builder_get_object (builder, widget_name.c_str() ));
	replace_this_strings=get_liststore_column_entries(treeview,0);
	replace_with_strings=get_liststore_column_entries(treeview,1);
	int max_size;
	if (replace_with_strings.size()<replace_this_strings.size()) max_size=replace_this_strings.size();
	else max_size=replace_with_strings.size();
	params.input.replacements=Tools::unify_2_vectors_to_matrix(replace_this_strings,replace_with_strings);
	gtkoverlay_params[widget_name]["type"]="treeview"; 
	gtkoverlay_params[widget_name]["content"]=Tools::format_matrix_to_string(&(params.input.replacements),"\n",config_params["config_delimiter"]);
	return true;
}

bool get_button_state_by_name(string widget_name) {
	bool pressed;
	GtkToggleButton *button = GTK_TOGGLE_BUTTON( gtk_builder_get_object (builder, widget_name.c_str()));
	pressed = gtk_toggle_button_get_active(button);
	if (pressed) gtkoverlay_params[widget_name]["content"]="1";
	else gtkoverlay_params[widget_name]["content"]="0";
	gtkoverlay_params[widget_name]["type"]="gtkcheckbutton";
// 	cout <<widget_name << ": " << gtkoverlay_params[widget_name]["content"] << endl;
	return pressed;
}
  
bool parse_buttons() {
	params.output.overwrite_protection=get_button_state_by_name("WRITEPROTECTION_CHECKBUTTON");
	params.input.remove_headers=get_button_state_by_name("REMOVE_HEADER_CHECKBUTTON");
	params.output.check_lines_in_header=get_button_state_by_name("check_lines_in_header");
	params.output.check_lines_in_data=get_button_state_by_name("check_lines_in_data");
	params.output.check_columns_in_header=get_button_state_by_name("check_columns_in_header");
	params.output.check_columns_in_data=get_button_state_by_name("check_columns_in_data");
	return true;
}  
  
bool parse_input_output_overlay() {
	setlocale(LC_ALL, "C");
	if (!parse_input_files()) {
		show_error_box("Could not parse input files");
		return false;
	}
	if (!get_all_delimiters()) return false;
	if (!parse_input_lines_recognition()) return false;
	if (!parse_input_lines_type()) return false;
	if (!parse_input_columns_recognition()) return false;	
	if (!parse_input_columns_type()) return false;
	
	if (!parse_replacement_liststore()) {cout << "False" << endl;return false;}
	
	if (!parse_buttons()) return false; 	

	if (!parse_output_filename()) return false;
	if (!parse_output_dir()) return false;
	if (!parse_output_format()) return false;
	
	
	return true;
}    

// 


// G_MODULE_EXPORT void toggle_deactivate_autodetection_checkbutton(GtkWidget *widget, gpointer data) {
// 	string widget_name="DEACTIVATE_AUTODETECTION_CHECKBUTTON";
// 	if (!params.input.autodetection) {
// 		params.input.autodetection=true;
// 		gtkoverlay_params[widget_name]["content"]="1";
// 		gtkoverlay_params[widget_name]["type"]="gtkcheckbutton";
// 		#ifdef DEBUG
// 		cout << "DEACTIVATE_AUTODETECTION_CHECKBUTTON is changed to TRUE" << endl;
// 		#endif
// 	  
// 	} else {	
// 		params.input.autodetection=false;
// 		gtkoverlay_params[widget_name]["content"]="0";
// 		gtkoverlay_params[widget_name]["type"]="gtkcheckbutton";
// 		#ifdef DEBUG
// 		cout << "DEACTIVATE_AUTODETECTION_CHECKBUTTON is changed to FALSE" << endl;
// 		#endif
// 	}
// 	
// 	
// }


bool parse_output_dir() {
	string widget_name = "AUSGABEVERZEICHNIS_ENTRY";
	GtkEntry *output_path = GTK_ENTRY( gtk_builder_get_object (builder, widget_name.c_str()));
	if (!output_path) {
	  #ifdef DEBUG 
	  cout << "output_path ist NULL" << endl;
	  #endif
	}
	params.output.directory=gtk_entry_get_text(output_path);
	
// 	params.output.directory=check_directory_string(params.output.directory);
	
	/*
	if (params.output.directory.compare(params.output.directory.size()-config_params["path_delimiter"].size(),config_params["path_delimiter"].size(),config_params["path_delimiter"])!=0) {
		params.output.directory.append(config_params["path_delimiter"]);
	}
	*/
	
	gtkoverlay_params[widget_name]["content"]=params.output.directory;
	gtkoverlay_params[widget_name]["type"]="gtkentry";
	return true;
}
  
bool parse_output_filename() {
	string widget_name = "OUTPUT_NAME_ENTRY";
	GtkEntry *output_filename = GTK_ENTRY( gtk_builder_get_object (builder, widget_name.c_str()));
	if (!output_filename) {
	  #ifdef DEBUG 
	  cout << "output_filename ist NULL" << endl;
	  #endif
	}
	params.output.filename=gtk_entry_get_text(output_filename);
	#ifdef DEBUG 
	  cout << params.output.filename << endl;
	  #endif
	
	
	  
	gtkoverlay_params[widget_name]["content"]=params.output.filename;
	gtkoverlay_params[widget_name]["type"]="gtkentry";
	return true;
}  
  
G_MODULE_EXPORT void set_input_delimiter_free(GtkWidget *widget, gpointer data) {
	GtkComboBox *input_delimiter_box = GTK_COMBO_BOX( gtk_builder_get_object (builder, "Eingabe_Trennzeichen_Box"));
	if (!input_delimiter_box) {
	  #ifdef DEBUG 
	  cout << "input_delimiter_box ist NULL" << endl;
	  #endif
	}
	gtk_combo_box_set_active(input_delimiter_box,2);
}
G_MODULE_EXPORT void set_output_delimiter_free(GtkWidget *widget, gpointer data) {
	GtkComboBox *output_delimiter_box = GTK_COMBO_BOX( gtk_builder_get_object (builder, "Ausgabe_Trennzeichen_Box"));
	if (!output_delimiter_box) {
	  #ifdef DEBUG 
	  cout << "output_delimiter_box ist NULL" << endl;
	  #endif
	}
	gtk_combo_box_set_active(output_delimiter_box,2);
}
G_MODULE_EXPORT void set_input_name_delimiter_free(GtkWidget *widget, gpointer data) {
	GtkComboBox *output_delimiter_box = GTK_COMBO_BOX( gtk_builder_get_object (builder, "Eingabe_Namen_Trennzeichen_Box"));
	if (!output_delimiter_box) {
	  #ifdef DEBUG 
	  cout << "output_delimiter_box ist NULL" << endl;
	  #endif
	}
	gtk_combo_box_set_active(output_delimiter_box,4);
}

string get_gtktextview_content_from_name_as_string(string textview_name) {
	GtkTextIter startIter;
	GtkTextIter endIter;
	GtkTextView *textview = NULL;
	GtkTextBuffer *textview_buffer = NULL;
	string textview_buffer_string="";
  
	textview=GTK_TEXT_VIEW( gtk_builder_get_object (builder, textview_name.c_str()));
	textview_buffer=gtk_text_view_get_buffer(textview);
	gtk_text_buffer_get_start_iter(textview_buffer, &startIter);
	gtk_text_buffer_get_end_iter(textview_buffer, &endIter);
	textview_buffer_string=string(gtk_text_buffer_get_text(textview_buffer,&startIter,&endIter,FALSE));
	
	return textview_buffer_string;
}

string get_gtkentry_content_from_name_as_string(string widget_name) {
	string result;
	GtkEntry *gtkentry = GTK_ENTRY( gtk_builder_get_object (builder, widget_name.c_str()));
	if (!gtkentry) {
	  #ifdef DEBUG 
	  cout << "output_path ist NULL" << endl;
	  #endif
	  return "";
	}
	result=gtk_entry_get_text(gtkentry);
	gtkoverlay_params[widget_name]["content"]=result;
	gtkoverlay_params[widget_name]["type"]="gtkentry";
	return result;
}


bool parse_input_files() {
	string widget_name = "EINGABEDATEIEN_TEXTVIEW";
	vector<string> filenames;
	filenames.clear();
	string filenames_string=get_gtktextview_content_from_name_as_string(widget_name.c_str());
	Tools::replace_chars_from_string(&filenames_string, "file://","");
	
	
	/* unmark if u want to save filenames also */ 
	//gtkoverlay_params[widget_name]["content"]=filenames_string;
	//gtkoverlay_params[widget_name]["type"]="gtktextview";
	/*                                         */
	
	filenames = Tools::get_strings_between_delimiter(filenames_string,"\n");
	filenames_string.clear();
	params.input.files.clear();
	for (int i=0; i<filenames.size();i++) {
		params.input.files.push_back(Tools::remove_linebreakers_from_string(filenames.at(i)));
	}
	if (params.input.files.size()>0) return true;
	else return false;
}

bool parse_input_lines_recognition() {
	string widget_name = "ZEILEN_TEXTVIEW";
	string delimiter_discrete=",";
	string delimiter_conti=":";
	
	string lines_string = get_gtktextview_content_from_name_as_string(widget_name.c_str());
	if (lines_string=="") params.input.lines.clear();
	
// 	gtkoverlay_params[widget_name]=lines_string;
	gtkoverlay_params[widget_name]["content"]=lines_string;
	gtkoverlay_params[widget_name]["type"]="gtktextview";
	Tools::replace_chars_from_string(&lines_string,"\r\n",delimiter_discrete); // win
	Tools::replace_chars_from_string(&lines_string,"\n",delimiter_discrete); // unix
	
	vector<string> elements = Tools::get_strings_between_delimiter(lines_string,delimiter_discrete);
	vector<string> sub_elements;
	vector<int> temp;
	for (int i=0;i<elements.size();i++) {
		  sub_elements.clear();
		  sub_elements=Tools::get_strings_between_delimiter(elements.at(i),delimiter_conti);
		  if (sub_elements.size()==1) {
			params.input.lines.push_back(Tools::str_to_int(sub_elements.at(0)));
// 			cout << sub_elements.at(0) << endl;
		  } else if (sub_elements.size()>1) {
			// ergänze alle werte zwischen min und max  (min:max)
			temp.clear();
			for (int j=0;j<sub_elements.size();j++) {
				if ((sub_elements.at(j)).size()>0) temp.push_back(Tools::str_to_int(Tools::remove_spaces_from_string(sub_elements.at(j))));
			}
			sort(temp.begin(),temp.end());
			for (int j=temp.at(0);j<=temp.back();j++) params.input.lines.push_back(j);
		  }
	}
	// 1:5,6,8:11 =1,2,3,4,5,6,8,9,10,11
	return true;
} 

bool parse_input_columns_recognition() {
	string widget_name = "SPALTEN_TEXTVIEW";
	string delimiter_discrete=",";
	string delimiter_conti=":";

	string lines_string = get_gtktextview_content_from_name_as_string("SPALTEN_TEXTVIEW");
	if (lines_string=="") params.input.columns.clear();
// 	gtkoverlay_params[widget_name]=lines_string;
	gtkoverlay_params[widget_name]["content"]=lines_string;
	gtkoverlay_params[widget_name]["type"]="gtktextview";
	Tools::replace_chars_from_string(&lines_string,"\r\n",delimiter_discrete); // win
	Tools::replace_chars_from_string(&lines_string,"\n",delimiter_discrete); // unix
	
	vector<string> elements = Tools::get_strings_between_delimiter(lines_string,delimiter_discrete);
	vector<string> sub_elements;
	vector<int> temp;
	for (int i=0;i<elements.size();i++) {
		  sub_elements.clear();
		  sub_elements=Tools::get_strings_between_delimiter(elements.at(i),delimiter_conti);
		  if (sub_elements.size()==1) {
			params.input.columns.push_back(Tools::str_to_int(sub_elements.at(0)));
// 			cout << sub_elements.at(0) << endl;
		  } else if (sub_elements.size()>1) {
			// ergänze alle werte zwischen min und max  (min:max)
			temp.clear();
			for (int j=0;j<sub_elements.size();j++) {
				if ((sub_elements.at(j)).size()>0) temp.push_back(Tools::str_to_int(Tools::remove_spaces_from_string(sub_elements.at(j))));
			}
			sort(temp.begin(),temp.end());
			for (int j=temp.at(0);j<=temp.back();j++) params.input.columns.push_back(j);
		  }
	}
	// 1:5,6,8:11 =1,2,3,4,5,6,8,9,10,11
	return true;
} 


string get_delimiter_from_comboboxtext(string comboboxtext_name) {
	
	string result;
	GtkComboBoxText *comboboxtext = GTK_COMBO_BOX_TEXT( gtk_builder_get_object (builder, comboboxtext_name.c_str())); // 
	
	if (!comboboxtext) {
	  #ifdef DEBUG 
	  cout << "input_delimiter_box ist NULL" << endl;
	  #endif
	}
	string active_text = gtk_combo_box_text_get_active_text(comboboxtext);
// 	gtkoverlay_params[comboboxtext_name]=active_text;
	gtkoverlay_params[comboboxtext_name]["content"]=active_text;
	gtkoverlay_params[comboboxtext_name]["type"]="gtkcomboboxtext";
	if (active_text == "tabulator") {
		#ifdef DEBUG 
		cout << "Tabulator" << endl;
		#endif
		result = "\t";
	} else if (active_text == "space") {
		#ifdef DEBUG 
		cout << "Space" << endl;
		#endif
		result = " ";
	} else {
		#ifdef DEBUG 
		cout << active_text << endl;
		#endif
		result = active_text;
	}
	return result;
}

string filter_delimiter(string delimiter) {
	string result;
	if (delimiter == "tabulator") {
		#ifdef DEBUG 
		cout << "Tabulator" << endl;
		#endif
		result = "\t";
	} else if (delimiter == "space") {
		#ifdef DEBUG 
		cout << "Space" << endl;
		#endif
		result = " ";
	} else {
		#ifdef DEBUG 
		cout << delimiter << endl;
		#endif
		result = delimiter;
	}
	return result;
}
  
bool get_all_delimiters() {
	params.input.data_delimiter=filter_delimiter(get_gtkentry_content_from_name_as_string("INPUT_DATA_DELIMITER_ENTRY"));
	params.input.filename_delimiter=filter_delimiter(get_gtkentry_content_from_name_as_string("INPUT_FILE_NAME_DELIMITER_ENTRY"));
	params.output.data_delimiter=filter_delimiter(get_gtkentry_content_from_name_as_string("OUTPUT_DATA_DELIMITER_ENTRY"));
	return true;
}

bool get_all_delimiters_old() {
// 	GtkComboBox *input_delimiter_box = GTK_COMBO_BOX( gtk_builder_get_object (builder, "Eingabe_Trennzeichen_Box")); // 
	params.input.data_delimiter=get_delimiter_from_comboboxtext( "Eingabe_Trennzeichen_Box");
	if (params.input.data_delimiter=="") {
		show_error_box("input data delimiter empty");
		return false;
		
	}
	params.input.filename_delimiter=get_delimiter_from_comboboxtext( "Eingabe_Namen_Trennzeichen_Box");
	params.output.data_delimiter=get_delimiter_from_comboboxtext( "Ausgabe_Trennzeichen_Box");
	if (params.output.data_delimiter=="") {
		show_error_box("output data delimiter empty");
		return false;
	}
	
	return true;
}

bool parse_profile_overlay() {
	get_profile_list();
	string widget_name="VORLAGENLISTE_COMBOBOX";
	GtkComboBoxText *comboboxtext = GTK_COMBO_BOX_TEXT( gtk_builder_get_object (builder, widget_name.c_str())); // 
	if (!comboboxtext) {
	  #ifdef DEBUG 
	  cout << "VORLAGENLISTE_COMBOBOX ist NULL" << endl;
	  #endif
	}
	params.profile.name = gtk_combo_box_text_get_active_text(comboboxtext);
// 	gtkoverlay_params[widget_name]["content"]=params.profile.name;
// 	gtkoverlay_params[widget_name]["type"]="gtkcomboboxtext";
	if (params.profile.name=="") {
		show_error_box("profil name empty");
		return false;
	}
	
	return true;
}
  
bool set_gtkcombobox_pos(string widget_name, int pos) {
	GtkComboBox *combobox = GTK_COMBO_BOX( gtk_builder_get_object (builder, widget_name.c_str())); // 
	if (!combobox) {
	  #ifdef DEBUG 
	  cout << "COLUMNS_BOX ist NULL" << endl;
	  #endif
	  return false;
	}
	gtk_combo_box_set_active(combobox,pos);
	gtkoverlay_params[widget_name]["content"]=to_string(pos);
	gtkoverlay_params[widget_name]["type"]="gtkcombobox";
	return true;
}

bool get_gtkcombobox_pos(string widget_name, int *pos) {
	GtkComboBox *combobox = GTK_COMBO_BOX( gtk_builder_get_object (builder, widget_name.c_str())); // 
	if (!combobox) {
	  #ifdef DEBUG 
	  cout << "COLUMNS_BOX ist NULL" << endl;
	  #endif
	  return false;
	}
	*pos = (gtk_combo_box_get_active(combobox));
	gtkoverlay_params[widget_name]["content"]=to_string(*pos);
	gtkoverlay_params[widget_name]["type"]="gtkcombobox";
	return true;
}

bool parse_input_columns_type() {
	int pos;
	if (!get_gtkcombobox_pos("COLUMNS_BOX",&pos)) return false;
	params.input.columns_type=pos;
	return true;
}

bool parse_input_lines_type() {
	int pos;
	if (!get_gtkcombobox_pos("LINES_BOX",&pos)) return false;
	params.input.lines_type=pos;
	return true;
}

void add_input_files_to_EINGABEDATEIEN_TEXTVIEW(char* content) {
	string widget_name = "EINGABEDATEIEN_TEXTVIEW";
	add_gtktextview_content_from_name_as_string(widget_name,string(content));
}

G_MODULE_EXPORT void dialog_accept (GtkWidget *widget, gpointer data) {
	GtkWidget *dialog = GTK_WIDGET( gtk_builder_get_object( builder, "FILE_DIALOG" ) );
	GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	GSList *filenames = gtk_file_chooser_get_filenames (chooser);
// 	get_gtktextview_content_from_name_as_string
	set_gtktextview_content_from_name_as_string("EINGABEDATEIEN_TEXTVIEW","");
	g_slist_foreach(filenames, (GFunc)add_input_files_to_EINGABEDATEIEN_TEXTVIEW, NULL);
	g_slist_foreach(filenames, (GFunc)g_free, NULL);
	g_slist_free(filenames);
	dialog_close(widget, data);
}

G_MODULE_EXPORT void dialog_close(GtkWidget *widget, gpointer data) {
	GtkWidget *dialog;
	dialog = GTK_WIDGET( gtk_builder_get_object( builder, "FILE_DIALOG" ) );
	gtk_widget_hide(dialog);
}

G_MODULE_EXPORT void error_accept(GtkWidget *widget, gpointer data) {
	GtkWidget *errorbox;
	errorbox = GTK_WIDGET( gtk_builder_get_object( builder, "ERROR_BOX" ) );
	gtk_widget_hide(errorbox);
}

G_MODULE_EXPORT void info_accept(GtkWidget *widget, gpointer data) {
	if (params.profile.action=="save") save_profile();
	else if (params.profile.action=="delete") delete_profile();
	params.profile.action=="";
	GtkWidget *INFO_BOX;
	INFO_BOX = GTK_WIDGET( gtk_builder_get_object( builder, "INFO_BOX" ) );
	gtk_widget_hide(INFO_BOX);
// 	string widget_name = string(gtk_widget_get_name(widget));
}

G_MODULE_EXPORT void info_cancel(GtkWidget *widget, gpointer data) {
	params.profile.action=="";
	GtkWidget *INFO_BOX;
	INFO_BOX = GTK_WIDGET( gtk_builder_get_object( builder, "INFO_BOX" ) );
	gtk_widget_hide(INFO_BOX);
}

G_MODULE_EXPORT void dialog_show(GtkWidget *widget, gpointer data) {
// 	GtkMessageDialog *dialog_dialog=NULL;
  
	GtkWidget *dialog = GTK_WIDGET( gtk_builder_get_object( builder, "FILE_DIALOG" ) );
	GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	if (config_params["current_directory"]!="") {
		gtk_file_chooser_set_current_folder(chooser,config_params["current_directory"].c_str());
	}
	
	gtk_dialog_run(GTK_DIALOG(dialog));
}

void show_error_box(string message) {
	show_box(message,"ERROR_BOX");
}

void show_info_box(string message) {
	show_box(message,"INFO_BOX");
}

void show_box(string message,string type) {
// 	type = ERROR_BOX;
// 	type = INFO_BOX;
	GtkMessageDialog *dialog=NULL;
	GtkWidget *box = GTK_WIDGET( gtk_builder_get_object( builder, type.c_str() ) );
	dialog=GTK_MESSAGE_DIALOG( gtk_builder_get_object (builder, type.c_str()));
	gtk_message_dialog_set_markup(dialog, message.c_str());
	gtk_widget_show( box );
}

bool clear_params() {
	parameter new_params;
	params = new_params;
}
  


G_MODULE_EXPORT void on_window_destroy (GtkWidget *widget, gpointer data)
{
    #ifdef DEBUG
    cout << "Program exit" << endl;
    #endif
    gtk_main_quit ();
}


bool clear_columns_from_treeview(GtkTreeView *treeview) {
	GtkTreeViewColumn *column;
	column=gtk_tree_view_get_column (treeview,0);
	while (column) {
		gtk_tree_view_remove_column(treeview, column);
		column=gtk_tree_view_get_column (treeview,0);
	}
	return true;
}

/*keys*/

G_MODULE_EXPORT gboolean check_escape(GtkWidget *widget, GdkEventKey *event, gpointer data) { 
  if (event->keyval == GDK_KEY_Escape) {
//     gtk_main_quit();
    gtk_widget_hide(widget);
    return TRUE;
  }
  return FALSE;
}

G_MODULE_EXPORT gboolean check_enter(GtkWidget *widget, GdkEventKey *event, gpointer data) { 
  if (event->keyval == GDK_KEY_KP_Enter) {
//     gtk_main_quit();
//     
    dialog_accept(widget,data);
	
	return TRUE;
  }
  return FALSE;
}

G_MODULE_EXPORT gboolean check_plus(GtkWidget *widget, GdkEventKey *event, gpointer data) { 
  if (event->keyval == GDK_KEY_plus) {
//     gtk_main_quit();
//     
	add_line_to_gtktreeview();
	
	return TRUE;
  }
  return FALSE;
}

G_MODULE_EXPORT gboolean check_delete(GtkWidget *widget, GdkEventKey *event, gpointer data) { 
  if (event->keyval == GDK_KEY_Delete) {
//     gtk_main_quit();
//     
// 	add_line_to_gtktreeview();
	remove_selected_lines_from_REPLACEMENT_TREEVIEW();
	
	return TRUE;
  }
  return FALSE;
}

/*(mouse)buttons*/

G_MODULE_EXPORT bool mouse_clicked(GtkWidget *widget, GdkEventButton *event, gpointer func_data)
{
  if ((event->type==GDK_2BUTTON_PRESS ||  event->type==GDK_3BUTTON_PRESS) && event->button!=1 ) {
		add_line_to_gtktreeview();
  }

  return FALSE;
}

G_MODULE_EXPORT bool signal_handler_event(GtkWidget *widget, GdkEventButton *event, gpointer func_data)
{
  if ((event->type==GDK_2BUTTON_PRESS ||  event->type==GDK_3BUTTON_PRESS) ) {
    printf("I feel %s clicked with button %d\n",
           event->type==GDK_2BUTTON_PRESS ? "double" : "triple",
           event->button);
  }

  return FALSE;
}

#ifdef __cplusplus  
}
#endif
