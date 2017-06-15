// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "global.h"

const bool HOMEBREW = envIsHomebrew();
bool NEW3DS = false;

volatile bool closing = false;

appState STATE = {
	-1, // debug (GFX_TOP, GFX_BOTTOM, -1)
	false // shouldSaveConfig
};

u64 ARCHIVE_SD;
u64 ARCHIVE_HomeExt;
u64 ARCHIVE_ThemeExt;

bool isError = false;
bool errorIsFatal = false;
wstring error = L"";

void throwError(string err, Result res, bool fatal){
	throwError(wstring(err.begin(), err.end()), res, fatal);
}

void throwError(wstring err, Result res, bool fatal){
	printf("%s:%d\n", __FILE__, __LINE__);
	error = err;

	if(res){
		wstringstream ss;
		ss << hex << res;
		error += L"\n\n(0x" + ss.str() + L")";
	}

	isError = true;
	errorIsFatal = fatal;
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

string wrap(string text, size_t line_length) {
	replace(text.begin(), text.end(), '\n', '\xFF');

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

	string toReturn = wrapped.str();
	replace(toReturn.begin(), toReturn.end(), '\xFF', '\n');

	return toReturn;
}

wstring wrap(wstring text, size_t line_length){
	replace(text.begin(), text.end(), L'\n', L'\xFF');

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

	wstring toReturn = wrapped.str();
	replace(toReturn.begin(), toReturn.end(), L'\xFF', L'\n');

	return toReturn;
}

int numOfDigits(const string str){
	int num = 0;
	for (size_t i = 0; i < str.size(); i++)
		if(isdigit(str[i]))
			num++;

	return num;
}

bool hasSuffix(const string& str, const string& suffix){
	return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
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

int fileToVector(string path, vector<char>& vector, int maxSize){
	ifstream file(path, ios::in | ios::binary);
	if(file.is_open()){
		file.seekg(0, ios_base::end);
		int fileSize = maxSize <= 0 ? (int)file.tellg() : maxSize;
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
	char* tmp_buf = new char[8192];

	do {
		err = unzReadCurrentFile(zipFile, tmp_buf, 8192);

		if(err < 0){
			if(tmp_buf)
				delete[] tmp_buf;

			return -1;
		} else
			vector.insert(vector.end(), tmp_buf, tmp_buf + err);
	} while(err > 0);

	if(tmp_buf)
		delete[] tmp_buf;

	return 0;
}
