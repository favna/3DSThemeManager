// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

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
