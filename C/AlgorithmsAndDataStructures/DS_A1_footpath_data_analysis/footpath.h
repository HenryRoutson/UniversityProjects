
#ifndef FOOTPATH
	#define FOOTPATH

	#define FOOTPATH_FORMAT "footpath_id: %i || address: %s || clue_sa: %s || asset_type: %s || deltaz: %.2lf || distance: %.2lf || grade1in: %.1lf || mcc_id: %i || mccid_int: %i || rlmax: %.2lf || rlmin: %.2lf || segside: %s || statusid: %i || streetid: %i || street_group: %i || start_lat: %lf || start_lon: %lf || end_lat: %lf || end_lon: %lf || "
	#define ARROW "--> "
	
	typedef struct {  
		int footpath_id;
		char *address;
		char *clue_sa;
		char *asset_type;
		double deltaz;
		double distance;
		double grade1in;
		int mcc_id;
		int mccid_int;
		double rlmax;
		double rlmin;
		char *segside;
		int statusid;
		int streetid;
		int street_group;
		double start_lat;
		double start_lon;
		double end_lat;
		double end_lon;
		
	} footpath_t;

	footpath_t *read_footpath_csv_line(FILE *fptr);
	void footpath_range_tests(footpath_t *footpath);
	list_t *read_footpath_csv(char data_file[]);
	void free_footpath_list(list_t *list);
	void print_footpath(footpath_t *footpath, FILE * stream);
	void freeFootpathStruct(void *pointer);

#endif

