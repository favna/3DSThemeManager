#pragma once

#include "global.h"
#include "thread.h"
#include "network.h"
#include "input.h"

extern bool QRMode;
extern bool cameraLoaded;

void startQRMode();
void stopQRMode();
void scanAndDraw();
