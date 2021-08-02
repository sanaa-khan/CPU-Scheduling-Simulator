#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace std;

int main() {
	
	int newfd[2];	// new will write to this pipe
	pipe(newfd);	// ready will read from it
	
	int newPipeWriteDesc = dup(newfd[1]);
	int newPipeReadDesc = dup(newfd[0]);
	
	pid_t pid1 = fork();

	if (pid1 > 0 ) {
		// parent code
		
		wait(NULL);
		
		int readyfd[2];	// ready will write procs to this
		pipe(readyfd);		// running will read procs from it
		
		int readyWrite = dup(readyfd[1]);
		int readyRead = dup(readyfd[0]);
		
		dup2(readyWrite, 9);	// as exec is not passing readyWrite succesfully
		
		int blockReadyfd[2];	// ready will read from blocked
		pipe(blockReadyfd);		// blocked will write processes
		
		int readyReadBlocked = dup(blockReadyfd[1]);
		int blockWrite = dup(blockReadyfd[0]);
		
		pid_t pid2 = fork();
		
		if (pid2 > 0) {
		
			wait(NULL);
			
			int runningfd[2];	// running will write
			pipe(runningfd);
			
			int runningWrite = dup(runningfd[1]);
			int runningRead = dup(runningfd[0]);	// exit will read executed processes from running
			
			int blockfd[2];
			pipe(blockfd);
			
			int blockRead = dup(blockfd[1]);	// blocked reading procs from running
			int runningWriteToBlocked = dup(blockfd[0]);		// running send procs to blocked
			
			pid_t pid3 = fork();
			
			if (pid3 > 0) {
				
				wait(NULL);
			
				
				pid_t pid4 = fork();
				
				
				if (pid4 > 0) {
				
					wait(NULL);
				
					pid_t pid5 = fork();
					
					if (pid5 > 0) {
						wait(NULL);
					}
					
					//child 5 -- exit
					else if (pid5 == 0) {
						string read_from_running = to_string(runningRead);
						execlp("./exit", "exit", read_from_running.c_str(), NULL);
					}
				} 
				
				//child 4 -- blocked
				else if (pid4 == 0) {
					string block_read_from_running = to_string(blockRead);
					string block_write = to_string(blockWrite);
					execlp("./blocked", "blocked", block_read_from_running.c_str(), block_write.c_str(), NULL);
				} 
			}
				
			//child 3 -- running
			else if (pid3 == 0) {
				wait(NULL);
				
				string read_from_ready = to_string(readyRead);
				string running_write = to_string(runningWrite); 
				string running_write_to_blocked = to_string(runningWriteToBlocked);
				execlp("./running", "running", read_from_ready.c_str(), running_write.c_str(), running_write_to_blocked.c_str(), NULL);
			}
		} 
		
		// child two -- ready
		else if (pid2 == 0) {
			wait(NULL);
		
			string ready_write = to_string(readyWrite);
			string read_from_new = to_string(newPipeReadDesc);
			string read_from_blocked = to_string(readyReadBlocked);
			execlp("./ready", "ready", read_from_new.c_str(), ready_write.c_str(), read_from_blocked.c_str(), NULL);
		}
	} 
	
	// child one -- new
	else if (pid1 == 0) {
	
		execl("new", "new", (void*)&newPipeWriteDesc, NULL);
	} 
	
	else {
		// fork failed
		cout << "Fork failed" << endl;
	}
}
