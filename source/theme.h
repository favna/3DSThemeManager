// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#pragma once

#include "global.h"
#include "ui.h"

struct Theme {
	string fileName;
	string title;
	string description;
	string author;
	sf2d_texture* icon;
	sf2d_texture* preview;

	bool isZip;

	bool hasBGM;
	bool hasPreview;

	//LightLock lock_icon;
	LightLock preview_lock;
};

extern vector<Theme> themes;
extern bool themesScanned;
extern bool isInstalling;
extern string installProgress;

void scanThemes(void*);
void loadPreview(void*);
void installTheme(void*);

void toggleBGM();
