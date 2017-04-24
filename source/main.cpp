// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "global.h"

#include "config.h"
#include "ui.h"
#include "input.h"
#include "thread.h"
#include "network.h"
#include "theme.h"
#include "audio.h"

// Initiates services, starts up modules
void start() {
	Result ret;

	// Initiate services
	romfsInit();
	httpcInit(0);
	fsInit();
	cfguInit();
	ndspInit();
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);
	/*
	ret = nsInit();
	printf("%lX\n", ret);

	ret = NS_TerminateProcessTID(0x0004013000001f02);
	printf("%lX\n", ret);
	ret = NS_LaunchTitle(0x0004013000001f02, 0, NULL);
	printf("%lX\n", ret);
	nsExit();

	Handle mcutrcHandle;

	ret = srvGetServiceHandle(&mcutrcHandle, "mcu::RTC");
    printf("%lX\n", ret);

    u32 *cmdbuf = getThreadCommandBuffer();
    cmdbuf[0] = IPC_MakeHeader(0x51,1,0); // 0x510040
    cmdbuf[1] = true;

    ret = svcSendSyncRequest(mcutrcHandle);
    printf("%lX\n", ret);

    svcCloseHandle(mcutrcHandle);
	*/
	// Initiate libraries
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0x21, 0x21, 0x21, 0xFF));
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

	// Create directory
	FSUSER_CreateDirectory(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/"), FS_ATTRIBUTE_DIRECTORY);
	FSUSER_CreateDirectory(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/"), FS_ATTRIBUTE_DIRECTORY);
	FSUSER_DeleteDirectoryRecursively(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/tmp"));
	FSUSER_CreateDirectory(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/tmp/"), FS_ATTRIBUTE_DIRECTORY);
	FSUSER_CreateDirectory(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/Themes"), FS_ATTRIBUTE_DIRECTORY);

	// Load config
	CONFIG_load();

	// Start UI
	UI_start();

	// Start thread workers
	startWorkers();
}

// Closes services, modules, etc.
void cleanup() {
	// clean task queue; wait for threads
	closing = true;

	cleanTaskQueue();
	while (true) {
		if(threadsRunning == 0)
			break;

		svcSleepThread(1000000ULL * 100);
	}

	httpcExit();

	if(STATE.shouldSaveConfig)
		CONFIG_save();

	FSUSER_DeleteDirectoryRecursively(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/tmp"));

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
}

int main(int argc, char **argv) {
	start();

	queueTask(checkForUpdate, 0);
	queueTask(scanThemes, 0);

	// Main loop
	while (aptMainLoop() && !(hidKeysDown() & KEY_START) && !closing){
		INPUT_handle();
		UI_update();
	}

	cleanup();

	return 0;
}
