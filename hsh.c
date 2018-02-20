
#define _GNU_SOURCE

#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_MAX_LENGTH 4096

// Max length of arguments including command itself and a NULL terminator.
#define CMD_MAX_ARGS 32

#define SHELL_NAME "hsh"
#define PROMPT "\n%s\n$ "

#include "commands.h"

/* hsh_exec - execute a command.
 * hsh_exec is a lot like exec(). It takes a string, tokenizes it into argv, and then runs the
 * associated command.
 */
void hsh_exec(char *buf) {
	int i;
	int argc = 0;
	char *argv[CMD_MAX_ARGS];
	char *pch = strtok(buf, " \t\n");
	while (pch != NULL && argc < CMD_MAX_ARGS) {
		argv[argc++] = strdup(pch);
		pch = strtok(NULL, " \t\n");
	}
	if (argc >= CMD_MAX_ARGS) {
		fprintf(stderr, SHELL_NAME ": too many arguments\n");
		return;
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
			printf(SHELL_NAME "error executing %s\n", argv[0]);
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
		printf(PROMPT, get_current_dir_name());
		fflush(stdout);
		char *line = fgets(buf, CMD_MAX_LENGTH - 1, stdin);

		/* Check for EOF */
		if (!line) {
			printf("\n");
			return;
		} else if (*line && line[strlen(line) - 1] != '\n') {
		} else {
			hsh_exec(buf);
		}
	}
}

int main(int argc, char *argv[]) {
	run();
	return 0;
}
