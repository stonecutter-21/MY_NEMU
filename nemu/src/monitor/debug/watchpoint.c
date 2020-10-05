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


void add_a_watch(char *str) {
	new_wp(str);
}



// we should use this wp from 0 to 31
WP* new_wp(char *str) {
	if (free_ == NULL) {
		assert(0);
	} 
	// first we build the list, add the node
	WP* temp_f = free_;
	WP* temp_h = head;
	// deal with free list
	free_ = free_->next;
	free_->prev = NULL;
	// deal with head list
    head = temp_f;
	head->next = temp_h;
	if (temp_h) {
		temp_h->prev = head;
	}
	head ->prev = NULL;
	//then we gave the value to the new node
	strcpy(head->str,str);
	return head;
}



void free_wp(WP *wp) {
	// first we deal with free_
	WP * temp = free_->next;
	if (free_ != NULL) {
		free_->prev = wp;
	}
	free_ = wp;
	// then we deal with head
	// if the one we want to delete is the head, we should change head
	if (wp->prev == NULL) {
		head = head->next;
		printf ("!!\n");
		if (head != NULL) {
			head->prev = NULL;
		}
	}
	else {
		wp->prev->next = wp->next;
		if (wp->next != NULL) {
			wp->next->prev = wp->prev;
		}
	}
	// last we deal with wp
	wp->prev = NULL;
	wp->next = temp;
}

void print_wp() {
	WP* node = head;
	if (head == NULL) {
		printf ("No watch point now!\n");
		return;
	}
	while (node != NULL) {
		printf("watch point %d: %s\n", node->NO,node->str);
		node = node->next;
	}
}

void delete_point(int num){
	WP* node = head;
	while (node != NULL) {
		
		if (node->NO == num) {
			free_wp(node);
			return;
		}
		node = node->next;
	}
	printf ("There is no watch point %d being used\n",num);
}

 


/* TODO: Implement the functionality of watchpoint */

