
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dir.h"
#include "commands.h"

/* Used by basename and dirname to split a path */
static int dirnameidx(char *s){
	int i = strlen(s) - 1;
	for(; i >= 0 && s[i] == '/'; i--); /* Ignore trailing slashes*/
	for(; i >= 0 && s[i] != '/'; i--);
	return i;
}

/* basename - Finds the last segment of a path */
static char *basename(char *s){
	return strdup(&s[dirnameidx(s) + 1]);
}

/* dirname - Finds all but the last segment of a path */
static char *dirname(char *s){
	int i = dirnameidx(s);
	if(i < 0){
		return ".";
	}else{
		char *ret = malloc(i + 2);
		memcpy(ret, s, i + 1);
		ret[i+1] = 0;
		return ret;
	}
}

/* mkdir - creates a new directory */
static void mkdir(int argc, char *argv[]){
	if(argc < 2){
		fprintf(stderr, "%s: missing operand\n", argv[0]);
	}else{
		int i;
		for(i = 1 ; i < argc; i++){
			if(dir_getdir(current, argv[1])){
				fprintf(stderr, "%s: cannot create directory '%s': File exists\n", argv[0], argv[i]);
			}else{
				char *dn = dirname(argv[i]);
				char *bn = basename(argv[i]);
				struct dir_t *p = dir_getdir(current, dn);
				if(p){
					struct dir_t *c = dir_new(bn);
					dir_addchild(p, c);
				}else{
					fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[i]);
				}
			}
		}
	}
}

/* exit the shell */
static void myexit(int argc, char *argv[]){
	exit(0);
}

/* Compare pointers to strings. Used as callback to qsort */
static int mycmp(const void *a, const void *b){
	return strcmp(*((char **)a), *((char **)b));
}

/* List a directory*/
static void ls(int argc, char *argv[]){
	struct dir_t *dir;
	dir = current; /* default to current directory*/
	if(argc == 2){
		/* a path has been supplied */
		dir = dir_getdir(dir, argv[1]);
		if(!dir){
			fprintf(stderr, "%s: %s: No such file directory\n", argv[0], argv[1]);
			return;
		}
	}
	int size = 0;
	const char *children[128];
	dir = dir->child;
	while(dir){
		children[size++] = dir->name;
		dir = dir->next;
	}
	/* Sort children by filename */
	qsort(children, size, sizeof (char *), mycmp);

	/* Print children */
	int i;
	for(i = 0 ; i < size; i++){
		printf("%s\n", children[i]);
	}
}

/* change directory */
static void cd(int argc, char *argv[]){
	if(argc == 1){
		/* cd with no arguments should change to home directory. We'll just change to the root */
		current = root;
	}else if(argc == 2){
		struct dir_t *dir = dir_getdir(current, argv[1]);
		if(!dir){
			fprintf(stderr, "%s: %s: No such file directory\n", argv[0], argv[1]);
		}else{
			current = dir;
		}
	}else{
		fprintf(stderr, "%s: too many arguments\n", argv[0]);
	}
}

/* Deletes a directory. Accepts multiple arguments to be deleted. */
static void rmdir(int argc, char *argv[]){
	if(argc < 2){
		fprintf(stderr, "%s: missing operand\n", argv[0]);
	}else{
		int i;
		for(i = 1; i < argc; i++){
			struct dir_t *dir = dir_getdir(current, argv[i]);
			if(!dir){
				fprintf(stderr, "%s: %s: No such file directory\n", argv[0], argv[i]);
			}else{
				dir_rmdir(dir);
			}
		}
	}
}

/*
 * This structure holds the commands the shell supports.
 * There is a string specifying the name and a function pointer to the method.
 */
struct executable execs[] = {
	{"mkdir", mkdir},
	{"exit", myexit},
	{"ls", ls},
	{"cd", cd},
	{"rmdir", rmdir},
	{NULL, NULL}
};

