// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "global.h"

const bool HOMEBREW = envIsHomebrew();

volatile bool closing = false;

appState STATE = {
	-1, // debug (GFX_TOP, GFX_BOTTOM, -1)
	false // shouldSaveConfig
};

u64 ARCHIVE_SD;
u64 ARCHIVE_HomeExt;
u64 ARCHIVE_ThemeExt;

bool isError = false;
wstring error = L"";

void throwError(string err){
	throwError(wstring(err.begin(), err.end()));
}

void throwError(wstring err){
	printf("%s:%d:%ls\n", __FILE__, __LINE__, err.c_str());
	error = err;
	isError = true;
}

wstring s2ws(const string& str){
	using convert_typeX = codecvt_utf8<wchar_t>;
	wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

string ws2s(const wstring& wstr){
	using convert_typeX = codecvt_utf8<wchar_t>;
	wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

string wrap(const string text, size_t line_length) {
	istringstream words(text);
	ostringstream wrapped;
	string word;

	if(words >> word){
		wrapped << word;
		size_t space_left = line_length - word.length();
		while (words >> word){
			if(space_left < word.length() + 1){
				wrapped << '\n' << word;
				space_left = line_length - word.length();
			} else {
				wrapped << ' ' << word;
				space_left -= word.length() + 1;
			}
		}
	}

	return wrapped.str();
}

wstring wrap(const wstring text, size_t line_length){
	basic_istringstream<wchar_t> words(text);
	basic_ostringstream<wchar_t> wrapped;
	wstring word;

	if(words >> word){
		wrapped << word;
		size_t space_left = line_length - word.length();
		while (words >> word){
			if(space_left < word.length() + 1){
				wrapped << '\n' << word;
				space_left = line_length - word.length();
			} else {
				wrapped << ' ' << word;
				space_left -= word.length() + 1;
			}
		}
	}

	return wrapped.str();
}

int numOfDigits(const string str){
	int num = 0;
	for (size_t i = 0; i < str.size(); i++)
		if(isdigit(str[i]))
			num++;

	return num;
}

bool fileExists(u16string name){
	Handle tempFileHandle;

	bool b = FSUSER_OpenFileDirectly(
		&tempFileHandle,
		ARCHIVE_SDMC,
		fsMakePath(PATH_EMPTY, ""),
		fsMakePath(PATH_UTF16, name.c_str()),
		FS_OPEN_READ,
		0x00000000
	) == 0;

	FSFILE_Close(tempFileHandle);

	return b;
}

string u16tstr(u16* str, size_t size){
	if(size == 0)
		return string("");

	char* out = new char[size + 1];
	size_t len = utf16_to_utf8((uint8_t*)out, str, size);

	return string(out, min(len, size));
}

u16string strtu16str(string& str){
	if(str.size() == 0)
		return u16string(u"");

	char16_t* out = new char16_t[str.size() + 1];
	size_t len = utf8_to_utf16((uint16_t*)out, (uint8_t*)&str[0], str.size());

	return u16string(out, min(len, str.size()));
}

int fileToVector(string path, vector<char>& vector){
	ifstream file(path, ios::in | ios::binary);
	if(file.is_open()){
		file.seekg(0, ios_base::end);
		streampos fileSize = file.tellg();
		if(fileSize > vector.size())
			vector.resize(fileSize);

		file.seekg(0, ios_base::beg);
		file.read(&vector[0], fileSize);
	} else {
		file.close();
		return -1;
	}

	file.close();
	return 0;
}

int zippedFileToVector(unzFile zipFile, vector<char>& vector){
	int err = 0;
	char* tmp_buf = new char[1024];

	do {
		err = unzReadCurrentFile(zipFile, tmp_buf, 1024);

		if(err < 0){
			if(tmp_buf)
				delete[] tmp_buf;

			return -1;
		} else
			for (int i = 0; i < (int)err; i++) vector.push_back(tmp_buf[i]);
	} while(err > 0);

	if(tmp_buf)
		delete[] tmp_buf;

	return 0;
}
