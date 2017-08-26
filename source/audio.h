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
#include "thread.h"
#include "i18n.h"

#include <tremor/ivorbisfile.h>

struct Audio {
	string filename;
	OggVorbis_File vf;
	ndspWaveBuf waveBuf[2];
	float mix[12];
	long status;
	int section;
	bool block;
	unsigned long blockPos;
	Thread thread;
};

extern struct Audio* currentPlayingAudio;
extern bool audioIsPlaying;

void AUDIO_load(string);
void AUDIO_stop();
