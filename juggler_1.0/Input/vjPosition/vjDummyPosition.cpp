
// vjDummyPosition - a dummy child of vjPosition.

#include <vjConfig.h>
#include <Input/vjPosition/vjDummyPosition.h>
#include <Math/vjCoord.h>
#include <Math/vjMatrix.h>

bool vjDummyPosition::config(vjConfigChunk *c)
{
   if(!vjPosition::config(c))
      return false;

   vjCoord coord;

   vjDEBUG(vjDBG_INPUT_MGR,3) << "  vjDummyPosition::config(vjConfigChunk*) " << endl << vjDEBUG_FLUSH;

   coord.pos[VJ_X] = c->getProperty("position",0);
   coord.pos[VJ_Y] = c->getProperty("position",1);
   coord.pos[VJ_Z] = c->getProperty("position",2);
   coord.orient[VJ_X] = c->getProperty("ori",0);
   coord.orient[VJ_Y] = c->getProperty("ori",1);
   coord.orient[VJ_Z] = c->getProperty("ori",2);

   mydata = vjMatrix(coord);
   mTimeStamp.set();

   active = 1;

   return true;
}


vjDummyPosition::~vjDummyPosition() {

}

int vjDummyPosition::startSampling() {
    return 1;
}

int vjDummyPosition::stopSampling() {
    return 1;
}


vjMatrix* vjDummyPosition::getPosData (int d) {
    return (&mydata);
}


vjTimeStamp* vjDummyPosition::getPosUpdateTime(int d) {
    return &mTimeStamp;
}


void vjDummyPosition::updateData () {

}

int vjDummyPosition::sample() {
    return 1;
}
