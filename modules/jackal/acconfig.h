/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _JCCL_DEFINES_H_
#define _JCCL_DEFINES_H_

/*
 * --------------------------------------------------------------------------
 * Library-wide configuration file.
 * Generated from acconfig.h.
 * --------------------------------------------------------------------------
 */

@TOP@

/* Define all of the following if building on IRIX or darwin. */
#undef _BSD_TYPES

/* Define all of the following if building on HP-UX. */
#undef JCCL_HPUX_VERSION
#undef _INCLUDE_TERMIO
#undef _CMA_NOWRAPPERS_

/* Define if building on a Win32 platform. */
#undef WIN32
#undef _MBCS

/* Define to unique values used for defining JCCL_PERFORMANCE. */
#undef JCCL_PERF_NONE
#undef JCCL_PERF_SGI
#undef JCCL_PERF_POSIX

/* Define to one of the above values to enable performance testing. */
#undef JCCL_PERFORMANCE

/* Define to `int' if the socklen_t type is not available.  */
#undef socklen_t

/* Define if you have the sinf function.  */
#undef HAVE_SINF

/* Define if you have the cosf function.  */
#undef HAVE_COSF

/* Define if you have the sqrtf function.  */
#undef HAVE_SQRTF

/* Define if you have the asinf function.  */
#undef HAVE_ASINF

/* Define if you have the acosf function.  */
#undef HAVE_ACOSF

/* Define if you have the atan2f function.  */
#undef HAVE_ATAN2F

/* Define if you have the atan2f function.  */
#undef HAVE_FABSF

@BOTTOM@

#endif	/* _JCCL_DEFINES_H_ */
