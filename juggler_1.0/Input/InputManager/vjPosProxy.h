/////////////////////////////////////////////////////////////////////////
// vjPosProxy.h
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
class vjPosProxy : public vjMemory
{
public:
   vjPosProxy(vjPosition* posPtr, int unitNum)  {
      assert( posPtr->FDeviceSupport(DEVICE_POSITION) );
      m_posPtr = posPtr;
      m_unitNum = unitNum;
      etrans = false;            // Default to no transformation
   }

   ~vjPosProxy() {}



   //: Update the proxy's copy of the data
   // Copy the device data to local storage, and transform it if necessary
   void UpdateData() {
      m_posData = *(m_posPtr->GetPosData(m_unitNum));

      if(etrans)
         TransformData();
   }

   //: Set the transform for this vjPosProxy
   // Sets the transformation matrix to
   //    mMatrixTransform = M<sub>trans</sub>.post(M<sub>rot</sub>)
   //! NOTE: This means that to set transform, you specific the translation
   //+       followed by rotation that takes the device from where it physically
   //+       is in space to where you want it to be.
   void SetTransform( float xoff, float yoff, float zoff,    // Translate
                      float xrot, float yrot, float zrot)   // Rotate
   {
      etrans = true;
      if((xoff != 0.0f) || (yoff != 0.0f) || (zoff != 0.0f))
         m_matrixTransform.makeTrans(xoff, yoff, zoff);
      if((xrot != 0.0f) || (yrot != 0.0f) || (xrot != 0.0f))
         m_matrixTransform.postXYZEuler(m_matrixTransform, xrot, yrot, zrot);
   }

   //: Set the vjPosProxy to now point to another device and subDevicenumber
   void Set(vjPosition* posPtr, int unitNum, int useTransform = 0)
   {
      assert( posPtr->FDeviceSupport(DEVICE_POSITION) );
      vjDEBUG(0) << "posPtr: " << posPtr << endl
                 << "unit  : " << unitNum << endl << endl << vjDEBUG_FLUSH;
      m_posPtr = posPtr;
      m_unitNum = unitNum;
      etrans = useTransform;
   }

   //: Get the data
   vjMatrix* GetData()
   { return &m_posData; }

   //: Return this device's subunit number
   int GetUnit()
   { return m_unitNum; }

   //: Return the vjPosition pointer held by this proxy
   vjPosition* GetPositionPtr()
   { return m_posPtr; }

   //: Get the transform being using by this proxy
   vjMatrix& GetTransform()
   { return m_matrixTransform; }


   //: Transform the data in m_posData
   //! PRE: m_posData needs to have most recent data
   //! POST: m_posData is transformed by the xform matrix
   //+       m_posData = old(m_posData).post(xformMatrix)
   //!NOTE: This moves the position data by the amount
   //+      specified in SetTransform
   void TransformData()
   { m_posData.postMult(m_matrixTransform); }

private:
   vjMatrix     m_posData;
   vjMatrix     m_matrixTransform;
   vjPosition*  m_posPtr;
   int          m_unitNum;
   bool         etrans;
};

#endif
