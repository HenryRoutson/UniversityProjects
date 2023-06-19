
#include <stdio.h>

#include "list.h"
#include "PR_QuadTree.h"
#include "footpath.h"
#include <string.h>

#define MAX_CHARS 1280

void stage4(qtree_t *qtree, FILE *fptr) {

		rectangle2D_t r;
		char buffer[MAX_CHARS];
		while (fgets(buffer, MAX_CHARS, stdin)) {
			fprintf(fptr, "%s", buffer);

			buffer[strlen(buffer) - 1] = '\0'; // remove newline from buffer
			printf("%s -->", buffer);

			sscanf(buffer, "%Lf%*c%Lf%*c%Lf%*c%Lf", &r.min_x, &r.min_y, &r.max_x, &r.max_y);

			print_footpaths_in_rectangle(qtree, &r, fptr);
	}
}
