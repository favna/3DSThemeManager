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

#include "global.h"

#include "config.h"
#include "ui.h"
#include "input.h"
#include "thread.h"
#include "network.h"
#include "theme.h"
#include "audio.h"
#include "i18n.h"

// Initiates services, starts up modules
void start(){
	if(!HOMEBREW)
		srvPublishToSubscriber(0x202, 0);

	Result ret;

	APT_SetAppCpuTimeLimit(30);
	osSetSpeedupEnable(true);

	APT_CheckNew3DS(&NEW3DS);

	// Initiate services
	romfsInit();
	httpcInit(0);
	fsInit();
	cfguInit();
	ndspInit();
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);

	// Initiate libraries
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0x11, 0x11, 0x11, 0xFF));
	sf2d_set_vblank_wait(1);

	sftd_init();

	if(STATE.debug == 0 || STATE.debug == 1)
		consoleInit((gfxScreen_t)STATE.debug, NULL);

	// Open archives
	FSUSER_OpenArchive(&ARCHIVE_SD, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
	u32 extdataID_homemenu, extdataID_theme;
	u8 region = 0;
	CFGU_SecureInfoGetRegion(&region);

	if(region == 1){ // USA
		extdataID_homemenu = 0x0000008f;
		extdataID_theme = 0x000002cd;
	} else if(region == 2){ // EUR
		extdataID_homemenu = 0x00000098;
		extdataID_theme = 0x000002ce;
	} else { // Elsewhere
		extdataID_homemenu = 0x00000082;
		extdataID_theme = 0x000002cc;
	}

	u32 homemenu_path[3] = {MEDIATYPE_SD, extdataID_homemenu, 0};
	FSUSER_OpenArchive(&ARCHIVE_HomeExt, ARCHIVE_EXTDATA, {PATH_BINARY, 0xC, homemenu_path});

	u32 theme_path[3] = {MEDIATYPE_SD, extdataID_theme, 0};
	FSUSER_OpenArchive(&ARCHIVE_ThemeExt, ARCHIVE_EXTDATA, {PATH_BINARY, 0xC, theme_path});

	// Get system language
	CFGU_GetSystemLanguage(&lang);

	// Create directory
	FSUSER_CreateDirectory(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/"), FS_ATTRIBUTE_DIRECTORY);
	FSUSER_CreateDirectory(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/3DSThemeManager/"), FS_ATTRIBUTE_DIRECTORY);
	FSUSER_DeleteDirectoryRecursively(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/3DSThemeManager/tmp"));
	FSUSER_CreateDirectory(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/3DSThemeManager/tmp/"), FS_ATTRIBUTE_DIRECTORY);
	FSUSER_CreateDirectory(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/Themes"), FS_ATTRIBUTE_DIRECTORY);

	// Load config
	CONFIG_load();

	// Start UI
	UI_start();

	// Start thread workers
	startWorkers();
}

// Closes services, modules, etc.
void cleanup(){
	// clean task queue; wait for threads
	closing = true;

	cleanTaskQueue();
	while (true){
		if(threadsRunning == 0)
			break;

		svcSleepThread(1000000ULL * 100);
	}

	httpcExit();

	if(QRMode)
		stopQRMode();

	if(STATE.shouldSaveConfig)
		CONFIG_save();

	FSUSER_DeleteDirectoryRecursively(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/3DSThemeManager/tmp"));

	FSUSER_CloseArchive(ARCHIVE_SD);
	FSUSER_CloseArchive(ARCHIVE_HomeExt);
	FSUSER_CloseArchive(ARCHIVE_ThemeExt);

	UI_cleanup();
	sftd_fini();

	sf2d_fini();

	ndspExit();
	romfsExit();
	fsExit();
	cfguExit();

	while(!HOMEBREW && aptMainLoop())
		svcSleepThread(20e6);
}

int main(int argc, char **argv){
	start();

	queueTask(checkForUpdate, 0);
	queueTask(scanThemes, 0);

	// Main loop
	while(!closing){
		if(INPUT_handle())
			break;

		UI_update();
	}

	cleanup();

	return 0;
}
