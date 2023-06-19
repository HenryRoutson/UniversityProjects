
#ifndef LIST
#define LIST

#include <stdlib.h>

#include "types.h"
#include "memory.h"


// -----------
// generic list

typedef struct node node_t;
struct node {
	node_t *next;
};

typedef struct {
	node_t *head;
	node_t *tail;
	size_t len;
} list_t;


// -----------

#define LIST_INIT { NULL, NULL, 0 }

void list_init(list_t *list);
node_t *create_node(size_t extra);
void push(list_t *list,  node_t *node);
void push_front(list_t *list, node_t *node);
void pop_front(list_t *list);
void sort_list(list_t *list, int (*compare)(const void *, const void *));
node_t *return_front(list_t *list);
void free_nodes(list_t *list);
node_t *node_from_data(void *p);
node_t **pointer_to_node(list_t *list, node_t *find_node);
void rm_node_wofree(list_t *list, node_t *remove_node);
void list_insert_into_next(list_t *list, node_t **insert_here, node_t *to_insert);
bool is_node_in_list(list_t *list, node_t *find_node);
void assert_valid_list(list_t *list);

#endif
