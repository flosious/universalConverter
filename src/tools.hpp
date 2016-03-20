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


#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <string.h>

#include <sys/stat.h>
#include "print.hpp"

#ifdef __unix__ 
#include <unistd.h>
#else
#include <direct.h>
#include <windows.h>
#endif

using namespace std;


class Tools{
private:  
public:
  

#ifdef __unix__ 
static int mkpath(string pfad, mode_t mode);
#else
static int mkpath(string pfad, int delme);  
#endif

static bool replace_umlaute_to_windows_compatible(string *mainstring);

static vector<vector<string> > unify_2_vectors_to_matrix(vector<string > vector1, vector<string > vector2);

static vector<vector<string> > transform_tensor_unifying_lines_to_matrix(vector<vector<vector<string> > > *tensor);

static string create_overview_string_from_matrix(vector< vector<string> > *overview);

static int do_mkdir(const char *path, mode_t mode);
  
static string extract_directory_from_filename(string filename,string filepath_delimiter);

static string extract_filename(string filename,string filepath_delimiter,string filetye_delimiter);
  
static vector<vector<string> > transform_tensor_unifying_columns_to_matrix(vector < vector < vector < string > > > *tensor);
  
static void remove_lines_or_columns_from_matrix(vector<vector<string> > *matrix,vector<int> *lines=nullptr, vector<int> *columns=nullptr);  

static vector<vector<string> > transpose_matrix(vector<vector<string> > *matrix);
  
static void fillup_matrix(vector<vector<string> > *matrix);
  
static vector<string> get_file_list_from_dir_name(string dir_name);
  
static bool transform_string_to_xml(string *content, string *header,string *result);

static bool transform_xml_to_string(string *content, string *header,string *xml,int iter=0);

static bool transform_xml_to_vector_string(vector<string> *content, vector<string> *header,string *xml);
  
static bool load_file_to_string(string filename_with_path,string *contents);
  
static bool file_exists(string filename);

static bool write_to_file(string filename_with_path,string *contents, bool append=true);
  
static bool load_file_to_vector_string(string filename_with_path, vector<string> *contents);
  
static vector<vector <string> > format_string_to_matrix(string *mainstring, string line_delimiter, string column_delimiter);  

static string format_matrix_to_string(vector<vector<string> > *matrix, string line_delimiter, string column_delimiter);

static string get_string_between_string_A_and_next_B(string *mainstring,string A, string B);

static vector<string> get_all_string_between_string_A_and_next_B(string *mainstring,string A, string B);

static string get_string_between_string_A_and_last_B(string *mainstring,string A, string B);

static vector<string> get_strings_between_delimiter(string mainstring, string delimiter, bool empty=false);

static double str_to_double(string mainstring);

static int str_to_int(string mainstring);

static int is_number(std::string str);

static void replace_chars_from_string(string *mainstring, string replacethis, string replacewith);

static string remove_spaces_from_string(string mainstring); // 

static string remove_linebreakers_from_string(string mainstring); // remove_linebreakers_from_string

static string remove_chars_from_string(string mainstring, string chars);

};

#endif // TOOLS_HPP