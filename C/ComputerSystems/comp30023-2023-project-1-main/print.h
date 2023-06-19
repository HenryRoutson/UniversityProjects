
#ifndef PRINT
#define PRINT

#include "types.h"
#include "list.h"

void print_input();
void print_ready(process_t *process);
void print_running();
void print_finished();
void print_finished_process(char *sha);
void print_CLI();
void print_all();

void print_process_list(process_list_t *list);


void print_memory_list();
void print_memory_node(node_t *node);
void print_memory(memory_t *memory);

#endif