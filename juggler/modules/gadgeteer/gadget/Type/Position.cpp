/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
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
#include <float.h>
#include <Input/vjPosition/vjPosition.h>
#include <Kernel/vjDebug.h>

// Set up the transformation information
bool vjPosition::config(vjConfigChunk *c)
{
  vjDEBUG(vjDBG_ALL,0) << "vjPosition::vjPosition(vjConfigChunk*)" << endl;
  if(!vjInput::config(c))
     return false;

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

      // This makes a rotation matrix that moves a pt in
      // the device's coord system to the vj coord system.
      // ==> world_M_transmitter
    rotMat.makeXYZEuler(xr, yr, zr);

    xformMat.makeTrans(xt, yt, zt);
    xformMat.postMult(rotMat);         // xformMat = T*R
  }

  return true;
}

vjPosition::vjPosition()
{
  vjDEBUG(vjDBG_ALL,0) << "vjPosition::vjPosition()" << endl;
  deviceAbilities = deviceAbilities | DEVICE_POSITION;
  theData = NULL;
  mDataTimes = NULL;
  xformMat.makeIdent();
}

vjPosition::~vjPosition()
{
    if (theData != NULL)
       getMyMemPool()->deallocate((void*)theData);
    if (mDataTimes != NULL)
	delete mDataTimes;
}

