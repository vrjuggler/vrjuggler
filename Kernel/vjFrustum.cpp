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

#include <Kernel/vjFrustum.h>

// ---- FRIEND FUNCTIONS ---- //
ostream& operator<<(ostream& out, vjFrustum& _frust)
{
    out << "l: " << _frust[vjFrustum::VJ_LEFT]
	<< "r: " << _frust[vjFrustum::VJ_RIGHT]
	<< "b: " << _frust[vjFrustum::VJ_BOTTOM]
	<< "t: " << _frust[vjFrustum::VJ_TOP]
	<< "n: " << _frust[vjFrustum::VJ_NEAR]
	<< "f: " << _frust[vjFrustum::VJ_FAR];
    out << endl;

    return out;	
}
