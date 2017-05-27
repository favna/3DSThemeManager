// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "i18n.h"

#include "i18n/english.cpp"
#include "i18n/french.cpp"
#include "i18n/german.cpp"
#include "i18n/italian.cpp"
#include "i18n/spanish.cpp"
#include "i18n/portuguese.cpp"
#include "i18n/russian.cpp"

u8 lang = 1;

wstring i18n(const string& token, ...){
	map<string, wstring> langMap;

	/*
		CFG_LANGUAGE_JP = 0,  ///< Japanese
		CFG_LANGUAGE_EN = 1,  ///< English
		CFG_LANGUAGE_FR = 2,  ///< French
		CFG_LANGUAGE_DE = 3,  ///< German
		CFG_LANGUAGE_IT = 4,  ///< Italian
		CFG_LANGUAGE_ES = 5,  ///< Spanish
		CFG_LANGUAGE_ZH = 6,  ///< Simplified Chinese
		CFG_LANGUAGE_KO = 7,  ///< Korean
		CFG_LANGUAGE_NL = 8,  ///< Dutch
		CFG_LANGUAGE_PT = 9,  ///< Portuguese
		CFG_LANGUAGE_RU = 10, ///< Russian
		CFG_LANGUAGE_TW = 11, ///< Traditional Chinese
	*/

	switch(lang){
		case CFG_LANGUAGE_FR:
			langMap = FRENCH;
			break;
		case CFG_LANGUAGE_DE:
			langMap = GERMAN;
			break;
		case CFG_LANGUAGE_IT:
			langMap = ITALIAN;
			break;
		case CFG_LANGUAGE_ES:
			langMap = SPANISH;
			break;
		case CFG_LANGUAGE_PT:
			langMap = PORTUGUESE;
			break;
		case CFG_LANGUAGE_RU:
			langMap = RUSSIAN;
			break;
		default:
			langMap = ENGLISH;
			break;
	}

	wstring toBeReturned;

	if(langMap.find(token) == langMap.end())
		if(langMap == ENGLISH){
			string toBeConverted = string(token);
			return wstring(toBeConverted.begin(), toBeConverted.end());
		} else
			if(ENGLISH.find(token) == ENGLISH.end()){
				string toBeConverted = string(token);
				return wstring(toBeConverted.begin(), toBeConverted.end());
			} else
				toBeReturned = ENGLISH[token];
	else
		toBeReturned = langMap[token];

	if(toBeReturned.size() == 0)
		return wstring(L"");

	int final_n, n = ((int)toBeReturned.size()) * 2;
	string str;
	unique_ptr<wchar_t[]> formatted;
	va_list ap;

	while(1){
		formatted.reset(new wchar_t[n]);
		wcscpy(&formatted[0], toBeReturned.c_str());
		va_start(ap, toBeReturned);
		final_n = vswprintf(&formatted[0], n, toBeReturned.c_str(), ap);
		va_end(ap);

		if(final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}

	return wstring(formatted.get());
}
