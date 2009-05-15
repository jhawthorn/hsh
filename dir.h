#ifndef DIR_H
#define DIR_H DIR_H

struct dir_t{
	const char *name;
	struct dir_t *parent;
	struct dir_t *child;
	struct dir_t *next;
};

extern struct dir_t *root;
extern struct dir_t *current;

struct dir_t *dir_new(const char *name);
void dir_pwd(struct dir_t *d);
void dir_addchild(struct dir_t *p, struct dir_t *c);
void dir_rmdir(struct dir_t *d);
struct dir_t *dir_subdir(struct dir_t *p, char *s);
struct dir_t *dir_getdir(struct dir_t *dir, char *s);

#endif
