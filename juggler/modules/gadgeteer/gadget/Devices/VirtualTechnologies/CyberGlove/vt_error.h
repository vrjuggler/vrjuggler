#ifndef VT_ERROR_H
#define VT_ERROR_H
/*  FILE: vt_error.h  --                                        */
/*                                                              */
/*  header for vt_error.c: error setting and notification       */
/*  routines                                                    */
/*                                                              */
/*  ==========================================================  */
/*  --  Copyright (C) 1993 Virtual Technologies             --  */
/*  ==========================================================  */

#include <vjConfig.h>

enum { Ok = 1, NOERROR = 0, UNIX_ERROR,
	 IO_ERROR,IO_ERROR1,IO_ERROR2,IO_ERROR3,IO_ERROR4,IO_ERROR5,
	 CG_ERROR,CG_ERROR1,CG_ERROR2,CG_ERROR3,CG_ERROR4,CG_ERROR5,CG_ERROR6,
	 CG_ERROR7,CG_ERROR8,CG_ERROR9,CG_ERROR10,CG_ERROR11,CG_ERROR12,
	 POLH_ERROR,POLH_ERROR1,POLH_ERROR2,POLH_ERROR3,
	 ASC_ERROR,ASC_ERROR1,ASC_ERROR2,ASC_ERROR3,
	 TR_ERROR,TR_ERROR1,TR_ERROR2,TR_ERROR3,
	 CAL_ERROR,CAL_ERROR1};

extern int vt_set_error(char *routine_name, int errorcode);
extern void vt_fatal_error(char *calling_routine);
extern int vt_print_error(char *calling_routine);
extern void vt_fatal_unix_error(char *calling_routine, char *unix_sys_routine);
extern int vt_print_unix_error(char *calling_routine, char *unix_sys_routine);
extern int vt_set_error(char *calling_routine, int errorcode);

#endif				/* VT_ERROR_H */
