	
#ifndef PR_QUADTREE
	#define PR_QUADTREE

	#include <stdbool.h>
	#include "list.h"
	

	typedef long double ld_t;

	enum quad {
		SW,
		NW,
		NE,
		SE
	};

	typedef struct {
		double x, y;
	} point2D_t;

	typedef struct {
		ld_t min_x, min_y, 
		     max_x, max_y; // define bounds
	} rectangle2D_t;



	void assert_rectangle_bounds(rectangle2D_t *rectangle);

	// ---------------------------------------------------------

	

	typedef struct { 
		point2D_t *point;
		list_t *list; 
	} leaf_t; // encapsulate leaf n 

	typedef struct qtree qtree_t;
  struct qtree {
    rectangle2D_t *area;
		qtree_t *child[4];
		leaf_t *leaf;
  };

	// ---------------------------------------------------------

	qtree_t *create_empty_quadtree(rectangle2D_t *rectangle);
	void add_to_qtree(qtree_t *qtree, void *data, point2D_t *add_point);
	void free_qtree(qtree_t *qtree, void (*free_data)(void *));
	list_t *search_qtree(qtree_t *qtree, point2D_t *point);
	bool rectangleOverlap(rectangle2D_t *r1, rectangle2D_t *r2) ;

	// ---------------------------------------------------------
	// visible for testing
	
	bool in_range(ld_t val, ld_t y1, ld_t y2);
	bool is_range_overlapping(ld_t x1, ld_t x2, ld_t y1, ld_t y2);
	enum quad determine_quad(point2D_t *point, rectangle2D_t *rectangle); 
	bool inRectangle(point2D_t *point, rectangle2D_t *rectangle);
	
 
#endif
