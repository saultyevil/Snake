/* ***************************************************************************
 *
 * @file functions.h
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief Header file containing function definitions.
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
int eddington_update (void);
int update_cell_opacities (void);
int find_vertical_tau (void);
int update_cell_temperatures (void);
double eddington_approximation (double T_eff, double tau);

/*
 * grid_set.c
 */

int allocate_1d_grid (void);
int get_grid_params (void);
int init_grid (void);
double density_profile_disk_height (double z);

/*
 * output.c
 */

int init_outfile (void);
int close_outfile (void);
int write_grid (void);

/*
 * read_pars.c
 */

int get_double (char *par_name, double *value);
int get_int (char *par_name, int *value);
int get_string (char *par_name, char *value);
int get_optional_int (char *par_name, int *value);
int input_double (char *par_name, double *value);
int input_int (char *par_name, int *value);
int input_string (char *par_name, char *value);
int find_par_file (char *file_path);
int init_parameter_file (char *par_filepath);
int close_parameter_file (void);

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

int float_compare (double a, double b);
int check_opacity_table (void);
void Exit (int error_code, char *fmt, ...);
int Log (char *fmt, ...);
int Verbose_log (char *fmt, ...);
int Log_error (char *fmt, ...);
int clean_up (void);
