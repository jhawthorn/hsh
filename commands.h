#ifndef COMMANDS_H
#define COMMANDS_H COMMANDS_H

struct executable{
	const char *name;
	void (*e)(int argc, char *argv[]);
};

extern struct executable execs[];

#endif

