/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


///////////////////////////////////////////////////////////////////////////
// Contains: class prototypes and data types for logiclass.c++
//     (based on Logitech's "logidrvr.h")
//
// Author:   Andy Himberger
//     Allen Bierbaum
//     (original) Terry Fong <terry@ptolemy.arc.nasa.gov>
// History:  27-Mar-92  original version
//     25-Jan-95  adapted to class structure by Allen
//     09-Mar-95  Added a device baseclass -- Allen
//          30-Apr-97   adapted for use in the C2
//
// Copyright (C) 1992, National Aeronautics and Space Administration
// NASA Ames Research Center, Moffett Field, CA 94035
//////////////////////////////////////////////////////////////////////////

#ifndef _LOGICLASS_H_
#define _LOGICLASS_H_

#include <gadget/Devices/DriverConfig.h>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>
#include <gmtl/Vec.h>
#include <vector>

//#include <Inventor/SbLinear.h> // For the vec classes

#ifndef TRUE
#define TRUE         1
#endif

#ifndef FALSE
#define FALSE        0
#endif

#define DIAGNOSTIC_SIZE    2
#define EULER_RECORD_SIZE  16

#define logitech_FLAGBIT        0x80
#define logitech_FRINGEBIT      0x40
#define logitech_OUTOFRANGEBIT  0x20
#define logitech_RESERVED       0x10
#define logitech_SUSPENDBUTTON  0x08
#define logitech_LEFTBUTTON     0x04
#define logitech_MIDDLEBUTTON   0x02
#define logitech_RIGHTBUTTON    0x01


namespace gadget
{

/** ThreeDMouse a positional device driver for the Logitech ThreeD mouse.
*
*  This is a vj hack of Allen's standalone driver for the ThreeD mouse.
*
*  Digital driver support could/should be added in the future.
*/
//class ThreeDMouse : public gadget::Input, public gadget::Position
class ThreeDMouse
   : public input_position_t
{
public:
   /** Default constructor. */
   ThreeDMouse() : mExitFlag(false)
   {
   }

   virtual ~ThreeDMouse()
   {
      stopSampling();
   }

   virtual bool config(jccl::ConfigElementPtr e);
   void controlLoop();

   /** Input pure virtual functions **/
   bool startSampling();
   bool stopSampling();
   void updateData();
   bool sample()
   {
      std::vector<gadget::PositionData> cur_samples(1);
      getRecord(&cur_samples[0]);

      addPositionSample(cur_samples);

      return 1;
   }

   static std::string getElementType();

   /** @name Internal functions from original implementation
    *
    *  not to be used on the outside
    */
   //@{
   int openMouse(const std::string& portName);
   int closeMouse();

   void cuDemandReporting();
   void cuEulerMode();
   void cuHeadtrackerMode();
   void cuMouseMode();
   void cuRequestDiagnostics();

   void cuRequestReport()     // Inline, most called function in package
   {
      // Demand a single report
      write(mouseFD, "*d", 2);
   }

   void cuResetControlUnit();

   void getDiagnostics(unsigned char data[]);
   int  getRecord(gadget::PositionData *data);
   void resetControlUnit();

   //void setBaseOrigin();
       // PURPOSE: Sets the current mouse X,Y,Z position to be the base origin

   //@}

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }

private:
   int mouseFD;
   bool mExitFlag;

   //gadget::PositionData mData[3];

   gmtl::Vec3f      baseVector; // Used to store the base location tooffset from
            // Originally set to 0,0,0

   std::string    mPortName;

   int  logitechOpen(const std::string& port_name);

   void eulerToAbsolute(vpr::Uint8 record[], gmtl::Matrix44f& data);
   void printBin(char a);
};
}

#endif   /* _LOGICLASS_H_ */
