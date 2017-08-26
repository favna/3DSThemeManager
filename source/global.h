// Copyright (C) 2017 ErmanSayin
// 
// This file is part of Themely.
// 
// Themely is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Themely is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Themely.  If not, see <http://www.gnu.org/licenses/>.
// 

#pragma once

#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <deque>
#include <locale>
#include <codecvt>
#include <regex>

#include <3ds.h>
#include <sf2d.h>

using namespace std;

#include "lib/unzip.h"

struct appState {
	int debug;
	bool shouldSaveConfig;
};

extern const bool HOMEBREW;
extern bool NEW3DS;

extern volatile bool closing;

extern appState STATE;

extern u64 ARCHIVE_SD;
extern u64 ARCHIVE_HomeExt;
extern u64 ARCHIVE_ThemeExt;

extern bool isError;
extern bool errorIsFatal;
extern wstring error;

void throwError(string, Result res = 0, bool fatal = false);
void throwError(wstring, Result res = 0, bool fatal = false);

wstring s2ws(const string&);
string ws2s(const wstring&);
string wrap(string, size_t);
wstring wrap(wstring, size_t);
int numOfDigits(const string);
bool hasSuffix(const string&, const string&);

bool fileExists(u16string);
string u16tstr(u16*, size_t);
u16string strtu16str(string&);
int fileToVector(string, vector<char>&, int maxSize = -1);
//size_t read_file_to_mem(char**, const char*, u32);

int zippedFileToVector(unzFile, vector<char>&);
