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
enum BIRD_FILT
{
   AC_NARROW, AC_WIDE, DC_FILTER
};

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
   int sample();

   /** Checks if the flock is active. */
   const bool& isActive() const;

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
   void readData();

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
    * @post Streamed data is output to screen.
    *
    * @param exam Attribute to query for - see the Flock manual.
    * @param format (1)regular output (0)binary output.
    * @param reps Number of reports.
    * @param data Not currently used.
    */
   void examineValue(char exam, int data, int reps, int format);

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
   vpr::ReturnStatus readSoftwareRevision();
   void printError( unsigned char ErrCode, unsigned char ExpandedErrCode );
   int checkError();
   vpr::ReturnStatus readSystemModel();
   vpr::ReturnStatus readHemisphere();

   /** Clears the reads bytes till buffer is zeroed. */
//  void clearBuffer();


// Private data members
private:
   CalStruct   mCalTable;
   char        mReportRate;
   BIRD_HEMI   mHemisphere;
   BIRD_FILT   mFilter;

   std::string mPort;
   std::string mCalibrationFileName;

   vpr::SerialPort* mSerialPort;
   int     mBaud;
   int     mSyncStyle;
   bool    mBlocking;
   bool    mExtendedRange;
   int     mNumBirds;
   int     mXmitterUnitNumber;
   bool    mUsingCorrectionTable;


   //    x,y,z,        r,y,p
   float mPosition[MAX_SENSORS][3], mOrientation[MAX_SENSORS][3];

   bool mActive;

   const int mSleepFactor;

private:
   vpr::ReturnStatus openPort(void);
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
