 #include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <bits/stdc++.h>
#include <sstream>
#include <time.h>
#include "queue.h"
//#include "structs.h"
using namespace std;

pthread_t tid1, tid2, tid3, tid4, tid5, tid6;
Queue inputQueue, outputQueue, printerQueue;
proc temp;
int writeFD;

// three types of queues

void* inputQueueThreadRead(void* arg) {
	inputQueue.enqueue(temp);
	return(NULL);
} 

void* outputQueueThreadRead(void* arg) {
	outputQueue.enqueue(temp);
	return(NULL);
} 

void* printerQueueThreadRead(void* arg) {
	printerQueue.enqueue(temp);
	return(NULL);
} 

void* inputQueueThreadWrite(void* arg) {

	proc tempProc;

	while (!inputQueue.isEmpty()) {
		int wait = 0;
		
		int ticks = 25 + rand()%6;	// 25 - 30 ticks
		
		while (wait < ticks)
			wait++;
	
		tempProc = inputQueue.dequeue();
		tempProc.waitingTime += wait;
		write(writeFD, &tempProc, sizeof(tempProc));
	}
	
	return(NULL);
} 

void* outputQueueThreadWrite(void* arg) {

	proc tempProc;

	while (!outputQueue.isEmpty()) {
		int wait = 0;
		
		int ticks = 25 + rand()%6;	// 25 - 30 ticks
		
		while (wait < ticks)
			wait++;
	
		tempProc = outputQueue.dequeue();
		tempProc.waitingTime += wait;
		write(writeFD, &tempProc, sizeof(tempProc));
	}
	
	return(NULL);
}  

void* printerQueueThreadWritw(void* arg) {

	proc tempProc;

	while (!printerQueue.isEmpty()) {
		int wait = 0;
		
		int ticks = 25 + rand()%6;	// 25 - 30 ticks
		
		while (wait < ticks)
			wait++;
	
		tempProc = printerQueue.dequeue();
		tempProc.waitingTime += wait;
		write(writeFD, &tempProc, sizeof(tempProc));
	}
	
	return(NULL);
} 

int main(int argc, char* argv[]) {

	srand(time(NULL));

	int readfd;
	istringstream(argv[1]) >> readfd;
	
	int writefd;
	istringstream(argv[2]) >> writefd;
	writeFD = writefd;
	
	// reading processes from running //
	
	Queue procQueue;
	proc tempProc;
	
	int i = 0;
	
	int status = read(readfd, &tempProc, sizeof(tempProc));
	
	while (status > 0) {
		
		wait(NULL);
		
	/*	if (status < 0) {
			wait(NULL);
			int status = read(readfd, &tempProc, sizeof(tempProc));
		} */
		
		if (tempProc.artime >= 0) {
			procQueue.enqueue(tempProc);
			temp = tempProc;
			
			// assigning processes to queues //
			
			int queueType = 1 + rand()%3;
			
			if (queueType == 1)
				pthread_create(&tid1, NULL, inputQueueThreadRead, NULL);
				
			else if (queueType == 2)
				pthread_create(&tid2, NULL, outputQueueThreadRead, NULL);
			
			else if (queueType == 3)
				pthread_create(&tid3, NULL, printerQueueThreadRead, NULL);
		}
			
		status = read(readfd, &tempProc, sizeof(tempProc));
	} 
	
/*	while (!procQueue.isEmpty()) {
		int wait = 0;
		
		int ticks = 25 + rand()%6;	// 25 - 30 ticks
		
		while (wait < ticks)
			wait++;
	
		tempProc = procQueue.dequeue();
		tempProc.waitingTime += wait;
		write(writefd, &tempProc, sizeof(tempProc));
	} */
	
	// writing processes from each queue //
	
	pthread_create(&tid1, NULL, inputQueueThreadRead, NULL);
	pthread_create(&tid2, NULL, outputQueueThreadRead, NULL);
	pthread_create(&tid3, NULL, printerQueueThreadRead, NULL);
	
	tempProc.artime = -1;	// this proc indicates that all other processes in the blocked queue have been sent
	tempProc.name[0] = 'e';
	tempProc.name[1] = 'x';
	tempProc.name[2] = 'i';
	tempProc.name[3] = 't';
	
	write(writefd, &tempProc, sizeof(tempProc));
	
}
