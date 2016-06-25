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


#include "tools.hpp"

string Tools::create_overview_string_from_matrix(vector< vector<string> > *overview) {
	string overview_string="";
	for (int i=0;i<overview->size();i++) {
		if ((overview->at(i)).size()==2) {
			overview_string.append((overview->at(i))[0]);
			overview_string.append("\t->\t");
			overview_string.append((overview->at(i))[1]);
			overview_string.append("\n");
		}
	}
	return overview_string;
}

/*remove path structure and optinally file ending from the name*/
string Tools::extract_filename(string filename,string filepath_delimiter,string filetye_delimiter) {
	string filename_with_type=(Tools::get_strings_between_delimiter(filename,filepath_delimiter)).back();
	if (filetye_delimiter=="") return filename_with_type;
	vector<string> file_parts=Tools::get_strings_between_delimiter(filename_with_type,filetye_delimiter);
	
	if (file_parts.size()<2) return filename;
	filename="";
	for (int i=0;i<file_parts.size()-1;i++) {
		filename=filename+file_parts[i];
	}
	return filename;
}

string Tools::extract_directory_from_filename(string filename,string filepath_delimiter) {
	string directory="";
	vector<string> directory_parts = Tools::get_strings_between_delimiter(filename,filepath_delimiter);
	if (directory_parts.size()<2) {
		directory=string(".")+filepath_delimiter;
		return directory;
	}
	for (int i=0; i<directory_parts.size()-1;i++) {
		directory.append(directory_parts[i]);
		directory.append(filepath_delimiter);
	}
	if (directory.compare(directory.size()-filepath_delimiter.size(),filepath_delimiter.size(),filepath_delimiter)!=0) {
		directory.append(filepath_delimiter);
	}
	return directory;
}

vector<vector<string> > Tools::transform_tensor_unifying_columns_to_matrix(vector<vector<vector<string> > > *tensor) {
	vector<vector<string> > matrix, matrix_temp;
	matrix.clear();
	if (tensor->size()==0) return matrix;
// 	cout << (tensor->size()) << endl;
	for (int i=0;i<tensor->size();i++) {
// 	  cout << (tensor->at(i).size()) << endl;
// 		  if ((tensor->at(i)).size()==1) { // vector
// 			  matrix.push_back(tensor->at(i)[0]);
// 			  cout << "in"<< endl;
// 		  }
		  if ((tensor->at(i).size())>0) { // matrix
		  matrix_temp=Tools::transpose_matrix(&(tensor->at(i)));
		  matrix.insert(matrix.end(),matrix_temp.begin(),matrix_temp.end());
		  }
	}
	return Tools::transpose_matrix(&matrix);
}

vector<vector<string> > Tools::unify_2_vectors_to_matrix(vector<string > vector1, vector<string > vector2) {
	vector<vector<string> > matrix;
	matrix.clear();
	int max_size;
	if (vector1.size()<vector2.size()) max_size=vector2.size();
	else max_size=vector1.size();
	vector1.resize(max_size,"");
	vector2.resize(max_size,"");
	for (int i=0;i<max_size;i++) {
		  matrix.push_back({vector1[i],vector2[i]});
	}
	return matrix;
}

vector<vector<string> > Tools::add_vector_to_matrix(vector<vector<string > > matrix_summand, vector<string > vec) {
	
	vector<vector<string> > matrix_T=Tools::transpose_matrix(&matrix_summand);
	matrix_T.push_back(vec);
	return Tools::transpose_matrix(&matrix_T);
}

vector<vector<string> > Tools::transform_tensor_unifying_lines_to_matrix(vector<vector<vector<string> > > *tensor) {
	vector<vector<string> > result_matrix;
	for (int i=0;i<tensor->size();i++) {
		result_matrix.insert(result_matrix.end(),(tensor->at(i)).begin(),(tensor->at(i)).end());
	}
	return result_matrix;
}

void Tools::remove_lines_or_columns_from_matrix(vector<vector<string> > *matrix,vector<int> *lines, vector<int> *columns) {
	
	if (lines==nullptr && columns==nullptr) return;
	sort(lines->begin(),lines->end());
	sort(columns->begin(),columns->end());
	
	if (lines!=nullptr && lines->size()>0) {
		for (int i=lines->size()-1;i>=0;i--) {
			if (lines->at(i)>=0 && matrix->size()>lines->at(i)) { 
			  matrix->erase(matrix->begin()+lines->at(i));
			}
		}
	}
	if (columns!=nullptr && columns->size()>0) {
		for (int i=0;i<matrix->size();i++) {
			for (int j=columns->size()-1;j>=0;j--) {
				if (columns->at(j)>=0 && (matrix->at(i)).size()>columns->at(j)) {
				  (matrix->at(i)).erase((matrix->at(i)).begin()+columns->at(j));
				}
			}
		}
	}
}

vector<vector<string> > Tools::transpose_matrix(vector<vector<string> > *matrix) {
	
	Tools::fillup_matrix(matrix);
	vector<vector<string> > matrix_T((matrix->at(0)).size(),vector<string>(matrix->size()));
	
	for (int i=0;i<matrix->size();i++) {
		for (int j=0;j<(matrix->at(i)).size();j++) {
			matrix_T[j][i]=(*matrix)[i][j];
		}
	}
	return matrix_T;
}


void Tools::fillup_matrix(vector<vector<string> > *matrix) {
	int max_column_size=0;
	int size;
	for (int i=0;i<matrix->size();i++) {
		size = (matrix->at(i)).size();
		if (size>max_column_size) max_column_size=size;
	}
	for (int i=0;i<matrix->size();i++) {
		(matrix->at(i)).resize(max_column_size,"");
	}
}

vector<string> Tools::get_file_list_from_dir_name(string dir_name) {
	vector<string> filelist;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (dir_name.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
// 			printf ("%s\n", ent->d_name);
			filelist.push_back(ent->d_name);
		}
		closedir (dir);
	}
	return filelist;
}
	

bool Tools::transform_string_to_xml(string *content, string *header,string *result) {
// 	string result;
	
	*result=string("<")+*header+string(">\n");
	*result=*result+*content;
	*result=*result+string("\n</")+*header+string(">\n");
	
	return true;
}

bool Tools::transform_xml_to_string(string *content, string *header,string *xml,int iter) {
	string xml_copy=*xml;
	string header_end;
// 	string content_temp;
	
	for (int i=0;i<=iter;i++) {
		*header=Tools::get_string_between_string_A_and_next_B(&xml_copy,"<",">");
		if (*header=="") return false;
		*content=Tools::get_string_between_string_A_and_next_B(&xml_copy,string("<")+*header+string(">\n"),string("\n</")+*header+string(">"));
		header_end=string("</")+*header+string(">");
		xml_copy.erase(0,xml_copy.find(header_end)+header_end.length());
	}
	return true;
}

bool Tools::transform_xml_to_vector_string(vector<string> *content, vector<string> *header,string *xml) {
// 	*header=Tools::get_all_string_between_string_A_and_next_B(*xml,"<",">");
// 	cout << header->size() << endl;
// 	for (int i=0;i<=(header->size()-1)/2;i=i+2) {
// 		if (header->at(i)=="") return false;
// 		content->push_back(Tools::get_string_between_string_A_and_next_B(xml,string("<")+header->at(i)+string(">\n"),string("\n</")+header->at(i)+string(">")));
// 		
// 	}
// 	return true;
  
	string xml_copy=Tools::remove_chars_from_string(*xml,"\r"); 
	string header_end;
	string header_temp;
	
	
	header_temp=Tools::get_string_between_string_A_and_next_B(&xml_copy,"<",">");
	while (header_temp!="") {
		content->push_back(Tools::get_string_between_string_A_and_next_B(&xml_copy,string("<")+header_temp+string(">\n"),string("\n</")+header_temp+string(">")));
		header->push_back(header_temp);
		header_end=string("</")+header_temp+string(">");
		xml_copy.erase(0,xml_copy.find(header_end)+header_end.length());
		header_temp=Tools::get_string_between_string_A_and_next_B(&xml_copy,"<",">");
	}
	return true;
  
}


#ifdef __unix__ 
typedef struct stat Stat;

int Tools::do_mkdir(const char *path, mode_t mode)
{
    Stat            st;
    int             status = 0;

    if (stat(path, &st) != 0)
    {
        /* Directory does not exist. EEXIST for race condition */
        if (mkdir(path, mode) != 0 && errno != EEXIST)
            status = -1;
    }
    else if (!S_ISDIR(st.st_mode))
    {
        errno = ENOTDIR;
        status = -1;
    }

    return(status);
}




/**
** mkpath - ensure all directories in path exist
** Algorithm takes the pessimistic view and works top-down to ensure
** each directory in path exists, rather than optimistically creating
** the last element and working backwards.
*/
int Tools::mkpath(string pfad, mode_t mode)
{
    const char *path = pfad.c_str();
    char           *pp;
    char           *sp;
    int             status;
    char           *copypath = strdup(path);

    status = 0;
    pp = copypath;
    while (status == 0 && (sp = strchr(pp, '/')) != 0)
    {
        if (sp != pp)
        {
            /* Neither root nor double slash in path */
            *sp = '\0';
            status = do_mkdir(copypath, mode);
            *sp = '/';
        }
        pp = sp + 1;
    }
    if (status == 0)
        status = do_mkdir(path, mode);
    free(copypath);
    return (status);
}
#else

int Tools::mkpath(string path, int delme) {
	replace_umlaute_to_windows_compatible(&path);
	string cmd = string("mkdir ") +path;
	system(cmd.c_str());
	return 1;
}
#endif

bool Tools::file_exists(string filename) {
	ifstream f(filename.c_str());
	if (f.good()) {
		f.close();
		return true;
	} else {
		f.close();
		return false;
	}
}

bool Tools::write_to_file(string filename_with_path,string *contents, bool append) {
// 	cout << filename_with_path << endl;
  #ifdef __unix__
#else
	replace_umlaute_to_windows_compatible(&filename_with_path);
#endif
// 	cout << filename_with_path << endl;
	ofstream myfile;
	if (append) myfile.open (filename_with_path.c_str(), ios::app);
	else myfile.open (filename_with_path.c_str());
	if (!myfile.is_open()) return false;
	myfile << *contents;
	myfile.close();
	return true;
}

bool Tools::load_file_to_string(string filename_with_path, string *contents) {
  #ifdef __unix__
#else
	replace_umlaute_to_windows_compatible(&filename_with_path);
#endif
	ifstream myfile;
	myfile.open (filename_with_path.c_str(),ios::in|ios::binary|ios::ate);
	if (!myfile.is_open()) return false;
	contents->clear();
// 	streampos size;
	int size;
	size = myfile.tellg();
// 	char * memblock = new char [size]; // <-- das scheint NICHT mit 0en zu initialisieren (?!)
	char *memblock = (char*) calloc(size,sizeof(char));
	memset (memblock, 0, sizeof (char) * size);
	myfile.seekg (0, ios::beg);
	myfile.read (memblock, size);
	myfile.close();
// 	memblock[size]='\0';
	*contents = string(memblock);
	delete[] memblock;
	return true;
}

void Tools::remove_empty_lines_from_matrix(std::vector<std::vector<std::string>> *mat) {
	for (int i=mat->size()-1;i>=0;i--) {
		bool empty=false;
		for (int j=0;j<mat->at(i).size();j++) {
			if (mat->at(i)[j]=="") empty=true;
			else {	
			  empty = false;
			  break;
			}
		}
		if (empty) mat->erase(mat->begin()+i);
	}
}

bool Tools::load_file_to_vector_string(string filename_with_path, vector<string> *contents) {
  #ifdef __unix__
#else
	replace_umlaute_to_windows_compatible(&filename_with_path);
#endif
	ifstream myfile;
	string line;
	myfile.open (filename_with_path.c_str(),ios::in);
	if (!myfile.is_open()) return false;
	contents->clear();
	while ( getline (myfile,line) ) {
		contents->push_back(Tools::remove_linebreakers_from_string(line));
	}
	myfile.close();
	return true;
}

vector<vector <string> > Tools::format_string_to_matrix(string *mainstring, string line_delimiter, string column_delimiter) {
	string line;
	vector<string> columns_in_line;
	vector<vector< string >> matrix;
	
	vector<string> lines = Tools::get_strings_between_delimiter(*mainstring,line_delimiter);
	for (int i=0;i<lines.size();i++) {
		line = lines[i];
		Tools::replace_chars_from_string(&(line),"\r",""); // win
		if (line.length()>0) {
			columns_in_line = Tools::get_strings_between_delimiter(line,column_delimiter,true);
			matrix.push_back(columns_in_line);
		}
	}
	return matrix;
}

string Tools::format_matrix_to_string(vector<vector<string> > *matrix, string line_delimiter, string column_delimiter) {
	string content;
	for (int i=0;i<matrix->size();i++) {
		if (i!=0) content=content+(line_delimiter);
		for (int j=0;j<(matrix->at(i)).size();j++) {
			if (((*matrix)[i][j]).size()>0) {
				if (j!=0) content=content +column_delimiter;
				content=content+((*matrix)[i][j]);
			} else {
				if (j!=0) content=content +column_delimiter;
				content=content+"";
			}
		}
		
	}
	return content;
}


string Tools::get_string_between_string_A_and_next_B(string *mainstring,string A, string B) {
	int len = mainstring->find(B)-mainstring->find(A)-A.length();
	if (len>0) { 
		return mainstring->substr(mainstring->find(A)+A.length(),len);
	}
	return "";
}

vector<string> Tools::get_all_string_between_string_A_and_next_B(string *mainstring,string A, string B) {
	int len,posA,posB;
	vector<string> results;
	posB = 0;
	do {
		posA=mainstring->find(A,posB);
		posB=mainstring->find(B,posA);
// 		cout << posA << endl << posB << endl;
		if (posA == string::npos || posB == string::npos) break;
		posA=posA+A.length();
		len = posB-posA;
		if (len>0) { 
			results.push_back(mainstring->substr(posA,len));
		}
// 		cout << ".";
	} while (len>0);
	return results;
}

string Tools::get_string_between_string_A_and_last_B(string *mainstring,string A, string B) {
	int len = mainstring->rfind(B)-mainstring->find(A)-1;
	if (len>0) { 
		return mainstring->substr(mainstring->find(A)+1,len);
	}
	return "";
}

vector<string> Tools::get_strings_between_delimiter(string mainstring, string delimiter, bool empty) {
	vector<string> results;
	string temp;
	temp.clear();
	results.clear();
	int pos_last=0;
	if (mainstring.find(delimiter,pos_last)==string::npos) {
		results.push_back(mainstring);
	} else {
	int pos_next=mainstring.find(delimiter,pos_last); // 0. Element besonders behandeln, da der mainstring mit einem Delimiter beginnen koennte
	do {
		results.push_back(mainstring.substr(pos_last,pos_next-pos_last));
		pos_last=pos_next+delimiter.length();
		pos_next=mainstring.find(delimiter,pos_last); // muss mindestens 1 weiter sein
	} while (pos_next>=pos_last); // Ende erreicht, kein weiterer Delimiter
	temp = mainstring.substr(pos_last); // letztes Eement nicht vergessen
	if (temp.size()>0 && pos_last>=pos_next) results.push_back(temp);  // anders als beim 0. element soll das letzte element nur beachtet werden wenn es nicht leer ist
	else if (empty) results.push_back("");
	
// 	cout << "DEBUG: " << results.back() << ";size: " << (results.back()).size() << endl;
	}
	return results;
}


double Tools::str_to_double(string mainstring) {
	stringstream temp;
	temp << mainstring;	
	double result;
	temp >> result;
// 	cout << "RESULT: " << result << endl;
	return result;	
}

int Tools::str_to_int(string mainstring) {
	stringstream temp;
	temp << mainstring;	
	int result;
	temp >> result;
	return result;	
}

int Tools::is_number(std::string str) {
	if (count(str.begin(),str.end(),'-')>1) return 0;
	if (count(str.begin(),str.end(),'+')>1) return 0;
	if (count(str.begin(),str.end(),'.')>1) return 0;
	if (count(str.begin(),str.end(),',')>1) return 0;
	if (count(str.begin(),str.end(),'e')>1) return 0;
	if (count(str.begin(),str.end(),'E')>1) return 0;
	if (str.find_first_not_of("0123456789eE.,+-") == std::string::npos) return 1;
	return 0; //scientific notation enthaelt E(A) = 10^A
}

/*
string Tools::replace_chars_from_string(string mainstring, string replacethis, string replacewith) {
	string copystring=mainstring;
	if (!(replacethis.length()>0))  return mainstring;
	while ( (copystring.find(replacethis)!=string::npos) ) {
		copystring.replace(copystring.find(replacethis),replacethis.length(),replacewith);
	}
	return copystring;
}
*/

void Tools::replace_chars_from_string(string *mainstring, string replacethis, string replacewith) {
	if (!(replacethis.length()>0))  return;
	int last_pos=mainstring->find(replacethis);
	while ( (last_pos!=string::npos) ) {
		mainstring->replace(last_pos,replacethis.length(),replacewith);
		last_pos=mainstring->find(replacethis,last_pos+replacewith.length());
	}
	return;
}

bool Tools::replace_umlaute_to_windows_compatible(string *mainstring) {
	
	replace_chars_from_string(mainstring,"Ä","\x00c4");
	replace_chars_from_string(mainstring,"Ü","\x00dc");
	replace_chars_from_string(mainstring,"Ö","\x00d6");
	replace_chars_from_string(mainstring,"ä","\x00e4");
	replace_chars_from_string(mainstring,"ü","\x00fc");
	replace_chars_from_string(mainstring,"ö","\x00f6");
	replace_chars_from_string(mainstring,"ß","\x00df");
// 	string result;
	return true;
}

string Tools::remove_spaces_from_string(string mainstring) {
	string copystring=mainstring;
	replace_chars_from_string(&copystring,"\n","");
	replace_chars_from_string(&copystring,"\r","");
	replace_chars_from_string(&copystring," ","");
	return copystring;
}

string Tools::remove_linebreakers_from_string(string mainstring) {
	string copystring=mainstring;
	replace_chars_from_string(&copystring,"\n","");
	replace_chars_from_string(&copystring,"\r","");
	return copystring;
}

string Tools::remove_chars_from_string(string mainstring, string chars) {
	string result = mainstring;
	for (int j=0;j<chars.length();j++) {
		for (int i=0;i<result.length();i++) {
			if (result[i] == chars[j]) { 
				result.erase(i,1);		
			}
		}
	}
	return result;
}

// static Tools Tools;