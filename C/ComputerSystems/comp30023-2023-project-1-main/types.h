
#ifndef PROCESS

#define PROCESS

//

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

typedef uint8_t byte_t;

typedef uint32_t uint_t;

#define PROCESS_NAME_NUM_CHARS 9

//


// -----------
// memory list

typedef struct {
	// MB

	uint_t memory_start; // inclusive 
	uint_t memory_requirement; 

} memory_t; // memory block

typedef struct memory_node memory_node_t;
struct memory_node {
	memory_node_t *next;
	memory_t memory;
};

typedef struct {
	memory_node_t *head;
	memory_node_t *tail;
	size_t len;
} memory_list_t;


// -----------
// process list


typedef struct { 
	int pid;
	uint_t time_arrived;
	char process_name[PROCESS_NAME_NUM_CHARS];
	uint_t service_time; // seconds
	uint_t service_time_unchanged; // seconds
	memory_node_t memory_node;

} process_t;

typedef struct process_node process_node_t;
struct process_node {
	process_node_t *next;
	process_t process;
};

typedef struct {
	process_node_t *head;
	process_node_t *tail;
	size_t len;
} process_list_t;

// -----------

typedef enum { 
	SJF, // Shortest job first
	RR   // Round robin
} scheduler_t;                                         // -s scheduler type
typedef enum { infinite, best_fit } memory_strategy_t; // -m memory strategy

#endif
