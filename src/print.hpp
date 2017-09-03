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



#ifndef PRINT_HPP
#define PRINT_HPP

#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

class Print {
private:
public:
  static void skalar_string(string ska);
  static void vector_string(vector<string> vec);
  static void vector_int(vector<int> vec);
  static void matrix_string(vector< vector<string> > *mat);
  static void tensor_string(vector < vector< vector<string> > > *ten);
};

#endif