// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#pragma once

#include "global.h"

extern string update;

Result HTTPGet(vector<char>&, string);

void checkForUpdate(void*);
void installUpdate();
