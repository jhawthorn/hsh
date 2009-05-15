
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dir.h"

struct dir_t *root;
struct dir_t *current;

struct dir_t *dir_new(const char *name){
	struct dir_t *d = malloc(sizeof(struct dir_t));
	d->name = strdup(name);
	d->parent = d->child = d->next = NULL;
	return d;
}

static void _dir_pwd(struct dir_t *d){
	if(d == root)
		return;
	_dir_pwd(d->parent);
	printf("/%s", d->name);
}
void dir_pwd(struct dir_t *d){
	if(d == root)
		printf("/");
	else
		_dir_pwd(d);
}

void dir_addchild(struct dir_t *p, struct dir_t *c){
	c->next = p->child;
	c->parent = p;
	p->child = c;
}

void dir_rmdir(struct dir_t *d){
	struct dir_t *p = d->parent;
	struct dir_t *c = p->child;
	if(c == d){
		p->child = d->next;
	}else{
		for(; c->next; c = c->next){
			if(c->next == d){
				c->next = c->next->next;
				return;
			}
		}
	}
}

struct dir_t *dir_subdir(struct dir_t *p, char *s){
	struct dir_t *c = p->child;
	if(!strcmp(s, ".")){
		return p;
	}else if(!strcmp(s, "..")){
		return p->parent;
	}else{
		for(; c; c = c->next){
			if(!strcmp(c->name, s)){
				return c;
			}
		}
		return NULL;
	}
}

struct dir_t *dir_getdir(struct dir_t *dir, char *s){
	s = strdup(s);
	if(s[0] == '/')
		dir = root;
	char *pch = strtok(s, "/");
	while(dir && pch != NULL){
		dir = dir_subdir(dir, pch);
		pch = strtok(NULL, "/");
	}
	free(s);
	return dir;
}
