#ifndef CSV

	#define CSV

	#define MAX_FIELD_CHARS 128
	#define MAX_LINE_CHARS 512

	#include "linked_list.h"
	#include <stdio.h>

	void getComma(FILE *fptr);
	char *csv_scan_string(FILE *fptr);
	list_t *read_csv(char data_file_path[], void *(*scan_line)(FILE *));



#endif
