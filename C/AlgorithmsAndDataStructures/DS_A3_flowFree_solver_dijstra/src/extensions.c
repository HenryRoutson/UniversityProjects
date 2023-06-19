#include "extensions.h"
#include "options.h"

//////////////////////////////////////////////////////////////////////
// For sorting colors

int color_features_compare(const void* vptr_a, const void* vptr_b) {

	const color_features_t* a = (const color_features_t*)vptr_a;
	const color_features_t* b = (const color_features_t*)vptr_b;

	int u = cmp(a->user_index, b->user_index);
	if (u) { return u; }

	int w = cmp(a->wall_dist[0], b->wall_dist[0]);
	if (w) { return w; }

	int g = -cmp(a->wall_dist[1], b->wall_dist[1]);
	if (g) { return g; }

	return -cmp(a->min_dist, b->min_dist);

}

//////////////////////////////////////////////////////////////////////
// Place the game colors into a set order

void game_order_colors(game_info_t* info,
                       game_state_t* state) {

	if (g_options.order_random) {
    
		srand(now() * 1e6);
    
		for (size_t i=info->num_colors-1; i>0; --i) {
			size_t j = rand() % (i+1);
			int tmp = info->color_order[i];
			info->color_order[i] = info->color_order[j];
			info->color_order[j] = tmp;
		}

	} else { // not random

		color_features_t cf[MAX_COLORS];
		memset(cf, 0, sizeof(cf));

		for (size_t color=0; color<info->num_colors; ++color) {
			cf[color].index = color;
			cf[color].user_index = MAX_COLORS;
		}
    

		for (size_t color=0; color<info->num_colors; ++color) {
			
			int x[2], y[2];
			
			for (int i=0; i<2; ++i) {
				pos_get_coords(state->pos[color], x+i, y+i);
				cf[color].wall_dist[i] = get_wall_dist(info, x[i], y[i]);
			}

			int dx = abs(x[1]-x[0]);
			int dy = abs(y[1]-y[0]);
			
			cf[color].min_dist = dx + dy;
			
		

		}


		qsort(cf, info->num_colors, sizeof(color_features_t),
		      color_features_compare);

		for (size_t i=0; i<info->num_colors; ++i) {
			info->color_order[i] = cf[i].index;
		}
    
	}

	if (!g_options.display_quiet) {

		printf("\n************************************************"
		       "\n*               Branching Order                *\n");
		if (g_options.order_most_constrained) {
			printf("* Will choose color by most constrained\n");
		} else {
			printf("* Will choose colors in order: ");
			for (size_t i=0; i<info->num_colors; ++i) {
				int color = info->color_order[i];
				printf("%s", color_name_str(info, color));
			}
			printf("\n");
		}
		printf ("*************************************************\n\n");

	}

}



//////////////////////////////////////////////////////////////////////
// Check for dead-end regions of freespace where there is no way to
// put an active path into and out of it. Any freespace node which
// has only one free neighbor represents such a dead end. For the
// purposes of this check, cur and goal positions count as "free".




// HENRY ROUTSON


bool color_is_completed(const game_state_t* state, int color) {
	return state->completed & (1 << color);
}




bool is_end_deadend(const game_info_t* info, const game_state_t* state, pos_t last_move_pos, int x, int y) {
	/*
	deadend if 
	init and goal have no surrounding squares of the same color or free
	*/
	
	pos_t cur_pos = pos_from_coords(x, y);
	cell_t cur_cell = state->cells[cur_pos];
	int cur_color = cell_get_color(cur_cell);

	for (int dir=0; dir < 4; ++dir) {

		pos_t neighbor_pos = offset_pos(info, x, y, dir);
		if (neighbor_pos == INVALID_POS) { continue; } // wall

		cell_t neighbor_cell = state->cells[neighbor_pos];

		if (cell_get_type(neighbor_cell) == TYPE_FREE) { return false; }
		if (cell_get_color(neighbor_cell) == cur_color) { return false; }
	}

	return true;

}


bool is_free_deadend(const game_info_t* info, const game_state_t* state, pos_t last_move_pos, int x, int y) {
	/*
	deadend if 
	three or more cells around an empty cell are complete
	*/

	int adj_filled = 0;

	// walls (could have added TYPE_WALL, but left out for simplicity)
	adj_filled += (x == 0);
	adj_filled += (y == 0);
	adj_filled += (x == info->size - 1);
	adj_filled += (y == info->size - 1);

	// neighbor cells --------
	
	for (int dir=0; dir < 4; ++dir) {

		pos_t neighbor_pos = offset_pos(info, x, y, dir);
		if (neighbor_pos == last_move_pos) { continue; } // *
		if (neighbor_pos == INVALID_POS) { continue; }

		cell_t neighbor_cell = state->cells[neighbor_pos];


		if (cell_get_type(neighbor_cell) == TYPE_PATH) { adj_filled++; } // don't need to check if completed if not last move*
		else if (cell_get_type(neighbor_cell) == TYPE_FREE) {}
		else { // init or goal
			if (color_is_completed(state, cell_get_color(neighbor_cell))) {
				adj_filled++;
			}
		}

	}

	return adj_filled >= 3;
}



bool is_deadend(const game_info_t* info, const game_state_t* state, pos_t last_move_pos, int x, int y) {
	/*
	check for any cell arrangements that make it impossible to create a solution
	*/

	if (!coords_valid(info, x, y)) { return false; }

	// get type of cell

	pos_t this_pos = pos_from_coords(x, y);
	cell_t this_cell = state->cells[this_pos];
	int this_type = cell_get_type(this_cell); 


	// call deadend function based on type

	if (this_type == TYPE_FREE) {      
		if (is_free_deadend(info, state, last_move_pos, x, y)) { return true; } 
	}
	else if (this_type == TYPE_PATH) { }
	else {                             
		if (is_end_deadend(info, state, last_move_pos, x, y)) { return true; } // extra optimisation
	}

	return false;
}


int game_check_deadends(const game_info_t* info,
                        const game_state_t* state) {

	/* 
		return true if is deadend 
	*/


	pos_t last_move_pos = state->pos[state->last_color];

	int x = -1, y = -1;
	pos_get_coords(last_move_pos, &x, &y);

	assert(0 <= x); assert(x < info->size);
	assert(0 <= y); assert(y < info->size);

	// test only around that position 
	// 12 cell check

	int coords[][2] = {
		{1, 0},
		{1, 1},
		{1,-1},
		{2, 0},
		{0, 1},
		{0, 2}
	};

	int n = 6;

	// +-

	for (int i = 0; i < n; i++) {

		if (is_deadend(info, state, last_move_pos, x+coords[i][0], y+coords[i][1]) ||
			is_deadend(info, state, last_move_pos, x-coords[i][0], y-coords[i][1])) { 
						
			return true; 
		}
	}

	return false; // return false if deadend is not found

}











