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


int main(int argc, char* argv[]) {

	srand(time(NULL));

	int time = 0;

	int readfd;
	istringstream(argv[1]) >> readfd;
	
	int writefd;
	istringstream(argv[2]) >> writefd;
	
	int writeToBlocked;
	istringstream(argv[3]) >> writeToBlocked;
	
	proc tempProc;
	bool procSentBack = false;
	
	// sterting reading processes //
	
	read(readfd, &tempProc, sizeof(tempProc));	// read from new
	
	int counter = 0;
	
	while(1) {
	
		procSentBack = false;
	
		if (tempProc.artime == -1)
			goto nextt;
		
		tempProc.waitingTime += counter;
		tempProc.sTime = time;
		
		counter = 0;
		
		while (time < tempProc.artime)
			time++;
			
		// execution of a process //
	
		while (counter < tempProc.burst) {
			sleep(1);
			counter++;
			time++;
			tempProc.execTime++;
			
			if (counter % 5 == 0) {
				int binary = 0 + rand()%2;
				
				if (binary == 1) {
					// continue execution of currecnt process
				}
				
				else if (binary == 0) { // blocked
					write(writeToBlocked, &tempProc, sizeof(tempProc));
					procSentBack = true;
				}
			}
			
			if (procSentBack) {
				read(readfd, &tempProc, sizeof(tempProc)); // read next process
				
				if (tempProc.artime == -1) // end of processes
					goto nextt;
					
				counter = 0;
				break;
			}
		}
		
		tempProc.cTime = time;
		write(writefd, &tempProc, sizeof(tempProc)); // write completed proc to exit
		cout << "\nProcess has completed execution" << endl;
		tempProc.print();
				
		read(readfd, &tempProc, sizeof(tempProc)); // read next process
		counter = 0;
	}
	
	nextt:
		tempProc.artime = -1;
		write(writefd, &tempProc, sizeof(tempProc));	// signal to reading process that all processes have been read
		close(writefd);
}
