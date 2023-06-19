
#include <stdio.h>
#include <assert.h>

#include "list.h"
#include "types.h"
#include "global.h"
#include "help.h"

void print_process_input(process_t *p) {

	// print process with the format from the input files

	if (p) {
			printf("%i,%s,%i,%i\n", p->time_arrived, p->process_name, p->service_time, p->memory_node.memory.memory_requirement);
	} else {
		if (DEBUG_PRINTS1) {
			printf("No process\n");	
		}
	}
}

void print_ready(process_t *process) {
	printf("%u,READY,process_name=%s,assigned_at=%u\n", cur_time,  process->process_name, process->memory_node.memory.memory_start);
}

void print_running() {
	if (running_process) {
		printf("%u,RUNNING,process_name=%s,remaining_time=%i\n", cur_time,  running_process->process_name, running_process->service_time);
	} else {
		if (DEBUG_PRINTS1) { printf("NO RUNNING\n"); }
	}
}

void print_finished() {
	if (running_process) {
		printf("%u,FINISHED,process_name=%s,proc_remaining=%li\n", cur_time,  running_process->process_name, ready_q.len + input_q.len);
	} else {
		if (DEBUG_PRINTS1) { printf("NO FINISHED\n"); }
	}
}

void print_finished_process(char *sha) {
	if (running_process) {
		printf("%u,FINISHED-PROCESS,process_name=%s,sha=%s\n", cur_time,  running_process->process_name, sha);
	} else {
		if (DEBUG_PRINTS1) { printf("NO FINISHED PROCESS\n"); }
	}
}

void print_CLI() {
	printf("\n");
	printf("-s %i\n", scheduler);
	printf("-m %i\n", memory_strategy);
	printf("-q %i\n", quantum);
	printf("\n");
}



void print_nodes(node_t *node, void (* print_node)(node_t *)) {
  while (node) {
    print_node(node);
		// printf("%p -> %p\n", node, node->next);
    node = node -> next;
  }
}

void print_list(list_t* list, void (* print_node)(node_t *)) { 

	if (list->len == 0) { printf(" EMPTY LIST\n\n"); return; }

	printf("\nlist start\n");
  assert(list);
  print_nodes(list->head, print_node); 
	printf("list end\n\n");
}

//
void print_process_node(node_t *node) { print_process_input(&((process_node_t *) node)->process); }
void print_process_list(process_list_t *list) { printf("process list");  print_list((list_t *) list, print_process_node); }

//
void print_memory(memory_t *memory) { printf("START %u    LENGTH  %u\n" , memory->memory_start, memory->memory_requirement); }
void print_memory_node(node_t *node) { print_memory(&((memory_node_t *) node)->memory); }
void print_memory_list() { printf("memory list"); print_list((list_t *) &memory_ll, print_memory_node); }



void print_all() {
	printf("all_start ----\n");
	printf("cur_time %i\n", cur_time);

	printf("unarrived_q <<<\n");
	print_process_list(&unarrived_q);

	printf("input_q <<<\n");
	print_process_list(&input_q);

	printf("ready_q <<<\n");
	print_process_list(&ready_q);

	printf("running_process <<<<<\n");
	print_process_input(running_process);

	printf("all_end ----\n\n\n");
}


