
// vjDummyPosition - a dummy child of vjPosition.

#include <vjConfig.h>
#include <Input/vjPosition/vjDummyPosition.h>
#include <Math/vjCoord.h>
#include <Math/vjMatrix.h>

vjDummyPosition::vjDummyPosition(vjConfigChunk *c) : vjPosition(c), vjInput(c) 
{
   vjCoord coord;

   cout << "  vjDummyPosition::vjDummyPosition(vjConfigChunk*) " << endl;
   coord.pos[VJ_X] = c->getProperty("position",0);
   coord.pos[VJ_Y] = c->getProperty("position",1);
   coord.pos[VJ_Z] = c->getProperty("position",2);
   coord.orient[VJ_X] = c->getProperty("ori",0);
   coord.orient[VJ_Y] = c->getProperty("ori",1);
   coord.orient[VJ_Z] = c->getProperty("ori",2);
   
   mydata = vjMatrix(coord);

   active = 1;
}


vjDummyPosition::~vjDummyPosition() {
    
}

int vjDummyPosition::StartSampling() {
    return 1;
}

int vjDummyPosition::StopSampling() {
    return 1;
}


vjMatrix* vjDummyPosition::GetPosData (int d) {
    return (&mydata);
}

void vjDummyPosition::UpdateData () {
    
}

int vjDummyPosition::Sample() {
    return 1;
}
