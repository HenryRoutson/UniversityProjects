
#ifndef LINKED_LIST

	#define LINKED_LIST

	typedef struct node node_t;
	struct node {
		void *data;
		node_t *next;
	};

	typedef struct {
		node_t *head;
		node_t *tail;
		size_t len;
	} list_t;

	list_t * create_empty_list();
	void add_to_list(list_t *list, void *pointer);
	void free_list(list_t *list, void (*free_data)(void *));
	void free_list_not_data(list_t *list);



#endif
