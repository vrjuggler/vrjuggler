/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>

#include <Kernel/vjFrustum.h>

// ---- FRIEND FUNCTIONS ---- //
ostream& operator<<(ostream& out, vjFrustum& _frust)
{
    out << "l: " << _frust[vjFrustum::LEFT]
	<< "r: " << _frust[vjFrustum::RIGHT]
	<< "b: " << _frust[vjFrustum::BOTTOM]
	<< "t: " << _frust[vjFrustum::TOP]
	<< "n: " << _frust[vjFrustum::NEAR]
	<< "f: " << _frust[vjFrustum::FAR];
    out << endl;

    return out;	
}
