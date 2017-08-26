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
#include "theme.h"
#include "thread.h"
#include "audio.h"
#include "network.h"
#include "camera.h"
#include "i18n.h"

#include <sftd.h>
#include <sfil.h>

struct texture {
	sf2d_texture* tx;
	bool loaded;
};

struct textureList {
	texture ui;
	texture rect;
};

struct fontList {
	sftd_font* normal;
	sftd_font* small;
	sftd_font* light;
	sftd_font* huge;
};

extern textureList TEXTURE;
extern fontList FONT;

extern int currentSelectedItem;
extern int updateTextOffset;

extern float previewX;

void UI_start();
void UI_cleanup();
void UI_update();

Result load_png(string, sf2d_texture**);
Result load_png_mem(vector<char>&, sf2d_texture**);

void selectTheme(int);
void toggleFullscreen();

struct Animation {
	float start;
	float end;
	float duration;

	float* valueToChange;

	int timeStarted;
	bool isActive;
};

int startAnimation(float*, float, float);
void runAnimations();

extern vector<Animation> animationList;

float easeOutQuint(float, float, float, float);
