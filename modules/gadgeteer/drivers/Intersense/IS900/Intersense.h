/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

//===============================================================
// Intersense (a Wrapper for IntersenseStandalone)
//===============================================================

#ifndef _GADGET_INTERSENSE_H_
#define _GADGET_INTERSENSE_H_

#include <gadget/Devices/DriverConfig.h>
#include <vector>
#include <vpr/Thread/Thread.h>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/Position.h>
#include <drivers/Intersense/IS900/IntersenseStandalone.h>

// maximum number of digital and analog buttons possible on a IS interface
// box.
#ifndef MAX_NUM_BUTTONS
#define MAX_NUM_BUTTONS 8
#endif

#ifndef MAX_NUM_STATIONS
#define MAX_NUM_STATIONS 8
#endif

#ifndef MAX_ANALOG_CHANNELS
#define MAX_ANALOG_CHANNELS 10
#endif

#define IS_BUTTON_NUM MAX_NUM_BUTTONS*MAX_NUM_STATIONS
#define IS_ANALOG_NUM MAX_ANALOG_CHANNELS*MAX_NUM_STATIONS


namespace gadget
{

struct ISStationConfig
{
   int stationIndex;
   bool enabled;

   int digMin;
   int digNum;
   int anaMin;
   int anaNum;

   bool useDigital;
   bool useAnalog;
};

/**
 * Position derived class for running an IS900 tracking system.
 *
 * Wraps the IntersenseStandalone driver.
 *
 * Intersense is a positional device driver for the Flock of Birds, the config
 * element in the constructor should set up all the settings, for these to be
 * changed the Flock has to be deleted and a new instance created with an
 * updated config element.
 *
 * @note Intersense inherits from Digital and Analog.  These base classes,
 *       however, can not handle multiple receivers in the same way as
 *       gadget::Position.  Therefore, to access buttons 0-3 on station 0,
 *       call for button 0-3. but to access buttons 0-3 on station 1,
 *       you must ask for units 4-7, and so on.
 * @note Some functions still remain for changing the options of
 *       the flock when its not in Sampling mode, but in order to stay
 *       consistent with the gadget::Position functionality these
 *       are only left for building apps without jccl::ConfigElement objects.
 * @note A note on receiver access:
 *   Clients of juggler should access tracker recievers as [0-n]
 *   For example, if you have recievers 1,2, and 4 with transmitter on 3,
 *   then you can access the data, in order, as 0,1,2.
 *
 * See also: Position
 */
//class Intersense : public Input, public Position, public Digital, public Analog
class Intersense
   : public input_digital_analog_position_t
{
protected:
   struct IsenseData
   {
      /** Constructor
       * Init digital with IS_BUTTON_NUM values
       * Init analog with IS_ANALOG_NUM values
       */
      IsenseData()
         : digital(IS_BUTTON_NUM)
         , analog(IS_ANALOG_NUM)
      {;}

      // Helper function to set all the times
      // @todo Replace this with a for_each function call
      // XXX
      void setTime()
      {
         digital[0].setTime();
         for ( std::vector<DigitalData>::iterator d=digital.begin();
               d != digital.end();
               ++d )
         {
            (*d).setTime(digital[0].getTime());
         }

         for ( std::vector<AnalogData>::iterator a=analog.begin();
               a != analog.end();
               ++a )
         {
            (*a).setTime(digital[0].getTime());
         }
      }

      std::vector<DigitalData> digital;
      std::vector<AnalogData>  analog;
   };

public:
   Intersense();
   virtual ~Intersense();

   /** Configures the device with a config element. */
   virtual bool config(jccl::ConfigElementPtr e);

   /** Begins sampling. */
   bool startSampling();

   /** Main thread of control for this active object. */
   void controlLoop();

   /** Stops sampling. */
   bool stopSampling();

   /** Samples data. */
   bool sample();

   /** Updates to the sampled data. */
   void updateData();

   /** Returns what element type is associated with this class. */
   static std::string getElementType();

   /** Gets current data from the receiver.
    *  @arg dev The receiver number.  Clients of juggler should access
    *           tracker receivers as [0-n].  For example, if you have
    *           receivers 1, 2, and 4, with transmitter on 3, then
    *           you can access them as devs 0, 1, and 2.
    *  @return a pointer to the receiver's current PositionData, or NULL
    *          if the device is not active.
    */
   // PositionData* getPositionData (int dev=0);

   /**
    * Gets the digital and analog data.
    * @param d  The button number.
    * @post returns a boolean value where 0 = false and 1 = true
    * @note Since the tracker has multiple possible devices but digital
    *       devices assume only one device, the buttons are layed out as
    *       d = MAX_NUM_BUTTONS*(device number) + (button number)
    *       device number is zero based
    *       button number is zero based
    */
    // ex.  for button number 4 of device 1
    // buttonValue = getDigitalData( 1*MAX_NUM_BUTTONS + 4);
    // Most configurations have the wand on port 2... so the device number is 1
    // The rest button layout is described in the Intersense Manual
    // (this value is set in the juggler config files)
    //
//   DigitalData* getDigitalData(int d = 0);
//   AnalogData* getAnalogData(int d = 0);

   /** Checks if the device is active. */
   bool isActive()
   {
      return mTracker.isActive();
   }

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
    int getStationIndex(int stationNum, int bufferIndex);

    IntersenseStandalone mTracker;

    //Isense_Data mInput[3];

    ISStationConfig* stations;

    //PositionData* mData;

    std::vector<DigitalData> mDigitalData;
    std::vector<AnalogData>  mAnalogData;

// KLUDGE: work around the inherent difference between Position and Digital
// (and Analog)
// Motivation: Positional expects multiple positional devices to be connected
// to the same port and provides a means for accesses each positional device.
// So, if there are four wands there is one tracker unit that allows access to
// each wand...  For digital devices, there may be limitless input, but each
// digital button is attached to the same digital device The tracker with
// digital io uses its natural subset of positional devices to encapsulate
// the digital IO as well. Therefore what is needed with four wands is a
// digital device that allows access to subsets of digital buttons.

    std::string mPortName;
    int         mBaudRate;
    bool        mExitFlag;
};

} // End of gadget namespace

#endif
