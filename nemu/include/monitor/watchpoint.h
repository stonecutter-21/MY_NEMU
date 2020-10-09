#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next; // next node
	struct watchpoint *prev; //before node
	char str[32];  // hold the expression
	int oldval;
	/* TODO: Add more members if necessary */

} WP;
bool spy_changed();
int add_a_watch(char *str);
int new_wp(char *str);
void free_wp(WP *wp);
void print_wp();
void print_free();
void delete_point(int num);
void delete_all();
#endif
