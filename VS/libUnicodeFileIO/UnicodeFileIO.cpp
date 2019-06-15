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
#include<cstdlib>
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


template<> Ret UNICODEFILEIO_CALL UnicodeFileIO::Manager<basic_istream<char16_t>>::OpenStream(const char* fileName, Endian _endian) {
	Ret ret;
	pStream = new basic_stringstream<char16_t>;

	if (pStream == nullptr) {
		ret = Ret::MemoryAllocationErr;
	}
	else {
		ret = ReadString(fileName, *((basic_stringstream<char16_t>*)pStream), endian);
		if (ret != Ret::OK) {
			delete pStream;
		}
	}
	return ret;
}
template<> Ret UNICODEFILEIO_CALL UnicodeFileIO::Manager<basic_istream<char16_t>>::CloseStream() {
	delete pStream;
	return Ret::OK;
}

template<> Ret UNICODEFILEIO_CALL UnicodeFileIO::Manager<basic_ostream<char16_t>>::OpenStream(const char* fileName, Endian _endian) {
	fileNameBuf = new char[bufSize];
	if (fileNameBuf == nullptr) return Ret::MemoryAllocationErr;
	if(strcpy_s(fileNameBuf, bufSize, fileName)!=0) return Ret::BufferErr;

	if (_endian == Endian::None) return Ret::DesFile_UnknownBom;
	endian = _endian;

	pStream = new basic_stringstream<char16_t>;
	if (pStream == nullptr) {
		return Ret::MemoryAllocationErr;
	}
	else {
		return Ret::OK;
	}
	return Ret::OK;
}

template<> Ret UNICODEFILEIO_CALL UnicodeFileIO::Manager<basic_ostream<char16_t>>::CloseStream() {
	Ret ret;
	if (pStream == nullptr) {
		ret = Ret::NullPointerRefernce;
	}
	else {
		basic_stringstream<char16_t>* pStringStream = dynamic_cast<basic_stringstream<char16_t>*>(pStream);
		if (pStringStream == nullptr) {
			ret = Ret::InvalidStreamObject;
			delete pStream;
		}
		else {
			ret = WriteString(fileNameBuf, *pStringStream, endian);
			delete pStringStream;
		}
	}
	delete[] fileNameBuf;
	return ret;
}

template<> Ret UNICODEFILEIO_CALL UnicodeFileIO::Manager<basic_istream<char>>::OpenStream(const char* fileName, Endian _endian) {
	Ret ret;
	pStream = new basic_ifstream<char>(fileName);

	if (pStream == nullptr) {
		ret = Ret::MemoryAllocationErr;
	}
	else {
		if (((basic_ifstream<char>*)pStream)->is_open()) {
			ret = Ret::OK;
		}
		else {
			delete pStream;
			ret = Ret::SrcFile_OpenErr;
		}
	}
	return ret;
}

template<> Ret UNICODEFILEIO_CALL UnicodeFileIO::Manager<basic_istream<char>>::CloseStream() {
	Ret ret;
	if (pStream == nullptr) {
		ret = Ret::NullPointerRefernce;
	}
	else {
		basic_ifstream<char>* pifstream = dynamic_cast<basic_ifstream<char>*>(pStream);
		if (pifstream == nullptr) {
			ret = Ret::InvalidStreamObject;
			delete pStream;
		}
		else {
			pifstream->close();
			delete pifstream;
			ret = Ret::OK;
		}
	}
	return ret;
}

template<> Ret UNICODEFILEIO_CALL UnicodeFileIO::Manager<basic_ostream<char>>::OpenStream(const char* fileName, Endian _endian) {
	Ret ret;
	pStream = new basic_ofstream<char>(fileName);

	if (pStream == nullptr) {
		ret = Ret::MemoryAllocationErr;
	}
	else {
		if (((basic_ofstream<char>*)pStream)->is_open()) {
			ret = Ret::OK;
		}
		else {
			delete pStream;
			ret = Ret::DesFile_OpenErr;
		}
	}
	return ret;
}
template<> Ret UNICODEFILEIO_CALL UnicodeFileIO::Manager<basic_ostream<char>>::CloseStream() {
	Ret ret;
	if (pStream == nullptr) {
		ret = Ret::NullPointerRefernce;
	}
	else {
		basic_ofstream<char>* pofstream = dynamic_cast<basic_ofstream<char>*>(pStream);
		if (pofstream == nullptr) {
			ret = Ret::InvalidStreamObject;
			delete pStream;
		}
		else {
			pofstream->close();
			delete pofstream;
			ret = Ret::OK;
		}
	}
	return ret;
}