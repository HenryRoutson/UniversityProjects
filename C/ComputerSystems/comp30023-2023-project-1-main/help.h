
#ifndef HELP
#define HELP

// define functions to help with development and debugging

void freenull(void **p, char*file_name, int line_number);

#define free(pointer) freenull((void **) &pointer, __FILE__, __LINE__ ) // comment to disable 

void free_without_null(void *pointer);


void *safe_malloc(unsigned long size, char*file_name, int line_number);
#define malloc(size) safe_malloc(size, __FILE__, __LINE__) // comment to disable

#endif
