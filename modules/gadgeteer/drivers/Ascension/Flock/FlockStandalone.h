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

#ifndef _GADGET_ASCENSION_FLOCKOFBIRD_STANDALONE_H_
#define _GADGET_ASCENSION_FLOCKOFBIRD_STANDALONE_H_

#include <vpr/IO/Port/SerialPort.h>
#include <vpr/vprTypes.h>
#include <string>
#include <boost/tuple/tuple.hpp>

#define POSITION_RANGE 12.0f
#define ANGLE_RANGE   180.0f
#define MAX_SENSORS    128


typedef struct
{
   float xmin, ymin, zmin;
   float xmax, ymax, zmax;
   float xloc[15][10][15],yloc[15][10][15],zloc[15][10][15];
} CalStruct;

enum BIRD_HEMI
{
   FRONT_HEM, AFT_HEM, UPPER_HEM, LOWER_HEM, LEFT_HEM, RIGHT_HEM
};
inline std::string getHemiString(BIRD_HEMI hemi)
{
   if(FRONT_HEM == hemi) return "Front";
   else if(AFT_HEM == hemi) return "Aft";
   else if(UPPER_HEM == hemi) return "Upper";
   else if(LOWER_HEM == hemi) return "Lower";
   else if(LEFT_HEM == hemi) return "Left";
   else if(RIGHT_HEM == hemi) return "Right";
   else return "Unknown";
}

enum BIRD_FILT
{
   AC_NARROW, AC_WIDE, DC_FILTER
};
inline std::string getFiltString(BIRD_FILT filt)
{
   if(AC_NARROW == filt) return "AC Narrow";
   else if(AC_WIDE == filt) return "AC Wide";
   else if(DC_FILTER == filt) return "DC Filter";
   else return "Unknown";
}

namespace ReportRate
{
   const vpr::Uint8 Max(0x51);
   const vpr::Uint8 EveryOther(0x52);
   const vpr::Uint8 Every8(0x53);
   const vpr::Uint8 Every32(0x54);
}

namespace Transmitter
{
   /** Type of transmitter */
   enum Type
   { Ert0 = 0, Ert1 = 1, Ert2 = 2, Ert3 = 3, Standard, None };

   inline std::string getTransmitterString(Type type)
   {
      if(Ert0 == type)
      { return "Ert 0"; }
      else if(Ert1 == type)
      { return "Ert 1"; }
      else if(Ert2 == type)
      { return "Ert 2"; }
      else if(Ert3 == type)
      { return "Ert 3"; }
      else if(Standard == type)
      { return "Standard"; }

      return "None";
   }
}

namespace Flock
{
   namespace Command
   {
      const vpr::Uint8 Angles(0x57);
      const vpr::Uint8 ChangeValue(0x50);
      const vpr::Uint8 ExamineValue(0x4F);
      const vpr::Uint8 FbbReset(0x2F);
      const vpr::Uint8 Hemisphere(0x4C);
      const vpr::Uint8 Matrix(0x58);
      const vpr::Uint8 NextTransmitter(0x30);
      const vpr::Uint8 Point(0x42);
      const vpr::Uint8 Position(0x56);
      const vpr::Uint8 PositionAngles(0x59);
      const vpr::Uint8 PositionMatrix(0x5A);
      const vpr::Uint8 PositionQuaternion(0x5D);
      const vpr::Uint8 Quaternion(0x5C);
      const vpr::Uint8 ReportRate1(0x51);
      const vpr::Uint8 ReportRate2(0x52);
      const vpr::Uint8 ReportRate8(0x53);
      const vpr::Uint8 ReportRate32(0x54);
      const vpr::Uint8 ToFbbNormal(0xF0);
      const vpr::Uint8 ToFbbExpanded(0xE0);
      const vpr::Uint8 ToFbbSuperExpanded(0xA0);
      const vpr::Uint8 Run(0x46);
      const vpr::Uint8 Sleep(0x47);
      const vpr::Uint8 Stream(0x40);
      const vpr::Uint8 StreamStop(0x3F);
   };

   namespace Parameter
   {
      const vpr::Uint8 BirdStatus(0x0);
      const vpr::Uint8 SoftwareRevision(0x01);
      const vpr::Uint8 ModelIdentification(0xF);
      const vpr::Uint8 AddressingMode(0x13);
      const vpr::Uint8 FbbAddress(0x15);
      const vpr::Uint8 Hemisphere(0x16);
      const vpr::Uint8 GroupMode(0x23);
      const vpr::Uint8 FlockSystemStatus(0x24);
      const vpr::Uint8 FbbAutoConfig(0x32);
   }

   // Masks for bird status return value
   namespace BirdStatus
   {
      const vpr::Uint16 MasterBit(1<<15);
      const vpr::Uint16 BirdInitBit(1<<14);
      const vpr::Uint16 ErrorBit(1<<13);
      const vpr::Uint16 RunningBit(1<<12);
      const vpr::Uint16 HostSyncBit(1<<11);
      const vpr::Uint16 AddressModeBit(1<<10);
      const vpr::Uint16 CrtSyncBit(1<<9);
      const vpr::Uint16 SyncModeBit(1<<8);
      const vpr::Uint16 FactoryTestBit(1<<7);
      const vpr::Uint16 XonXoffBit(1<<6);
      const vpr::Uint16 SleepBit(1<<5);
      //const vpr::Uint16 PositionMask(0
      const vpr::Uint16 PointModeBit(1<<0);

      inline bool isMaster(vpr::Uint16 status)
      { return (MasterBit & status); }
   }

   namespace SystemStatus
   {
      const vpr::Uint8 AccessibleBit(1<<7);
      const vpr::Uint8 RunningBit(1<<6);
      const vpr::Uint8 SensorBit(1<<5);
      const vpr::Uint8 ErtBit(1<<4);
      const vpr::Uint8 Ert3Bit(1<<3);
      const vpr::Uint8 Ert2Bit(1<<2);
      const vpr::Uint8 Ert1Bit(1<<1);
      const vpr::Uint8 TransmitterBit(1<<0);

      inline bool isAccessible(vpr::Uint8 bstatus)
      { return (AccessibleBit & bstatus); }

      /** Is the bird running.  Running --> power on, auto-configed and awake */
      inline bool isRunning(vpr::Uint8 bstatus)
      { return (RunningBit & bstatus); }

      /** Does the unit have a sensor? */
      inline bool hasSensor(vpr::Uint8 bstatus)
      { return (AccessibleBit & bstatus); }

      inline bool isErt(vpr::Uint8 bstatus)
      { return (ErtBit && bstatus); }

      inline bool hasStandardTransmitter(vpr::Uint8 bstatus)
      { return ((TransmitterBit & bstatus) && (!isErt(bstatus)) ); }

      inline bool hasTransmitter(vpr::Uint8 bstatus)
      { return (isErt(bstatus) || hasStandardTransmitter(bstatus)); }

      inline Transmitter::Type getTransmitterType(vpr::Uint8 bstatus)
      {
         Transmitter::Type type;

         if(!hasTransmitter(bstatus))
         {
            type = Transmitter::None;
         }
         else if(hasStandardTransmitter(bstatus))
         {
            type = Transmitter::Standard;
         }
         else
         {
            if (Ert1Bit & bstatus)
            {
               type = Transmitter::Ert1;
            }
            else if (Ert2Bit & bstatus)
            {
               type = Transmitter::Ert2;
            }
            else if (Ert3Bit & bstatus)
            {
               type = Transmitter::Ert3;
            }
            else if (TransmitterBit & bstatus)
            {
               type = Transmitter::Ert0;
            }
         }

         return type;
      }


   }
}

enum AddressingMode
{
   NormalAddressing = 0,
   ExpandedAddressing = 1,
   SuperExpandedAddressing = 3,
   UnknownAddressing = 16
};
inline std::string getAddressingModeString(AddressingMode addrMode)
{
   if(NormalAddressing == addrMode) return "Normal Addressing";
   else if(ExpandedAddressing == addrMode) return "Expanded Addressing";
   else if(SuperExpandedAddressing == addrMode) return "Super Expanded Addressing";
   else return "Uknown addressing";
}

/**
 * class for running a Flock of Birds.
 * FlockStandalone is a positional device driver for the Flock of Birds <br>
 */
class FlockStandalone
{
public:
   /**
    * Configure constructor.
    *
    * @param port  such as "/dev/ttyd3"
    * @param baud  such as 38400, 19200, 9600, 14400, etc...
    * @param sync  sync type
    * @param block  blocking
    * @param numBrds  number of birds in flock (without transmitter)
    * @param transmit  transmitter unit number
    * @param hemi  hemisphere to track from
    * @param filt  filtering type
    * @param report  flock report rate
    * @param calfile  a calibration file, if "", then use none
    *
    * @post configures internal data members,
    *          doesn't actually talk to the FOB yet.
    */
   FlockStandalone(const char* const port = "/dev/ttyd3",
                   const int& baud = 38400,
                   const int& sync = 1,
                   const bool& block = false,
                   const int& numBrds = 3,
                   const int& transmit = 3,
                   const BIRD_HEMI& hemi = LOWER_HEM,
                   const BIRD_FILT& filt = AC_NARROW,
                   const char& report = 'R',
                   const char* const calfile = "");

   /**  Destructor */
   ~FlockStandalone();

   /** Send command.
   * @param cmd - cmd to send
   */
   vpr::ReturnStatus sendCommand(vpr::Uint8 cmd);

   /** Open the flock on the configured port.
   * @note Calls readFlockConfiguration to find out current flock settings.
   */
   vpr::ReturnStatus openPort(void);

   /** Read the current configuration of the flock.
   * Asks the flock for it's current configuration.
   * This sets internal vars like addressing mode, software rev, etc.
   */
   vpr::ReturnStatus readFlockConfiguration();

   /**
    * Call this to connect to the Flock device.
    * @note flock.isActive() must be false to use this function.
    */
   vpr::ReturnStatus start();

   /** Stops the Flock. */
   int stop();

   /**
    * Call this repeatedly to update the data from the birds.
    * @note flock.isActive() must be true to use this function.
    */
   bool sample();

   /** Checks if the flock is active. */
   bool isActive() const;

   /**
    * Sets the port to use.
    * This will be a string in the form of the native OS descriptor.<BR>
    * ex: unix - "/dev/ttyd3", win32 - "COM3"
    *
    * @note flock.isActive() must be false to use this function.
    */
   void setPort( const std::string& serialPort );

   /**
    * Gets the port used.
    * This will be a string in the form of the native OS descriptor.<BR>
    * ex: unix - "/dev/ttyd3", win32 - "COM3"
    */
   const std::string& getPort() const;

   /**
    * Sets the baud rate.
    * This is generally 38400, consult Flock manual for other rates.
    *
    * @note flock.isActive() must be false to use this function.
    */
   void setBaudRate( const int& baud );

   /** Gets the baud rate. */
   const int& getBaudRate() const
   {
      return mBaud;
   }

   /**
    * Sets the unit number of the transmitter.
    * @param Transmit An integer that is the same as the dip switch
    *                 setting on the transmitter box (for the unit number).
    *
    * @note flock.isActive() must be false to use this function.
    */
   void setTransmitter( const int& Transmit );

   /**
    * Gets the unit number of the transmitter.
    *
    * @return An integer that is the same as the dip switch
    *         setting on the transmitter box (for the unit number).
    */
   inline const int& getTransmitter() const
   {
      return mXmitterUnitNumber;
   }

   /**
    * Sets the number of birds to use in the Flock.
    *
    * @param n An integer number not more than the number of
    *          birds attached to the system.
    * @note flock.isActive() must be false to use this function.
    */
   void setNumBirds( const int& n );

   /**
    * Gets the number of birds to use in the Flock.
    *
    * @return An integer number not more than the number of
    *         birds attached to the system.
    */
   inline const int& getNumBirds() const
   {
      return mNumBirds;
   }

   /**
    * Sets the video sync type.  This option allows the Flock to syncronize its
    * pulses with your video display.  This will eliminate most flicker caused
    * by the magnetic distortion.  Refer to your Flock manual for what number
    * to use.
    *
    * @note flock.isActive() must be false to use this function.
    */
   void setSync( const int& sync );

   /**
    * Gets the video sync type.  This option allows the Flock to syncronize its
    * pulses with your video display.  This will eliminate most flicker caused
    * by the magnetic distortion. Refer to your Flock manual for what the
    * returned number means.
    */
   inline const int& getSync() const
   {
      return mSyncStyle;
   }

   /**
    * Sets blocking of Flock.  See the Flock manual for details.
    *
    * @note flock.isActive() must be false to use this function.
    */
   void setBlocking( const bool& blVal );

   void setExtendedRange( const bool& blVal );

   /**
    * Gets the Flock's blocking type.  See the Flock manual for details.
    */
   inline const bool& getBlocking() const
   {
      return mBlocking;
   }

   /**
    * Sets the type of filtering that the Flock uses.
    * @note flock.isActive() must be false to use this function.
    */
   void setFilterType( const BIRD_FILT& f );

   /** Sets the type of filtering that the Flock uses. */
   inline const BIRD_FILT& getFilterType() const
   {
      return mFilter;
   }

   /**
    * Stes the hemisphere that the transmitter transmits from.
    *
    * @note flock.isActive() must be false to use this function.
    */
   void setHemisphere( const BIRD_HEMI& h );

   /** Sets the hemisphere that the transmitter transmits from. */
   inline const BIRD_HEMI& getHemisphere() const
   {
      return mHemisphere;
   }

   /**
    * Sets the report rate that the flock uses.
    *
    * @note flock.isActive() must be false to use this function.
    */
   void setReportRate( const char& rRate );

   /** Sets the report rate that the Flock uses. */
   inline const char& getReportRate() const
   {
      return mReportRate;
   }

   /** Gets the x position of the i'th reciever. */
   float& xPos( const int& i );

   /** Gets the y position of the i'th reciever. */
   float& yPos( const int& i );

   /** Gets the z position of the i'th reciever. */
   float& zPos( const int& i );

   /** Gets the z rotation of the i'th reciever. */
   float& zRot( const int& i );

   /** Gets the y rotation of the i'th reciever. */
   float& yRot( const int& i );

   /** Gets the x rotation of the i'th reciever. */
   float& xRot( const int& i );

// public methods
public:
   /**
    * With the calibration table info, correct a given position.
    *
    * @param x The x position value in Euclidean coordinates.
    * @param y The y position value in Euclidean coordinates.
    * @param z The z position value in Euclidean coordinates.
    *
    * @return A corrected position in x, y, z Euclidian coordinates.
    */
   void positionCorrect( float& x, float& y, float& z );

   /**
    * Initializes the correction table from a file.
    * @post Initializes the correction table with the file's info.
    * @param filename A file name of the calibration file.
    */
   void initCorrectionTable( const std::string& filename );

// testing functions
public:
   /** Reads in data from the Flock. */
//   void readData();

   /**
    * Checks the group state.
    * @post The group state is output to screen
    */
   void checkGroup();

   /**
    * Checks configuration.
    * @post Outputs each bit of the config character to screen.
    */
   void checkConfig();

   /**
    * Checks position and angles.
    * @note NOT IMPLEMENTED
    */
   void checkPosAngles();

   /**
    * Checks rep and stream.
    * @note NOT IMPLEMENTED
    */
   void checkRepAndStream();

   /**
    * Prints the bits of the given byte.
    *
    * @post The binary representation of var is printed.
    * @param var A character value.
    */
   void showbits(char var);
   void showbits16(float var);

   /**
    * Expands the given list of bits to a 32-bit integer.
    *
    * @param var A character.
    *
    * @return The integer value of var.
    */
   int hexToInt(char var);

   /**
    * Checks the data ready char.
    * @post Outputs to screen "I= " some number when flock returns the data
    *       ready character.
    */
   void checkDataReadyChar();

   /**
    * Examines an attribute.
    *
    * @param attrib - Attribute to query for - see the Flock manual.
    * @param respSize - Expected size of the response
    * @param respData   - Returned data
    */
   vpr::ReturnStatus getAttribute(vpr::Uint8 attrib, unsigned respSize, std::vector<vpr::Uint8>& respData);

// additional set functions
public:
   /** Sets to run mode. */
   void setRunMode();

   /** Sets to sleep mode. */
   void setSleepMode();

   /**
    * Sets Value.
    *
    * @post Flock is set.
    *
    * @param exam    Examine paramater (see manual pg 37)
    * @param setdata Data to assign to the paramater.
    * @param reps    Number of repitions
    */
   void setValue(char exam, char setdata, int reps);

   /** Sets to position mode. */
   void setPosMode();

   /** Sets to poition and angle mode. */
   void setPosAngMode();

   /** Sets to stream mode. */
   void setStreamMode();

   /** Sets to point mode. */
   void setPointMode();

   /**
    * Sets the report rate.
    *
    * @post Rate is set.
    *
    * @param rate The new report rate.
    */
   void setDeviceReportRate(char rate);

   vpr::ReturnStatus readStatus(const int birdNum = 1);

   // ---- Getters for flock state ---- //
   vpr::ReturnStatus getSoftwareRevision(unsigned& major, unsigned& minor);
   vpr::ReturnStatus getModelIdString(std::string& modelId);
   vpr::ReturnStatus getAddressingMode(AddressingMode& model);
   vpr::ReturnStatus getBirdStatus(vpr::Uint16& status);
   vpr::ReturnStatus getSystemStatus(std::vector<vpr::Uint8>& sysStatus);

   // ---- Helpers for printing information of interest to users --- //
   vpr::ReturnStatus printSystemStatus();

   void printError( unsigned char ErrCode, unsigned char ExpandedErrCode );
   int checkError();
   vpr::ReturnStatus readSystemModel();
   vpr::ReturnStatus readHemisphere();

   // ---- Attribute getters ------ //
   AddressingMode getAddressingMode()
   {  return mAddrMode; }

   /** Clears the reads bytes till buffer is zeroed. */
//  void clearBuffer();


public:   // --- Enums --- //
   enum Status
   {
      CLOSED,     /**< Flock is closed */
      OPEN,       /**< Flock is open */
      STREAMING   /**< Flock is streaming data */
   };

// Private data members
private:
   FlockStandalone::Status    mStatus;    /**< Current status of the flock */

   CalStruct   mCalTable;

   std::string mPort;
   std::string mCalibrationFileName;

   vpr::SerialPort* mSerialPort;
   int     mBaud;
   bool    mBlocking;

   bool    mUsingCorrectionTable;

   AddressingMode mAddrMode;           /**< The addressing mode of the flock */
   bool           mExtendedRange;      /**< True when using extended range flock */

   int            mNumBirds;           /**< Number of birds in flock */
   int            mXmitterUnitNumber;

   char           mReportRate;
   BIRD_HEMI      mHemisphere;
   BIRD_FILT      mFilter;
   int            mSyncStyle;

   //    x,y,z,        r,y,p
   float mPosition[MAX_SENSORS][3], mOrientation[MAX_SENSORS][3];

   bool mActive;

   const int mSleepFactor;

protected:
   void setBlocking(void);
   void sendSync(void);
   void sendGroup(void);
   void sendAutoconfig(void);
   void sendTransmitter(void);
   void sendFilter(void);
   void sendHemisphere(void);
   void sendPosAngles(void);
   void pickBird(const int birdID);
   void sendRepAndStream(void);

   /**
    * Gets a reading.
    *
    * @param n The bird unit number.
    * @param xPos Storage for the x position.
    * @param yPos Storage for the y position.
    * @param zPos Storage for the z position.
    * @param zRot Storage for the rotation about the z-axis.
    * @param yRot Storage for the rotation about the y-axis.
    * @param xRot Storage for the rotation about the x-axis.
    */
   vpr::ReturnStatus getReading(const int& n, float& xPos, float& yPos, float& zPos,
                                float& zRot, float& yRot, float& xRot );

   float rawToFloat(char& r1, char& r2);

   /**
    * Tests if the given bird ID is mapped to the transmitter.
    */
   bool isTransmitter(int birdID) const;
};


#endif
