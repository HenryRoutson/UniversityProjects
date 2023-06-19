
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "PR_QuadTree.h"
#include "footpath.h"
#include "footpath.h"

#define MAX_CHARS 128

void stage3(qtree_t *qtree, FILE *fptr) {

    point2D_t p;
    char buffer[MAX_CHARS];
    while (fgets(buffer, MAX_CHARS, stdin)) {
        fprintf(fptr, "%s", buffer);

        buffer[strlen(buffer) - 1] = '\0'; // remove newline from buffer
        printf("%s -->", buffer);

        sscanf(buffer, "%lf%*c%lf", &p.x, &p.y);

        list_t *list = search_qtree(qtree, &p);
        if (list) {
            node_t *node = list -> head;
            while (node) {
                print_footpath(node -> data, fptr);
                node = node -> next;
            }
            
        }
    }
}











