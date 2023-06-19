#ifndef CSV

	#define CSV

	#define MAX_FIELD_CHARS 128
	#define MAX_LINE_CHARS 512

	#include "list.h"
	#include "PR_QuadTree.h"
	
	#include <stdio.h>

	void getComma(FILE *fptr);
	char *csv_scan_string(FILE *fptr);
  list_t *read_csv_to_linkedlist(char csv_file[], void *(*scan_line)(FILE *));
	qtree_t *read_csv_to_quadtree(char csv_file[], void *(*scan_line)(FILE *), rectangle2D_t *area, point2D_t *(*f_get_point)(void *data));
	array_t *read_csv_to_dynamic_array(char csv_file[], void *(*scan_line)(FILE *));

#endif
