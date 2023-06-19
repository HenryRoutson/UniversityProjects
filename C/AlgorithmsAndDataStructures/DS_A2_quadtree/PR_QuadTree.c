/*

A quadtree is a data structure that stores d-dimensional points and enables efficient search for the stored points. We will only consider the case d = 2. One particular quadtree which can be used to store 2-dimensional points is the point-region quadtree, simply referred as a PR quadtree. A binary tree can be defined as a finite set of nodes that are either empty or have a root and two binary trees T_l and T_r (the left and right subtree). A quadtree has a similar recursive definition: instead of two subtrees we have four subtrees, hence the name quad. This means that each node either has four children or is a leaf node. The four leaf nodes are often referred to as NW, NE, SW, SE.

*/

// Point-region quadtree

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "PR_QuadTree.h"
#include "list.h"

///

void assert_rectangle_bounds(rectangle2D_t *rectangle) {
	assert(rectangle);
	assert(rectangle -> min_x <= rectangle -> max_x);
	assert(rectangle -> min_y <= rectangle -> max_y);
}

// From
// https://stackoverflow.com/questions/3269434/whats-the-most-efficient-way-to-test-if-two-ranges-overlap --------------------------

ld_t min_2(ld_t a, ld_t b) {
	if (a < b) { return a; }
	return b;
}

ld_t max_2(ld_t a, ld_t b) {
	if (a > b) { return a; }
	return b;
}

bool is_range_overlapping(ld_t x1, ld_t x2, ld_t y1, ld_t y2) {
	// Given two ranges [x1,x2], [y1,y2]
	assert(x1 <= x2);
	assert(y1 <= y2);
	return max_2(x1,y1) <= min_2(x2,y2);
}

/// --------------------------

bool in_range(ld_t val, ld_t y1, ld_t y2) {
	// Given two ranges [val,val], [y1,y2]

	assert(y1 <= y2);
	return y1 <= val && val <= y2;
}


bool inRectangle(point2D_t *p, rectangle2D_t *r) {
	// inequalities needs to be inclusive so points on border aren't 'outside' the rectangle
	assert_rectangle_bounds(r);



	if (!in_range(p -> x, r -> min_x, r -> max_x)) { return false; }
	if (!in_range(p -> y, r -> min_y, r -> max_y)) { return false; }

	return true;

}
    
bool rectangleOverlap(rectangle2D_t *r1, rectangle2D_t *r2) {
	/* 
	rectangles overlapp if both their x and y ranges overlap
	below is an example of overlap of a range
	-------
	    -------
	*/

	return is_range_overlapping(r1->min_x, r1->max_x, r2->min_x, r2->max_x) && // x
		     is_range_overlapping(r1->min_y, r1->max_y, r2->min_y, r2->max_y);   // y
}		


ld_t rgl_center_y(rectangle2D_t *rectangle) {
	return (rectangle -> min_y + rectangle -> max_y) / 2.0L;
}

ld_t rgl_center_x(rectangle2D_t *rectangle) {
	return (rectangle -> min_x + rectangle -> max_x) / 2.0L;
}





enum quad determine_quad(point2D_t *point, rectangle2D_t *rectangle) {
	// https://static.au.edusercontent.com/files/PkhH9mF1jgS4oVrdVaGcxFgm

	assert(inRectangle(point, rectangle));

	ld_t mid_y = rgl_center_y(rectangle);
	ld_t mid_x = rgl_center_x(rectangle);

	bool N = (point -> y) >= mid_y, S = 1 - N;
	bool E = (point -> x) >  mid_x, W = 1 - E;


	if (S && W) { return SW; }
	if (N && W) { return NW; }
	if (N && E) { return NE; }
	if (S && E) { return SE; }
	assert(0);
}


qtree_t *create_empty_quadtree(rectangle2D_t *rectangle) {

	qtree_t *qtree = malloc(sizeof(qtree_t));
	assert(qtree);

	assert_rectangle_bounds(rectangle);
	qtree -> area = rectangle;

	for (int i = 0; i < 4; i++) {
		qtree -> child[i] = NULL;
	}

	qtree -> leaf = NULL;

	return qtree;
}


rectangle2D_t *create_quad_rectangle(rectangle2D_t *area, enum quad q) {

	rectangle2D_t *quad = malloc(sizeof(rectangle2D_t));
	assert(quad);

	memcpy(quad, area, sizeof(rectangle2D_t));

	if (q == NE || q == NW) {
		quad -> min_y = rgl_center_y(area); // move min up
	} else {
		quad -> max_y = rgl_center_y(area); // move max down
	}

	if (q == NE || q == SE) {
		quad -> min_x = rgl_center_x(area);
	} else {
		quad -> max_x = rgl_center_x(area);
	}

	return quad;
}


leaf_t *create_leaf(void *data, point2D_t *point) {
	leaf_t *leaf = malloc(sizeof(leaf_t));
	leaf -> point = point;
	leaf -> list = create_empty_list();
	add_to_list(leaf -> list, data);
	return leaf;
}


qtree_t *get_leaf_qtnode(qtree_t *qtree, point2D_t *point, bool print) {

	while (true) {
		assert(qtree);

		enum quad q = determine_quad(point, qtree -> area);
		
		if (print) {
			printf(" ");
			switch (q) {
 				case NW: printf("NW"); break;
				case SW: printf("SW"); break;
				case NE: printf("NE"); break;
				case SE: printf("SE"); break;
			}
		}

		if (qtree -> leaf) { break; }
		if (!qtree -> child[q]) { break; }
		qtree = qtree -> child[q];
	}

	return qtree;
}

bool areSamePoint(point2D_t *p1, point2D_t *p2) {
	return memcmp(p1, p2, sizeof(point2D_t)) == 0;
}


void add_to_qtree(qtree_t *qtree, void *data, point2D_t *add_point) {
	assert(qtree);

	qtree = get_leaf_qtnode(qtree, add_point, false);

	if (!qtree -> leaf) { // list is empty
		qtree -> leaf = create_leaf(data, add_point);

	} else if (areSamePoint(qtree -> leaf -> point, add_point)) {

		add_to_list(qtree -> leaf -> list, data);

	} else { // list has different point

		enum quad q = determine_quad(qtree -> leaf -> point, qtree -> area);

		qtree -> child[q] = create_empty_quadtree( create_quad_rectangle(qtree -> area, q) );
		qtree -> child[q] -> leaf = qtree -> leaf;
		qtree -> leaf = NULL;

		add_to_qtree(qtree, data, add_point); // add point reccursively, incase same quad
	}
}


void free_qtree(qtree_t *qtree, void (*free_data)(void *)) {
	// recursive implimentation

	if (!qtree) { return; }

	free(qtree ->area);

	for (int q = 0; q < 4; q++) {

		if (qtree -> child[q]) {
			free_qtree(qtree -> child[q], free_data);
		}
	}

	if (qtree -> leaf) {
		free_list(qtree -> leaf -> list, free_data);
		free(qtree -> leaf);
	} 
	
	free(qtree);
}


list_t *search_qtree(qtree_t *qtree, point2D_t *point) {
	assert(qtree);

	qtree = get_leaf_qtnode(qtree, point, true);
	printf("\n");

	if (!qtree -> leaf) { return NULL; }
	if (areSamePoint(qtree -> leaf -> point, point)) { return NULL; }

	return qtree -> leaf -> list;
}




	// searchPoint
	// rangeQuery






