/*
Copyright [2019] [tkfmhmnk]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
limitations under the License.
*/


#include <iostream>
#include <fstream>
#include<sstream>
#include <string>
#include"UnicodeFileIO.h"

using namespace std;
using namespace UnicodeFileIO;

void test() {
	basic_stringstream<char16_t> ss;
	basic_string<char16_t> str;
	Endian endian;
	if (ReadString("..\\TestData\\Unicode(UTF-16,BOM).txt", ss, endian) == UnicodeFileIO::Ret::OK) {
		getline(ss, str);

		WriteString("..\\TestData\\Unicode(UTF-16,BOM)_out.txt", ss, endian);
	}

}

bool test2() {
	UnicodeFileIO::Manager<basic_istream<char16_t>> ism;
	UnicodeFileIO::Manager<basic_ostream<char16_t>> osm;
	basic_string<char16_t> str;
	if (ism.OpenStream("..\\TestData\\Unicode(UTF-16,BOM).txt") == Ret::OK) {
		basic_istream<char16_t>& is = *(ism.pStream);
		getline(is, str);

		ism.CloseStream();

		if (osm.OpenStream("..\\TestData\\Unicode(UTF-16,BOM)_out.txt", ism.endian) == Ret::OK) {
			basic_ostream<char16_t>& os = *(osm.pStream);
			os << str;

			osm.CloseStream();
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}


int main() {
	//test();
	test2();
	return 0;
}