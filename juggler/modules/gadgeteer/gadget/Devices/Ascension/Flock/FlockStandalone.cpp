/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <iostream>        // for std::cout
#include <fstream>         // for std::ifstream
#include <string.h>

#include <vpr/vpr.h>
#include <vpr/Util/Assert.h> /* for vprASSERT */
#include <vpr/System.h>

#include <gadget/Devices/Ascension/FlockStandalone.h>


//: Configure Constructor
// Give:                                                 <BR>
//   port - such as "/dev/ttyd3"                         <BR>
//   baud - such as 38400, 19200, 9600, 14400, etc...    <BR>
//   sync - sync type.                                   <BR>
//   block - blocking                                    <BR>
//   numBrds - number of birds in flock,                 <BR>
//   transmit - transmitter unit number,                 <BR>
//   hemi - hemisphere to track from,                    <BR>
//   filt - filtering type,                              <BR>
//   report -                                            <BR>
//   calfile - a calibration file, if "", then use none. <BR>
//                                                       <BR>
// Result: configures internal data members,
//         doesn't actually talk to the FOB yet.
FlockStandalone::FlockStandalone(const char* const port, const int& baud,
                                 const int& sync, const bool& block,
                                 const int& numBrds, const int& transmit,
                                 const BIRD_HEMI& hemi, const BIRD_FILT& filt,
                                 const char& report, const char* const calfile)
   : mReportRate(report), mHemisphere(hemi), mFilter( filt ), mBaud(baud),
     mSyncStyle(sync), mBlocking(block), mNumBirds(numBrds),
     mXmitterUnitNumber(transmit), mUsingCorrectionTable(false),
     mActive(false), mSleepFactor(3)
{
   if ( port != NULL )
   {
      mPort = port;
      mSerialPort = new vpr::SerialPort(mPort);
   }
   else
   {
      mSerialPort = NULL;
   }

   // fix the report rate if it makes no sense.
   if ( (mReportRate != 'Q') && (mReportRate != 'R') &&
        (mReportRate != 'S') && (mReportRate != 'T') )
   {
      // illegal report rate, defaulting to "every other cycle" (R)
      vprASSERT(false);
      mReportRate = 'R';
   }

   if ( calfile != NULL && calfile[0] != '\0' )
   {
      mCalibrationFileName = calfile;
      this->initCorrectionTable(mCalibrationFileName);
      mUsingCorrectionTable = true;
   }
}

//: Destructor
FlockStandalone::~FlockStandalone()
{
   if ( mActive )
   {
      this->stop();
   }

   if ( mSerialPort != NULL )
   {
      delete mSerialPort;
      mSerialPort = NULL;
   }
}

//: see if the flock is active or not
const bool& FlockStandalone::isActive() const
{
   return mActive;
}

//: set the port to use
//  this will be a string in the form of the native OS descriptor <BR>
//  ex: unix - "/dev/ttyd3", win32 - "COM3" <BR>
//  NOTE: flock.isActive() must be false to use this function
void FlockStandalone::setPort(const std::string& serialPort)
{
   if ( mActive )
   {
      std::cout << "Flock: Cannot change the serial Port while active\n";
      return;
   }

   if ( ! serialPort.empty() )
   {
      mPort = serialPort;

      if ( mSerialPort != NULL )
      {
         delete mSerialPort;
      }

      mSerialPort = new vpr::SerialPort(mPort);
   }
}

//: get the port used
//  this will be a string in the form of the native OS descriptor <BR>
//  ex: unix - "/dev/ttyd3", win32 - "COM3"
const std::string& FlockStandalone::getPort() const
{
   return mPort;
}

//: set the baud rate
//  this is generally 38400, consult flock manual for other rates. <BR>
//  NOTE: flock.isActive() must be false to use this function
void FlockStandalone::setBaudRate(const int& baud)
{
   if ( mActive )
   {
      std::cerr << "Flock: Cannot change the baud rate while active\n";
      return;
   }
   else
   {
      mBaud = baud;
   }
}

//: call this to connect to the flock device.
//  NOTE: flock.isActive() must be false to use this function
int FlockStandalone::start()
{
   int retval;

   if ( ! mActive )
   {
      if ( openPort() == -1 )
      {
         retval = 0;
      }
      else
      {
         std::cout << "[FlockStandalone] Setting blocking state ("
                   << (mBlocking ? "" : "non-") << "blocking)\n"
                   << std::flush;
         setBlocking();
         vpr::System::sleep(1);

         std::cout << "[FlockStandalone] Setting sync\n" << std::flush;
         setSync();
         vpr::System::sleep(1);

         std::cout << "[FlockStandalone] Setting group\n" << std::flush;
         setGroup();
         vpr::System::sleep(1);

         std::cout << "[FlockStandalone] Setting autoconfig\n" << std::flush;
         setAutoconfig();
         vpr::System::sleep(1);

         std::cout << "[FlockStandalone] Setting transmitter\n" << std::flush;
         setTransmitter();
         vpr::System::sleep(1);

         std::cout << "[FlockStandalone] Setting filter\n" << std::flush;
         setFilter();
         vpr::System::sleep(1);

         std::cout << "[FlockStandalone] Setting hemisphere\n" << std::flush;
         setHemisphere();
         vpr::System::sleep(1);

         std::cout << "[FlockStandalone] Setting pos_angles\n" << std::flush;
         setPosAngles();
         vpr::System::sleep(1);

         std::cout << "[FlockStandalone] Setting pickBird\n" << std::flush;
         pickBird(mXmitterUnitNumber);
         vpr::System::sleep(1);

         std::cout << "[FlockStandalone] Setting rep_and_stream\n" << std::flush;
         setRepAndStream();
         vpr::System::sleep(1);

         std::cout  << "[FlockStandalone] Ready to go!\n\n" << std::flush;

         // flock is active.
         mActive = true;

         // return success
         retval = 1;
      }
   }
   else
   {
      retval = 0; // already sampling
   }

   return retval;
}

//: call this repeatedly to update the data from the birds.
//  NOTE: flock.isActive() must be true to use this function
int FlockStandalone::sample()
{
   // can't sample when not active.
   vprASSERT( mActive == true );
   int i;
   int loopCount = mNumBirds + 1;
   if ( mXmitterUnitNumber <= mNumBirds )
   {
      loopCount++;
   }

   // for [1..n] birds, get their reading:
   int j = 0;
   for ( i=1; i < loopCount && i < MAX_SENSORS; i++ )
   {
      j++;
// If the transmitter number is less than or equal to the number of birds, we need to ignore it.

      if ( i == mXmitterUnitNumber )
      {
         j--;
         continue;
      }

// However, we need to still copy the data into consecutive values in the wrapper class, so we
// introduce "j" to account for that correction.  It is equal to "i" while we haven't encountered
// the transmitter, but equal to "i-1" afterwards.

      // you can never go above the maximum number of sensors.
      vprASSERT( i < MAX_SENSORS );
      getReading(i, xPos(j), yPos(j), zPos(j), zRot(j), yRot(j), xRot(j));

      if ( mUsingCorrectionTable )
      {
         this->positionCorrect( this->xPos(j),
                                this->yPos(j),
                                this->zPos(j) );
      }
   }

   return 1;
}

//: stop the flock
int FlockStandalone::stop ()
{
   int retval;
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char bird_command[4];

      std::cout << "[FlockStandalone] Stopping the flock ...\n" << std::flush;

      bird_command[0] = 'B';
      mSerialPort->write(bird_command, 1, written);
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      vpr::System::usleep(500 * mSleepFactor);

      bird_command[0] = 'G';
      mSerialPort->write(bird_command, 1, written);
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      vpr::System::usleep(200 * FlockStandalone::mSleepFactor);

      mSerialPort->close();

      // flock is not active now.
      mActive = false;

      std::cout << "[FlockStandalone] Stopped." << std::endl << std::flush;

      retval = 1;
   }
   else
   {
      retval = 0;
   }

   return retval;
}

//: Set the hemisphere that the transmitter transmits from.
//  NOTE: flock.isActive() must be false to use this function
void FlockStandalone::setHemisphere( const BIRD_HEMI& h )
{
   if ( mActive )
   {
      std::cout << "Flock: Cannot change the hemisphere\n" << std::flush;
      return;
   }
   else
   {
      // Set it.
      mHemisphere = h;
   }
}

//: Set the type of filtering that the flock uses
//  NOTE: flock.isActive() must be false to use this function
void FlockStandalone::setFilterType( const BIRD_FILT& f )
{
   if ( mActive )
   {
      std::cout << "Flock: Cannot change filter type while active\n"
                << std::flush;
      return;
   }
   else
   {
      // Set it.
      mFilter = f;
   }
}

//: Set the report rate that the flock uses
//  NOTE: flock.isActive() must be false to use this function
void FlockStandalone::setReportRate( const char& rRate )
{
   if ( mActive )
   {
      std::cout << "Flock: Cannot change report rate while active\n"
                << std::flush;
      return;
   }
   else
   {
      // Set it.
      mReportRate = rRate;
   }
}

//: Set the unit number of the transmitter
//  give - an integer that is the same as the dip switch
//         setting on the transmitter box (for the unit number) <BR>
//  NOTE: flock.isActive() must be false to use this function
void FlockStandalone::setTransmitter( const int& Transmit )
{
   if ( mActive )
   {
      std::cout << "Flock: Cannot change transmitter while active\n"
                << std::flush;
      return;
   }
   else
   {
      // Set it.
      mXmitterUnitNumber = Transmit;
   }
}

//: Set the number of birds to use in the flock.
//  give - an integer number not more than the number of
//         birds attached to the system <BR>
//  NOTE: flock.isActive() must be false to use this function
void FlockStandalone::setNumBirds( const int& n )
{
   if ( mActive )
   {
      std::cout << "Flock: Cannot change num birds while active\n"
                << std::flush;
      return;
   }
   else
   {
      // Set it.
      mNumBirds = n;
   }
}

//: set the video sync type
//  this option allows the Flock to syncronize its pulses with
//  your video display.  This will eliminate most flicker caused
//  by the magnetic distortion. <BR>
//  - Refer to your flock manual for what number to use.
//  NOTE: flock.isActive() must be false to use this function
void FlockStandalone::setSync(const int& sync)
{
   if ( mActive )
   {
      std::cout << "Flock: Cannot change report rate while active\n"
                << std::flush;
      return;
   }
   else
   {
      // Set it.
      mSyncStyle = sync;
   }
}

//: set blocking of flock
//  see flock manual for details.
//  NOTE: flock.isActive() must be false to use this function
void FlockStandalone::setBlocking( const bool& blVal )
{
   if ( mActive )
   {
      std::cout << "Flock: Cannot change blocking while active\n" << std::flush;
      return;
   }
   else
   {
      // Set it.
      mBlocking = blVal;
   }
}

//: with the calibration table info, correct a given position
//  give - a position in x, y, z euclidian coordinates
//  returns - a corrected position in x, y, z euclidian coordinates
void FlockStandalone::positionCorrect( float& x, float& y, float& z )
{
   int xlo,ylo,zlo,xhi,yhi,zhi;
   float a,b,c,a1,b1,c1;

   // Find corners
   xlo = (int)(x-mCalTable.xmin);
   ylo = (int)(y-mCalTable.ymin);
   zlo = (int)(z-mCalTable.zmin);
   xhi = xlo+1;
   yhi = ylo+1;
   zhi = zlo+1;
   a1 = 1 - (a = mCalTable.xmin + xhi - x);
   b1 = 1 - (b = mCalTable.ymin + yhi - y);
   c1 = 1 - (c = mCalTable.zmin + zhi - z);

   x = a * b* c*mCalTable.xloc[xlo][ylo][zlo] +
       a * b*c1*mCalTable.xloc[xlo][ylo][zhi] +
       a *b1* c*mCalTable.xloc[xlo][yhi][zlo]+
       a *b1*c1*mCalTable.xloc[xlo][yhi][zhi]+
       a1* b* c*mCalTable.xloc[xhi][ylo][zlo]+
       a1* b*c1*mCalTable.xloc[xhi][ylo][zhi]+
       a1*b1* c*mCalTable.xloc[xhi][yhi][zlo]+
       a1*b1*c1*mCalTable.xloc[xhi][yhi][zhi];
   y = a * b* c*mCalTable.yloc[xlo][ylo][zlo] +
       a * b*c1*mCalTable.yloc[xlo][ylo][zhi] +
       a *b1* c*mCalTable.yloc[xlo][yhi][zlo]+
       a *b1*c1*mCalTable.yloc[xlo][yhi][zhi]+
       a1* b* c*mCalTable.yloc[xhi][ylo][zlo]+
       a1* b*c1*mCalTable.yloc[xhi][ylo][zhi]+
       a1*b1* c*mCalTable.yloc[xhi][yhi][zlo]+
       a1*b1*c1*mCalTable.yloc[xhi][yhi][zhi];
   z = a * b* c*mCalTable.zloc[xlo][ylo][zlo] +
       a * b*c1*mCalTable.zloc[xlo][ylo][zhi] +
       a *b1* c*mCalTable.zloc[xlo][yhi][zlo]+
       a *b1*c1*mCalTable.zloc[xlo][yhi][zhi]+
       a1* b* c*mCalTable.zloc[xhi][ylo][zlo]+
       a1* b*c1*mCalTable.zloc[xhi][ylo][zhi]+
       a1*b1* c*mCalTable.zloc[xhi][yhi][zlo]+
       a1*b1*c1*mCalTable.zloc[xhi][yhi][zhi];
   return;
}

//: init the correction table from a file
//  give - a file name of the calibration file.
//  result - initializes the correction table with the file's info
void FlockStandalone::initCorrectionTable( const std::string& fName )
{
   int i,j,k, xsize,ysize,zsize;
   float dump;
   std::ifstream inFile;

   std::cout << "      Initializing calibration table ... " << std::endl
             << "        " << fName << std::endl << std::flush;
   inFile.open( fName.c_str() );
   if ( !inFile )
   {
      std::cout << "Unable to open calibration.table\n" << std::flush;
      return;
   }
   else
   {
      std::cout << "   Calibration table opened sucessfully." << std::endl
                << std::flush;
   }

   inFile >> mCalTable.xmin >> mCalTable.xmax
          >> mCalTable.ymin >> mCalTable.ymax
          >> mCalTable.zmin >> mCalTable.zmax;

   xsize = (int) (mCalTable.xmax - mCalTable.xmin) + 1;
   ysize = (int) (mCalTable.ymax - mCalTable.ymin) + 1;
   zsize = (int) (mCalTable.zmax - mCalTable.zmin) + 1;

   for ( i = 0; i < xsize; i++ )
   {
      for ( j = 0; j < ysize; j++ )
      {
         for ( k = 0; k < zsize; k++ )
         {
            inFile >> dump >> dump >> dump
                   >> mCalTable.xloc[i][j][k]
                   >> mCalTable.yloc[i][j][k]
                   >> mCalTable.zloc[i][j][k];
         }
      }
   }

   inFile.close();
}

float& FlockStandalone::xPos( const int& i )
{
   vprASSERT( i < MAX_SENSORS );
   return mPosition[i][0];
}

float& FlockStandalone::yPos( const int& i )
{
   vprASSERT( i < MAX_SENSORS );
   return mPosition[i][1];
}

float& FlockStandalone::zPos( const int& i )
{
   vprASSERT( i < MAX_SENSORS );
   return mPosition[i][2];
}

float& FlockStandalone::zRot( const int& i )
{
   vprASSERT( i < MAX_SENSORS );
   return mOrientation[i][0];
}

float& FlockStandalone::yRot( const int& i )
{
   vprASSERT( i < MAX_SENSORS );
   return mOrientation[i][1];
}

float& FlockStandalone::xRot( const int& i )
{
   vprASSERT( i < MAX_SENSORS );
   return mOrientation[i][2];
}

///////////////////////////////////////////////////////////////////
// Local functions to FlockStandalone.cpp
//////////////////////////////////////////////////////////////////
//: get a reading
//  give - n:    the bird unit number  <BR>
//  give - port: the flock port number <BR>
//  give - xyz positions               <BR>
//  give - zyx rotations
int FlockStandalone::getReading (const int& n, float& xPos, float& yPos,
                                 float& zPos, float& zRot, float& yRot,
                                 float& xRot)
{
   int addr;
   vpr::Uint64 timeout=10000;  // How long to wait for data to arrive
   vpr::Uint32 num_read;
   if ( mSerialPort != NULL )
   {
      char buff[12], group;
      int  c, i;

      do
      {
         c = i = 0;

         while ( ! i && c < 99999 )
         {
            c++;
            mSerialPort->readn(&buff[0], 1, num_read,
                               vpr::Interval(timeout, vpr::Interval::Msec));
            if ( (num_read == 1) )
            {
               i = 1;
            }
         }


         while ( i != 12 && c < 99999 )
         {
            c++;
            mSerialPort->read(&buff[i], 12 - i, num_read,
                              vpr::Interval(timeout, vpr::Interval::Msec));
            i += num_read;
         }

         mSerialPort->read(&group, 1, num_read,
                           vpr::Interval(timeout, vpr::Interval::Msec));
         while ( (num_read == 0) &&
                 (c < 99999) )
         {
            vpr::System::usleep(100 * mSleepFactor);
            c++;
            mSerialPort->read(&group, 1, num_read,
                              vpr::Interval(timeout, vpr::Interval::Msec));
         }

         addr = group;
      } while ( addr != n );

      //std::cout << "addr: " << addr << std::endl;

      // Position
      xPos = rawToFloat(buff[1], buff[0]) * POSITION_RANGE;
      yPos = rawToFloat(buff[3], buff[2]) * POSITION_RANGE;
      zPos = rawToFloat(buff[5], buff[4]) * POSITION_RANGE;


      // Orientation
      zRot = rawToFloat(buff[7], buff[6])   * ANGLE_RANGE;
      yRot = rawToFloat(buff[9], buff[8])   * ANGLE_RANGE;
      xRot = rawToFloat(buff[11], buff[10]) * ANGLE_RANGE;
   }
   else
   {
      addr = -1;
   }

   return addr;
}

float FlockStandalone::rawToFloat (char& r1, char& r2)
{
   // return ((float) (((r1 & 0x7f) << 9) | (r2 & 0x7f) << 2) / 0x7fff);

   short int ival1,ival2,val;
   ival1 = r1 & 0x7f;
   ival2 = r2 & 0x7f;
   val = (ival1 << 9) | ival2<<2;
   return((float)val) / 0x7fff;
}

void FlockStandalone::pickBird (const int birdID)
{
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff = 0xF0 + birdID;

      mSerialPort->write(&buff, sizeof(buff), written);
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

      vpr::System::usleep(100 * mSleepFactor);
   }
}

// Open the port.
//  give - a serial port
//  give - a baud rate
//  returns portId twice (created by the open function)
//  NOTE: portId is returned from both ends of this function.
//  if portId == -1 then function failed to open the port.
int FlockStandalone::openPort ()
{
   int retval;

   ///////////////////////////////////////////////////////////////////
   // Open and close the port to reset the tracker, then
   // Open the port
   ///////////////////////////////////////////////////////////////////
   if ( mSerialPort != NULL )
   {
      mSerialPort->setOpenReadWrite();


      if ( ! mSerialPort->open().success() )
      {
         std::cerr << "[FlockStandalone] Port reset failed (because port "
                   << "open failed)\n";
         retval = -1;
      }
      else
      {
         vpr::System::sleep(2);
         mSerialPort->close();

         std::cout << "[FlockStandalone] Port reset successfully (port was "
                   << "opened then closed)\n" << std::flush;

         if ( ! mSerialPort->open().success() )
         {
            std::cerr << "[FlockStandalone] Port open failed\n";
            retval = -1;
         }
         else
         {
            mSerialPort->clearAll();
            mSerialPort->enableRead();
            mSerialPort->enableLocalAttach();
            mSerialPort->enableBreakByteIgnore();

            std::cout << "[FlockStandalone] Port opened successfully\n"
                      << std::flush;

            mSerialPort->setUpdateAction(vpr::SerialTypes::NOW);

            // Setup the port.
            std::cout << "[FlockStandalone] Setting new baud rate: "
                      << mBaud << " bits per second\n" << std::flush;
            mSerialPort->setInputBaudRate(mBaud);

            std::cout << "Setting output baud rate\n" << std::flush;
            mSerialPort->setOutputBaudRate(mBaud);

            std::cout << "Setting character size\n" << std::flush;
            mSerialPort->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
/*
                std::cout << "Setting local attachment\n" << std::flush;
                mSerialPort->enableLocalAttach();
*/
            std::cout << "Setting read stuff\n" << std::flush;
            if ( mSerialPort->enableRead().failure() )
            {
               retval = -1;
            }
         }
      }
   }
   else
   {
      std::cerr << "[FlockStandalone] ERROR -- No port has been set in FlockStandalone::open_port()!\n";
      retval = -1;
   }

   return retval;
}

void FlockStandalone::setBlocking ()
{
   //////////////////////////////////////////////////////////////////
   // Setup a non/blocked port & Flush port
   //////////////////////////////////////////////////////////////////
   if ( mSerialPort != NULL )
   {
      if ( mBlocking )
      {
         mSerialPort->enableBlocking();
         std::cerr << "enable Blocking" << std::endl;
      }
      else
      {
         mSerialPort->enableNonBlocking();
      }

      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

      vpr::System::usleep(1000 * mSleepFactor);
   }
}

void FlockStandalone::setSync ()
{
   /////////////////////////////////////////////////////////////////
   // Set CRT sync: (manual page 82)
   //   set crt sync
   //   nosync    -   0
   //   > 72Hz    -   1 (type 1)
   //                 2 (type 2)
   /////////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      unsigned char buff[4] = {'A', 1};
      buff[1] = mSyncStyle;

      mSerialPort->write(buff, 2, written);
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   }
}

void FlockStandalone::setHemisphere()
{
   /////////////////////////////////////////////////////////////////
   // Set Hemisphere for birds taking input
   //
   //  buff   [1]   [2]
   // Front: 0x00, 0x00
   // Aft  : 0x00, 0x01
   // Upper: 0x0C, 0x01
   // Lower: 0x0C, 0x00
   // Left : 0x06, 0x01
   // Right: 0x06, 0x00
   /////////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[3];

      buff[0] = 'L';

      // This will loop over all the birds and the transmitter.  When it
      // encounters the transmitter, it skips it.
      for ( int i = 1; i <= (mNumBirds + 1); i++ )
      {
         // Skip the transmitter.
         if ( i == mXmitterUnitNumber )
         {
            continue;
         }

         pickBird(i);

         switch ( mHemisphere )
         {
            case FRONT_HEM:
               buff[1] = 0x00;
               buff[2] = 0x00;
               break;
            case AFT_HEM:
               buff[1] = 0x00;
               buff[2] = 0x01;
               break;
            case UPPER_HEM:
               buff[1] = 0x0C;
               buff[2] = 0x01;
               break;
            case LOWER_HEM:
               buff[1] = 0x0C;
               buff[2] = 0x00;
               break;
            case LEFT_HEM:
               buff[1] = 0x06;
               buff[2] = 0x01;
               break;
            case RIGHT_HEM:
               buff[1] = 0x06;
               buff[2] = 0x00;
               break;
         }

         mSerialPort->write(buff, sizeof(buff), written);
         mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

         vpr::System::usleep(500 * mSleepFactor);
      }
   }
}

void FlockStandalone::setRepAndStream ()
{
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[1];
      /////////////////////////////////////////////////////////////////
      // Set report rate
      //             Q  Every cycle
      //  buff[0] - 'R' Every other bird cycle
      //             S  every 8 cycles
      //             T  every 32 cycles
      /////////////////////////////////////////////////////////////////
      buff[0] = mReportRate;
      mSerialPort->write(buff, sizeof(buff), written);
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      vpr::System::usleep(2000 * mSleepFactor);

      ////////////////////////////////////////////////////////////////
      // set stream mode
      ////////////////////////////////////////////////////////////////
      buff[0] = '@';
      mSerialPort->write(buff, sizeof(buff), written);
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      vpr::System::usleep(500 * mSleepFactor);
   }
}

void FlockStandalone::setPosAngles ()
{
   //////////////////////////////////////////////////////////////////
   // Set Position Angles
   /////////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[1];

      for ( int i = 1; i < (mNumBirds + 1); i++ )
      {
         // Skip the transmitter unit.
         if ( i == mXmitterUnitNumber )
         {
            continue;
         }

         pickBird(i);

         buff[0] = 'Y';
         mSerialPort->write(buff, sizeof(buff), written);
         mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

         vpr::System::usleep(500 * mSleepFactor);
      }
   }
}

void FlockStandalone::setFilter ()
{
   ///////////////////////////////////////////////////////////////
   // Turn filters on (manual page 48)
   // 0s turn AC NARROW notch filter ON
   //         AC WIDE notch filter ON
   //         DC filter ON
   ///////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[4];

      buff[0] = 'P';
      buff[1] = 0x04;
      buff[2] = 0x00;
      buff[3] = 0;

      mSerialPort->write(buff, sizeof(buff), written);
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

      //TODO: Do I need to sleep here?
      vpr::System::usleep(12000 * mSleepFactor);
   }
}

void FlockStandalone::setTransmitter ()
{
   ///////////////////////////////////////////////////////////////
   // Sets up the device for Transmitting (manual page 67)
   // Command (0x30) for Next Transmitter
   ///////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[2];

      buff[0] = (unsigned char) (0x30);
      buff[1] = (unsigned char) mXmitterUnitNumber << 4;

      mSerialPort->write(buff, sizeof(buff), written);
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

      vpr::System::usleep(12000 * FlockStandalone::mSleepFactor);
   }
}

void FlockStandalone::setAutoconfig ()
{
   ///////////////////////////////////////////////////////////////
   // FBB AUTO-CONFIGURATION (manual page 60)
   //
   // Must wait 300 milliseconds before and after this command
   ///////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[3];

      buff[0] = 'P';
      buff[1] = 0x32;
      buff[2] = mNumBirds + 1;  //number of input devices + 1 for transmitter

      vpr::System::sleep(3);
      mSerialPort->write(buff, sizeof(buff), written);
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      vpr::System::sleep(2);
   }
}

void FlockStandalone::setGroup ()
{
   ////////////////////////////////////////////////////////////////
   // Setup group mode: (manual page 59)
   // 'P' Change Parameter
   // Number 35 (hex 23),
   // Set flag to 1 enabling group mode.
   ////////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[3];

      buff[0] = 'P';
      buff[1] = 0x23;
      buff[2] = 0x01;

      mSerialPort->write(buff, sizeof(buff), written);
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      vpr::System::sleep(2);
   }
}

/****************************************************
 * Begin testing functions
 *
 ****************************************************/

void FlockStandalone::checkGroup()
{
   char exam1[1], exam2[1];
   exam1[0] = 'O';
   exam2[0] = 0x23;
   vpr::Uint32 buf=0, buf1=0;
   char in[2];
   in[0]='a';


   while ( true )
   {
      mSerialPort->write(exam1, sizeof(char), buf);
      mSerialPort->write(exam2, sizeof(char), buf);

      mSerialPort->readn(in, sizeof(char), buf1);
      std::cout << in[0] << " " << buf << " " << buf1 << std::endl;
   }
}

void FlockStandalone::checkConfig()
{
   if ( mSerialPort != NULL )
   {

      char exam1[1], exam2[1];
      exam1[0] = 'O';
      exam2[0] = 0x0;
      vpr::Uint32 buf=0, buf1=0;
      char in[2] = {'a','a'};
      int i = 0;

//   clearBuffer();

      while ( i < 400 )
      {
         i++;
         mSerialPort->write(exam1, sizeof(char), buf);
         mSerialPort->write(exam2, sizeof(char), buf);
         mSerialPort->readn(&in, sizeof(char)*2, buf1);
         showbits(in[1]); std::cout << "|"; showbits(in[0]);
         std::cout << std::endl;
      }
   }
}

void FlockStandalone::checkPosAngles()
{
   return;
}

void FlockStandalone::checkRepAndStream()
{
   return;
}

void FlockStandalone::showbits(char var)
{
   static unsigned mask[8]={1,2,4,8,16,32,64,128};

   unsigned bitval;
   for ( int bit=7; bit>=0; bit-- )
   {
      bitval=mask[bit]&var;
      printf("%c",bitval?'1':'0');
   }
}

int FlockStandalone::hexToInt(char var)
{
   static unsigned mask[8]={1,2,4,8,16,32,64,128};
   int bitval=0;
   for ( int bit=7; bit>=0; bit-- )
   {
      bitval+=mask[bit]&var;
   }
   return bitval;
}

void FlockStandalone::checkDataReadyChar()
{
   char exam1[1], exam2[1];
   exam1[0] = 'O';
   exam2[0] = 0x9;
   vpr::Uint32 buf=0, buf1=0;
   char in[2] = {'a','a'};
   int i = 0;

   // Initial exam
   do
   {
      i++;
      mSerialPort->write(exam1, sizeof(char), buf);
      mSerialPort->write(exam2, sizeof(char), buf);
      mSerialPort->readn(&in, sizeof(char)*2, buf1);
   } while ( in[0] != ',' );

   std::cout << "I=" << i << std::endl;
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   vpr::System::usleep(500 * mSleepFactor);
}


void FlockStandalone::examineValue(char exam, int data, int reps, int format)
{
   char exam1[1];
   char exam2[1];
   exam1[0] = 'O';
   exam2[0] = exam;
   vpr::Uint32 buf=0;
   char in[1] = {'a'};
   int i = 0, j = 0;


   i = 1;
   while ( i < reps )
   {
      i++;
      mSerialPort->write(exam1, sizeof(char), buf);
      mSerialPort->write(exam2, sizeof(char), buf);
      while ( j < data )
      {
         mSerialPort->readn(in, sizeof(char), buf);
         //mSerialPort->readn(&in, sizeof(char), buf1);
         if ( format == 1 )
         {
            std::cout << in[0];
         }
         else
         {
            showbits(in[0]);
         }

         std::cout << std::endl;
         j++;
      }
      j = 0;
      // mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      // vpr::System::usleep(500 * mSleepFactor);
   }
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   vpr::System::usleep(500 * mSleepFactor);
}


/***********************************************************
 * Here are some additional methods for setting values of
 * the flock.
 **********************************************************/

void FlockStandalone::setRunMode()
{
   char exam1[1];
   exam1[0] = 'F';  // Put the flock into RUN mode
   vpr::Uint32 buf=0;

   for ( int i = 0; i < 150; i++ )
   {
      mSerialPort->write(exam1, sizeof(char), buf);
   }
}

void FlockStandalone::setSleepMode()
{
   char exam1[1];
   exam1[0] = 'G';  // Put the flock into SLEEP mode
   vpr::Uint32 buf=0;

   for ( int i = 0; i < 150; ++i )
   {
      mSerialPort->write(exam1, sizeof(char), buf);
   }
}

void FlockStandalone::setValue(char exam, char setdata, int reps)
{
   char exam1[1];
   char exam2[1];
   char data[1];
   exam1[0] = 'P';
   exam2[0] = exam;
   data[0] = setdata;
   vpr::Uint32 buf=0;
   int i = 1;

   while ( i < reps )
   {
      i++;
      mSerialPort->write(exam1, sizeof(char), buf);
      mSerialPort->write(exam2, sizeof(char), buf);
      mSerialPort->write(data, sizeof(char), buf);
   }

   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   vpr::System::usleep(500 * mSleepFactor);
}

void FlockStandalone::setPosMode()
{
   char exam3[1];
   exam3[0] = 0x56;  // Position mode
   vpr::Uint32 buf=0;
   int i = 0;

   while ( i < 150 )
   {
      i++;
      mSerialPort->write(exam3, sizeof(char), buf);
   }

}

void FlockStandalone::setPosAngMode()
{
   char exam3[1];
   exam3[0] = 0x59;  // Position/angle mode
   vpr::Uint32 buf=0;
   int i = 0;

   while ( i < 150 )
   {
      i++;
      mSerialPort->write(exam3, sizeof(char), buf);
   }
}

void FlockStandalone::setStreamMode()
{
   char exam1[1];
   exam1[0] = '@';  // Put the flock into STREAM mode
   vpr::Uint32 buf=0;

   for ( int i = 0; i < 150; i++ )
   {
      mSerialPort->write(exam1, sizeof(char), buf);
   }
}

void FlockStandalone::setPointMode()
{
   char exam1[1];
   exam1[0] = 'B';  // Put the flock into STREAM mode
   vpr::Uint32 buf=0;

   for ( int i = 0; i < 150; ++i )
   {
      mSerialPort->write(exam1, sizeof(char), buf);
   }
}

void FlockStandalone::setDeviceReportRate(char rate)
{
   char exam1[1];
   vpr::Uint32 buf=0;

   switch ( rate )
   {
      case 'Q':
         std::cout << "setting report rate to every cycle" << std::endl;
         exam1[0] = 'Q';
         break;
      case 'R':
         std::cout << "setting report rate to every other cycle" << std::endl;
         exam1[0] = 'R';
         break;
      case 'S':
         std::cout << "setting report rate to every 8th cycle" << std::endl;
         exam1[0] = 'S';
         break;
      case 'T':
         std::cout << "setting report rate to every 32nd cycle" << std::endl;
         exam1[0] = 'T';
         break;
   }

   for ( int i = 0; i < 500; ++i )
   {
      mSerialPort->write(exam1, sizeof(char), buf);
   }
}

/*
void FlockStandalone::clearBuffer()
{
  vpr::Uint32 buf1;
  vpr::IOSys::Handle handle = mSerialPort->getHandle();
  int bytes;
  char in[1];

  do
  {
     mSerialPort->readn(in, sizeof(in), buf1);
    ioctl(handle, FIONREAD, &bytes);
  } while(bytes != 0);
}
*/
