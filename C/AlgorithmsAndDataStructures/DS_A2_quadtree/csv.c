
/* 
	MODULAR CSV LIBRARY
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "list.h"
#include "PR_QuadTree.h"
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
  assert(malloc_str);
  strcpy(malloc_str, str);
  assert(malloc_str[len] == '\0');
  return malloc_str;
}



void *read_csv(void *structs, void (*add_to)(void *, void *), char csv_file[], void *(*scan_line)(FILE *)) {
  // generic function to read from a csv into data structures

  assert(structs);

  FILE * fptr = fopen(csv_file, "r"); 
  assert(fptr);

  fscanf(fptr, "%*s"); // remove header

  while (fgetc(fptr) == '\n') { // if there is no new line, break

    void *p_data = scan_line(fptr);
    if (p_data == NULL) { break; }
    add_to(structs, p_data);
  }

  assert(fgetc(fptr) == EOF);
  fclose(fptr);

  return structs;
}


list_t *read_csv_to_linkedlist(char csv_file[], void *(*scan_line)(FILE *)) {
  return read_csv(  (void *) create_empty_list(), 
                    (void (*)(void *, void *)) add_to_list, 
                    csv_file,
                    scan_line );
}

array_t *read_csv_to_dynamic_array(char csv_file[], void *(*scan_line)(FILE *)) {
  return read_csv(  (void *) create_empty_array(16), 
                    (void (*)(void *, void *)) add_to_array, 
                    csv_file,
                    scan_line );
}
