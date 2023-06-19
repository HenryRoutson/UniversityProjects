

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "stage1.h"
#include "stage2.h"
#include "driver.h"

int main(int argc, char *argv[]) {
	/*

	argc: # of strings in argv

	argv[0]: program name
	argv[1]: stage number
	argv[2]: csv data file
	argv[3]: texdt output file

	*/


	assert(argc == 4);

	// assert possible stage values

	assert('1' <= argv[1][0]);
	assert(argv[1][0] <= '2');

	// assert data file ends in .csv

	assert(string_ends_with(argv[2], ".csv"));

	// change file locations to paths

	char data_file_path[MAX_PATH];
	char output_file_path[MAX_PATH];

	getcwd(data_file_path, MAX_PATH);
	getcwd(output_file_path, MAX_PATH);

	strcat(data_file_path, "/");
	strcat(output_file_path, "/");

	strcat(data_file_path, argv[2]);
	strcat(output_file_path, argv[3]);

	// call stage

	switch (argv[1][0]) {
		
		case '1': stage1(data_file_path, output_file_path); break;
		case '2': stage2(data_file_path, output_file_path); break;
		default : return 1; // should never run
	}

	return 0;
}


int string_ends_with(char string[], char end[]) {
	return strcmp(string + (strlen(string) - strlen(end)), end) == 0;
}


