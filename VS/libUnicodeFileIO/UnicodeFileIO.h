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
	enum class Endian : int {
		None,								//!<�G���f�B�A������
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
		MemoryAllocationErr,				//!<�������m�ۂ̃G���[
		NullPointerRefernce,				//!<�����̃|�C���^��Null
		InvalidStreamObject,				//!<�s���ȃX�g���[����object���^����ꂽ
	};

	UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL ReadString	(const char* srcFileName, std::basic_stringstream<char16_t>& des, Endian &endian);
	UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL WriteString	(const char* desFileName, std::basic_stringstream<char16_t>& src, const Endian &endian);

	UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL OpenStream	(const char* srcFileName, std::basic_istream<char>** ppStream, Endian& endian);
	UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL OpenStream	(const char* desFileName, std::basic_ostream<char>** ppStream, Endian& endian);
	UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL CloseStream	(const char* srcFileName, std::basic_istream<char>** ppStream, const Endian& endian);
	UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL CloseStream	(const char* desFileName, std::basic_ostream<char>** ppStream, const Endian& endian);

	UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL OpenStream	(const char* srcFileName, std::basic_istream<char16_t>** ppStream, Endian& endian);
	UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL OpenStream	(const char* desFileName, std::basic_ostream<char16_t>** ppStream, Endian& endian);
	UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL CloseStream	(const char* srcFileName, std::basic_istream<char16_t>** ppStream, const Endian& endian);
	UNICODEFILEIO_ATTRBT Ret UNICODEFILEIO_CALL CloseStream	(const char* desFileName, std::basic_ostream<char16_t>** ppStream, const Endian& endian);

}

#endif