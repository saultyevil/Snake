/* ***************************************************************************
 *
 * @file error.c
 *
 * @author E. J. Parkinson
 *
 * @date 12 Dec 2018
 *
 * @brief Definition for errors in the program.
 *
 * @details
 *
 * ************************************************************************** */

enum ERRORS
{
  FILE_OPEN_ERR = 1,
  FILE_CLOSE_ERR,
  MEM_ALLOC_ERR,
  TABLE_BOUNDS,
  UNKNOWN_PARAMETER,
  NO_INPUT,
  PAR_FILE_SYNTAX_ERR,
  FILE_IN_ERR,
  UNKNOWN_MODE,
  INVALID_VALUE,
  INVALID_TABLE,
  NO_LOG_RMO_RETURNED,
  NEGATIVE_OPACITY
};
