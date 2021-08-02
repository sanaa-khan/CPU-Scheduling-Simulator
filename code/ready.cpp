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
#include "queue.h"
//#include "structs.h"
using namespace std;

pthread_t thread_id;
Queue procQueue;
int readNew;

/*
void* getProcsFromNew(void* arg) {

	proc tempProc;

	while (tempProc.artime != -1) {
		int status = read(readNew, &tempProc, sizeof(tempProc));
		
		if (!status) {
			cout << "No data for ready queue" << endl;
			exit(0);
		}
		
		if (tempProc.artime != -1)
			procQueue.enqueue(tempProc);
	}
	
	return(NULL);
} */

int main(int argc, char* argv[]) {

	int* readfd = (int*)argv[1];
	*readfd = *readfd % 10;
	readNew = *readfd;
	
	int writefd = 9;	// the write end of ready pipe was used for dup2() with 9
	
	int readFromBlocked;
	istringstream(argv[1]) >> readFromBlocked;
	
	// reading processes from new //
	
	proc tempProc;
	
	while (tempProc.artime != -1) {
		int status = read(readNew, &tempProc, sizeof(tempProc));
		
		if (!status) {
			cout << "No data for ready queue" << endl;
			exit(0);
		}
		
		if (tempProc.artime != -1)
			procQueue.enqueue(tempProc);
	}
	
/*	pthread_create(&thread_id, NULL, getProcsFromNew, NULL);
	pthread_exit(NULL); */
	
	// reading processes from blocked //
	
	int status = 1;	// read(readFromBlocked, &tempProc, sizeof(tempProc));
	
	if (status > 0) {
		while (tempProc.artime != -1) {
			read(readFromBlocked, &tempProc, sizeof(tempProc));
		
			if (tempProc.artime != -1)
				procQueue.enqueue(tempProc);
		}
	} 
	
	close(readFromBlocked);
	close(*readfd);
	
	cout << "\nReady queue:" << endl;
	procQueue.display();
	
	// writing to running //
	
	while (!procQueue.isEmpty()) {
		tempProc = procQueue.dequeue();
		write(writefd, &tempProc, sizeof(tempProc));
	}
	
	tempProc.artime = -1;	// this proc indicates that all other processes in the ready queue have been executed
	tempProc.name[0] = 'e';
	tempProc.name[1] = 'x';
	tempProc.name[2] = 'i';
	tempProc.name[3] = 't';
	
	write(writefd, &tempProc, sizeof(tempProc));
	 
}
