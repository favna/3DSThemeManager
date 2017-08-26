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

#include "thread.h"

Thread THREADS[4];
deque<Task> taskQueue;
LightLock taskQueueLock;
volatile uint threadsRunning = 0;

Thread createThread(ThreadFunc entrypoint, void* arg){
	threadsRunning++;
	s32 prio = 0;
	svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);

	Thread thread = threadCreate(entrypoint, arg, 1024 * 28, 0x18, -2, false);

	return thread;
}

void queueTask(ThreadFunc entrypoint, void* arg, bool putInFront){
	Task task = {entrypoint, arg};
	while(true){
		if(!LightLock_TryLock(&taskQueueLock))
			break;

		svcSleepThread(0x400000LL);
	}

	if(!putInFront)
		taskQueue.push_back(task);
	else
		taskQueue.push_front(task);

	LightLock_Unlock(&taskQueueLock);
}

void worker(void* arg){
	while (!closing){
		while(true){
			if(!LightLock_TryLock(&taskQueueLock))
				break;

			svcSleepThread(0x400000LL);
		}

		Task task;
		bool taskReady = false;

		if(!taskQueue.empty()){
			task = taskQueue.front();
			taskQueue.erase(taskQueue.begin());

			taskReady = true;
		}

		LightLock_Unlock(&taskQueueLock);

		if(taskReady)
			task.entrypoint(task.arg);

		svcSleepThread(0x400000LL);
	}

	threadsRunning--;
	threadExit(0);
}

void startWorkers(){
	LightLock_Init(&taskQueueLock);

	int threadCount = NEW3DS ? 4 : 2;

	for (size_t i = 0; i < threadCount; i++)
		THREADS[i] = createThread(worker, (void*)i);
}

void cleanTaskQueue(){
	taskQueue.clear();
}
