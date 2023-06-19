
// code by Henry Routson

#include <stdlib.h>
#include <stdio.h>

void freenull(void **p, char*file_name, int line_number) {
	// always null after free

	if (*p == NULL) { 
		printf("\n	You may be freeing twice\n"); 
		printf("on line %u \nin file %s\n\n", line_number, file_name);
		exit(1);
	}
	free(*p);
	*p = NULL;
}

void free_without_null(void *pointer) {
	free(pointer);
}

void *safe_malloc(unsigned long size, char*file_name, int line_number) {
	// always assert after malloc

	if (size == 0) { 
		printf("\nError: trying to malloc 0 bytes\n");  
		exit(1);
	} 

	if ((long) size < 0) {
		printf("\n	malloc size may be negative, unsigned value was %li\n", (long) size);
		printf("on line %u \nin file %s\n\n", line_number, file_name);
	}

	void *p = malloc(size);
	if (p == NULL) {
		printf("\n\nError: malloc of size %lu failed \n", size);
		printf("on line %u \nin file %s\n\n", line_number, file_name);
		exit(1);
	}
	return p;
}

