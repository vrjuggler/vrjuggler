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


/////////////////////////////////////////////////////////////////////////
//
// positional proxy class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_POSPROXY_H_
#define _VJ_POSPROXY_H_

#include <vjConfig.h>
#include <math.h>
#include <assert.h>

#include <Input/vjPosition/vjPosition.h>
#include <Input/InputManager/vjProxy.h>
#include <Math/vjMatrix.h>

//-----------------------------------------------------------------------
//: A proxy class to positional devices, used by the vjInputManager.
//
//  A vjPosProxy always points to a positional device and subUnit number,
//  the inputgroup can therefore keep an array of these around and
//  treat them as positional devices which only return a single
//  subDevice's amount of data.  (one vjPOS_DATA)
//
// See also: vjPosition
//------------------------------------------------------------------------
//!PUBLIC_API:
class vjPosProxy : public vjMemory, public vjProxy
{
public:
   vjPosProxy() : m_posPtr(NULL), m_unitNum(-1), etrans(false)
   {;}

   ~vjPosProxy() {}


   //: Update the proxy's copy of the data
   // Copy the device data to local storage, and transform it if necessary
   void updateData() {
      m_posData = *(m_posPtr->getPosData(m_unitNum));
      m_posUpdateTime = *(m_posPtr->getPosUpdateTime(m_unitNum));

      if(etrans)
         transformData();
   }

    //: returns time of last update...
    vjTimeStamp* getUpdateTime () {
	return &m_posUpdateTime;
    }


   //: Set the transform for this vjPosProxy
   // Sets the transformation matrix to
   //    mMatrixTransform = M<sub>trans</sub>.post(M<sub>rot</sub>)
   //! NOTE: This means that to set transform, you specific the translation
   //+       followed by rotation that takes the device from where it physically
   //+       is in space to where you want it to be.
   void setTransform( float xoff, float yoff, float zoff,    // Translate
                      float xrot, float yrot, float zrot);   // Rotate

   //: Set the vjPosProxy to now point to another device and subDevicenumber
   void set(vjPosition* posPtr, int unitNum);

   //: Get the data
   vjMatrix* getData()
   { return &m_posData; }

   //: Return this device's subunit number
   int getUnit()
   { return m_unitNum; }

   //: Return the vjPosition pointer held by this proxy
   vjPosition* getPositionPtr()
   { return m_posPtr; }

   //: Get the transform being using by this proxy
   vjMatrix& getTransform()
   { return m_matrixTransform; }


   //: Transform the data in m_posData
   //! PRE: m_posData needs to have most recent data
   //! POST: m_posData is transformed by the xform matrix
   //+       m_posData = old(m_posData).post(xformMatrix)
   //!NOTE: This moves the wMr to the modifed reciever system wMmr
   //+  where w = world, mr = world of the reciever, and r = reciever
   void transformData()
   { m_posData.postMult(m_matrixTransform); }

   static std::string getChunkType() { return "PosProxy"; }

   bool config(vjConfigChunk* chunk);

   virtual vjInput* getProxiedInputDevice()
   {
      vjInput* ret_val = dynamic_cast<vjInput*>(m_posPtr);
      vjASSERT(ret_val != NULL);
      return ret_val;
   }

private:
   vjMatrix     m_posData;
   vjTimeStamp  m_posUpdateTime;
   vjMatrix     m_matrixTransform;
   vjPosition*  m_posPtr;
   int          m_unitNum;
   bool         etrans;
};

#endif
