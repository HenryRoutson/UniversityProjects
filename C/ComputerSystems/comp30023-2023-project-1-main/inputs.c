

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "global.h"




void input_CLI(int argc, char* argv[]) {
		
		// process cli arguements

    if (argc % 2 != 1) { printf("Number of args needs to be odd"); exit(1); }

    for (int arg = 1; arg < argc; arg += 2) {

        if (argv[arg][0] != '-') { 
            printf("\n\nArguement should start with - not %c\n\n", argv[arg][0]); 
            printf("\n\nArguement: %s\n\n", argv[arg]); 
            exit(1);
        }

        switch (argv[arg][1]) {

            case 'f':

                file_name = argv[arg + 1];
                break;

            case 's':

                if      (strcmp(argv[arg+1], "SJF") == 0) { scheduler = SJF; }
                else if (strcmp(argv[arg+1], "RR") == 0)  { scheduler = RR; }
                else { printf("Not a valid scheduler\n"); exit(1); }
                break;

            case 'm':

                if      (strcmp(argv[arg+1], "infinite") == 0) { memory_strategy = infinite; }
                else if (strcmp(argv[arg+1], "best-fit") == 0) { memory_strategy = best_fit; }
                else { printf("Not a valid memory_strategy\n"); exit(1); }
                break;

            case 'q':
                quantum = atoi(argv[arg + 1]);
                assert(1 <= quantum); 
                assert(quantum <= 3);
                break;

            default: 
                printf("\nNot a valid flag\n");
                exit(1);
        }

    }


    // check all arguements have been given

    assert(file_name != NULL);
    assert(scheduler != -1);
    assert(memory_strategy != -1);
    assert(quantum != -1);

    // process and store process text input
    


}



void input_FILE() {

	FILE *fptr = NULL; 
	fptr = fopen(file_name, "r"); 
	if (!fptr) {
		printf("No such file exists\n%s\n", file_name);
		exit(1);
	}

	while (true) {

        process_node_t *node = (process_node_t *) create_node(sizeof(process_t));
        process_t *p = &node->process;

        int assigned = fscanf(fptr, "%i %s %i %i\n", &p->time_arrived, (char *) &p->process_name, &p->service_time, &p->memory_node.memory.memory_requirement);

        p->service_time_unchanged = p->service_time;
        p->memory_node.next = NULL;
        p->memory_node.memory.memory_start = UNINITIALISED_INT;
        p->pid = -1;

        if (assigned != 4) {
                assert(assigned == -1);
                free(node);
                break;
        }

        push((list_t *) &unarrived_q, (node_t *) node);
	}

    fclose(fptr);
	
}