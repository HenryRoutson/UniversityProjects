
#ifndef REAL_PROCESS
#define REAL_PROCESS


#include "types.h"

void setup_pipes();

void create_process();
void suspend_process();
void terminate_process(char *sha);
void continue_process();


#endif