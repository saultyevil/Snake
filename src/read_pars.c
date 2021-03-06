/* ***************************************************************************
 *
 * @file read_pars.c
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief Functions related to management of the parameter file and reading
 *        parameters from it.
 *
 * @details
 *
 * ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "snake.h"

FILE *PAR_FILE_PTR;
char par_file[LINE_LEN];

// Prompt the user to input the path to a parameter file
void
find_par_file (char *file_path)
{
  int ierr;
  
  Log ("Please input the file path to the parameter file: ");
  ierr = scanf ("%s", file_path);
  if (ierr == EOF)
    Exit (NO_INPUT, "Nothing entered for parameter file file path\n");
}

// Open up the parameter file
void
init_parameter_file (char *file_path)
{
  strcpy (par_file, file_path);
  if (!(PAR_FILE_PTR = fopen (file_path, "r")))
    Exit (FILE_OPEN_ERR, "Could not find parameter file %s\n", file_path);
  Log (" - Loaded parameter file %s\n\n", file_path);
}

// Close the parameter file
void
close_parameter_file (void)
{
  if (fclose (PAR_FILE_PTR))
    Exit (FILE_CLOSE_ERR, "Couldn't close parameter file %s\n", par_file);
  Log_verbose (" - Closed %s successfully\n", par_file);
}

// Read a double from file
void
get_double (char *par_name, double *value)
{
  int line_num = 0;
  int found_par = FALSE;
  char line[LINE_LEN], ini_par_name[LINE_LEN], par_sep[LINE_LEN], par_value[LINE_LEN];

  rewind (PAR_FILE_PTR);
  
  while (fgets (line, LINE_LEN, PAR_FILE_PTR) != NULL)
  {
    line_num++;
    if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')
      continue;
    if (sscanf (line, "%s %s %s", ini_par_name, par_sep, par_value) != 3)
      Exit (PAR_FILE_SYNTAX_ERR, "Syntax error on line %i in parameter file\n",
            line_num);
    if (strcmp (par_name, ini_par_name) == 0)
    {
      *value = atof (par_value);
      found_par = TRUE;
    }
  }
  
  if (!found_par)
    input_double (par_name, value);
}

// Read an integer from file
void
get_int (char *par_name, int *value)
{
  int line_num = 0;
  int found_par = FALSE;
  char line[LINE_LEN], ini_par_name[LINE_LEN], par_sep[LINE_LEN], par_value[LINE_LEN];

  rewind (PAR_FILE_PTR);
  
  while (fgets (line, LINE_LEN, PAR_FILE_PTR) != NULL)
  {
    line_num++;
    if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')
      continue;
    if (sscanf (line, "%s %s %s", ini_par_name, par_sep, par_value) != 3)
      Exit (PAR_FILE_SYNTAX_ERR, "Syntax error on line %i in parameter file\n",
            line_num);
    if (strcmp (par_name, ini_par_name) == 0)
    {
      *value = atoi (par_value);
      found_par = TRUE;
    }
  }
  
  if (!found_par)
    input_int (par_name, value);
}

// Read a string in from file
void
get_string  (char *par_name, char *value)
{
  int line_num = 0;
  int found_par = FALSE;
  char line[LINE_LEN], ini_par_name[LINE_LEN], par_sep[LINE_LEN], par_value[LINE_LEN];

  rewind (PAR_FILE_PTR);
  
  while (fgets (line, LINE_LEN, PAR_FILE_PTR) != NULL)
  {
    line_num++;
    if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')
      continue;
    if (sscanf (line, "%s %s %s", ini_par_name, par_sep, par_value) != 3)
      Exit (PAR_FILE_SYNTAX_ERR, "Syntax error on line %i in parameter file\n",
            line_num);
    if (strcmp (par_name, ini_par_name) == 0)
    {
      strcpy (value, par_value);
      found_par = TRUE;
    }
  }
  
  if (!found_par)
    input_string (par_name, value);
}

// Get an optional integer from file
void
get_optional_int (char *par_name, int *value)
{
  int line_num = 0;
  char line[LINE_LEN], ini_par_name[LINE_LEN], par_sep[LINE_LEN], par_value[LINE_LEN];

  rewind (PAR_FILE_PTR);

  while (fgets (line, LINE_LEN, PAR_FILE_PTR) != NULL)
  {
    line_num++;
    if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')
      continue;
    if (sscanf (line, "%s %s %s", ini_par_name, par_sep, par_value) != 3)
      Exit (PAR_FILE_SYNTAX_ERR, "Syntax error on line %i in parameter file\n",
            line_num);
    if (strcmp (par_name, ini_par_name) == 0)
      *value = atoi (par_value);
  }
}

// Prompt the user to input a double
void
input_double (char *par_name, double *value)
{
  int ierr;
  char input_value[LINE_LEN];
  
  Log ("%s not found, input value:\n%s (%f): ", par_name, par_name, *value);
  ierr = scanf ("%s", input_value);
  if (ierr == EOF)
    Exit (NO_INPUT, "Nothing entered for input par\n");
  *value = atof (input_value);
}

// Prompt the user to input an int
void
input_int (char *par_name, int *value)
{
  int ierr;
  char input_value[LINE_LEN];
  
  Log ("%s not found, input value:\n%s (%i): ", par_name, par_name, *value);
  ierr = scanf ("%s", input_value);
  if (ierr == EOF)
    Exit (NO_INPUT, "Nothing entered for input par\n");
  *value = atoi (input_value);
}

// Prompt the user to input a string
void
input_string (char *par_name, char *value)
{
  int ierr;
  char input_value[LINE_LEN];
  
  Log ("%s not found, input value: \n%s (%s): ", par_name, par_name, value);
  ierr = scanf ("%s", input_value);
  if (ierr == EOF)
    Exit (NO_INPUT, "Nothing entered for input par\n");
  strcpy (value, input_value);
}

// Check if a parameter exists in the parameter file
int check_for_parameter (char *par_name)
{
  int line_num = 0;
  char line[LINE_LEN], ini_par_name[LINE_LEN], par_sep[LINE_LEN],
    par_value[LINE_LEN];

  rewind (PAR_FILE_PTR);

  while (fgets (line, LINE_LEN, PAR_FILE_PTR) != NULL)
  {
    line_num++;
    if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')
      continue;
    if (sscanf (line, "%s %s %s", ini_par_name, par_sep, par_value) != 3)
      Exit (PAR_FILE_SYNTAX_ERR, "Syntax error on line %i in parameter file\n",
            line_num);
    if (strcmp (par_name, ini_par_name) == 0)
      return (int) strlen (par_name);
  }

  return FAILURE;
}
