#ifndef VT_TYPES_H
#define VT_TYPES_H
/*  FILE: vt_types.h  --                                        */
/*                                                              */
/*  useful constants and type declarations not associated with  */
/*  any specific file                                           */
/*                                                              */
/*  ==========================================================  */
/*  --  Copyright (C) 1991,92,93 Virtual Technologies       --  */
/*  ==========================================================  */

#include <vjConfig.h>

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif


/* NOTE: for the following to work do not include this file before <gl.h>  */
/* or <X11/Intrinsic.h>. Also note that <gl.h> should always follow        */
/* <X11/Intrinsic.h> and that if both <gl.h> and <X11/Intrinsic.h> are     */
/* included you must use GL_Boolean if you want to declare a variable that */
/* matches GL's idea of Boolean. Finally, we assume that if this file is   */
/* included along <gl.h> alone then we assume that normally we want to use */
/* our definition of Boolean so you must use GL_Boolean to get the GL kind */

#if !defined(_XtIntrinsic_h) && !defined(__GL_GL_H__)

typedef char Boolean;		/* this is what we like and it matches X's def */

#elif defined(_XtIntrinsic_h)

/* Boolean is already defined the way we like it */

#elif defined(__GL_GL_H__)

/* if we are here the GL has already defined Booleans as longs */
typedef char VT_Boolean;
typedef long GL_Boolean;
#define Boolean VT_Boolean	/* we assume we normally want our Boolean def! */

#endif

#endif				/* VT_TYPES_H */
