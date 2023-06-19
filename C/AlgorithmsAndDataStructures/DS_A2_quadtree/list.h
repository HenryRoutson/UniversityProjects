
#ifndef LIST

	#define LIST

	// -----------

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
	node_t *create_node(void *pointer);
	void add_to_list(list_t *list, void *pointer);
	void free_list(list_t *list, void (*free_data)(void *));
	void free_list_not_data(list_t *list);

	// -----------

	typedef struct {
		void **ptrs; 
		size_t len;
		size_t capacity;
	} array_t; // dynamic pointer array

	array_t *create_empty_array(size_t capacity);
	void add_to_array(array_t *array, void *data);
	void free_array_not_data(array_t *array);

	// -----------

	array_t *data_array_from_llist(list_t *list);
	list_t *llist_from_array(array_t *array, int n);

	// -----------


#endif
