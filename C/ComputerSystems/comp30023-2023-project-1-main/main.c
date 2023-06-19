
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "list.h"
#include "help.h"
#include "types.h"
#include "print.h"
#include "scheduler.h"
#include "inputs.h"
#include "global.h"
#include "real_process.h"



/*
PLEASE NOTE

malloc is automatically asserted by an overwriting function in help.c
freed variables are automatically Nulled by an overwriting function in help.c

*/


// init global variables --------

char *file_name = NULL;
uint_t cur_time = 0;

scheduler_t scheduler       = -1;
memory_strategy_t memory_strategy = -1;
uint_t quantum         = -1;

process_list_t unarrived_q = LIST_INIT;
process_list_t input_q = LIST_INIT;
process_list_t ready_q = LIST_INIT;

memory_list_t memory_ll = LIST_INIT;

process_t *running_process = NULL; 

int to_process_pipe[2] = {-1, -1};
int from_process_pipe[2] = {-1, -1};

// 

uint_t process_count__START_ = 0;
uint_t process_count_SIGCONT = 0;
uint_t process_count_SIGTSTP = 0;
uint_t process_count_SIGTERM = 0;

//


int main(int argc, char* argv[]) {

    input_CLI(argc, argv);
    input_FILE();
    process_manager();

    assert(unarrived_q.len == 0);
    assert(input_q.len == 0);
    assert(ready_q.len == 0);
    assert(memory_ll.len == 0);
    assert(process_count__START_ == 
           process_count_SIGTERM);


	return 0; 
}
