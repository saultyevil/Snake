/* ***************************************************************************
 *
 * @file
 *
 * @author
 *
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

/*
 * convergence.c
 */

int check_cell_convergence (void);
double report_convergence (void);

/*
 * default_pars.c
 */

int init_default_pars (void);

/*
 * eddington.c
 */

int eddington_iterations (void);
int get_rosseland_opac (void);

/*
 * grid_set.c
 */

int allocate_1d_grid (void);
int get_grid_params (void);
int init_grid (void);


/*
 * output.c
 */

int init_filenames (void);
int write_grid_to_file (void);

/*
 * read_pars.c
 */

int get_double (char *par_name, double *value);
int get_int (char *par_name, int *value);
int get_string (char *par_name, char *value);
int get_optional_int (char *par_name, int *value);
int get_optional_double (char *par_name, double *value);
int input_double (char *par_name, double *value);
int input_int (char *par_name, int *value);
int input_string (char *par_name, char *value);
int find_par_file (char *file_path);
int init_parameter_file (char *par_filepath);

/*
 * setup.c
 */

int init_geo (void);


/*
 * time.c
 */

int print_time_date (void);
struct timespec get_time (void);
int print_duration (struct timespec start_time, char *message);

/*
 * utility.c
 */

int check_opacity_table (void);
void Exit (int error_code, char *fmt, ...);
int Log (char *fmt, ...);
int Log_verbose (char *fmt, ...);
int Log_error (char *fmt, ...);

