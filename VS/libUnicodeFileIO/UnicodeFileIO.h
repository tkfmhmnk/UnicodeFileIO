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

#ifndef _UNICODE_FILE_INPUT_H
#define _UNICODE_FILE_INPUT_H

#ifdef LIBUNICODEFILEIO_EXPORTS
#define UNICODEFILEIO_ATTRBT __declspec(dllexport)
#else
#define UNICODEFILEIO_ATTRBT __declspec(dllimport)
#endif

#define UNICODEFILEIO_CALL __stdcall

#include <iostream>
#include <fstream>
#include<sstream>
#include <string>
namespace UnicodeFileIO {
	enum class utf16_endian : int {
		BE,									//!<�r�b�O�G���f�B�A��
		LE									//!<���g���G���f�B�A��
	};
	enum class Ret : int {
		Unknown = 0,
		OK,									//!<����
		SrcFile_OpenErr,					//!<�\�[�X�t�@�C���̃I�[�v���G���[
		SrcFile_UnknownBom,					//!<�\�[�X�t�@�C���̕s����BOM
		SrcFileStream_BadStatus,			//!<�\�[�X�t�@�C���̃X�g���[���̃G���[
		DesFile_OpenErr,					//!<�o�̓t�@�C���̃I�[�v���G���[
		SrcStringStream_BadStatus,			//!<����StringStream�̃X�g���[���̃G���[
		DesFileStream_BadStatus,			//!<�o�̓t�@�C���̃X�g���[���̃G���[

	};

	UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL ReadString(const char* srcFileName, std::basic_stringstream<char16_t>& des, utf16_endian &endian);
	UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL WriteString(const char* desFileName, std::basic_stringstream<char16_t>& src, const utf16_endian &endian);
}

#endif