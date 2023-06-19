#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "linked_list.h"
#include "footpath.h"
#include "csv.h"


/*

LD_PRELOAD=
rm output.*
valgrind --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all -s ./dict1 1 tests/dataset_1000.csv output.out < tests/test1000.s1.in > output.stdout.out
gdb dict1

*/

void stage1(char data_file_path[], char output_file_path[]) {
	/*
	takes address queries from stdin and prints matches from the csv data file
	*/

	// move data into a linked list
	list_t *list = read_footpath_csv(data_file_path);
	assert(list);
	assert(list -> head);

	// open output file pointer
	FILE *fptr = fopen(output_file_path, "w");
	assert(fptr);


	// init
	char query[MAX_FIELD_CHARS];
	int count;
	footpath_t *footpath;
	node_t *node = NULL;
	int n_queries = 0;

	// start queries
	while (fgets(query, MAX_FIELD_CHARS, stdin)) {
		query[strlen(query) - 1] = '\0'; // remove newline charact

		// file prints ---------
		if (n_queries) { fprintf(fptr, "\n"); }
		fprintf(fptr, "%s", query);

		// stdout prints ---------
		printf("%s ", query);
		
		// inits
		node = list -> head;
		count = 0;

		// find matches
		while (node) {

			assert(node -> data);
			footpath = node -> data;

			if (footpath->address) { // can be NULL
				if (strcmp(footpath->address, query) == 0) {
					count++;
					print_footpath(footpath, fptr);
				}
			}

			node = node -> next;
		}

		// stdout prints ---------
		
		printf("%s", ARROW);
		if (count) {
			printf("%i\n" , count);
		} else {
			printf("NOT FOUND\n");
		}

		//
		
		fflush(fptr); // flush to file incase of ^C
		n_queries++;
	}

	// free 
	free_footpath_list(list);

	// close file
	fclose(fptr);
	
}


