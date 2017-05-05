// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "input.h"

touchPosition initialTouch;
touchPosition lastTouch;

int XHeldLength = 0;

bool checkTouch(int ax1, int ay1, int ax2, int ay2){
	int ix = initialTouch.px;
	int iy = initialTouch.py;
	int lx = lastTouch.px;
	int ly = lastTouch.py;

	if(ax1 <= ix && ix <= ax2 + ax1 && ay1 <= iy && iy <= ay2 + ay1 &&
	   ax1 <= lx && lx <= ax2 + ax1 && ay1 <= ly && ly <= ay2 + ay1)
		return true;

	return false;
}

void INPUT_handle(){
	hidScanInput();

	if(isError || isInstalling || themes.size() == 0 || !themesScanned)
		return;

	u32 kDown = hidKeysDown();
	u32 kHeld = hidKeysHeld();
	u32 kUp = hidKeysUp();
	touchPosition touchPos;
	hidTouchRead(&touchPos);
	circlePosition circlePos;
	hidCircleRead(&circlePos);

	if(kDown & KEY_TOUCH){
		lastTouch = touchPos;
		initialTouch = touchPos;
	}

	if(update.size() != 0){
		if(kDown & KEY_A){
			update = "Updating...";
			UI_update();
			installUpdate();
		}

		if(kDown & KEY_B)
			update = "";

		return;
	}

	if(deletePrompt){
		if(kUp & KEY_A)
			deleteTheme();
		else if(kUp & KEY_B)
			deletePrompt = false;

		return;
	}

	if(dumpPrompt){
		if(kUp & KEY_A)
			dumpTheme();
		else if(kUp & KEY_B)
			dumpPrompt = false;

		return;
	}

	if(kUp & KEY_B && shuffleMode)
		exitShuffleMode();

	if(themesScanned){
		if(previewX == 8.f){
			if(kDown & KEY_Y)
				toggleBGM();

			if(currentPlayingAudio || audioIsPlaying)
				return;

			if(kDown & KEY_X || kHeld & KEY_X)
				XHeldLength++;
			else
				XHeldLength = 0;

			if(XHeldLength >= 15)
				deletePrompt = true;

			if(kDown & KEY_DOWN)
				selectTheme(currentSelectedItem + 1);

			if(kDown & KEY_UP)
				selectTheme(currentSelectedItem - 1);

			if(kDown & KEY_LEFT)
				selectTheme(max(0, currentSelectedItem - 5));

			if(kDown & KEY_RIGHT)
				selectTheme(min((int)themes.size() - 1, currentSelectedItem + 5));

			if(kUp & KEY_A){
				if(shuffleMode){
					int selected = 0;
					for (size_t i = 0; i < themes.size(); i++)
						if(themes[i].toShuffle)
							selected++;

					if(!themes[currentSelectedItem].toShuffle && selected < 10){
						themes[currentSelectedItem].toShuffle = true;

						if(kHeld & KEY_R)
							themes[currentSelectedItem].shuffleNoBGM = true;
					} else {
						themes[currentSelectedItem].toShuffle = false;
						themes[currentSelectedItem].shuffleNoBGM = false;
					}
				} else {
					isInstalling = true;
					if(kHeld & KEY_R)
						queueTask(installTheme, (void*)true);
					else
						queueTask(installTheme, (void*)false);
				}
			}

			if(kUp & KEY_TOUCH){
				if(shuffleMode){
					if(checkTouch(320 - 3*2 - 24*2, 3, 24, 24)){
						int selected = 0;
						for (size_t i = 0; i < themes.size(); i++)
							if(themes[i].toShuffle)
								selected++;

						if(selected > 1){
							isInstalling = true;
							queueTask(installShuffle, (void*)0);
						}
					}

					if(checkTouch(320 - 3 - 24, 3, 24, 24))
						exitShuffleMode();
				} else {
					if(checkTouch(320 - 3 - 24, 3, 24, 24))
						dumpPrompt = true;

					if(checkTouch(3, 3, 24, 24))
						shuffleMode = true;
				}
			}
		}

		if(kUp & KEY_X)
			if((previewX == 8.f || previewX == 0.f) && themes[currentSelectedItem].hasPreview)
				toggleFullscreen();
	}

	if(kHeld & KEY_TOUCH)
		lastTouch = touchPos;
}
