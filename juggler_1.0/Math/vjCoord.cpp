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
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>
#include <Math/vjCoord.h>
#include <Math/vjMatrix.h>

vjCoord::vjCoord(vjMatrix mat)
{
   mat.getXYZEuler(orient[0], orient[1], orient[2]);
   mat.getTrans(pos[0], pos[1], pos[2]);
}
