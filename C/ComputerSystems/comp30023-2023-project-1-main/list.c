 

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "list.h"
#include "help.h"
#include "types.h"
#include "print.h"



// ---------------------------------------------
//         L I N K E D     L I S T 



uint_t node_count(list_t *list, node_t *node) {
	assert(list);
	assert(node);

	uint_t count = 0;

	node_t *cur_node = list->head;
	while (cur_node) {
		if (cur_node == node) {
			count++;
		}
		cur_node = cur_node->next;
	}

	return count;
}


size_t nodes_length(node_t *node) {
	size_t len = 0;
	while (node) {
		len++;
		assert(node != node -> next);
		node = node -> next;
	}
	return len;
}



node_t *get_tail(list_t *list) {
	assert(list);
	node_t *node = list->head;
	assert(node);
	while(node->next) {
		node = node -> next;
	}
	assert(node);
	return node;
}


void assert_once(list_t *list) {

	node_t *cur_node = list->head;
	while (cur_node) {
		assert(node_count(list, cur_node) == 1);
		cur_node = cur_node -> next;
	}
}

void assert_valid_list(list_t *list) {

	if (list->tail) { 
		assert(list->head);
		assert(list->tail->next == NULL); 
		assert(get_tail(list) == list->tail);
	}
	
	assert_once(list);
	
	size_t actual_length = nodes_length(list->head);
	if (list->len != actual_length) {
		printf("\n\nLengths: List %li Actual %li\n\n", list->len, actual_length);
		assert(false);
	}
	if ( list->head) { assert( list->tail); }
	if (!list->head) { assert(!list->tail); }

}


void list_init(list_t *list) {
	assert(list);

	list->head = NULL;
	list->tail = NULL;
	list->len = 0;
}


node_t *create_node(size_t extra) {
	// Data is not initialised for performance benifits

	node_t *newNode = malloc(sizeof(node_t) + extra);
	newNode->next = NULL;
	return newNode;
}

void push(list_t *list,  node_t *node) {
	assert(list);
	assert(node->next == NULL);
	assert(is_node_in_list(list, node) == false);

	if (list->tail) { 
		list -> tail -> next = node;
	} else { // list is empty
		list -> head = node;
		assert(list->len == 0);
	}

	list -> tail = node;	
	list -> len ++;

	assert(is_node_in_list(list, node) == true);
	assert_valid_list(list);
}

bool is_node_in_list(list_t *list, node_t *find_node) {
	node_t *cur_node = list->head;
	while (cur_node) {
		if (cur_node == find_node) {
			return true;
		}
		cur_node = cur_node->next;
	}
	return false;
}



void push_front(list_t *list,  node_t *node) {
	assert(list);
	assert(node->next == NULL);
	assert(is_node_in_list(list, node) == false);

	node->next = list->head;

	if (!list->tail) { 
		assert(list->len == 0);	
		list -> tail = node;
	}

	list -> head = node;	
	list -> len ++;


	assert(is_node_in_list(list, node) == true);
	assert_valid_list(list);
}






node_t *return_front(list_t *list) {
	assert(list);
	assert(list->len > 0);
	assert(list->head);
	assert(list->tail);

	node_t *return_node = list->head;
	list->head = list->head->next;
	return_node->next = NULL;

	list->len --;

	if (list->len == 0) {
		assert(!list->head);
		list -> tail = NULL;
	}

	assert(is_node_in_list(list, return_node) == false);
	assert_valid_list(list);

	return return_node;
}


void pop_front(list_t *list) {
	assert(list);
	node_t *front = return_front(list);
	free(front);
}


void free_nodes(list_t *list) {
	assert(list);
	while ((list)->len) {
		pop_front(list);
	}
	assert(!list->head);
	assert_valid_list(list);
}



void sort_list(list_t *list, int (*compare)(const void *, const void *)) {
	assert(list);

	if (list->len <= 1) { return; }

	// convert to array of node_pointers
	node_t **node_pointers = malloc(sizeof(node_t *) * list->len);

	assert(list->len == nodes_length(list->head));
	node_t *cur = list->head;
	for (int cur_i = 0; cur_i < list->len; cur_i++) {
		assert(cur);
		node_pointers[cur_i] = cur;
		cur = cur->next;
	}

	assert(!cur);

	// sort with qsort
	qsort(node_pointers, list->len, sizeof(node_t *), compare);

	// update head and tail
	list->head = node_pointers[0];
	list->tail = node_pointers[list->len - 1];

	// update linked list order
	node_t *node = list->head;
	for (int i = 1; i< list->len; i++) {
		node->next = node_pointers[i];
		node = node_pointers[i];
	}
	node->next = NULL;


	assert_valid_list(list);
	free(node_pointers);
}


node_t *node_from_data(void *p) {

	/*


	typedef struct node node_t;
	struct node {
		node_t *next;
		DATA HERE
	};
	
	*/


	assert(p);
	return p - sizeof(node_t *); 

}


node_t **pointer_to_node(list_t *list, node_t *find_node) {

	// find the memory address storing the pointer to a node in a list
	// this can be in the head or in the next inside on of the nodes

	assert(list);
	assert(find_node);

	if (list->head == find_node) { return &list->head; }

	node_t *cur_node = list->head;
	while (cur_node) {

		if (cur_node->next == find_node) {
			return &cur_node->next;
		}
		cur_node = cur_node -> next;
	}
	assert(false);

}


node_t *previous_node(list_t *list, node_t *node) {
	assert(list);
	assert(node);

	node_t *cur_node = list->head;
	while (cur_node && cur_node->next != node) {
		cur_node = cur_node -> next;
	}
	return cur_node;
}



void rm_node_wofree(list_t *list, node_t *remove_node) {

	assert(list);
	assert(remove_node);
	assert(list->head);
	assert(list->len != 0);


	// update head and tail
	if (list->len == 1) {
		list->head = NULL;
		list->tail = NULL;
	} else if (remove_node == list->tail) {
		list->tail = previous_node(list, remove_node);
		list->tail->next = NULL;
	} else if (remove_node == list->head) {
		list->head = remove_node->next;
	} else {
		// update pointer to node
		node_t *prev = previous_node(list, remove_node);
		prev->next = remove_node ->next;
	}
	

	list->len--;
	remove_node->next = NULL;

	assert_valid_list(list);
}



void list_insert_into_next(list_t *list, node_t **insert_here, node_t *to_insert) {

	// allows nodes to be inserted in a list at any point
	// including at the head
	// -HERE> [INSERT] -> 

	assert(list);
	assert(insert_here);
	assert(to_insert);
	assert(to_insert->next == NULL);
	assert(is_node_in_list(list, to_insert) == false);
	assert_valid_list(list);

	if (list->len == 0) {
			list->head = to_insert; 
			list->tail = to_insert;
	} else {

			// there are two pointers to the tail
			if (*insert_here == NULL) { assert(&list->tail->next == insert_here); }
			if (*insert_here == NULL || &list->tail == insert_here) { 

					list->tail->next = to_insert;
					list->tail = to_insert; 

			// 
			} else {
				
				to_insert->next = (*insert_here); 
				(*insert_here) = to_insert;
			}
	}

	list->len++;

	assert(is_node_in_list(list, to_insert) == true);
	assert_valid_list(list);
}
