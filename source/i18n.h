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

#include "global.h"
#include "config.h"
#include "audio.h"

#include <map>
#include <stdarg.h>
#include <memory>

extern u8 lang;

wstring i18n(const string&);
template<typename ... Args>
wstring i18n(const string& token, Args ... args){
	wstring toBeReturned = i18n(token);

	if(toBeReturned.size() == 0)
		return wstring(L"");

	wchar_t* tmp = new wchar_t[100];
	size_t size = swprintf(tmp, 100, toBeReturned.c_str(), args ...) + 1;
	delete[] tmp;
	unique_ptr<wchar_t[]> buf(new wchar_t[size]);
	swprintf(buf.get(), size, toBeReturned.c_str(), args ...);
	return wstring(buf.get(), buf.get() + size - 1);
}
