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
	WP* temp_f = free_;
	WP* temp_w = wp;
	//first we deal with the free_
	free_ = wp;
	wp->next = temp_f;
	wp ->prev = NULL;
	if (temp_f != NULL) {
		temp_f->prev = wp;
	}
	// then we deal with the head
	//if the head one is being delete, we should change the head
	if (temp_w->prev == NULL) {
		head = head->next;
		head->prev = NULL;
	}
	// now we think the one is not head, which has a prev
	else {
		temp_w->prev->next = temp_w->next;
		if (temp_w->next != NULL) {
			 temp_w->next->prev = temp_w->prev;
		}
	}
}
/*
void free_wp(WP *wp) {
	WP* temp_f = free_;
	WP* temp_w = wp;

	// if head == NULL, that means there is nothing to free
	if (head == NULL) {
		printf ("Free error! No watch point now.\n");
		return;
	}

	free_ = wp;
	wp = temp_f->next;
	// after add a node ,if there is at least two node in free_ , just put the link to wp
	if (temp_f!= NULL) {
		temp_f->prev = wp;
	}
	// up to now, we have built connection for free_ and wp
	
	// if there is only one node to be free
	if (head -> next = NULL) {
		head = NULL;
	}
	else {
		// if it is the first node to be freed
		if (temp_w->prev == NULL) {
			head = temp_w->next;
			head->prev = NULL;
		}
		else {

		}
		

	}
}
*/


void print_wp() {
	WP* node = head;
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
}

 


/* TODO: Implement the functionality of watchpoint */

