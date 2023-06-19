#ifndef GLOBAL
#define GLOBAL

#include <stdbool.h>

#include "types.h"
#include "list.h"



extern char *file_name;

extern uint_t cur_time;

extern scheduler_t scheduler;
extern memory_strategy_t memory_strategy;
extern uint_t quantum;

extern process_list_t unarrived_q;
extern process_list_t input_q;
extern process_list_t ready_q;

extern memory_list_t memory_ll;

extern process_t *running_process;

//------- debugging variables -------

extern uint_t process_count__START_;
extern uint_t process_count_SIGCONT;
extern uint_t process_count_SIGTSTP;
extern uint_t process_count_SIGTERM;

// -------

// define IMPLEMENTS_REAL_PROCESS

extern int to_process_pipe[2];
extern int from_process_pipe[2];


#define DEBUG_PRINTS1 false
#define DEBUG_PRINTS2 false

#define MEMORY_CAPACITY_MB 2048
#define UNINITIALISED_INT MEMORY_CAPACITY_MB + 1
#define BYTE_SIZE 8 // num bits

#define NUM_SHA_BYTES 64 + 1 // 1 for new line '\n'

#define CHILD_PID 0

#define FILE_READ_INDX 0
#define FILE_WRITE_INDX 1

#endif
