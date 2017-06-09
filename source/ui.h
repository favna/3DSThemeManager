// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

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
