
/* 
	MODULAR LINKED LIST LIBRARY
*/

// Inspiration taken from
// https://groklearning.com/learn/unimelb-comp10002-2022-s1/assignment2/0/
// https://edstem.org/au/courses/9173/lessons/25902/slides/185173


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "linked_list.h"


list_t *create_empty_list() {

	list_t *list = malloc(sizeof(list_t));
	assert(list);
	
	// init
	list->head = NULL;
	list->tail = NULL;
	list->len = 0;

	return list;
}



void add_to_list(list_t *list, void *pointer) {
	/*
	add to end of linked list

	... -> node -> tail -> NULL
	... -> node -> tail -> newNode
	... -> node -> node -> tail

	*/

	node_t *newNode = malloc(sizeof(node_t));
	assert(newNode);

	newNode->next = NULL;
	newNode->data = pointer;

	//

	if (list->tail) { 
		list->tail->next = newNode;

	} else { // list is empty
		list->head = newNode;
	}

	list->tail = newNode;	
	list->len ++;
}


void free_list(list_t *list, void (*free_data)(void *)) {
	assert(list);

	node_t *cur;
	node_t *nxt = list -> head;

	while ((cur = nxt)) {
		nxt = cur -> next;

		free_data(cur -> data);
		free(cur); // node
	}

	free(list);
}



void free_list_not_data(list_t *list) {
	assert(list);

	node_t *cur;
	node_t *nxt = list -> head;

	while ((cur = nxt)) {
		nxt = cur -> next;
		free(cur); // node
	}

	free(list);
}

