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
                      float xrot, float yrot, float zrot);   // Rotate

   //: Set the vjPosProxy to now point to another device and subDevicenumber
   void Set(vjPosition* posPtr, int unitNum, int useTransform = 0);

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

   static string getChunkType() { return "PosProxy"; }

   bool config(vjConfigChunk* chunk);

private:
   vjMatrix     m_posData;
   vjMatrix     m_matrixTransform;
   vjPosition*  m_posPtr;
   int          m_unitNum;
   bool         etrans;
};

#endif
