// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#pragma once

#include "global.h"
#include "theme.h"
#include "thread.h"
#include "audio.h"
#include "network.h"

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
};

extern textureList TEXTURE;
extern fontList FONT;

extern int currentSelectedItem;

extern float previewX;

void UI_start();
void UI_cleanup();
void UI_update();

Result load_png(string, sf2d_texture**, bool safe = true);
Result load_png_mem(vector<char>&, sf2d_texture**, bool safe = true);

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
