
#include <unistd.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "commands.h"

struct bgtask_t{
  struct bgtask_t *next;
  char *name;
  int pid;
};

struct bgtask_t *tasks = NULL;

/* exit the shell */
static void myexit(int argc, char *argv[]){
	exit(0);
}

/* change directory */
static void cd(int argc, char *argv[]){
  int r = 0;
	if(argc == 1){
    /* cd with no arguments should change to home directory. We'll just change to the root */
    r = chdir(getenv("HOME"));
	}else if(argc == 2){
    if(!strcmp("~", argv[1])){
      r = chdir(getenv("HOME"));
    }else{
      r = chdir(argv[1]);
    }
	}else{
		fprintf(stderr, "%s: too many arguments\n", argv[0]);
    return;
	}
  if(r == -1){
    perror("cd");
  }
}

#include <signal.h>

static void bgstop(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "USAGE: %s PID\n", argv[0]);
        return;
    }
    int pid = atoi(argv[0]);
    kill(pid, SIGKILL);
}

static void bglist(int argc, char *argv[]){
  struct bgtask_t *tmp;
  for(tmp = tasks; tmp; tmp = tmp->next){
    printf("%i:\t%s\n", tmp->pid, tmp->name);
  }
}

static void bg(int argc, char *argv[]){
  /* Double fork construct. Allows us to wait on the status of the background task */
  int pid = fork();
  if(!pid){
    int pid2 = fork();
    if(!pid2){
        execvp(argv[1], &argv[1]);
        printf("error executing %s\n", argv[0]);
        exit(1);
    }else{
        waitpid(pid2, NULL, 0);
        printf("rsi: '%s' (pid %i) has exit\n", argv[1], pid2);
        exit(0);
    }
  }else{
    struct bgtask_t *tmp = malloc(sizeof(struct bgtask_t));
    tmp->name = strdup(argv[1]);
    tmp->pid = pid;
    tmp->next = tasks;
    tasks = tmp;
  }
}

/*
 * This structure holds the commands the shell supports.
 * There is a string specifying the name and a function pointer to the method.
 */
struct executable execs[] = {
	{"cd", cd},
	{"bg", bg},
	{"bgstop", bgstop},
	{"bglist", bglist},
	{"exit", myexit},
	{NULL, NULL}
};

