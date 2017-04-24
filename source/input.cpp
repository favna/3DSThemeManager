// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "input.h"

void INPUT_handle() {
	hidScanInput();

	if(isError || isInstalling || themes.size() == 0)
		return;

	u32 kDown = hidKeysDown();
	u32 kHeld = hidKeysHeld();
	u32 kUp = hidKeysUp();
	touchPosition touchPos;
	hidTouchRead(&touchPos);
	circlePosition circlePos;
	hidCircleRead(&circlePos);

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

	if(themesScanned){
		if(previewX == 8.f){
			if(kDown & KEY_Y)
				toggleBGM();

			if(currentPlayingAudio || audioIsPlaying)
				return;

			if(kDown & KEY_DOWN)
				selectTheme(currentSelectedItem + 1);

			if(kDown & KEY_UP)
				selectTheme(currentSelectedItem - 1);

			if(kDown & KEY_LEFT)
				selectTheme(max(0, currentSelectedItem - 5));

			if(kDown & KEY_RIGHT)
				selectTheme(min((int)themes.size() - 1, currentSelectedItem + 5));

			if(kDown & KEY_A){
				isInstalling = true;
				queueTask(installTheme, (void*)currentSelectedItem);
			}
		}

		if(kDown & KEY_X)
			if((previewX == 8.f || previewX == 0.f) && themes[currentSelectedItem].hasPreview)
				toggleFullscreen();
	}
}
