#include <config.h>
#include <iostream.h>

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
