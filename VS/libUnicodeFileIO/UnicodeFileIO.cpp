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

#include "stdafx.h"

#include"UnicodeFileIO.h"

using namespace std;
using namespace UnicodeFileIO;

UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL UnicodeFileIO::ReadString(const char* srcFileName, basic_stringstream<char16_t>& des, Endian &endian) {
	Ret ret;
	union {
		char c[sizeof(char16_t)];
		char16_t w;
	};
	basic_ifstream<char> ifs(srcFileName, ios_base::binary);
	if (!ifs.is_open()) {
		return Ret::SrcFile_OpenErr;
	}

	try {
		ifs.read(c, sizeof(char16_t));
		if ((c[0] == (char)0xFF) && (c[1] == (char)0xFE)) {
			endian = Endian::LE;
		}
		else if ((c[0] == (char)0xFE) && (c[1] == (char)0xFF)) {
			endian = Endian::BE;
		}
		else {
			throw Ret::SrcFile_UnknownBom;
		}

		switch (endian) {
		case Endian::LE:
			while (1) {
				ifs.read(c, sizeof(char16_t));
				if (ifs.good()) {
					des.put(w);
				}
				else {
					break;
				}
			}
			break;

		case Endian::BE:
			while (1) {
				ifs.get(c[1]);
				ifs.get(c[0]);
				if (ifs.good()) {
					des.put(w);
				}
				else {
					break;
				}
			}
			break;

		default:
			throw Ret::SrcFile_UnknownBom;
			break;
		}

		if (ifs.eof()) {
			ret = Ret::OK;
		}
		else {
			ret = Ret::SrcFileStream_BadStatus;
		}
	}
	catch (const Ret& ret) {
		ifs.close();
		return ret;
	}

	ifs.close();
	return ret;
}

UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL UnicodeFileIO::WriteString(const char* desFileName, basic_stringstream<char16_t>& src, const Endian& endian) {
	Ret ret = Ret::Unknown;
	union {
		char c[sizeof(char16_t)];
		char16_t w;
	};
	basic_ofstream<char> ofs(desFileName, ios_base::binary);
	if (!ofs.is_open()) {
		return Ret::DesFile_OpenErr;
	}

	try {

		src.seekg(ios_base::beg);
		switch (endian) {
		case Endian::LE:
			ofs.put((char)0xFF);
			ofs.put((char)0xFE);
			while (1) {
				src.get(w);
				if (src.good()) {
					ofs.write(c, 2);
				}
				else {
					break;
				}
			}
			break;

		case Endian::BE:
			ofs.put((char)0xFE);
			ofs.put((char)0xFF);
			while (1) {
				src.get(w);
				if (src.good()) {
					ofs.put(c[1]);
					ofs.put(c[0]);
				}
				else {
					break;
				}
			}
			break;

		default:
			throw Ret::SrcFile_UnknownBom;
			break;
		}


		if (src.eof()) {
			if (ofs.good()) {
				ret = Ret::OK;
			}
			else {
				ret = Ret::DesFileStream_BadStatus;
			}
		}
		else {
			ret = Ret::SrcStringStream_BadStatus;
		}
	}
	catch (const Ret& ret) {
		ofs.close();
		return ret;
	}

	ofs.close();
	return ret;
}