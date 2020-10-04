#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next; // next node
	struct watchpoint *prev; //before node
	char *str;  // hold the expression

	/* TODO: Add more members if necessary */


} WP;
void add_a_watch(char *str);
WP* new_wp(char *str);
void free_wp(WP *wp);
void print_wp();
#endif
