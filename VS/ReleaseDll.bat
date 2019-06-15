mkdir UnicodeFileIO
mkdir UnicodeFileIO\lib
mkdir UnicodeFileIO\lib\x86
mkdir UnicodeFileIO\lib\x86\Debug
mkdir UnicodeFileIO\lib\x86\Release
mkdir UnicodeFileIO\lib\x64
mkdir UnicodeFileIO\lib\x64\Debug
mkdir UnicodeFileIO\lib\x64\Release
mkdir UnicodeFileIO\include
mkdir UnicodeFileIO\PropertySheet

copy Debug\libUnicodeFileIO.* UnicodeFileIO\lib\x86\Debug
copy Release\libUnicodeFileIO.* UnicodeFileIO\lib\x86\Release

copy x64\Debug\libUnicodeFileIO.* UnicodeFileIO\lib\x64\Debug
copy x64\Release\libUnicodeFileIO.* UnicodeFileIO\lib\x64\Release

copy libUnicodeFileIO\UnicodeFileIO.h UnicodeFileIO\include

copy UnicodeFileIO.props UnicodeFileIO\PropertySheet

del UnicodeFileIO.zip
powershell compress-archive UnicodeFileIO UnicodeFileIO.zip
pause
rmdir /s /q UnicodeFileIO
pause