#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <queue>
#include <bits/stdc++.h>
#include <sstream>
#include "queue.h"
using namespace std;

int convertToInt(string);
string convertToString(int);

int main(int argc, char* argv[]) {

	int* writefd = (int*)argv[1];
	*writefd = *writefd % 10;
	
	// reading from processes file //

	int filefd = open("processes.txt", O_RDONLY);
	
	char buffer[1000];
	read(filefd, buffer, 1000);
	
	close(filefd);
	
	// type of scheduling algorithm //
	
	string type;
	int i = 0;
	
	while (buffer[i] != '\n') {
		type += buffer[i];
		i++;
	}
	
	i++;
	
	// initializing the temporary proc //
	
	int index = 0;
	proc tempProc;
	tempProc.artime = 0;
	tempProc.burst = 0;
	tempProc.cTime = 0;
	tempProc.execTime = 0;
	
	// beginning making processes //
	
	if ( (type[0] == 'S' && type[1] == 'J' && type[2] == 'F') or (type[0] == 'F' and type[1] == 'C' and type[2] == 'F' and type[3] == 'S') ) {
	
		while (buffer[i] == 'P') {
		
			if (type[0] == 'S' && type[1] == 'J' && type[2] == 'F')	// sJF
				tempProc.algo = 1;
				
			if (type[0] == 'F' and type[1] == 'C' and type[2] == 'F' and type[3] == 'S')	// FCFS
				tempProc.algo = 2;

			string temp;
		
			while (buffer[i] != '\n') {
				temp += buffer[i];
				i++;
			}
			
			int j = 0;
			while (temp[j] != '\0') {
				tempProc.name[j] = temp[j];
				j++;
			}
			
			temp.clear();
			i++;
			
			while (buffer[i] != '\n') {
				temp += buffer[i];
				i++;
			}
			
			tempProc.artime = convertToInt(temp);
			
			temp.clear();
			i++;
			
			while (buffer[i] != '\n') {
				temp += buffer[i];
				i++;
			}
			
			tempProc.burst = convertToInt(temp);
			
			temp.clear();
			i++; 
			
			write(*writefd, &tempProc, sizeof(proc));
			
			cout << "\nAdmitting process:" << endl;
			tempProc.print();
		} 
		
		tempProc.artime = -1;	// signals to the next process that all processes have been read
		write(*writefd, &tempProc, sizeof(proc));
	}
	
}

int convertToInt(string str) 
{
	stringstream ss(str);
	int val;
	ss >> val;
	return val;
}

string convertToString(int val) 
{
	stringstream ss;
	ss << val;
	string str = ss.str();
	return str;
}
