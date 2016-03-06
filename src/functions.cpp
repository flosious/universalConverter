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

#include "functions.hpp"


// global vars //
// parameter params;
map<string, string>  config_params;
vector<vector<string> > overview_conversion; // an overview what was converted: from where to where

// UML - Ablauf
/*
 * 1. Program start 	-> load config		-> load profiles (standart)
 * 2. load profile    	-> parse profile 	-> set vars 	-> set gtk output
 * 
 * 2. "anwenden" 	-> read options 	-> check & set vars
 * 		 	-> read format 		-> prase formats 	-> set format
 * 		 	-> read files 		-> parse files 		-> parse header
 * 									-> prase data 	-> filter data
 * 		 	-> output data		
 * 
 * 3. save profile	-> format vars -> to file (profiles)
 * 4. delte profile	-> load profiles -> parse profiles -> delete profile -> overwrite profiles
 * 
 * 
 */




void init(int   argc, char **argv ) {
	
	// default config params //
	config_params["config_delimiter"]="||";
	config_params["current_directory"]="";
	config_params["profile_ending"]=".profile";
	config_params["file_type_delimiter"]=".";
	#ifdef __unix__
	config_params["profiles_dir"]="./profiles/";
	config_params["path_delimiter"]="/";
	config_params["line_delimiter"]="\n";
	#else
	config_params["profiles_dir"]=".\\profiles\\";
	config_params["path_delimiter"]="\\";
	config_params["line_delimiter"]="\r\n";
	#endif
	
	/**/
	
	set_args(argc, argv);

}

bool set_args(int    argc, char **argv) {
	string filenames="";
	for (int i=1; i<argc;i++) {
		if (i!=1) filenames.append("\n");
		filenames.append(argv[i]);
	}
	set_gtktextview_content_from_name_as_string("EINGABEDATEIEN_TEXTVIEW",filenames);
	return true;
}

// data was parsed from gtk-overlay and is now in the params-datastructure

bool execution() {
	/*execute each file*/
	for (int i=0;i<params.input.files.size();i++) {
		execute_on_file(params.input.files[i]);
	}
	/*print overview*/
	if (overview_conversion.size()>0) show_info_box(Tools::create_overview_string_from_matrix(&overview_conversion));
	overview_conversion.clear();
	return true;
}

bool filter_replacements(string *contents) {
	for (int i=0;i<params.input.replacements.size();i++) {
		Tools::replace_chars_from_string(contents,params.input.replacements[i][0],params.input.replacements[i][1]);
	}
	return true;
}

bool execute_on_file(string filename) {
	if (filename=="") return false;
	string contents, output_filename, output_directory, output_format;
	vector<vector<string> > matrix;
	vector<vector<vector<string> > > datas, headers;
	
	/*load file*/
	if (!Tools::load_file_to_string(filename,&contents)) return false;
	
	/*filter replacements from raw content string*/
	filter_replacements(&contents);
	
	/*format content to matrix*/
	vector<vector<string> > content_matrix = Tools::format_string_to_matrix(&contents,"\n",params.input.data_delimiter);
	
	/*fill up columns of matrix to MxN matrix style*/
	Tools::fillup_matrix(&content_matrix);
	
	/*discard lines or columns before parsing*/
	if (!params.output.check_lines_in_header && !params.output.check_lines_in_data) filter_lines(&content_matrix);
	if (!params.output.check_columns_in_data && !params.output.check_columns_in_header) filter_columns(&content_matrix);
	
	/*parse the headers and the datas (tensors)*/
	parse_data_and_header_parts(&content_matrix,&datas,&headers);
	
	/*unify the headers to one header matrix*/
	vector<vector<string> > header = Tools::transform_tensor_unifying_columns_to_matrix(&headers);
	
	/*discard lines or columns after parsing*/
	if (params.output.check_lines_in_header) filter_lines(&header);
	if (params.output.check_columns_in_header) filter_columns(&header);
	
	/*unify the datas to one data matrix*/
	vector<vector<string> > data = Tools::transform_tensor_unifying_columns_to_matrix(&datas);
	
	/*discard lines or columns after parsing*/
	if (params.output.check_lines_in_data) filter_lines(&data);
	if (params.output.check_columns_in_data) filter_columns(&data);
	
	/*translate commands in output directory form input filename*/
	string op_dir = translate_input_to_output(params.output.directory,filename,&header);
	
	/*translate commands in output filename form input filename*/
	string op_filen =translate_input_to_output(params.output.filename, filename,&header);
	if (Tools::extract_filename(op_filen,config_params["path_delimiter"],config_params["file_type_delimiter"])=="") op_filen=="";
	
	/*translate commands in output format from input filename*/
	string op_format = translate_input_to_output(gtkoverlay_params["AUSGABEFORMAT_TEXTVIEW"]["content"], filename,&header);
	
	/*convert output format to matrix*/
	vector<vector<string> > op_format_matrix = Tools::format_string_to_matrix(&op_format,"\n",params.input.data_delimiter);
	Tools::fillup_matrix(&op_format_matrix);
	
	/*unify matritzes to one*/
	vector<vector<vector<string> > > unify; 
	unify.push_back(op_format_matrix);
	if (!params.input.remove_headers) unify.push_back(header);   
	unify.push_back(data);
	vector<vector<string> > unification = Tools::transform_tensor_unifying_lines_to_matrix(&unify);
	
	/*create output string*/
	string output_string = Tools::format_matrix_to_string(&unification,config_params["line_delimiter"],params.output.data_delimiter);
	
	/*if no output filename, set filename of input as default*/
	if (op_filen=="") {
		op_filen = Tools::extract_filename(filename,config_params["path_delimiter"],config_params["file_type_delimiter"]) + string("_conv.txt");
	}
	
	/*if no output directory, set directory of input as default*/
	if (op_dir=="") {
		op_dir = Tools::extract_directory_from_filename(filename,config_params["path_delimiter"]);
	}
	
	/*check if directory is correct formatted - don t forget path delimiters*/
	op_dir=check_directory_string(op_dir);
	
	/*create directory tree if not exists*/
	Tools::mkpath(op_dir,0750);
	
	/*add directory and filename together*/
	string filename_with_path= op_dir+op_filen;
	
	/*check if outputfile already exists*/
	if (params.output.overwrite_protection) {
		if (Tools::file_exists(filename_with_path)) {
			show_error_box(string("output file: \"") + filename_with_path + string("\" already exists"));
			return false;
		}
	}
	
	/*record overview*/
	overview_conversion.push_back({Tools::extract_filename(filename,config_params["path_delimiter"],""),filename_with_path});
	
	/*write output string to file*/
	Tools::write_to_file(filename_with_path,&output_string,false);
	
	return true;
}

string check_directory_string(string directory) {
	if (directory=="") {
		directory.append(".");
		directory.append(config_params["path_delimiter"]);
		return directory;
	}
	if (directory.compare(directory.size()-config_params["path_delimiter"].size(),config_params["path_delimiter"].size(),config_params["path_delimiter"])!=0) {
		directory.append(config_params["path_delimiter"]);
	}
	return directory;
}


/*search the input(filename/directory) for cmds and replaces  them with their function values*/
string translate_input_to_output (string format, string input_filename, vector<vector<string> > *header_matrix) {
	if (params.input.filename_delimiter=="") return format;
	vector<string> path_parts = Tools::get_strings_between_delimiter(input_filename,config_params["path_delimiter"]);
// 	cout << path_parts.size() << endl;
	input_filename=Tools::extract_filename(input_filename,config_params["path_delimiter"],config_params["file_type_delimiter"]);
	vector<string> input_filename_parts=Tools::get_strings_between_delimiter(input_filename,params.input.filename_delimiter);
	vector<string> output_filename_placeholders = Tools::get_all_string_between_string_A_and_next_B(&format,"{","}");
	map<string, string > translation_map;
	
	for (int i=0; i<output_filename_placeholders.size();i++) {
		vector<string> parts_string = Tools::get_all_string_between_string_A_and_next_B(&(output_filename_placeholders[i]),"[","]");
		vector<int> parts;
		parts.clear();
		for (int z=0;z<parts_string.size();z++) {
			parts.push_back(Tools::str_to_int(parts_string[z])-1);
		}
		int part=0;
		if (parts.size()>0) part= parts[0];
		string cmd = Tools::get_strings_between_delimiter(output_filename_placeholders[i],"[").front();
		cmd = Tools::remove_spaces_from_string(cmd);
		/*look for replacers in input_filename_parts*/
		if (cmd=="filename") {
			if (part>=0 && part<input_filename_parts.size())
				translation_map[string("{")+output_filename_placeholders[i]+string("}")]=input_filename_parts[part];
			else {
				show_error_box("output is bad formated: set higher than 1 and below/equal the count of name-delimiter in filename ");
				translation_map[string("{")+output_filename_placeholders[i]+string("}")]="";
			}
		} else if (cmd=="dirname" || cmd=="pathname") {
			if (part>=0 && part<path_parts.size()) {
			part++; //0th part would be the filename itself
				translation_map[string("{")+output_filename_placeholders[i]+string("}")]=path_parts[path_parts.size()-1-part];
			} else {
				show_error_box("output is bad formated: set higher than 1 and below/equal the count of directory-delimiter in path of filename");
				translation_map[string("{")+output_filename_placeholders[i]+string("}")]="";
			}
		} else if (cmd=="header") {
			if ((header_matrix->size())>0 && parts[0]>=0 && parts[1]>=0 && parts[0]<(header_matrix->size()) && parts[1]<(header_matrix->at(0).size())) {
				translation_map[string("{")+output_filename_placeholders[i]+string("}")]=(*header_matrix)[parts[0]][parts[1]];
			} else {
				show_error_box("output is bad formated: set higher than 1 and below/equal the size of lines/columns of header matrix");
				translation_map[string("{")+output_filename_placeholders[i]+string("}")]="";
			}
		}
	}
	
	/*replace output_filname commands with translation_map*/
	for (map<string,string >::iterator it=translation_map.begin(); it!=translation_map.end(); ++it) {
		Tools::replace_chars_from_string(&format,it->first,it->second);
	}
	return format;
}


bool parse_data_and_header_parts(vector<vector<string> > *content_matrix,vector<vector<vector<string> > > *data, vector < vector<vector<string> > > *header) {
	vector<vector<string> > header_temp, data_temp;
	
	bool data_scanned=false;
	bool header_scanned=false;
	if (content_matrix->size()<1) return false;
	int check_number;
// 	sort(params.input.columns.begin(),params.input.columns.end());
// 	sort(params.input.lines.begin(),params.input.lines.end());
	for (int i=0;i<content_matrix->size();i++) {
		check_number=1;
		for (int j=0;j<(content_matrix->at(i)).size();j++) {
			check_number=check_number*Tools::is_number((*content_matrix)[i][j]);
		}
		if (check_number==0 || ((content_matrix->at(i).size())==2 && ((content_matrix->at(i)[1].size())==0) )) { // no number, so its a header-part
			header_temp.push_back((*content_matrix)[i]);
			header_scanned=true;
			if (data_scanned ) { 
				data->push_back(data_temp);
				data_temp.clear();
				data_scanned=false;
			}
		} else { // it's a number, so its a data-part
			data_temp.push_back((*content_matrix)[i]);
			data_scanned=true;
			if (header_scanned) {
				
				header->push_back(header_temp);
				header_temp.clear();
				header_scanned=false;
			}
		}
	}
	if (data_scanned ) { 
		data->push_back(data_temp);
		data_temp.clear();
		data_scanned=false;
	}
	if (header_scanned) {
		
		header->push_back(header_temp);
		header_temp.clear();
		header_scanned=false;
	}
	
	return true;
}




bool filter_columns(vector<vector<string> > *matrix) {
	
	
	if (matrix->size()<1) return false;
	vector<int> lines;
	vector<int> columns;
	if (params.input.columns.size()>0) {
	  sort(params.input.columns.begin(),params.input.columns.end());
	if (params.input.columns_type==1) {
		for (int i=0;i<(matrix->at(0)).size();i++) {
			columns.push_back(i);
		}
		for (int i=params.input.columns.size()-1;i>=0;i--) {
// 			columns.erase(columns.begin()+params.input.columns[i]-1);
			if (params.input.columns[i]>0) columns[params.input.columns[i]-1]=-1;
			if (params.input.columns[i]<0) columns[columns.size()+params.input.columns[i]]=-1;
			if (columns.size()<1) return false;
		}
	} else {
	  for (int i=0;i<params.input.columns.size();i++) {
// 		  columns.push_back(params.input.columns[i]-1);
		  if (params.input.columns[i]>0) columns.push_back(params.input.columns[i]-1);
		  if (params.input.columns[i]<0) columns.push_back((matrix->at(0)).size()+params.input.columns[i]);
	  }
	  
	}
	}
	Tools::remove_lines_or_columns_from_matrix(matrix,&lines,&columns);
	
	return true;
}


bool filter_lines(vector<vector<string> > *matrix) {
	if (matrix->size()<1) return false;
	vector<int> lines;
	vector<int> columns;
	if (params.input.lines.size()>0) {
	sort(params.input.lines.begin(),params.input.lines.end());
	if (params.input.lines_type==1) {
		for (int i=0;i<matrix->size();i++) {
			lines.push_back(i);
		}
		for (int i=params.input.lines.size()-1;i>=0;i--) {
// 			if (params.input.lines[i]>0) lines.erase(lines.begin()+params.input.lines[i]-1);
			if (params.input.lines[i]>0) lines[params.input.lines[i]-1]=-1;
			if (params.input.lines[i]<0) lines[lines.size()+params.input.lines[i]]=-1;
			if (lines.size()<1) return false;
		}
	} else {
	  for (int i=0;i<params.input.lines.size();i++) {
		  if (params.input.lines[i]>0) lines.push_back(params.input.lines[i]-1);
		  if (params.input.lines[i]<0) lines.push_back(matrix->size()+params.input.lines[i]);
	  }
	}
	}
	Print::vector_int(lines);
	Tools::remove_lines_or_columns_from_matrix(matrix,&lines,&columns);
	
	return true;
}

bool delete_profile(string profile_name) {
	Tools::replace_chars_from_string(&profile_name,config_params["path_delimiter"],"");
	string profile_file = config_params["profiles_dir"]+profile_name+config_params["profile_ending"];
	if (remove(profile_file.c_str())!=0) return false;
	else return true;
}

bool load_profile(string profile_name) {
	string profile_file = config_params["profiles_dir"]+profile_name+config_params["profile_ending"];
	string contents;
	Tools::load_file_to_string(profile_file,&contents);
	load_xml_to_gtkoverlay_params(&contents);
	/*hier geht beim übergang von oben nach unten was schief*/
	print_gtkoverlay_params_to_gtkoverlay();
}

bool get_profile_list() {
	vector<string> filelist;
	string filename;
	params.profile.list.clear();
	filelist=Tools::get_file_list_from_dir_name(config_params["profiles_dir"]);
	for (int i=0;i<filelist.size();i++) {
		if (filelist.at(i)!="." && filelist.at(i)!="..") {
			filename = filelist.at(i);
			filename.erase(filename.end()-config_params["profile_ending"].length(),filename.end());
			params.profile.list.push_back(filename);
		}
	}
	sort(params.profile.list.begin(),params.profile.list.end());
	return true;
}


bool load_xml_to_gtkoverlay_params(string *xml_content) {
	vector<string> content;
	vector<string> header;
	vector<string> sub_header;
	vector<string> sub_content;
	string temp;
	::gtkoverlay_params.clear();
	if (!Tools::transform_xml_to_vector_string(&content, &header, xml_content)) return false;
	for (int j=0;j<header.size();j++) {
		if (!Tools::transform_xml_to_vector_string(&sub_content, &sub_header, &(content.at(j)))) return false;
		
		for (int i=0;i<sub_header.size();i++) {
			if ((sub_content.at(i)).size()>0) temp = sub_content.at(i);
			else temp = "";
			::gtkoverlay_params[header.at(j)][sub_header.at(i)]=temp;
// 			cout << header.at(j) << "\t\t" << sub_header.at(i) << "\t\t" << temp <<  endl;
// 			cout << header.at(j) << "\t\t" << sub_header.at(i) << "\t\t" << gtkoverlay_params[header.at(j)][sub_header.at(i)] <<  endl;
		}
		sub_header.clear();
		sub_content.clear();
	}
	return true;
}

bool transform_gtkoverlay_params_to_xml(string *xml_content) {
	// gtkoverlay_params[widget_name]["type"]=$type 
	// gtkoverlay_params[widget_name]["content"]=$content 
	string xml="";
	string xml_temp2="";
	string xml_temp1="";
	string content="";
	string header="";
	for (map<string,map<string,string> >::iterator it=gtkoverlay_params.begin(); it!=gtkoverlay_params.end(); ++it) {
		header="type";
		content=it->second["type"];
// 		cout << content << endl;
		if (!Tools::transform_string_to_xml(&content,&header,&xml_temp1)) return false;
		header="content";
		content=it->second["content"];
		if (!Tools::transform_string_to_xml(&content,&header,&xml_temp2)) return false;
		xml=xml_temp1+xml_temp2;
		// Rahmen nach gtk objekten bennen
		header=it->first;
		content=xml;
		if (!Tools::transform_string_to_xml(&content,&header,&xml)) return false;
		*xml_content = *xml_content + xml;
	}
	return true;
}




