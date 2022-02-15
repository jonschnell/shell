//jonschnell.com

#ifndef proj1_h
#define proj1_h

	#include <stdlib.h>
	#include <stdio.h>
	#include <signal.h>
	#include <string.h>
	#include <unistd.h>
	#include <wait.h>
	#include <stdbool.h> 


	
	int getpid();
	int getppid();
	int nonBuiltIn(char input[]);
	void parse(char input[], char *parsed[]);
	
#endif
