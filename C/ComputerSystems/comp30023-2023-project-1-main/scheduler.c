#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>


#include "list.h"
#include "print.h"
#include "global.h"
#include "help.h"
#include "memory.h"
#include "real_process.h"




void cycle_RR() {


    if (running_process && ready_q.len != 0) {
        
        #ifdef IMPLEMENTS_REAL_PROCESS
        suspend_process(); // Task 4 Suspend process
        #endif
    }

    if (running_process) { // put to back of queue

        push((list_t *) &ready_q, node_from_data(running_process));
        running_process = NULL;
    }
    

    if (ready_q.len) { // if there are any ready processes
    
        running_process = &((process_node_t *) return_front((list_t *) &ready_q))->process;

        #ifdef IMPLEMENTS_REAL_PROCESS

        bool is_new_process = (running_process->pid == -1);
        if (is_new_process) { create_process(); } // Task 4 Create process
        else {  continue_process(); }             // Task 4 Continue process

        #endif
    }

    print_running();
}



// credit https://stackoverflow.com/questions/37373421/lexicographic-order-in-c
int lexographic_cmp(const char *a, const char *b) {
    for (; *a && *b && *a == *b; ++a, ++b)
        ;

    return *b - *a;
}



int SJF_cmp(const void * a, const void * b) {

    // service_time

    uint_t time_a = (*(process_node_t **) a)->process.service_time;
    uint_t time_b = (*(process_node_t **) b)->process.service_time;

    if (time_a != time_b) { return time_a - time_b; }

    // arrival - if not in place

    uint_t arrival_a = (*(process_node_t **) a)->process.time_arrived;
    uint_t arrival_b = (*(process_node_t **) b)->process.time_arrived;

    if (arrival_a != arrival_b) { return arrival_a - arrival_b; }

    // lexicographical

    char *name1 = (*(process_node_t **) a)->process.process_name;
    char *name2 = (*(process_node_t **) b)->process.process_name;

    return lexographic_cmp(name1, name2);    
}


void cycle_SJF() {

    if (running_process) {
        #ifdef IMPLEMENTS_REAL_PROCESS
        continue_process();
        #endif
    }

    else if (ready_q.len) {
        sort_list((list_t *) &ready_q, SJF_cmp);

        running_process = &((process_node_t *) return_front(((list_t *) &ready_q)))->process;

        // Task 4 Create process
        #ifdef IMPLEMENTS_REAL_PROCESS
        create_process();
        #endif

        print_running();
    }
}

void process_manager() {

    uint_t num_processes = unarrived_q.len;
    uint_t total_turn_around = 0;
    double total_overhead = 0;
    double max_overhead = - 1;

    if (DEBUG_PRINTS1) { print_all(); }
    
    #ifdef IMPLEMENTS_REAL_PROCESS
    setup_pipes(); 
    #endif

    while (unarrived_q.head || 
           input_q.head || 
           ready_q.head || running_process) {

        if (DEBUG_PRINTS1) { 
            printf("time %2i", cur_time); 
            printf(" unarrived.len %li inupt.len %li ready.len %li ", unarrived_q.len, input_q.len, ready_q.len);
            printf("running_process %i", running_process != NULL);
            if (running_process) {
                printf(" dur %i", running_process->service_time);
            }
            printf("\n");
        }

        // -- 1  Deallocate running process if completed.
        if (running_process && running_process->service_time == 0) { // 
            print_finished();

            char sha[NUM_SHA_BYTES] = "";

            // Task 4 Terminate process
            #ifdef IMPLEMENTS_REAL_PROCESS
            terminate_process(sha);
            #endif

            print_finished_process(sha);

            uint_t turn_around = cur_time - running_process->time_arrived;
            total_turn_around += turn_around;
            assert(turn_around >=0);

            double time_overhead = turn_around / (double) running_process->service_time_unchanged;

            if (total_overhead == 0 || time_overhead > max_overhead) {
                max_overhead = time_overhead;
            }

            total_overhead += time_overhead;

            if (memory_strategy == best_fit) {
                rm_node_wofree((list_t *) &memory_ll, (node_t *) &running_process->memory_node);
            }
            free_without_null(node_from_data(running_process));
            running_process = NULL;
        }

        // -- 2  Add all processes submitted since the  last cycle

        while (unarrived_q.head && 
               unarrived_q.head->process.time_arrived <= cur_time) {

            if (memory_strategy == best_fit) { push((list_t *) &input_q, return_front((list_t *) &unarrived_q)); }
            if (memory_strategy == infinite) { push((list_t *) &ready_q, return_front((list_t *) &unarrived_q)); } // push directly to ready
        }

        // -- 3  Allocate memory

        if (memory_strategy == best_fit) {

            process_node_t *cur_node = input_q.head;
            while (cur_node) {

                if (request_memory(&cur_node->process)) {
                    push((list_t *) &ready_q, return_front((list_t *) &input_q));
                    print_ready(&cur_node->process);
                }

                cur_node = cur_node->next;
            }
        }
        

        // -- 4   Pick and run process for a quantum
       
        if (ready_q.head) { // if there are any ready processes

            if (scheduler == SJF) { cycle_SJF(); }
            if (scheduler == RR) { cycle_RR(); }
        } 
        
        if (running_process) {

            if (running_process->service_time < quantum) {
                running_process->service_time = 0;
            } else {
                running_process->service_time -= quantum;
            }
        }
        // sleep(quantum) // run real time
        cur_time += quantum;
        
    }

    cur_time -= quantum;

    printf("Turnaround time %.0lf\n", ceil(total_turn_around / (double) num_processes));
    printf("Time overhead %.2lf %.2lf\n", max_overhead, total_overhead / (double) num_processes);
    printf("Makespan %i\n", cur_time);


    if (DEBUG_PRINTS2) {
        printf("\n\nSTOP %i CONT %i\n\n\n", process_count_SIGTSTP, process_count_SIGCONT);
    }
}