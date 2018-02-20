
#define _GNU_SOURCE

#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_MAX_LENGTH 4096

#include "commands.h"

/* myexec - execute a command.
 * myexec is a lot like exec(). It takes a string, tokenizes it into argv, and then runs the
 * associated command.
 */
void myexec(char *buf) {
	int i;
	int argc = 0;
	char *argv[32];
	char *pch = strtok(buf, " \t\n");
	while (pch != NULL) {
		argv[argc++] = strdup(pch);
		pch = strtok(NULL, " \t\n");
	}
	argv[argc] = NULL;
	if (argc) {
		for (i = 0; execs[i].name; i++) {
			if (!strcmp(execs[i].name, argv[0])) {
				execs[i].e(argc, argv);
				return;
			}
		}
		int pid = fork();
		if (pid) {
			waitpid(pid, NULL, 0);
		} else {
			execvp(argv[0], argv);
			printf("error executing %s\n", argv[0]);
			exit(1);
		}
	}
}

/* run - main loop
 * This is the main loop. It forever prompts the user for input, then executes it.
 */
void run() {
	char buf[CMD_MAX_LENGTH];
	for (;;) {
		printf("SSI: %s > ", get_current_dir_name());
		fflush(stdout);
		char *line = fgets(buf, CMD_MAX_LENGTH - 1, stdin);

		/* Check for EOF */
		if (!line) {
			printf("\n");
			return;
		}

		myexec(buf);
	}
}

int main(int argc, char *argv[]) {
	run();
	return 0;
}
