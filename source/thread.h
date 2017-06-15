// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#pragma once

#include "global.h"

struct Task {
	ThreadFunc entrypoint;
	void* arg;
};

void queueTask(ThreadFunc, void*, bool putInFront = false);
void startWorkers();
void cleanTaskQueue();

extern Thread THREADS[4];
extern deque<Task> taskQueue;
extern LightLock taskQueueLock;
extern volatile uint threadsRunning;
