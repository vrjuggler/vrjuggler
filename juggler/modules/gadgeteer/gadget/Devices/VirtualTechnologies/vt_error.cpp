/*  FILE: vt_error.c  --                                        */
/*                                                              */
/*  error setting and notification routines                     */
/*                                                              */
/*  ==========================================================  */
/*  --  Copyright (C) 1993 Virtual Technologies             --  */
/*  --                                                      --  */
/*  --    Author: Larry Edwards                             --  */
/*  ==========================================================  */

#include <vrj/vjConfig.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>

#include <vrj/Input/Devices/VirtualTechnologies/vt_globals.h>
#include <vrj/Input/Devices/VirtualTechnologies/vt_error.h>

static int new_error = FALSE;
static char routine_with_error[256];
static FILE *errorfp = stderr;

static char *error_messages[] =
{
  "",
  "Unix system error. Error message is printed by perror()",
  "Serial IO error.",
  "baud rate not supported.",
  "Attempt to open more than the available number of serial ports.",
  "Serial port already in use.",
  "Serial port write did not write requested number of bytes.",
  "Serial port read timed out before reading expected number of bytes.",
  "vjCyberGlove error.",
  "CGIU is not responding.",
  "vjCyberGlove is not initialized properly.",
  "vjCyberGlove is not connected to the Interface Unit.",
  "vjCyberGlove has a corrupt EEPROM.",
  "CGIU not responding or returned invalid response.",
  "",
  "",
  "Glove sensor value returned was zero. Possible cause: CGIU not connected.",
  "Glove is not connected and CGIU can not sustain requested sample rate.",
  "CGIU can not sustain requested sample rate.",
  "Wrong head byte encountered when attempting to read sensor values",
  "",
  "Polhemus Error",
  "Unexpected tail bytes, tail should be 0x0D 0x0A",
  "Unexpected head bytes",
  "",
  "Ascension Error",
  "",
  "",
  "",
  "Tracker Error",
  "Unknown setup string keyword",
  "",
  "",
  "Calabration file read error.",
  "Error reading calibration file",
};

void
vt_fatal_error(char *calling_routine)
{
  if (calling_routine[0])
  {
    fprintf(errorfp,"\n\nFATAL ERROR in %s:\n",routine_with_error);
    fprintf(errorfp,"     %s\n",error_messages[vt_error_code]);
    fprintf(errorfp,"=>\tcalled by: %s\n",calling_routine);
  }
  else
  {
    fprintf(errorfp,"\n\nFATAL ERROR in %s:\n",routine_with_error);
    fprintf(errorfp,"     %s\n",error_messages[vt_error_code]);
  }
  fflush(errorfp);

  exit(1);
}

int
vt_print_error(char *calling_routine)
{
  if (new_error)
  {
    if (vt_error_level > 0)
    {
      fprintf(errorfp,"\n\nERROR in %s:\n",routine_with_error);
      fprintf(errorfp,"     %s\n",error_messages[vt_error_code]);
      if (vt_error_level > 1)
	fprintf(errorfp,"=>\tcalled by: %s\n",calling_routine);
      fflush(errorfp);
    }
  }
  else
  {
    if (vt_error_level > 1)
    {
      fprintf(errorfp,"=>\tcalled by: %s\n",calling_routine);
      fflush(errorfp);
    }
  }
  new_error = FALSE;
  /* we always return !Ok to simplify calling code ... can be ignored */
  return (!Ok);
}

void
vt_fatal_unix_error(char *calling_routine, char *unix_sys_routine)
{
  static char failed_routine[256];

  sprintf(failed_routine,"\n\nFATAL UNIX SYSTEM ERROR in %s",unix_sys_routine);
  fprintf(errorfp,"%s: %s\n",failed_routine,strerror(errno));
  fprintf(errorfp,"=>\tcalled by: %s\n",calling_routine);
  fflush(errorfp);
  exit(1);
}

int
vt_print_unix_error(char *calling_routine, char *unix_sys_routine)
{
  static char failed_routine[256];

  vt_error_code = UNIX_ERROR;

  if (vt_error_level > 0)
  {
    sprintf(failed_routine,"\n\nUNIX SYSTEM ERROR in %s",unix_sys_routine);
    fprintf(errorfp,"%s: %s\n",failed_routine,strerror(errno));
    fflush(errorfp);
  }
  if (vt_error_level > 1)
  {
    fprintf(errorfp,"=>\tcalled by: %s\n",calling_routine);
    fflush(errorfp);
  }

  new_error = FALSE;
  /* we always return !Ok to simplify calling code ... can be ignored */
  return (!Ok);
}

int
vt_set_error(char *calling_routine, int errorcode)
{
  vt_error_code = errorcode;
  new_error = TRUE;
  if (vt_error_level > 0)
    strcpy(routine_with_error,calling_routine);

  /* we always return !Ok to simplify calling code ... can be ignored */
  return (!Ok);
}
