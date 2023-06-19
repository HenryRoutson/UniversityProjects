
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "csv.h"
#include "footpath.h"


footpath_t *read_footpath_csv_line(FILE *fptr) {

	footpath_t *footpath = malloc(sizeof(footpath_t));

	int asrt;

	asrt = fscanf(fptr, "%i", &footpath->footpath_id);
	if (asrt == -1) { // if next line is blank, break
		free(footpath);
		return NULL; 
	} 
	assert(fgetc(fptr) == ',');

	footpath->address = csv_scan_string(fptr);
	assert(fgetc(fptr) == ',');

	footpath->clue_sa = csv_scan_string(fptr);
	assert(fgetc(fptr) == ',');

	footpath->asset_type = csv_scan_string(fptr);
	assert(fgetc(fptr) == ',');

	// use scan to be concise, compared to atoi...
	asrt = fscanf(fptr, "%lf,%lf,%lf,%i.0,%i.0,%lf,%lf", 
		&footpath->deltaz,
		&footpath->distance,
		&footpath->grade1in,
		&footpath->mcc_id,
		&footpath->mccid_int,
		&footpath->rlmax,
		&footpath->rlmin);
	assert(asrt);
	assert(fgetc(fptr) == ',');

	footpath->segside = csv_scan_string(fptr);
	assert(fgetc(fptr) == ',');

	asrt = fscanf(fptr, "%i.0,%i.0,%i.0,%lf,%lf,%lf,%lf", 
		&footpath->statusid,
		&footpath->streetid,
		&footpath->street_group,
		&footpath->start_lat,
		&footpath->start_lon,
		&footpath->end_lat,
		&footpath->end_lon);
	assert(asrt);

	footpath_range_tests(footpath);

	return footpath;
}

list_t *read_footpath_csv(char data_file[]) {
	return read_csv(data_file, (void *) read_footpath_csv_line);
}


void footpath_range_tests(footpath_t *footpath) {
	
	assert(11508 <= footpath->footpath_id);
    assert(footpath->footpath_id <= 32663);

    assert(0 <= footpath->deltaz);
    assert(footpath->deltaz <= 10.4);

    assert(0 <= footpath->distance);
    assert(footpath->distance <= 408.72);

    assert(0 <= footpath->grade1in);
    assert(footpath->grade1in <= 8721.7);

    assert(1384068 <= footpath->mcc_id);
    assert(footpath->mcc_id <= 1556956);

    assert(0 <= footpath->mccid_int);
    assert(footpath->mccid_int <= 30722);

    assert(0 <= footpath->rlmax);
    assert(footpath->rlmax <= 49.07);

    assert(0 <= footpath->rlmin);
    assert(footpath->rlmin <= 48.68);
    
    assert(footpath->rlmax >= footpath->rlmin);

    assert(0 <= footpath->statusid);
    assert(footpath->statusid <= 9);

    assert(0 <= footpath->streetid);
    assert(footpath->streetid <= 120083);

    assert(13518 <= footpath->street_group);
    assert(footpath->street_group <= 32663);

    assert(-38 < footpath->start_lat);
    assert(footpath->start_lat < -37);
    
    assert(-38 < footpath->end_lat);
    assert(footpath->end_lat < -37);
    
    assert(144 < footpath->start_lon);
    assert(footpath->start_lon < 146);
    
    assert(144 < footpath->end_lon);
    assert(footpath->end_lon < 146);
}


void freeFootpathStruct(void *pointer) {

	// free strings with malloced memory
	footpath_t *footpath = pointer;
	assert(footpath);

	free(footpath->address);
	free(footpath->clue_sa);
	free(footpath->asset_type);
	free(footpath->segside);

	// free struct itself
	assert(pointer);
	free(pointer);
}

void free_footpath_list(list_t *list) {
	free_list(list, freeFootpathStruct);
}

void print_footpath(footpath_t *footpath, FILE * stream) {
	fprintf(stream, "\n");
	fprintf(stream, ARROW);
	fprintf(stream, FOOTPATH_FORMAT,
		footpath->footpath_id,
		footpath->address,
		footpath->clue_sa,
		footpath->asset_type,
		footpath->deltaz,
		footpath->distance,
		footpath->grade1in,
		footpath->mcc_id,
		footpath->mccid_int,
		footpath->rlmax,
		footpath->rlmin,
		footpath->segside,
		footpath->statusid,
		footpath->streetid,
		footpath->street_group,
		footpath->start_lat,
		footpath->start_lon,
		footpath->end_lat,
		footpath->end_lon);
}


