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

#ifndef GLOBALVARS_HPP
#define GLOBALVARS_HPP

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <map>

using namespace std;

struct prof {
	string name;
	vector<string> list;
	string action;
	bool full_auto_tofsims;
	bool full_auto_dsims;
};

struct op {
	string directory;
	string filename;
	string data_delimiter;
	vector< vector<string> > format;
	bool overwrite_protection;
// 	bool check_lines_after_parsing;
// 	bool check_columns_after_parsing;
	bool check_lines_in_header;
	bool check_lines_in_data;
	bool check_columns_in_header;
	bool check_columns_in_data;
};

struct ip {
	string data_delimiter;
	string filename_delimiter;
	vector<string> files;
	vector<int> lines;
	vector<int> columns;
	int lines_type; // 0==discard; 1==include
	int columns_type; // 0==discard; 1==include
	bool remove_headers;
	vector<vector<string> > replacements;
};


struct parameter {
// 	  string profile_name;
	  prof profile;
	  ip input;
	  op output;
};

extern map<string, string>   config_params;
extern parameter params; // parsed parameters from overlay
extern map<string, map<string, string> >   gtkoverlay_params; // raw overlay parameters for saving
// gtkoverlay_params[widget_name]["type"]=$type 
// gtkoverlay_params[widget_name]["content"]=$content 

extern GtkBuilder *builder;




#endif // GLOBALVARS_HPP