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
#include "ui.h"

struct Theme {
	string fileName;
	wstring title;
	wstring description;
	wstring author;

	sf2d_texture* icon;
	sf2d_texture* preview;

	bool isZip;
	bool infoIsloaded;
	bool infoIsFullyLoaded;
	bool previewIsLoaded;

	bool hasBGM;
	bool hasPreview;

	bool toShuffle;
	bool shuffleNoBGM;

	LightLock lock;
};

extern deque<Theme> themes;
extern bool themesScanned;
extern bool isInstalling;
extern wstring installProgress;

extern bool deletePrompt;
extern bool dumpPrompt;

extern bool shuffleMode;

void loadTheme(void*);
void scanThemes(void*);
void loadThemeInfo(void*);
void checkInfosToBeLoaded(int);
void loadPreview(void*);
void installTheme(void*);

void installShuffle(void*);
void exitShuffleMode();

void deleteTheme();
void dumpTheme();

void toggleBGM();
