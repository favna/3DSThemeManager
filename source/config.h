#pragma once

#include "global.h"

struct appConfig {
	string lang;
};

extern appConfig CONFIG;

void CONFIG_load();
void CONFIG_save();
