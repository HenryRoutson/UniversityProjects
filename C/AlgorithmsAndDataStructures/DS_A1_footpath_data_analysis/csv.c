
/* 
	MODULAR CSV LIBRARY
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "linked_list.h"
#include "csv.h"

void getComma(FILE *fptr) {
  char comma = fgetc(fptr); 
  assert(comma == ',');
}

char *csv_scan_string(FILE *fptr) {

  char str[MAX_FIELD_CHARS];

  // scanf for string with quotes
  if (fscanf(fptr, "\"%[^\"]s", str)) {
    char c = fgetc(fptr);
    assert(c == '"');

  // scanf for string without quotes
  } else if (fscanf(fptr, "%[^,]s", str)) {
    // (for interpretability)

  } else {

    // blank string
    char *empty = malloc(sizeof(char));
    assert(empty);
    *empty = '\0';
    return empty;
  }

  // if string is found
  int len = strlen(str);
  assert(len <= MAX_FIELD_CHARS);
  assert(len != 0);

  // malloc exact size
  char *malloc_str = malloc((len + 1) * sizeof(char)); // include '\0'
  strcpy(malloc_str, str);
  assert(malloc_str[len] == '\0');
  return malloc_str;
}


list_t *read_csv(char data_file_path[], void *(*scan_line)(FILE *)) {
  /*
  generic function to read data from a csv into a linked list
  */

  list_t *list = create_empty_list();
  
  FILE * fptr = fopen(data_file_path, "r"); 
  assert(fptr);

  fscanf(fptr, "%*s"); // remove header

  while (fgetc(fptr) == '\n') { // if there is no new line, break

    void *p_data = scan_line(fptr);
    if (p_data == NULL) { break; }
    add_to_list(list, p_data);
  }

  assert(fgetc(fptr) == EOF);
  fclose(fptr);
  fptr = NULL;

  return list;
}


