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
string error = "";

void throwError(string err){
	printf("%s:%d:%s\n", __FILE__, __LINE__, err.c_str());
	error = err;
	isError = true;
}

void utf2ascii(char* dst, u16* src){
	if(!src || !dst)
		return;

	while (*src)
		*(dst++) = (*(src++)) & 0xFF;

	*dst = 0x00;
}

void ascii2utf(u16* dst, char* src){
	if(!src || !dst)
		return;

	while (*src)
		*(dst++) = (*src++);

	*dst = 0x00;
}

bool invalidChar(char c){
	return !(c>=0 && c <128);
}

void stripUnicode(string &str){
	str.erase(remove_if(str.begin(),str.end(), invalidChar), str.end());
}

string wrap(const string text, size_t line_length){
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
		fsMakePath(PATH_UTF16, name.data()),
		FS_OPEN_READ,
		0x00000000
	) == 0;

	FSFILE_Close(tempFileHandle);

	return b;
}

string u16tstr(u16* str, size_t size){
	if(size == 0)
		return string("");

	char* out = new char[size];
	size_t len = utf16_to_utf8((uint8_t*)out, str, size);
	out[min(len, size)] = '\0';

	return string(out);
}

u16string strtu16str(string& str){
	if(str.size() == 0)
		return u16string(u"");

	char16_t* out = new char16_t[str.size()];
	size_t len = utf8_to_utf16((uint16_t*)out, (uint8_t*)&str[0], str.size());
	out[min(len, str.size())] = '\0';

	return u16string(out);
}

/*
size_t read_file_to_mem(char** data, const char* path, u32 offset){
	FILE* fp;
	u32 file_size;

	fp = fopen(path, "rb");
	if(!fp)
		return 0;

	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);

	if(file_size <= offset){
		fclose(fp);
		return 0;
	}

	fseek(fp, offset, SEEK_SET);
	file_size -= offset;

	*data = (char*)malloc(file_size);
	if(!*data){
		fclose(fp);
		return 0;
	}

	if(fread(*data, 1, file_size, fp) != file_size){
		free(*data);
		fclose(fp);
		return 0;
	}

	fclose(fp);

	return file_size;
}
*/

int fileToVector(string path, vector<char>& vector){
	ifstream file(path, ios::in | ios::binary);
	if(file.is_open()){
		file.seekg(0, ios_base::end);
		streampos fileSize = file.tellg();
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
