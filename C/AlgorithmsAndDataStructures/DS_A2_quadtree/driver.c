
// Code here and in many of the included directory libraries is based on my assignment 1 work
// https://edstem.org/au/courses/9173/lessons/25539/slides/182812

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "PR_QuadTree.h"
#include "list.h"
#include "footpath.h"
#include "csv.h"
#include "stage3.h"
#include "stage4.h"
#include "driver.h"



int main(int argc, char *argv[]) {

	/*

	argc: # of strings in argv

	argv[0]: program name
	argv[1]: stage number
	argv[2]: csv data file
	argv[3]: text output file
	argv[4]: (x) longitude left
	argv[5]: (y) latitude bottom
	argv[6]: (x) longitude right
	argv[7]: (y) latitude max_y

	*/


	//qtree_stability_test();

	// CHECK ARGUMENTS ----------------------

	// assert number of command line arguements
	assert(argc == 8);

	// assert possible stage values
	assert('3' <= argv[1][0]);
	assert(argv[1][0] <= '4');


	// OPEN WRITE FILE ----------------------

	FILE *fptr = fopen(argv[3], "w");
	assert(fptr);

	// CREATE RECTANGLE FROM INPUT ARGS ----------------------

	rectangle2D_t *area = malloc(sizeof(rectangle2D_t));
	assert(area);
	char *chars_after;
	area->min_x = strtold(argv[4], &chars_after); assert(chars_after[0] == '\0');
	area->min_y = strtold(argv[5], &chars_after); assert(chars_after[0] == '\0');
	area->max_x = strtold(argv[6], &chars_after); assert(chars_after[0] == '\0');
	area->max_y = strtold(argv[7], &chars_after); assert(chars_after[0] == '\0');
	assert_rectangle_bounds(area);

	// READ DATA FROM CSV INTO QUADTREE ------------------

	// assert data file ends in .csv
	assert(string_ends_with(argv[2], ".csv"));

	// read into array
	array_t *array = read_csv_to_dynamic_array(argv[2], scan_footpath_csv_line);

	// Where multiple footpaths are present in the found point region, footpaths should be printed in order of footpath_id.
	qsort(array -> ptrs, array -> len, sizeof(void *), (int (*)(const void *, const void *)) footpathid_cmp);

	// create quadtrees
	qtree_t *qtree = create_empty_quadtree(area);
	
	

	// add into quadtrees from array
	for (int i = 0; i < array -> len; i++) {
		add_to_qtree(qtree, array -> ptrs[i], srt_point_from_footpath(array -> ptrs[i]));
		add_to_qtree(qtree, array -> ptrs[i], end_point_from_footpath(array -> ptrs[i]));
	}

	

	// CALL STAGE ------------------
	

	switch (argv[1][0]) {
		
		case '3': stage3(qtree, fptr); break;
		case '4': stage4(qtree, fptr); break;
		default: printf("Not an accepted stage number");
	}


	for (int i = 0; i < array -> len; i++) {
		freeFootpathStruct(array->ptrs[i]);
	}
	free_array_not_data(array);
	free_qtree(qtree, NULL);
	fclose(fptr);

	return 0;
}


bool string_ends_with(char string[], char end[]) {
	return strcmp(string + (strlen(string) - strlen(end)), end) == 0;
}



point2D_t *get_point(void *p) { return p; }

void qtree_stability_test() {

	#define MAX 32

	srand(1); // make rand deterministic for debugging purposes

	rectangle2D_t *r = malloc(sizeof(rectangle2D_t));
	r->max_x = MAX;
	r->min_x = 0;
	r->max_y = MAX;
	r->min_y = 0;

	qtree_t *qtree = create_empty_quadtree(r);
	for (int i = 0; i < 100000; i++) {
		
		point2D_t *p = malloc(sizeof(point2D_t));
		p -> x = rand() % MAX;
		p -> y = rand() % MAX;

 		add_to_qtree(qtree, NULL, p);
	}

	free_qtree(qtree, free);
}



