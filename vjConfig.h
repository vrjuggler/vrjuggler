/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_CONFIG_H_
#define _VJ_CONFIG_H_
#pragma once

/*
 * ----------------------------------------------------------------------------
 * This file (vjConfig.h) includes header files common to most, if not all,
 * files in the VR Juggler source tree.  It must be included at the top of
 * every .h and .cpp file before any other headers because it includes system
 * headers.
 * ----------------------------------------------------------------------------
 */

/* This should always be included first. */
#ifdef HAVE_CONFIG_H
#include <vjDefines.h>
#endif	/* HAVE_CONFIG_H */

#ifdef _DEBUG
#   define VJ_DEBUG
#else
#   define VJ_OPT
#endif

/* Common includes */
/* Put here for pre-compiled headers */
#ifdef __cplusplus
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
//#include <Kernel/vjDebug.h>
#endif


/* --- Macros ---- */
#define VJ_EPS 1e-8
#define VJ_DEG2RAD(x) ((x)*M_PI/180.0)
#define VJ_RAD2DEG(x) ((x)*180.0/M_PI)
#define VJ_ZERO_CLAMP(x) ((fabs(x) < VJ_EPS)? 0.0f : x)
#define VJ_IS_ZERO(x) (fabs(x) < VJ_EPS)
#define VJ_CLAMP(x,y) ((x>y)? y : x)
#define VJ_MIN2(x,y) ((x>y)? y : x)
#define VJ_MIN3(x,y,z) VJ_MIN2(VJ_MIN2(x,y),z)
#define VJ_MIN4(w,x,y,z) VJ_MIN2(VJ_MIN2(w,x), VJ_MIN2(y,z))
#define VJ_MAX2(x,y) ((x>y)? x : y)
#define VJ_MAX3(x,y,z) VJ_MAX2(VJ_MAX2(x,y),z)
#define VJ_MAX4(w,x,y,z) VJ_MAX2(VJ_MAX2(w,x),VJ_MAX2(y,z))


#endif	/* _VJ_CONFIG_H_ */
