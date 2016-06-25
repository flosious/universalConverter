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

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP



#include <iostream>
#include <string>
#include <gtk/gtk.h>
#include <vector>
#include <map>

#include "globalvars.hpp"
#include "tools.hpp"
#include "gtkstuff.hpp"
#include "print.hpp"
#include "dirent.h"

// #include <boost/filesystem.hpp>

// #define DEBUG

using namespace std;

// string translate_input_to_output_filename (string output_filename, string input_filename);

vector<vector<string> > full_auto_dsims(vector<vector<vector<string> > > *headers);

vector<vector<string> > full_auto_tofsims(vector<vector<vector<string> > > *headers);

string check_directory_string(string directory);

string translate_input_to_output (string input_filename, string format, vector<vector<string> > *header_matrix);

bool parse_data_and_header_parts(vector<vector<string> > *content_matrix,vector<vector<vector<string> > > *data, vector < vector<vector<string> > > *header);

void init(int    argc, char **argv );

bool set_args(int    argc, char **argv);

bool filter_lines(vector<vector<string> > *matrix);
bool filter_columns(vector<vector<string> > *matrix);

bool get_profile_list();

bool delete_profile(string profile_name);

bool save_profile(string profile); 

bool load_profile(string profile_name);

bool transform_gtkoverlay_params_to_xml(string *xml_content);

bool load_xml_to_gtkoverlay_params(string *xml_content);

bool execute_on_file(string filename, bool write_merge_unifi=false);

bool execution();


#endif // FUNCTIONS_HPP