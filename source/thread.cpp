// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "thread.h"

Thread THREADS[4];
std::queue<Task> taskQueue;
LightLock taskQueueLock;
volatile uint threadsRunning = 0;

Thread createThread(ThreadFunc entrypoint, void* arg){
	threadsRunning++;
	s32 prio = 0;
	svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);

	Thread thread = threadCreate(entrypoint, arg, 1024 * 16, 0x18, -1, false);

	return thread;
}

void queueTask(ThreadFunc entrypoint, void* arg) {
	Task task = {entrypoint, arg};
	LightLock_Lock(&taskQueueLock);
	taskQueue.push(task);
	LightLock_Unlock(&taskQueueLock);
}

void worker(void* arg) {
	while (!closing) {
		LightLock_Lock(&taskQueueLock);
		Task task;
		bool taskReady = false;

		if(!taskQueue.empty()) {
			task = taskQueue.front();
			taskQueue.pop();

			taskReady = true;
		}
		LightLock_Unlock(&taskQueueLock);

		if(taskReady)
			task.entrypoint(task.arg);

		svcSleepThread(1000000ULL * (u32)arg);
	}

	threadsRunning--;
	threadExit(0);
}

void startWorkers() {
	LightLock_Init(&taskQueueLock);

	for (size_t i = 0; i < 4; i++) {
		THREADS[i] = createThread(worker, (void*)((i+1)*100));
	}
}

void cleanTaskQueue() {
	std::queue<Task> empty;
	swap(taskQueue, empty);
}

/*
void queueThread(ThreadFunc entrypoint, void* arg){
	if(threadsRunning < 6){
		createThread(entrypoint, arg);
		return;
	}

	queuedThread thread = {entrypoint, arg};
	threadQueue.push(thread);
}

void finishThread(){
	threadsRunning--;

	printf("current size: %i,%i\n", threadQueue.size(), threadsRunning);
}

void emptyThreadQueue(){
	queue<NETWORK::queuedThread> empty;
	swap(NETWORK::threadQueue, empty);
}

//volatile uint threadsRunning = 0;
//queue<queuedThread> threadQueue;
*/
