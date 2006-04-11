/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_ASCENSION_FLOCK_OF_BIRDS_H_
#define _GADGET_ASCENSION_FLOCK_OF_BIRDS_H_

#include <gadget/Devices/DriverConfig.h>
#include <vpr/Thread/Thread.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/InputMixer.h>
#include <gadget/Devices/Ascension/Flock/FlockStandalone.h>


namespace gadget
{
   class InputManager;
}

extern "C" GADGET_DRIVER_API(void) initDevice(gadget::InputManager* inputMgr);

namespace gadget
{

/**
 * Position derived class for running a Flock of Birds.
 * Also a wrapper class for the real Ascension Flock class "FlockStandalone"
 *
 *  Flock adds to the FlockStandalone class shared memory and threading.<br>
 *  Flock is a positional device driver for the Flock of Birds, the config
 *  chunk in the constructor should set up all the settings, for these to be
 *  changed the Flock has to be deleted and a new instance created with an
 *  updated configchunk.
 *
 * @note Some functions still remain for changing the options of
 *     the flock when its not in Sampling mode, but in order to stay
 *     consistent with the Input/vjPosition functionality these
 *     are only left for building apps without jccl::ConfigChunks
 * @note A note on reciever access:
 *   Clients of juggler should access tracker recievers as [0-n]
 *   For example, if you have recievers 1,2, and 4 with transmitter on 3,
 *   then you can access the data, in order, as 0,1,2.
 *
 * @see gadget::Position
 */
//class Flock : public Input, public Position
class Flock : public InputMixer<Input,Position>
{
public:
   /**
    * Configure Constructor.
    *
    * @param port  such as "/dev/ttyd3"
    * @param baud  such as 38400, 19200, 9600, 14400, etc...
    * @param sync  sync type.
    * @param block  blocking
    * @param numBrds  number of birds in flock
    * @param transmit  transmitter unit number
    * @param hemi  hemisphere to track from
    * @param filt  filtering type
    * @param report  flock report rate
    * @param calfile  a calibration file, if "", then use none
    *
    * @post configures internal data members,
    *          doesn't actually talk to the FOB yet.
    */
   Flock(const char* const port = "/dev/ttyd3",
         const int& baud = 38400,
         const int& sync = 1,
         const bool& block = false,
         const int& numBrds = 3,
         const int& transmit = 3,
         const BIRD_HEMI& hemi = LOWER_HEM,
         const BIRD_FILT& filt = AC_NARROW,
         const char& report = 'R',
         const char* const calfile = "");
   ~Flock();


   /**  configure the flock with a config chunk */
   virtual bool config(jccl::ConfigChunkPtr c);

   /**  begin sampling */
   int startSampling();

   /**  stop sampling */
   int stopSampling();

   /**  sample data */
   int sample();

   /**  update to the sampled data. */
   void updateData();

   /**  return what chunk type is associated with this class. */
   static std::string getChunkType()
   {
      return std::string("Flock");
   }

   /**  see if the flock is active or not */
   inline const bool& isActive() const
   {
      return mFlockOfBirds.isActive();
   }

   /**
    * Sets the port to use.
    * This will be a string in the form of the native OS descriptor.<BR>
    * ex: unix - "/dev/ttyd3", win32 - "COM3"
    *
    * @pre flock.isActive() must be false to use this function
    */
   void setPort( const char* const serialPort );

   /**
    * Gets the port used.
    * This will be a string in the form of the native OS descriptor.<BR>
    * ex: unix - "/dev/ttyd3", win32 - "COM3"
    */
   inline const std::string& getPort() const
   {
      return mFlockOfBirds.getPort();
   }

   /**
    * Sets the baud rate.
    * This is generally 38400.  Consult the Flock manual for other rates.
    * @note flock.isActive() must be false to use this function.
    */
   void setBaudRate( const int& baud );

   /**
    * Gets the baud rate.
    */
   inline const int& getBaudRate() const
   {
      return mFlockOfBirds.getBaudRate();
   }

   /**
    * Sets the unit number of the transmitter.
    * @param Transmit  An integer that is the same as the dip switch
    *          setting on the transmitter box (for the unit number).
    * @note flock.isActive() must be false to use this function
    */
   void setTransmitter( const int& Transmit );

   /**
    * Gets the unit number of the transmitter.
    * @post Returns an integer that is the same as the dip switch
    *       setting on the transmitter box (for the unit number).
    */
   inline const int& getTransmitter() const
   {
      return mFlockOfBirds.getTransmitter();
   }

   /**
    * Sets the number of birds to use in the Flock.
    * @param n  an integer number not more than the number of
    *          birds attached to the system.
    * @note flock.isActive() must be false to use this function.
    */
   void setNumBirds( const int& n );

   /**
    * Gets the number of birds to use in the Flock.
    * @return An integer number not more than the number of
    *         birds attached to the system.
    */
   inline const int&  getNumBirds() const
   {
      return mFlockOfBirds.getNumBirds();
   }

   /**
    * Set the video sync type.  This option allows the Flock to syncronize its
    * pulses with your video display.  This will eliminate most flicker caused
    * by the magnetic distortion.
    *
    * @pre flock.isActive() must be false to use this function.
    * @note Refer to your Flock manual for what number to use.
    */
   void setSync( const int& sync );

   /**
    * Gets the video sync type.  This option allows the Flock to syncronize its
    * pulses with your video display.  This will eliminate most flicker caused
    * by the magnetic distortion.
    *
    * @post returns the sync type.
    * @note Refer to your Flock manual for what the return number means.
    */
   inline const int& getSync() const
   {
      return mFlockOfBirds.getSync();
   }

   /**
    * Sets blocking of the Flock.
    * @pre flock.isActive() must be false to use this function.
    * @note See the Flock manual for details.
    */
   void setBlocking( const bool& blVal );

   /**
    * Gets the Flock's blocking type.
    * @note See the Flock manual for details.
    */
   inline const bool& getBlocking() const
   {
      return mFlockOfBirds.getBlocking();
   }

   /**
    * Sets the type of filtering that the Flock uses.
    * @note flock.isActive() must be false to use this function.
    */
   void setFilterType( const BIRD_FILT& f );

   /** Sets the type of filtering that the flock uses. */
   inline const BIRD_FILT& getFilterType() const
   {
      return mFlockOfBirds.getFilterType();
   }

   /**
    * Sets the hemisphere from which the transmitter transmits.
    * @note flock.isActive() must be false to use this function.
    */
   void setHemisphere( const BIRD_HEMI& h );

   /** Sets the hemisphere that the transmitter transmits from. */
   inline const BIRD_HEMI& getHemisphere() const
   {
      return mFlockOfBirds.getHemisphere();
   }

   /**
    * Sets the report rate that the flock uses.
    * @note flock.isActive() must be false to use this function.
    */
   void setReportRate( const char& rRate );

   /** Sets the report rate that the flock uses. */
   inline const char& getReportRate() const
   {
      return mFlockOfBirds.getReportRate();
   }

   /** Gets the x position of the i'th reciever. */
   inline float& xPos( const int& i )
   {
      return mFlockOfBirds.xPos( i );
   }

   /** Gets the y position of the i'th reciever. */
   inline float& yPos( const int& i )
   {
      return mFlockOfBirds.yPos( i );
   }

   /** Gets the z position of the i'th reciever. */
   inline float& zPos( const int& i )
   {
      return mFlockOfBirds.zPos( i );
   }

   /** Gets the z rotation of the i'th reciever. */
   inline float& zRot( const int& i )
   {
      return mFlockOfBirds.zRot( i );
   };

   /** Gets the y rotation of the i'th reciever. */
   inline float& yRot( const int& i )
   {
      return mFlockOfBirds.yRot( i );
   }

   /** Gets the x rotation of the i'th reciever. */
   inline float& xRot( const int& i )
   {
      return mFlockOfBirds.xRot( i );
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
   void controlLoop(void* nullParam);
   void positionCorrect(float&x,float&y,float&z);
   void initCorrectionTable(const char*);

   int getBirdIndex(int birdNum, int bufferIndex);

   vpr::Thread*      mThread;      /**< The thread doing the flock sampling */
   FlockStandalone   mFlockOfBirds; /**< The actual Flock device object */
   
   std::string       mPortName;
   int               mBaudRate; 
};

} // End of gadget namespace

#endif
