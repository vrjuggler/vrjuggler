/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_ASCENSION_FLOCKOFBIRD_STANDALONE_H_
#define _GADGET_ASCENSION_FLOCKOFBIRD_STANDALONE_H_

#include <string>
#include <sstream>

#include <gmtl/Matrix.h>

#include <vpr/vpr.h>
#include <vpr/vprTypes.h>
#include <vpr/IO/Port/SerialPort.h>
#include <vpr/Util/Exception.h>


enum BIRD_HEMI
{
   FRONT_HEM, AFT_HEM, UPPER_HEM, LOWER_HEM, LEFT_HEM, RIGHT_HEM
};

inline std::string getHemiString(BIRD_HEMI hemi)
{
   if(FRONT_HEM == hemi)
   {
      return "Front";
   }
   else if(AFT_HEM == hemi)
   {
      return "Aft";
   }
   else if(UPPER_HEM == hemi)
   {
      return "Upper";
   }
   else if(LOWER_HEM == hemi)
   {
      return "Lower";
   }
   else if(LEFT_HEM == hemi)
   {
      return "Left";
   }
   else if(RIGHT_HEM == hemi)
   {
      return "Right";
   }
   else
   {
      return "Unknown";
   }
}

enum BIRD_FILT
{
   AC_NARROW, AC_WIDE, DC_FILTER
};

inline std::string getFiltString(BIRD_FILT filt)
{
   if(AC_NARROW == filt)
   {
      return "AC Narrow";
   }
   else if(AC_WIDE == filt)
   {
      return "AC Wide";
   }
   else if(DC_FILTER == filt)
   {
      return "DC Filter";
   }
   else
   {
      return "Unknown";
   }
}

namespace Transmitter
{
   /** Type of transmitter. */
   enum Type
   {
      Ert0 = 0,
      Ert1 = 1,
      Ert2 = 2,
      Ert3 = 3,
      Standard,
      None
   };

   inline std::string getTransmitterString(Type type)
   {
      if(Ert0 == type)
      {
         return "Ert 0";
      }
      else if(Ert1 == type)
      {
         return "Ert 1";
      }
      else if(Ert2 == type)
      {
         return "Ert 2";
      }
      else if(Ert3 == type)
      {
         return "Ert 3";
      }
      else if(Standard == type)
      {
         return "Standard";
      }

      return "None";
   }

   inline bool isErt(const Type type)
   {
      return ((type == Ert0) || (type == Ert1) || (type == Ert2) ||
              (type == Ert3));
   }
}

namespace Flock
{
   /** Gets string description of the given error. */
   std::string getErrorDescription(vpr::Uint8 errCode, vpr::Uint8 expandedErr);

   // ------ Flock exceptions ------ //
   /** Base exception for Flock errors. */
   class FlockException : public vpr::Exception
   {
   public:
      FlockException(const std::string& msg, const std::string& location = "")
         : vpr::Exception(msg, location)
      {
         /* Do nothing. */ ;
      }

      virtual ~FlockException() throw ()
      {
         /* Do nothing. */ ;
      }
   };

   /** Thrown when there are errors with the connection. */
   class ConnectionException : public FlockException
   {
   public:
      ConnectionException(const std::string& msg = "Connection exception",
                          const std::string& location = "")
         : FlockException(msg, location)
      {
         /* Do nothing. */ ;
      }

      virtual ~ConnectionException() throw ()
      {
         /* Do nothing. */ ;
      }
   };

   /** Thrown when there are timeouts reading or writing. */
   class TimeoutException : public FlockException
   {
   public:
      TimeoutException(const std::string& msg = "Time out exception",
                       const std::string& location = "")
         : FlockException(msg, location)
      {
         /* Do nothing. */ ;
      }

      virtual ~TimeoutException() throw ()
      {
         /* Do nothing. */ ;
      }
   };

   /** Thrown when there is invalid data read from the Flock. */
   class InvalidDataException : public FlockException
   {
   public:
      InvalidDataException(const std::string& msg = "Time out exception",
                           const std::string& location = "")
         : FlockException(msg, location)
      {
         /* Do nothing. */ ;
      }

      virtual ~InvalidDataException() throw ()
      {
         /* Do nothing. */ ;
      }
   };

   /** Thrown when there is invalid data read from the Flock. */
   class CommandFailureException : public FlockException
   {
   public:
      CommandFailureException(const std::string& msg = "Time out exception",
                              const std::string& location = "")
         : FlockException(msg, location)
      {
         /* Do nothing. */ ;
      }

      virtual ~CommandFailureException() throw ()
      {
         /* Do nothing. */ ;
      }
   };

   /** Thrown when the Flock is in an error state. */
   class BirdErrorException : public FlockException
   {
   public:
      BirdErrorException(vpr::Uint8 err, vpr::Uint8 expandedErr,
                         const std::string& location = "")
         : FlockException("Bird error", location)
      {
         std::string err_desc = Flock::getErrorDescription(err,expandedErr);
         std::ostringstream msg;
         msg << "Bird error: [" << err << "," << expandedErr << "]: "
             << err_desc;
         mDescription = msg.str();
         mErr = err;
         mExpandedErr = expandedErr;
      }

      virtual ~BirdErrorException() throw ()
      {
         /* Do nothing. */ ;
      }

      vpr::Uint8 mErr;
      vpr::Uint8 mExpandedErr;
   };

   // ---------------------------------------------------------------------- //

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
      const vpr::Uint8 Sync(0x41);
   };

   namespace Parameter
   {
      const vpr::Uint8 BirdStatus(0x0);
      const vpr::Uint8 SoftwareRevision(0x01);
      const vpr::Uint8 CrystalSpeed(0x2);
      const vpr::Uint8 PositionScaling(0x3);
      const vpr::Uint8 FilterStatus(0x4);
      const vpr::Uint8 BirdRateCount(0x6);
      const vpr::Uint8 BirdRate(0x7);
      const vpr::Uint8 BirdErrorCode(0xA);
      const vpr::Uint8 BirdErrorMask(0xB);
      const vpr::Uint8 BirdSuddenChange(0xE);
      const vpr::Uint8 ModelIdentification(0xF);
      const vpr::Uint8 BirdExpandedErrorCode(0x10);
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
      {
         return (MasterBit & status);
      }

      inline bool isInititalized(vpr::Uint16 status)
      {
         return (BirdInitBit & status);
      }

      inline bool hasError(vpr::Uint16 status)
      {
         return (ErrorBit & status);
      }

      inline bool isRunning(vpr::Uint16 status)
      {
         return (RunningBit & status);
      }

      inline bool isSleeping(vpr::Uint16 status)
      {
         return (SleepBit & status);
      }
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
      {
         return (AccessibleBit & bstatus);
      }

      /** Is the bird running.  Running --> power on, auto-configed and awake */
      inline bool isRunning(vpr::Uint8 bstatus)
      {
         return (RunningBit & bstatus);
      }

      /** Does the unit have a sensor? */
      inline bool hasSensor(vpr::Uint8 bstatus)
      {
         return (SensorBit & bstatus);
      }

      inline bool isErt(vpr::Uint8 bstatus)
      {
         return (ErtBit & bstatus);
      }

      inline bool hasStandardTransmitter(vpr::Uint8 bstatus)
      {
         return ((TransmitterBit & bstatus) && (!isErt(bstatus)) );
      }

      inline bool hasTransmitter(vpr::Uint8 bstatus)
      {
         return (isErt(bstatus) || hasStandardTransmitter(bstatus));
      }

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

   namespace Output
   {
      /**
       * Format specifiers.
       * Indexed for bird status results and happily also so if you
       * Add (Command::Postion-1) to the value you get the command value to
       * send to switch to that mode.
       */
      enum Format
      {
         Position = 0x1,
         Angle = 0x2,
         Matrix = 0x3,
         PositionAngle = 0x4,
         PositionMatrix = 0x5,
         Quaternion = 0x7,
         PositionQuaternion = 0x8
      };

      inline std::string getFormatString(Format format)
      {
         if(Position == format)
         {
            return "Position";
         }
         else if(Angle == format)
         {
            return "Angle";
         }
         else if(Matrix == format)
         {
            return "Matrix";
         }
         else if(PositionAngle == format)
         {
            return "PositionAngle";
         }
         else if(PositionMatrix == format)
         {
            return "PositionMatrix";
         }
         else if(Quaternion == format)
         {
            return "Quaternion";
         }
         else if(PositionQuaternion == format)
         {
            return "PositionQuaternion";
         }
         else
         {
            return "Unknown format";
         }
      }
      /** Returns the size in bytes of a data record for the given format */
      inline unsigned int getDataSize(Format format)
      {
         if(Position == format)
         {
            return 6;
         }
         else if(Angle == format)
         {
            return 6;
         }
         else if(Matrix == format)
         {
            return 18;
         }
         else if(PositionAngle == format)
         {
            return 12;
         }
         else if(PositionMatrix == format)
         {
            return 24;
         }
         else if(Quaternion == format)
         {
            return 8;
         }
         else if(PositionQuaternion == format)
         {
            return 14;
         }
         else
         {
            return 0;
         }
      }

   }

   /** Report rate for bird sampling */
   enum ReportRate
   {
      MaxRate = 0x51,          /**< Use maximum rate */
      EveryOther = 0x52,   /**< Every other sample */
      Every8 = 0x53,       /**< Every 8th sample */
      Every32 = 0x54       /**< Every 32nd sample */
   };

   inline std::string getReportRateString(ReportRate rate)
   {
      if(MaxRate == rate)
      {
         return "MaxRate";
      }
      else if(EveryOther == rate)
      {
         return "EveryOther";
      }
      else if(Every8 == rate)
      {
         return "Every8";
      }
      else if(Every32 == rate)
      {
         return "Every32";
      }
      else
      {
         return "Unknown";
      }
   }

   /**
    * Mode the Flock is executing in.
    * This is not specifically defined in the Flock manual, but falls out of
    * how you need to manage the system.
    */
   enum Mode
   {
      UnknownMode = 0,     /**< Default unknown mode */
      Standalone = 1,      /**< Mode with single unit with transmitter and bird */
      Standard = 2,        /**< Mode with only a single standard transmitter */
      ExtendedRange = 3    /**< Mode with one or more extended range transmitters (ERTs) */
   };

   inline std::string getModeString(Mode mode)
   {
      if(Standalone == mode)
      {
         return "Standalone";
      }
      else if(Standard == mode)
      {
         return "Standard";
      }
      else if(ExtendedRange == mode)
      {
         return "Extended Range";
      }
      else
      {
         return "Unknown mode";
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
      if(NormalAddressing == addrMode)
      {
         return "Normal Addressing";
      }
      else if(ExpandedAddressing == addrMode)
      {
         return "Expanded Addressing";
      }
      else if(SuperExpandedAddressing == addrMode)
      {
         return "Super Expanded Addressing";
      }
      else
      {
         return "Unknown addressing";
      }
   }
}

/**
 * Class for running a Flock of Birds.
 * FlockStandalone is a positional device driver for the Flock of Birds.
 *
 * This driver tries to treat the Flock as a single group of birds to read
 * data from.  Because of this, most things are done in group mode and through
 * the use of streaming.  The driver could be more flexible and allow this to
 * be configured, but it would take a lot more effort and we feel this group
 * mode is the right one to use in most cases.
 *
 * @note All methods handle errors using the Flock exceptions described above.
 */
class FlockStandalone
{
public:
   // --- Enums --- //
   enum Status
   {
      CLOSED,     /**< Flock is closed */
      OPEN,       /**< Flock is open */
      RUNNING,    /**< Configured and running (but not streaming) */
      STREAMING   /**< Flock is streaming data */
   };

public:
   /**
    * Configure constructor.
    *
    * @param port  such as "/dev/ttyd3".
    * @param baud  such as 38400, 19200, 9600, 14400, etc.
    * @param sync  sync type.
    * @param block  blocking.
    * @param numBrds  number of birds in Flock (without transmitter).
    * @param transmit  transmitter unit number.
    * @param hemi  hemisphere to track from.
    * @param filt  filtering type.
    * @param report  Flock report rate.
    * @param calfile  a calibration file, if "", then use none.
    *
    * @post configures internal data members, doesn't actually talk to the
    *       FOB yet.
    */
   FlockStandalone(std::string port, const int& baud = 38400,
                   const int& sync = 1, const BIRD_HEMI& hemi = LOWER_HEM,
                   const BIRD_FILT& filt = AC_NARROW,
                   Flock::ReportRate = Flock::MaxRate);

   /** Destructor. */
   ~FlockStandalone();

   /**
    * Opens the Flock on the configured port.
    * This call opens the connection to the Flock and asks the Flock for its
    * current configuration.  Upon completion of this call, the driver should
    * be able to communicate with the Flock.
    *
    * @note We ask the Flock for its configuration here so we can change it
    *       later before calling the configure commands.
    */
   bool open();

   /**
    * Configures the Flock for execution.
    *
    * @pre Flock is open and all configurable settings have been passed.
    * @post Flock is setup and configured to start getting samples.  It is
    *       ready to run.
    */
   void configure();

   /** Close the connection to the Flock. */
   int close();

   /** Call this repeatedly to update the data from the birds. */
   void sample();

   /** Return the position of a bird sensor */
   gmtl::Matrix44f getSensorPosition(unsigned int sensorNumber)
   {
      vprASSERT(sensorNumber < mNumSensors);
      return mSensorData[sensorNumber];
   }

   /**
    * Start the Flock streaming data.
    * @pre Flock must have been configured and in READY mode.
    */
   void startStreaming();

   /** Stop streaming data.
   * @pre Flock must be currently streaming.
   */
   void stopStreaming();

   /**
    * Sets the port to use.
    * This will be a string in the form of the native OS descriptor.<BR>
    * ex: unix - "/dev/ttyd3", win32 - "COM3"
    */
   void setPort( const std::string& serialPort );

   /**
    * Gets the port used.
    * @see setPort for a description of the string format
    */
   const std::string& getPort() const
   {
      return mPort;
   }

   /** Sets the baud rate. */
   void setBaudRate( const int& baud );

   /** Gets the baud rate. */
   const int& getBaudRate() const
   {
      return mBaud;
   }

   /**
    * Sets the number of birds to use in the Flock.
    *
    * @param n An integer number not more than the number of
    *          birds attached to the system.
    */
   void setNumSensors( const unsigned int& n );

   /**
    * Gets the number of birds to use in the Flock.
    */
   unsigned int getNumSensors() const
   {
      return mNumSensors;
   }

   /**
    * Sets the video sync type. Refer to your Flock manual for sync methods
    * and usage.
    */
   void setSync( const vpr::Uint8& sync );

   /**
    * Gets the video sync type.  This option allows the Flock to syncronize its
    * pulses with your video display.  This will eliminate most flicker caused
    * by the magnetic distortion. Refer to your Flock manual for what the
    * returned number means.
    */
   vpr::Uint8 getSync() const
   {
      return mSyncStyle;
   }

   void setExtendedRange( const bool& blVal );

   /**
    * Sets the type of filtering that the Flock uses.
    */
   void setFilterType( const BIRD_FILT& f );

   /** Sets the type of filtering that the Flock uses. */
   const BIRD_FILT& getFilterType() const
   {
      return mFilter;
   }

   /**
    * Sets the hemisphere that the transmitter transmits from.
    */
   void setHemisphere( const BIRD_HEMI& h );

   /** Sets the hemisphere that the transmitter transmits from. */
   const BIRD_HEMI& getHemisphere() const
   {
      return mHemisphere;
   }

   /**
    * Sets the report rate that the Flock uses.
    */
   void setReportRate(Flock::ReportRate rRate );

   /** Sets the report rate that the Flock uses. */
   Flock::ReportRate getReportRate() const
   {
      return mReportRate;
   }

   /** Set the output format to use. */
   void setOutputFormat(Flock::Output::Format format);

   Flock::Output::Format getOutputFormat() const
   {
      return mOutputFormat;
   }

public:
   /**
    * Send command.
    * @param cmd Command to send.
    */
   void sendCommand(vpr::Uint8 cmd,
                    std::vector<vpr::Uint8> data = std::vector<vpr::Uint8>(0));

   /** Send command to all units (except excluded types). */
   void sendCommandAll(vpr::Uint8 cmd, std::vector<vpr::Uint8> data,
                       bool excludeErc = false);

   /**
    * Examines an attribute.
    *
    * @param attrib   Attribute to query for.  See the Flock manual.
    * @param respSize Expected size of the response.
    * @param respData Returned data.
    */
   void getAttribute(vpr::Uint8 attrib, unsigned int respSize,
                     std::vector<vpr::Uint8>& respData);

   /**
    * Change an attribute.
    *
    * @param attrib   Attribute to set.  See the Flock manual.
    * @param respData Argument data.
    */
   void setAttribute(vpr::Uint8 attrib, std::vector<vpr::Uint8>& attribData);

public:  // ---- Query methods for Flock state ---- //
   /** Find the software revision. */
   std::pair<unsigned int,unsigned int> querySoftwareRevision();

   /** Find the model id string. */
   std::string queryModelIdString();

   /** Find the addressing mode. */
   Flock::AddressingMode queryAddressingMode();

   /** Get the address of the master bird. */
   vpr::Uint8 queryAddress();

   /** Get the bird status. */
   vpr::Uint16 queryBirdStatus(unsigned int addr=0);

   /** Get the system status information. */
   std::vector<vpr::Uint8> querySystemStatus();

   /** Get the current error code. */
   vpr::Uint8 queryErrorCode();

   /** Get the current expanded error code. */
   std::pair<vpr::Uint8,vpr::Uint8> queryExpandedErrorCode();

   vpr::Uint8 queryPositionScaleFactor();

   void setErrorModeIgnore();
   void setErrorModeStandard();

   /**
    * Check for error, if there is, print out error message and throw
    * exception.
    *
    * @throws BirdErrorException
    */
   void checkError();

   // ---- Helpers for printing information of interest to users --- //
   /**
    * Print the information we have about the status of all units in the Flock.
    */
   void printFlockStatus();

   void setAddressingMode(const Flock::AddressingMode mode)
   {
      mAddrMode = mode;
   }

   void setMasterAddress(const unsigned int addr)
   {
      mMasterAddr = addr;
   }

   // ---- Attribute getters ------ //
   // These methods are only valid after the initial open command completes
   Flock::AddressingMode getAddressingMode() const
   {
      return mAddrMode;
   }

   float getSoftwareRevision() const
   {
      return mSwRevision;
   }

   const std::string& getModelId() const
   {
      return mModelId;
   }

   Status getStatus() const
   {
      return mStatus;
   }

protected:     // -- Bird commands --- //
   void pickBird(const vpr::Uint8 birdID);

   // -- Bird commands --- //
   // Documented in impl
   void sendGroupCmd(bool newVal);
   void sendSyncCmd(vpr::Uint8 syncMethod);
   void sendHemisphereCmd(BIRD_HEMI hemi, bool sendToAll);

   void sendAutoconfigCmd(vpr::Uint8 numUnits);
   void sendNextTransmitterCmd(vpr::Uint8 addr, vpr::Uint8 transmitterNumber);
   void sendOutputFormatCmd(Flock::Output::Format format, bool sendToAll);
   //void sendFilter();

   void sendReportRateCmd(Flock::ReportRate rate);
   void sendStreamStartCommand();
   void sendStreamStopCommand();


protected: // -- Helpers --- //
   /** Process a reading (of all sensors) from the Flock. */
   void processDataRecord(std::vector<vpr::Uint8> dataRecord);

   /** Get a matrix position from bird input data in the buffer. */
   gmtl::Matrix44f processSensorRecord(vpr::Uint8* buff);

   /** Helper to convert raw binary data read to float value. */
   float rawToFloat(const vpr::Uint8& r1, const vpr::Uint8& r2);

   /**
    * Reads the current configuration of the Flock.
    * Asks the Flock for its current configuration.
    * This sets internal vars like addressing mode, software rev, etc.
    */
   void readInitialFlockConfiguration();

   /**
    * Sets up the internal Flock units data structure.
    * Reads data from System status and bird status to update the structure.
    *
    * @pre Port must be opened and master address must be set.
    */
   void setupFlockUnitsDataStructure();

   /** Get the maximum bird address for the current mode. */
   unsigned int getMaxBirdAddr();

   std::string getStatusString()
   {
      std::string status_string;
      if (FlockStandalone::CLOSED == getStatus())
      {
         status_string = "Closed";
      }
      else if (FlockStandalone::OPEN == getStatus())
      {
         status_string = "Open";
      }
      else if (FlockStandalone::RUNNING == getStatus())
      {
         status_string = "Running";
      }
      else if (FlockStandalone::STREAMING == getStatus())
      {
         status_string=  "Streaming";
      }
      else
      {
         status_string = "Unknown";
      }

      return status_string;
   }

public:
   /**
    * Struct for holding sys information about a single unit in the Flock.
    */
   struct FlockUnit
   {
      FlockUnit()
         : mAddr(0)
         , mIsMaster(false)
         , mAccessible(false)
         , mIsRunning(false)
         , mHasSensor(false)
         , mTransmitterType(Transmitter::None)
         , mHasError(false)
         , mHasBeenInitialized(false)
      {;}

      unsigned int mAddr;         /**< Address of this unit on the fbb */
      bool     mIsMaster;
      bool     mAccessible;
      bool     mIsRunning;
      bool     mHasSensor;
      Transmitter::Type mTransmitterType;
      bool     mHasError;
      bool     mHasBeenInitialized;

      bool hasTransmitter() const
      {
         return (mTransmitterType != Transmitter::None);
      }
   };


private:  // --- Data members --- //
   FlockStandalone::Status    mStatus;    /**< Current status of the Flock */

   std::string       mPort;         /**< Port name to open for serial port connection */
   vpr::SerialPort*  mSerialPort;   /**< Serial port object connected to the bird */
   int               mBaud;         /**< Baud rate to use for connection */

   // --- Configuration information for the Flock --- //
   Flock::Mode           mMode;           /**< The mode the Flock is operating in */
   Flock::AddressingMode mAddrMode;       /**< The addressing mode of the Flock */
   float                 mSwRevision;     /**< Software revision of the Flock */
   std::string           mModelId;        /**< Model id for the system we are connected to */
   unsigned int          mMasterAddr;     /**< Address of the master */
   unsigned int          mNumSensors;     /**< Number of sensors in the Flock */
   float                 mMaxPos;         /**< Maximum position based on scaling factor. */

   Flock::Output::Format mOutputFormat;   /**< The output format to configure the Flock to use */
   Flock::ReportRate     mReportRate;     /**< The report rate we to use when configuring the Flock */
   BIRD_HEMI             mHemisphere;     /**< The hemisphere to use when configuring the Flock */
   BIRD_FILT             mFilter;         /**< The Filter to use when configuring the Flock */
   vpr::Uint8            mSyncStyle;      /**< The sync method to use when configuring the Flock */

   typedef std::vector<FlockUnit>   flock_units_t; /**< Type of the Flock units sequence */
   flock_units_t           mFlockUnits;              /**< List of all the Flock units we have */
   unsigned int            mActiveUnitEndIndex;      /**< Index of end of active units (one past last active) */
   std::vector<vpr::Uint8> mXmitterIndices;          /**< Indices into mFlockUnits of the transmitters */

   /**
    * Maps the addr of a unit (FBB addr) to the index of the associated sensor
    * in mSensorData (vrj unit). -1 means no sensor at that addr.
    */
   std::vector<int>        mAddrToSensorIdMap;

   /**
    * Current sensor data for all sensors in Flock.
    *
    * @note This vector is NOT indexed by bird address (like mFlockUnits).
    *       Instead, it remaps the addresses (using mAddrToSensorMap) to a
    *       sequential list of sensors.  This is meant to make it easier to
    *       look at the Flock as a sequence of position sensors. Values stored
    *       in feet.
    */
   std::vector<gmtl::Matrix44f>  mSensorData;

   // --- Default params --- //
   vpr::Interval        mReadTimeout;  /**< Standard timeout for all reads */
};


#endif
