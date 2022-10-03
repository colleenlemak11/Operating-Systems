/*
Colleen Lemak
Dr. Zhang
CPSC 346
3 October 2022
Project 2: building a functional C shell with parent and child processes.
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

#define MAX_LINE		80 /* 80 chars per line, per command */
#define MAX_COMMANDS	9 /* size of history */

char history[MAX_COMMANDS][MAX_LINE]; // stores history commands
char display_history [MAX_COMMANDS][MAX_LINE]; // stores clean history commands
int command_count = 0;

/**
 * Add the most recent command to the history.
 */
void addtohistory(char inputBuffer[]) {
	// check validity with maximum capacity (9)
	if (command_count < 9) {
		strcpy(history[command_count], inputBuffer);
	}
	else {
		for (int i = 0; i < command_count; i++) {
			strcpy(history[i], history[i+1]);
			strcpy(display_history[i], display_history[i+1]);
		}
		strcpy(history[command_count-1], inputBuffer);
	}
	// clean up display_history
	for (int i = 0; i < strlen(inputBuffer); i++) {
		switch (history[command_count][i]) {
			case '\n':
			case '\t':
				display_history[command_count][i] = '\0';
				break;
			default:
				display_history[command_count][i] = history[command_count][i];
				break;
		}
	}
	command_count++;

	return;
}

/** 
 * The setup function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings. 
 */
int setup(char inputBuffer[], char *args[], int *background)
{
    int length,		
	i,				
	args_idx = 0,
	command_idx = 0;

    // read in command line input
	do {
		printf("osh> ");
		fflush(stdout);
		length = read(STDIN_FILENO,inputBuffer,MAX_LINE); 
	}
	while (inputBuffer[0] == '\n'); // take out newline characters

    if (length == 0) {
        perror("No valid command input.");
		exit(0);
	}

    if ((length < 0)) {
		perror("Error reading command.");
		exit(-1); 
    }

	// check if user is using history
	if (inputBuffer[0] == '!') {
		// user wants to use history
		if (command_count == 0) {
			printf("No commands in history.\n");
			return 1;
		}
		else {
			if (inputBuffer[1] == '!') {
				// recent command to IB
				strcpy(inputBuffer, history[(command_count - 1) % MAX_COMMANDS]);
				length = strlen(inputBuffer);
			}
			if (isdigit(inputBuffer[1])) {
				// convert the digit to int
				command_idx = atoi(&inputBuffer[1]);
				if (command_idx > command_count)
					printf("No such command in history.");
				else {
					strcpy(inputBuffer, display_history[command_idx-1]);
					length = strlen(inputBuffer);
				}
			}
		}
	}
	
	addtohistory(inputBuffer); 
	
	// parse all characters of inputBuffer
	args[args_idx] = NULL;

    for (i = 0; i < length;i++) { 	
		
        switch (inputBuffer[i]) {
			case ' ':
			case '\t' :
				inputBuffer[i] = '\0';
				*background = 0;
				// initialize next args to NULL if we are not at the end of args array
				if (args[args_idx] != NULL) {
					args_idx++;
					args[args_idx] = NULL;
					break;
				}
				
			case '\n':
				inputBuffer[i] = '\0';
				if (args[args_idx] != NULL) {
					args_idx++;
					args[args_idx] = NULL;
					break;
				}
				
			default :
				if (inputBuffer[i] == '&') {
					// flag 1 when user enters &
					*background = 1;
					break;
				}
				else if (*background == 0 && args[args_idx] == NULL) {
					// add inputBuffer value to args if valid
					args[args_idx] = &inputBuffer[i];
					break;
				}
		}
	}
	return 1;
}	


int main(void)
{
	char inputBuffer[MAX_LINE]; 	/* buffer to hold the command entered */
	int background;             	/* equals 1 if a command is followed by '&' */
	char *args[MAX_LINE/2 + 1];		/* command line (of 80) has max of 40 arguments */
	pid_t child;            		/* process id of the child process */
	int shouldrun = 1;				/* flag for if process should begin*/
	int status;						/* child status to be passed into waitpid() */
	
    while (shouldrun) {
		background = 0;
		
		// setup and read in command
		shouldrun = setup(inputBuffer,args,&background);

		// if user types exit, the shell terminates
		if (strncmp(inputBuffer, "exit", 4) == 0)
			return 0;

		// if user types history, history is displayed
		if (strncmp(inputBuffer, "history", 7) == 0) {
			for (int i = 0; i < command_count; i++) {
				printf("%d \t %s\n", i+1, display_history[i]);
			}	
		}
		
		if (shouldrun) {
			// create process
			child = fork();

			switch (child) {
				case -1:
					// did not fork
					perror("Error: could not fork process.");
					break;
				case 0:
					// child process
					status = execvp(args[0], args);
					return 0;
					break;
				default:
					// parent process
					if (background == 0) {
						waitpid(child, &status, 0);
					}
			}
		}
	}
	return 0;
}
