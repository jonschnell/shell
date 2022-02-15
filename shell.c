//jonschnell.com

#include "shell.h"

int main(int argc, char **argv) {
	// Set the prompt
	char prompt[1024] = "308sh> ";
	
	int waitreturn;
	int status;
	
	//user defined prompt
	if (argc == 3){
		//set user defined prompt
		strcpy(prompt, argv[argc-1]);

	}
	
	// User input infinite loop
	while (1) {
		printf("%s",prompt);
		
		// Get user input
		char input[1024];
		int length = strlen(fgets(input,1024,stdin));
		//null terminate input string
		input[length-1] = '\0';
		
		//check to see if any children have died
		waitreturn = waitpid(-1, &status, WNOHANG);
		//if a child has died since last iteration print exit status of that child
		if (waitreturn > 0){
			if (WIFEXITED(status)){
				printf("[%d] Exit %d\n", waitreturn, WEXITSTATUS(status));
			}
			else if (WIFSIGNALED(status)) {
				printf("[%d] Killed (%d)\n", waitreturn, WTERMSIG(status));
			}
		}


		// ---------- Builtin Commands ---------------------------------------
		//---------- exit ----------
		if (strcmp(input, "exit") == 0) {
			return 0;
		} 
		//---------- pid ----------
		else if (strcmp(input, "pid") == 0) {
			printf("Shell Process ID: %d\n", getpid());
		}
		// ---------- ppid ----------
		else if (strcmp(input, "ppid") == 0) {
			printf("Shell Parent Process ID: %d\n", getppid());
		}
		// ---------- pwd ----------
		else if (strcmp(input, "pwd") == 0) {
			char pwd[1024];
			getcwd(pwd, sizeof(pwd));
			printf("%s\n", pwd);
		}
		//---------- cd ----------
		else if ((input[0] == 'c') && (input[1] == 'd')) {
			char target[1024];
			//nullterminator is in the position input must be "cd"
			if (input[2] == '\0'){
				//sety target to eviroment variable home
				strcpy(target, getenv("HOME"));
				//printf("%s/n", getenv("HOME"));
			}
			//set target to the string after "cd "
			else{
				//strip the first 3 chars off of input
				for (int i = 0; i < sizeof(input)-3; i++){
					target[i] = input[i+3];
				}
			}
			chdir(target);
		}
		//---------- no input ----------
		else if (length == 1){
		//do nothing
		}
		//---------- non-builtin ----------
 		else {
			nonBuiltIn(input);
		}
	}
	return 0;
}

// ----------------------------------- Helper Functions ----------------------------------
int nonBuiltIn(char input[]) {
	int status;

	//NULLing all indexes enusres that arguments do not carry over to the next command
	char *arglist[1024] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

	//background flag
	bool bg = false;
	//check for '&' for a backround process
	if (input[strlen(input)-1] == '&'){
		//set flag
		bg = true;
		//parse out the '&'
		input[strlen(input)-1] = '\0';
	}

	//fork to create a child ------- FORK
	pid_t child = fork();	
			
	//parse input into arglist
	parse(input, arglist);

	//catch error in forking
	if(child < 0){
		printf("fork failed");
	}
	
	//if thread is the ------------ child ------------
	else if(child == 0) {
			
		//execvp will take default path if '/' is not present in the input
		//default path is /bin:/user/bin
		execvp(arglist[0], arglist);
		
		//if exec returns the binary was not found
		printf("Cannot exec %s: No such file or directory\n", arglist[0]);
		exit(255);
	}
	
	//if thread is the ------------ parent ------------
	else if(child > 1){
		//print newly spawnned child's PID
		printf("[%d] %s\n", child, arglist[0]);
		
		
		//if background was NOT specified
		if (bg == false){
			//wait for child to exit before taking more input
			waitpid(child, &status, 0);
			//print its exit status
			if (WIFEXITED(status)){
				printf("[%d] Exit %d\n", child, WEXITSTATUS(status));
			}
			else if (WIFSIGNALED(status)) {
				printf("[%d] Exit SIGNAL %d\n", child, WTERMSIG(status));
			}
		}
		
		//if background true
		else{
		//wait for more input
		}
				
	}
		
}

//parse input string into array of arguments
void parse(char input[], char *parsed[]){
	int count = 0;
	char flag[] = " ";
	
	char *ptr = strtok(input, flag);
	
	while(ptr != NULL){
		parsed[count] = ptr;
		//printf("%s\n", parsed[count]);
		ptr = strtok(NULL, flag);
		count++;
	}
}
