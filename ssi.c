
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CMD_MAX_LENGTH 4096

#include "dir.h"
#include "commands.h"

/* myexec - execute a command. 
 * myexec is a lot like exec(). It takes a string, tokenizes it into argv, and then runs the associated command.
 */
void myexec(char *buf){
	int i;
	int argc = 0;
	char *argv[32];
	char *pch = strtok(buf, " \t\n");
	while(pch != NULL){
		argv[argc++] = strdup(pch);
		pch = strtok(NULL, " \t\n");
	}
	if(argc){
		for(i = 0; execs[i].name; i++){
			if(!strcmp(execs[i].name, argv[0])){
				execs[i].e(argc, argv);
				return;
			}
		}
		fprintf(stderr, "%s: command not found\n", argv[0]);
	}
}

/* run - main loop
 * This is the main loop. It forever prompts the user for input, then executes it.
 */
void run(){
	char buf[CMD_MAX_LENGTH];
	for(;;){
		printf("SSI: ");
		dir_pwd(current);
		printf(" > ");
		fflush(stdout);
		char *line = fgets(buf, CMD_MAX_LENGTH - 1, stdin);

		/* Check for EOF */
		if(!line)
			return;

		myexec(buf);
	}
}

int main(int argc, char *argv[]){
	current = root = dir_new("");
	root->parent = root;
	run();
	return 0;
}

