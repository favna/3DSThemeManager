// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "audio.h"
#include <tremor/ivorbiscodec.h>
#include <tremor/ivorbisfile.h>

Audio* currentPlayingAudio;
bool audioIsPlaying = false;

string ov_error_to_string(int err){
	switch (err){
		case OV_EREAD:
			return "A read from media returned an error";
		case OV_ENOTVORBIS:
			return "Bitstream is not Vorbis data";
		case OV_EVERSION:
			return "Vorbis version mismatch";
		case OV_EBADHEADER:
			return "Invalid Vorbis bitstream header";
		case OV_EFAULT:
			return "Internal logic fault";
		default:
			return string("Unknown error (") + to_string(err) + ")";
	}
}

void ogg_play(){
	long size = currentPlayingAudio->waveBuf[currentPlayingAudio->block].nsamples * 4 - currentPlayingAudio->blockPos;

	if(currentPlayingAudio->waveBuf[currentPlayingAudio->block].status == NDSP_WBUF_DONE){
		read:
		currentPlayingAudio->status = ov_read(&currentPlayingAudio->vf, (char*)currentPlayingAudio->waveBuf[currentPlayingAudio->block].data_vaddr + currentPlayingAudio->blockPos, size, &currentPlayingAudio->section);

		if(currentPlayingAudio->status <= 0){
			ov_clear(&currentPlayingAudio->vf);

			if(currentPlayingAudio->status < 0)
				ndspChnReset(0);
			else {
				currentPlayingAudio->status = ov_open(fopen(currentPlayingAudio->filename.c_str(), "rb"), &currentPlayingAudio->vf, NULL, 0);
				goto read;
			}
		} else {
			currentPlayingAudio->blockPos += currentPlayingAudio->status;
			if (currentPlayingAudio->status == size){
				currentPlayingAudio->blockPos = 0;
				ndspChnWaveBufAdd(0, &currentPlayingAudio->waveBuf[currentPlayingAudio->block]);
				currentPlayingAudio->block = !currentPlayingAudio->block;
			}
		}
	}
}

void ogg_thread(void*){
	while (audioIsPlaying && !closing)
		ogg_play();

	currentPlayingAudio = NULL;
	threadsRunning--;
}

void AUDIO_load(string path){
	currentPlayingAudio = new Audio;

	currentPlayingAudio->filename = path;
	currentPlayingAudio->blockPos = 0;
	currentPlayingAudio->block = false;

	memset(&(currentPlayingAudio->waveBuf[0]), 0, sizeof(ndspWaveBuf));
	memset(&(currentPlayingAudio->waveBuf[1]), 0, sizeof(ndspWaveBuf));

	memset(currentPlayingAudio->mix, 0, sizeof(currentPlayingAudio->mix));
	currentPlayingAudio->mix[0] = currentPlayingAudio->mix[1] = 1.0;

	ndspChnSetInterp(0, NDSP_INTERP_LINEAR);
	ndspChnSetRate(0, 44100);
	ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);
	ndspChnSetMix(0, currentPlayingAudio->mix);

	FILE *mus = fopen(path.c_str(), "rb");
	currentPlayingAudio->status = ov_open(mus, &currentPlayingAudio->vf, NULL, 0);
	if(currentPlayingAudio->status){
		currentPlayingAudio = NULL;
		audioIsPlaying = false;

		return throwError(i18n("err_bgm_fail") + L" -- " + s2ws(ov_error_to_string(currentPlayingAudio->status)));
	}

	vorbis_info *vi = ov_info(&currentPlayingAudio->vf, -1);

	ndspChnSetRate(0, vi->rate);

	currentPlayingAudio->waveBuf[0].nsamples = currentPlayingAudio->waveBuf[1].nsamples = vi->rate / 4;

	currentPlayingAudio->waveBuf[0].status = currentPlayingAudio->waveBuf[1].status = NDSP_WBUF_DONE;

	currentPlayingAudio->waveBuf[0].data_vaddr = linearAlloc(40960);
	currentPlayingAudio->waveBuf[1].data_vaddr = linearAlloc(40960);

	threadsRunning++;
	currentPlayingAudio->thread = threadCreate(ogg_thread, nullptr, 0x1000, 0x3F, -2, false);
}

void AUDIO_stop(){
	audioIsPlaying = false;
}
