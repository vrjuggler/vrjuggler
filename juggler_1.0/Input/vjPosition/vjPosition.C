#include <config.h>
#include <float.h>
#include <Input/vjPosition/vjPosition.h>

vjPosition::vjPosition(vjConfigChunk *c) : vjInput(c) , xformMat()
{
  cerr << "    vjPosition::vjPosition(vjConfigChunk*)" << endl;
  port_id = -1;
  active = 0;
  theData = NULL;

  if ((c->getNum("translate") == 3) && (c->getNum("rotate") == 3))
  {
    // These are the transforms from the base tracker coord system

    float xt, yt, zt;
    xt = c->getProperty("translate",0);
    yt = c->getProperty("translate",1);
    zt = c->getProperty("translate",2);


    float xr, yr, zr;
    xr = c->getProperty("rotate",0);
    yr = c->getProperty("rotate",1);
    zr = c->getProperty("rotate",2);

      // This makes a rotation matrix that moves the device's coord system
      // to the vj coord system.
    rotMat.makeXYZEuler(xr, yr, zr);


    xformMat.makeTrans(xt, yt, zt);
    xformMat.postMult(rotMat);         // xformMat = T*R
  }

  deviceAbilities = deviceAbilities | DEVICE_POSITION;

}

vjPosition::vjPosition()
{
  cerr << "    vjPosition::vjPosition()" << endl;
  deviceAbilities = deviceAbilities | DEVICE_POSITION;
  theData = NULL;
}

vjPosition::~vjPosition()
{
    if (theData != NULL)
       getMyMemPool()->deallocate((void*)theData);
}

