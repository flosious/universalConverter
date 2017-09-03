
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





#include "print.hpp"

void Print::skalar_string(string ska) {
	cout << "\t"<<ska; cout << endl;
}

void Print::vector_string(vector<string> vec) {
	for (int i=0; i<vec.size();i++) {
		cout << "[" << i << "]";
		skalar_string(vec[i]);
		
	}
}

void Print::matrix_string(vector< vector<string> > *mat) {
	for (int i=0;i<mat->size();i++) {
		for (int j=0; j<(mat->at(i)).size();j++) {
			cout << "[" << i << "]";
			cout << "[" << j << "]";
			cout << "\t";
			skalar_string((*mat)[i][j]);
		}
	}
}




void Print::tensor_string(vector< vector< vector<string> > > *ten) {
	for (int z=0;z<ten->size();z++) {
		for (int i=0;i<(ten->at(z)).size();i++) {
			for (int j=0; j<(ten->at(z)[i]).size();j++) {
				cout << "[" << z << "]";
				cout << "[" << i << "]";
				cout << "[" << j << "]";
				cout << "\t";
				skalar_string((*ten)[z][i][j]);
			}
		}
	}
}

void Print::vector_int(vector<int> vec) {
	for (int i=0; i<vec.size();i++) {
		cout << "[" << i << "]";
		skalar_string(to_string(vec[i]));
	}
}