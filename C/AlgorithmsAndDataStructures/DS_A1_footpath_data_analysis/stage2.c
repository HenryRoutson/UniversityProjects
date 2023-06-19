#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "linked_list.h"
#include "footpath.h"
#include "csv.h"
#include "search.h"
#include "stage2.h"

/*

LD_PRELOAD=
rm output.*
valgrind --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all -s ./dict1 2 tests/dataset_1000.csv output.out < tests/test1000.s2.in > output.stdout.out
gdb dict1


./dict1 2 tests/dataset_20.csv output.out < tests/test20.s2.in > output.stdout.out

*/




void stage2(char data_file_path[], char output_file_path[]) {
	/*
	takes grade1in queries from stdin and prints closest match from the csv data file
	*/


    // move data into a linked list
	list_t *list = read_footpath_csv(data_file_path);
	assert(list);
	assert(list -> head);

	int len = list -> len;

	// open output file
	FILE *fptr = fopen(output_file_path, "w");
	assert(fptr);

	// create array
	footpath_t **footpaths = malloc(sizeof(footpath_t *) * len);

    // place footpath pointers into array
	node_t *cur = list -> head;
	int i = 0;

	while (cur) {
		footpaths[i++] = cur -> data;
		cur = cur -> next;
	}
	assert(i == len);	

	// free list
	free_list_not_data(list);


	// sort array
	assert(len);
	qsort(footpaths, len, sizeof(footpath_t *), grade1in_cmp);
	assert((footpaths[0] -> grade1in) <= (footpaths[len - 1] -> grade1in));


	// start queries
	int n_queries = 0;
	footpath_t *closest_record;

	char query[MAX_FIELD_CHARS];
	while (fgets(query, MAX_FIELD_CHARS, stdin)) {
		query[strlen(query) - 1] = '\0'; // remove newline charact


		// file prints ---------
		if (n_queries) { fprintf(fptr, "\n"); }
		fprintf(fptr, "%s", query);

		// stdout prints ---------
		if (n_queries) { putchar('\n'); }
		printf("%s ", query);
		printf("%s", ARROW);

		// get closest
		double d_query = atof(query);
		int index = approx_binary_search((void **)footpaths, len, &d_query, grade1in_minus);
		closest_record = footpaths[index];

		// file prints ---------
		print_footpath(closest_record, fptr);

		// stdout prints ---------
		printf("%.1lf", closest_record -> grade1in);
		
		fflush(fptr); // flush to file incase of ^C
		n_queries++;
	}


	// free
	for (int i = 0; i < len; i++) {
		freeFootpathStruct(footpaths[i]);
	}
	free(footpaths);

	fclose(fptr);
	
}



int grade1in_cmp(const void *p1, const void *p2) {

	assert(p1);
	assert(p2);
	assert(p1 != p2);

    double d1  = (*(footpath_t **)p1) -> grade1in
	           - (*(footpath_t **)p2) -> grade1in; 

	// int conversion rounds double down
	// 	   (int) 0.5 == 0
	// so can't just return difference

	if (d1 > 0) { return  1; }
	if (d1 < 0) { return -1; }
	return  0;
}


double grade1in_minus(void *p_struct, void *p_query) {
	// get difference between query and value in struct

	assert(p_struct);
	assert(p_query);

	footpath_t *footpath = p_struct;


	double query = * (double *) p_query;
	 
	return (footpath -> grade1in) - query;
}
