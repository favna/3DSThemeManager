// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#pragma once

#include "global.h"

struct appConfig {
	string lang;
};

extern appConfig CONFIG;

void CONFIG_load();
void CONFIG_save();
