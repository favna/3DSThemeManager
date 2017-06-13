// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "camera.h"

extern "C" {
	#include "lib/quirc/quirc.h"
}

bool QRMode = false;
bool cameraLoaded = false;

struct quirc* qr;
struct quirc_data data;
u16* buf;

sf2d_texture* cameraOutput;

void startQRMode(){
	Result ret;

	QRMode = true;

	ret = camInit();
	if(ret){
		QRMode = false;
		return throwError("Failed to initiate camera.", ret);
	}

	CAMU_SetSize(SELECT_OUT1_OUT2, SIZE_CTR_TOP_LCD, CONTEXT_A);
	CAMU_SetOutputFormat(SELECT_OUT1_OUT2, OUTPUT_RGB_565, CONTEXT_A);

	CAMU_SetNoiseFilter(SELECT_OUT1_OUT2, true);
	CAMU_SetAutoExposure(SELECT_OUT1_OUT2, true);
	CAMU_SetAutoWhiteBalance(SELECT_OUT1_OUT2, true);

	CAMU_SetTrimming(PORT_CAM1, false);

	qr = quirc_new();
	if(!qr)
		return throwError("Failed to allocate memory for quirc");

	if(quirc_resize(qr, 400, 240) < 0)
		return throwError("Failed to allocate video memory");

	buf = new u16[400 * 240 * 2 * 2];

	cameraOutput = sf2d_create_texture(400, 240, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	sf2d_texture_tile32(cameraOutput);

	cameraLoaded = true;
}

void stopQRMode(){
	QRMode = false;
	cameraLoaded = false;

	CAMU_Activate(SELECT_NONE);
	camExit();

	quirc_destroy(qr);

	delete[] buf;
}

void scanAndDraw(){
	u32 bufSize;
	CAMU_GetMaxBytes(&bufSize, 400, 240);
	CAMU_SetTransferBytes(PORT_BOTH, bufSize, 400, 240);

	CAMU_Activate(SELECT_OUT1_OUT2);

	Handle camReceiveEvent = 0;

	CAMU_ClearBuffer(PORT_BOTH);
	CAMU_SynchronizeVsyncTiming(SELECT_OUT1, SELECT_OUT2);

	CAMU_StartCapture(PORT_BOTH);

	CAMU_SetReceiving(&camReceiveEvent, buf, PORT_CAM1, 400 * 240 * 2, (s16)bufSize);
	svcWaitSynchronization(camReceiveEvent, 300000000ULL);

	CAMU_StopCapture(PORT_BOTH);

	svcCloseHandle(camReceiveEvent);

	CAMU_Activate(SELECT_NONE);

	for (size_t j = 0; j < 240; j++){
		for(size_t i = 0; i < 400; i++){
			u16 px = buf[j * 400 + i];

			uint8_t r = ((px & 0x1F) << 3);
			uint8_t g = (((px >> 5) & 0x3F) << 2);
			uint8_t b = (((px >> 11) & 0x1F) << 3);

			// NOTE: colour order on the RGBA8 macro is different for this function
			sf2d_set_pixel(cameraOutput, i, j, RGBA8(255, r, g, b));
		}
	}

	sf2d_draw_texture(cameraOutput, 0, 0);

	int w, h;

	u8* image = (u8*)quirc_begin(qr, &w, &h);

	for (size_t x = 0; x < w; x++){
		for (size_t y = 0; y < h; y++){
			u16 px = buf[y * 400 + x];
			image[y * w + x] = (u8)(((((px >> 11) & 0x1F) << 3) + (((px >> 5) & 0x3F) << 2) + ((px & 0x1F) << 3)) / 3);
		}
	}

	quirc_end(qr);

	int num_codes = quirc_count(qr);

	for (size_t i = 0; i < num_codes; i++){
		struct quirc_code code;
		quirc_extract(qr, i, &code);

		if(!quirc_decode(&code, &data)){
			stopQRMode();

			queueTask(downloadThemeFromURL, (void*)data.payload, true);
		}
	}
}
