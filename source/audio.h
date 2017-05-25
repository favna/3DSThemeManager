// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

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
