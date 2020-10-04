#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
		wp_pool[i+1].prev = &wp_pool[i];

	}
	wp_pool[NR_WP - 1].next = NULL;
	wp_pool[0].prev = NULL;

	head = NULL;
	free_ = wp_pool;
}
/*
// we should use this wp from 0 to 31
WP* new_wp() {
	if (free_ == NULL) {
		assert(0);
	}
	WP* temp_f = free_;
	free_ = free_->next;
	free_->prev = NULL;
	temp_f->next = head;
	head->prev = temp_f;
	head = temp_f;
	return head;
}
void free_wp(WP *wp) {
	wp->prev->next = wp->next;
	wp->next->prev = wp->prev;
	wp->next = free_;
	free_->prev = wp;
	wp->prev = NULL;
}



*/
/* TODO: Implement the functionality of watchpoint */

