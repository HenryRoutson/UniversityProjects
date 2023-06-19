
/* 
	MODULAR LINKED LIST & ARRAY LIBRARY
*/


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"





// ---------------------------------------------
//         L I N K E D     L I S T

// Inspiration taken from
// https://groklearning.com/learn/unimelb-comp10002-2022-s1/assignment2/0/
// https://edstem.org/au/courses/9173/lessons/25902/slides/185173



list_t *create_empty_list() {

	list_t *list = malloc(sizeof(list_t));
	assert(list);

	list->head = NULL;
	list->tail = NULL;
	list->len = 0;

	return list;
}


node_t *create_node(void *data) {

	node_t *newNode = malloc(sizeof(node_t));
	assert(newNode);

	newNode->next = NULL;
	newNode->data = data;

	return newNode;
}


void add_to_list(list_t *list, void *data) {
	/*
	add to end of linked list

	... -> node -> tail -> NULL
	... -> node -> tail -> newNode
	... -> node -> node -> tail

	*/

	assert(list);

	node_t *newNode = create_node(data);

	if (list->tail) { 
		list -> tail -> next = newNode;

	} else { // list is empty
		list -> head = newNode;
	}

	list -> tail = newNode;	
	list -> len ++;
}



void free_node(node_t *node, void (*free_data)(void *)) {
	assert(node);

	if (free_data) { free_data(node -> data); }
	free(node);
}


void free_list(list_t *list, void (*free_data)(void *)) {
	if (list) {
		
		node_t *cur;
		node_t *nxt = list -> head;

		while ((cur = nxt)) {
			nxt = cur -> next;

			free_node(cur, free_data);
		}

		free(list);
	}
}

void merge_lists(list_t *dst, list_t *src) {
	assert(dst);
	assert(src);

	dst -> tail -> next = src -> head;
	dst -> tail = src -> tail;
	free(src);
}


list_t *list_filter(list_t *list, int(*condition)(void *)) {

	assert(list);

	list_t *new_list = create_empty_list();
	node_t *node = list -> head;

	while (node) {
		if (condition(node -> data)) {
			add_to_list(new_list, node -> data);
		}

		node = node -> next;
	}

	return new_list;
}











// ---------------------------------------------
//               A R R A Y


array_t *create_empty_array(size_t capacity) {
	assert(capacity > 0);

	array_t *array = malloc(sizeof(array_t));
	assert(array);

	array -> ptrs = malloc(sizeof(void *) * capacity);
	assert(array -> ptrs);

	array -> len = 0;
	array -> capacity = capacity;

	return array;
}


void add_to_array(array_t *array, void *data) {
  assert(array);
  assert((array -> len) >= 0);
  assert(array -> capacity > 0);
  assert((array -> len) <= (array -> capacity));
  

  array -> len ++;

  if (array -> len > array -> capacity) {
    array -> capacity *= 2;
    array -> ptrs = realloc(array -> ptrs, array -> capacity * sizeof(void *));
    assert(array -> ptrs);
  }

	(array -> ptrs)[array -> len - 1] = data;
}


void free_array_not_data(array_t *array) {
	assert(array);

	free(array -> ptrs);
	free(array);
}
