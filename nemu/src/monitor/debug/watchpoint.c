#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP-1; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
		wp_pool[i+1].prev = &wp_pool[i];

	}
	wp_pool[NR_WP - 1].next = NULL;
	wp_pool[0].prev = NULL;

	head = NULL;
	free_ = wp_pool;
}

// we should use this wp from 0 to 31
WP* new_wp(char *str) {
	if (free_ == NULL) {
		assert(0);
	}
	// if head is NULL , five it a value
	if (head == NULL) {
		head = free_;
		head->next = NULL;
		head->prev = NULL;
		free_ = free_->next;
		free_->prev = NULL;
		strcpy(head->str,str);
	    return head;
	}
	else {
		// first we build the list, add the node
	    WP* temp_f = free_;
	    free_ = free_->next;
	    free_->prev = NULL;
	    temp_f->next = head;
	    head->prev = temp_f; // here
	    head = temp_f;

	    //then we gave the value to the new node
	    strcpy(head->str,str);
	    return head;
	}
	
}
void free_wp(WP *wp) {
	wp->prev->next = wp->next;
	wp->next->prev = wp->prev;
	wp->next = free_;
	free_->prev = wp;
	wp->prev = NULL;
}

void print_wp() {
	WP* node = head;
	while (node != NULL) {
		printf("watch point %d: %s\n", node->NO,node->str);
		node = node->next;
	}
}

 


/* TODO: Implement the functionality of watchpoint */

