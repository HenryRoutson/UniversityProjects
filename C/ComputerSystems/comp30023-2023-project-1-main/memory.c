
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#include "types.h"
#include "global.h"
#include "memory.h"
#include "print.h"


bool request_memory(process_t *requesting_process) {
	// impliment best fix memory allocation

	assert_valid_list((list_t *) &memory_ll);
	//printf("\n%p\n", &requesting_process->memory_node); // TODO
	assert(is_node_in_list((list_t *) &memory_ll, (node_t *) &requesting_process->memory_node) == false); // no second requests for memory
	assert(requesting_process);

	long free_block_start = 0;
	long free_block_end = UNINITIALISED_INT;
	long best_excess_memory = UNINITIALISED_INT;

	// list insert positions
	memory_node_t **best_position = NULL;
	memory_node_t **position = &memory_ll.head; // current

	while (true) {
		assert_valid_list((list_t *) &memory_ll);
		if (memory_ll.tail) {
			assert(memory_ll.tail->memory.memory_start + 
						 memory_ll.tail->memory.memory_requirement <= MEMORY_CAPACITY_MB);
		}

		if (DEBUG_PRINTS1) {
			print_memory_list();
		}

		// if there is another memory node position points to
		if (*position) {  
			
			// the curent memory block will end when it starts
			assert((*position)->memory.memory_start != UNINITIALISED_INT);
			assert((*position)->memory.memory_start >= 0);
			if (free_block_end != UNINITIALISED_INT) {
				assert((*position)->memory.memory_start >= free_block_end);
			}
			assert((*position)->memory.memory_start <= MEMORY_CAPACITY_MB);

			free_block_end = (*position)->memory.memory_start;

		} else { // points to end of list if NULL

			// free_block_end can be uninitialised here
			free_block_end = MEMORY_CAPACITY_MB;			
		}
		
		long available_memory = free_block_end - free_block_start; 
		long excess_memory = available_memory - requesting_process->memory_node.memory.memory_requirement; // current

		if (excess_memory >= 0) {

			if (!best_position) { assert(best_excess_memory == UNINITIALISED_INT); }
			if (!best_position || excess_memory < best_excess_memory) {

				best_position = position;
				best_excess_memory = excess_memory;
				requesting_process->memory_node.memory.memory_start = free_block_start;

				assert(best_excess_memory >= 0);
				assert(best_excess_memory < MEMORY_CAPACITY_MB);
			}
		}

		if (excess_memory == 0) { break; }
		if (!*position) { break; }
		
		free_block_start = (*position)->memory.memory_start + 
											 (*position)->memory.memory_requirement;
		position = &(*position)->next;
	}

	if (best_position) { // if there is a slot of memory big enough

		// insert into list
		list_insert_into_next((list_t *) &memory_ll, (node_t **) best_position, (node_t *) &requesting_process->memory_node);

		// assert in list
		assert(is_node_in_list((list_t *) &memory_ll, (node_t *) &requesting_process->memory_node));

		return true;
	}

	return false;
}