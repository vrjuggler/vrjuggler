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

#include <iostream>        // for std::cout
#include <fstream>         // for std::ifstream
#include <string.h>

#include <vpr/vpr.h>
#include <vpr/Util/Assert.h> /* for vprASSERT */
#include <gadget/Util/Debug.h>
#include <vpr/System.h>

#include <gadget/Devices/Ascension/Flock/FlockStandalone.h>


/** Configure constructor. */
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

/** Checks if the flock is active. */
const bool& FlockStandalone::isActive() const
{
   return mActive;
}

/** Sets the port to use. */
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

/** Gets the port used. */
const std::string& FlockStandalone::getPort() const
{
   return mPort;
}

/** Sets the baud rate. */
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

/** Call this to connect to the Flock device. */
vpr::ReturnStatus FlockStandalone::start()
{
   int retval;
   
   if ( ! mActive )
   {
      if ( openPort() == vpr::ReturnStatus::Fail )
      {
         vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] **** ERROR, can't open serial port: " <<  mPort << " ****\n" << vprDEBUG_FLUSH;
         return(vpr::ReturnStatus::Fail);
      }
      else
      {
         vpr::Uint32 written;
         char stop_command;
         
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) 
            << "============================================================================\n" << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) 
            << " NOTE: Version number below might be wrong if we need to restart the flock!\n" << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) 
            << "============================================================================\n" << vprDEBUG_FLUSH;

         this->readSoftwareRevision();
         
         vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] Restarting the flock.\n" << vprDEBUG_FLUSH;
      
         //vpr::System::msleep(500);
         stop_command = 'B';
         mSerialPort->write(&stop_command, 1, written);
      
         //vpr::System::msleep(500);
         stop_command = 'G';
         mSerialPort->write(&stop_command, 1, written);
      
         mSerialPort->close();
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)  << " [FlockStandalone] Flock has been Stopped." << std::endl << vprDEBUG_FLUSH;
         vpr::System::sleep(3);
      }
      
      if ( openPort() == vpr::ReturnStatus::Fail )
      {
         vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] **** ERROR, can't open serial port: " <<  mPort << " ****\n" << vprDEBUG_FLUSH;
         return(vpr::ReturnStatus::Fail);
      }
      else
      {
         this->readSoftwareRevision();
         this->readSystemModel();
         this->readStatus();

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting Serial Port blocking state ("
                   << (mBlocking ? "" : "non-") << "blocking)\n"
                   << vprDEBUG_FLUSH;
         setBlocking();
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting sync\n" << vprDEBUG_FLUSH;
         sendSync();
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting group\n" << vprDEBUG_FLUSH;
         sendGroup();
         //vpr::System::sleep(1);
         
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting hemisphere\n" << vprDEBUG_FLUSH;
         sendHemisphere();
         //vpr::System::sleep(1);
         
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting autoconfig\n" << vprDEBUG_FLUSH;
         sendAutoconfig();
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting transmitter\n" << vprDEBUG_FLUSH;
         sendTransmitter();
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting filter\n" << vprDEBUG_FLUSH;
         sendFilter();
         //vpr::System::sleep(1);


         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting pos_angles\n" << vprDEBUG_FLUSH;
         sendPosAngles();
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting pickBird\n" << vprDEBUG_FLUSH;
         pickBird(mXmitterUnitNumber);
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting rep_and_stream\n" << vprDEBUG_FLUSH;
         sendRepAndStream();
         //vpr::System::sleep(1);
         
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Ready to go!\n\n" << vprDEBUG_FLUSH;
         
         // flock is active.
         mActive = true;

         // return success
         return(vpr::ReturnStatus::Succeed);
      }
   }
   else
   {
      return(vpr::ReturnStatus::Succeed);
      retval = 0; // already sampling
   }
}

/** Call this repeatedly to update the data from the birds. */
int FlockStandalone::sample()
{
   // can't sample when not active.
   vprASSERT( mActive == true );

   int loop_count = mNumBirds;
   if (mExtendedRange)
   {
      loop_count++;
   }    
   
   int buffer_location=0;
   for (int bird_id=1;
        bird_id <= loop_count && bird_id < MAX_SENSORS;
        bird_id++)
   {
      if (! isTransmitter(bird_id))
      {
         vprASSERT( bird_id < MAX_SENSORS );
         vpr::ReturnStatus status = getReading(bird_id, xPos(buffer_location), yPos(buffer_location), zPos(buffer_location), 
                                                        zRot(buffer_location), yRot(buffer_location), xRot(buffer_location));
         if (!status.success())
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "[Flock of Birds] Error reading packet for bird #: "
                  << bird_id << "\n" << vprDEBUG_FLUSH;
         }
         if ( mUsingCorrectionTable )
         {
            this->positionCorrect( this->xPos(buffer_location),
                                   this->yPos(buffer_location),
                                   this->zPos(buffer_location) );
         }
         
         vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL) << std::endl << "\n===================================================\n" << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL) << " Position of Bird #: " << bird_id
                     << " x: " << xPos(buffer_location) 
                     << " y: " << yPos(buffer_location) 
                     << " z: " << zPos(buffer_location) << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL)   << " \nRotation of Bird #: " << bird_id
                     << " x: " << xRot(buffer_location) 
                     << " y: " << yRot(buffer_location) 
                     << " z: " << zRot(buffer_location) << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL) << "\n===================================================\n\n" << vprDEBUG_FLUSH;

         buffer_location++;
      }


   }
   return 1;
}

/** Stops the Flock. */
int FlockStandalone::stop ()
{
   int retval;
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char bird_command[4];

      std::cout << " [FlockStandalone] Stopping the flock ...\n" << std::flush;

      vpr::System::msleep(500);
      bird_command[0] = 'B';
      mSerialPort->write(bird_command, 1, written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      
      vpr::System::msleep(500);
      bird_command[0] = 'G';
      mSerialPort->write(bird_command, 1, written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      //vpr::System::usleep(200 * FlockStandalone::mSleepFactor);

      mSerialPort->close();

      // flock is not active now.
      mActive = false;

      std::cout << " [FlockStandalone] Stopped." << std::endl << std::flush;

      retval = 1;
   }
   else
   {
      retval = 0;
   }

   return retval;
}

/** Sets the hemisphere that the transmitter transmits from. */
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

/** Sets the type of filtering that the Flock uses. */
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

/** Set the report rate that the Flock uses. */
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

/** Sets the unit number of the transmitter. */
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

/** Sets the number of birds to use in the Flock. */
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

/** Sets the video sync type. */
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

/** Sets blocking of Flock. */
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

void FlockStandalone::setExtendedRange( const bool& blVal )
{
   if ( mActive )
   {
      std::cout << "Flock: Cannot extended range\n" << std::flush;
      return;
   }
   else
   {
      // Set it.
      mExtendedRange = blVal;
   }
}

/** With the calibration table info, correct a given position. */
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

/** Initializes the correction table from a file. */
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

/**
 * Gets a reading.
 * @param n    The bird unit number.
 * @param port The flock port number.
 */
vpr::ReturnStatus FlockStandalone::getReading (const int& n, float& xPos, float& yPos,
                                 float& zPos, float& zRot, float& yRot,
                                 float& xRot)
{
   vpr::Uint64 timeout=10000;  // How long to wait for data to arrive
   vpr::Uint32 num_read;
   char buff[14];

   if ( mSerialPort != NULL )
   {
      // buff[0-5] are position
      // buff[6-11] are orientation
      // buff[12] is group

      // Do until we get the unit ID we want
      // Find the beginning of a data packet
      // Read in the data packet

      // -loop until we get the packet for the bird we want
      //   -loop until we get the start of a data packet
      //     -read in the packet
      //     -convert the data into floats
      //   -end loop
      // -end loop

         // Wait until we get the data for the correct bird
      do
      {
            // Wait until the first bit in a byte is a 1, which means start of a packet      
         do
         {
            vpr::ReturnStatus status = mSerialPort->read(&buff,1,num_read,vpr::Interval(timeout, vpr::Interval::Msec));
            if (!status.success())
            {
               return(status);
            }
         } while( !(0x80 & buff[0]));
         
            // Read in the rest of the data, [ (6*2) + 1 for the group byte ] - 1 byte already read
         vpr::ReturnStatus status = mSerialPort->read(&buff[1],12,num_read,vpr::Interval(timeout, vpr::Interval::Msec));
         if (!status.success())
         {
            return(status);
         }
      } while((int)buff[12] != n);
      // buff[0] LSbyte #1
      // buff[1] MSbyte #1
      
      // buff[2] LSbyte #2
      // buff[3] MSbyte #2

      // etc.

      // Position
      
      xPos = rawToFloat(buff[1], buff[0]) * POSITION_RANGE;
      yPos = rawToFloat(buff[3], buff[2]) * POSITION_RANGE;
      zPos = rawToFloat(buff[5], buff[4]) * POSITION_RANGE;

//      std::cout   << "Float X: " << xPos
//                  << "Float Y: " << yPos
//                  << "Float Z: " << zPos << std::endl;

      // Orientation
      zRot = rawToFloat(buff[7], buff[6])   * ANGLE_RANGE;
      yRot = rawToFloat(buff[9], buff[8])   * ANGLE_RANGE;
      xRot = rawToFloat(buff[11], buff[10]) * ANGLE_RANGE;
      
      return vpr::ReturnStatus::Succeed;
   }
   else
   {
      return vpr::ReturnStatus::Fail;
   }
}

float FlockStandalone::rawToFloat (char& MSchar, char& LSchar)
{
   // return ((float) (((r1 & 0x7f) << 9) | (r2 & 0x7f) << 2) / 0x7fff);
   
   // short int ival1,ival2,val;
   // ival1 = r1 & 0x7f;                  // Set 8th bit to 0
   // ival2 = r2 & 0x7f;                  // Set 8th bit to 0
   // val = (ival1 << 9) | ival2<<2;
   //return((float)val) / 0x7fff;

   vpr::Int8 MSbyte;
   vpr::Int8 LSbyte;
   vpr::Int16 returnVal;

//   std::cout << "Bits before MS: ";
//   this->showbits(MSchar);
//   std::cout << " LS:";
//   this->showbits(LSchar);
//   std::cout << std::endl;

//   if (MSchar >= 120)
//   {
//      std::cout << "ERRORBIT";
//      std::cout << std::endl;
//   }

      // 1) Changes 8th bit from a "1" back to "0" after catching it
      //    0x7f = 01111111  so it masks out the 8th but
   MSbyte = MSchar & 0x7f;
   LSbyte = LSchar & 0x7f;
   
//   std::cout << "After 1): ";
//   this->showbits(MSbyte);
//   std::cout << " LS:";
//   this->showbits(LSbyte);
//   std::cout << std::endl;


      // 2) Shifts each LSByte left 1 bit
   LSbyte <<= 1;           // Shift the LS byte left 2 bits becasue flock data format

//   std::cout << "After 2): ";
//   this->showbits(MSbyte);
//   std::cout << " LS:";
//   this->showbits(LSbyte);
//   std::cout << std::endl;

      // 3) Combine each MSbytee/LSbyte pair
   returnVal = MSbyte;
//      std::cout << "After 3a): ";
//      this->showbits16(returnVal);
//      std::cout << std::endl;

   returnVal <<= 8;
//      std::cout << "After 3b): ";
//      this->showbits16(returnVal);
//      std::cout << std::endl;

   returnVal += LSbyte;
//      std::cout << "After 3c): ";
//      this->showbits16(returnVal);
//      std::cout << std::endl;




      // 4) Shift each word pair left 1 more bit
   returnVal <<= 1;

//   std::cout << std::endl << "------------------------------------------" << std::endl;
//   std::cout << "[vpr::Int16] Before DIV): " << returnVal  << std::endl;
//   std::cout << "[float]      Before DIV): " << (float)returnVal  << std::endl;
//   std::cout << "[float]      After DIV): " << ( (float)returnVal ) / 0x7fff  << std::endl;
//   std::cout << "------------------------------------------" << std::endl << std::endl;

   
/*   if ( vpr::System::isLittleEndian() )
   {
      returnVal = MSbyte;
//      std::cout << "ReturnVal-1: " << returnVal << std::endl;
      returnVal <<= 8;        // Shit the MS byte left 8 bits to make it the MS 
                              // and 1 more because of flock data format
//      std::cout << "ReturnVal-2: " << returnVal << std::endl;
      returnVal |= LSbyte;
      std::cout << "RAW DATA: " << returnVal << std::endl;
   }
   else if ( vpr::System::isBigEndian() )
   {
      vprASSERT(1==2);
//      MSbyte <<= 8;           // Shit the MS byte left 8 bits to make it the MS 
                              // and 1 more because of flock data format
//      returnVal = MSbyte & LSbyte;
   }
   else
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "Endianess not specified!\n" << vprDEBUG_FLUSH;
      vprASSERT(1==2);
   }

*/
   return( ( (float)returnVal ) / 0x7fff );  
}

vpr::ReturnStatus FlockStandalone::readSoftwareRevision()
{
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      vpr::Uint64 timeout=10000;  // How long to wait for data to arrive
      vpr::Uint32 num_read;
      char buff[2];
      
      
      // pickBird(1);
      
      buff[0] = 0x4F;
      buff[1] = 0x1;

      vpr::System::msleep(600);
      mSerialPort->write(&buff, sizeof(buff), written);
      
      vpr::ReturnStatus status = mSerialPort->read(&buff,2,num_read,vpr::Interval(timeout, vpr::Interval::Msec));
      if (!status.success())
      {
         return(status);
      }
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)  << " [FlockStandalone] Software Revision is: " << (int)buff[0] << "." << (int)buff[1] << std::endl << vprDEBUG_FLUSH;
      
      return(vpr::ReturnStatus::Succeed);
   }
   else
   {
      return(vpr::ReturnStatus::Fail);
   }
}

vpr::ReturnStatus FlockStandalone::readSystemModel()
{
   // Look on p. 121 of manual to get meanings
   // 6DFOB       = Stand Alone
   // 6DERC       = Extended Range Controller
   // 6DBOF       = MotionStar (old name)
   // PCBIRD      = pcBIRD
   // SPACEPAD    = SpacePad
   // MOTIONSTAR  = MotionStar (new name)
   // WIRELESS    = MotionStar Wireless

   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      vpr::Uint64 timeout=10000;  // How long to wait for data to arrive
      vpr::Uint32 num_read;
      char buff[2];
      char model_id[10];
      memset(model_id,' ',10);
      
      buff[0] = 0x4F;
      buff[1] = 0xF;

      // pickBird(1);

      vpr::System::msleep(600);
      mSerialPort->write(&buff, sizeof(buff), written);
      
      vpr::ReturnStatus status = mSerialPort->read(&model_id,10,num_read,vpr::Interval(timeout, vpr::Interval::Msec));
      if (!status.success())
      {
         return(status);
      }
      
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] System ID is: " << model_id << std::endl << vprDEBUG_FLUSH;
      
      return(vpr::ReturnStatus::Succeed);
   }
   else
   {
      return(vpr::ReturnStatus::Fail);
   }
}


   // Does not work properly
vpr::ReturnStatus FlockStandalone::readStatus(const int birdNum)
{
   
   // pickBird(birdNum);
   
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      vpr::Uint64 timeout=10000;  // How long to wait for data to arrive
      vpr::Uint32 num_read;
      char buff[2];
      
      pickBird(birdNum);
      
      buff[0] = 'O';
      buff[1] = 0;

      vpr::System::msleep(600);
      mSerialPort->write(&buff, sizeof(buff), written);

      vpr::ReturnStatus status = mSerialPort->read(&buff,2,num_read,vpr::Interval(timeout, vpr::Interval::Msec));
      if (!status.success())
      {
         return(status);
      }
      // B0-B7  use buff[0]
      // B8-B15 use buff[1]

      // buff[0]           buff[1]
      // B0 ->    0x1      <- B8
      // B1 ->    0x2      <- B9
      // B2 ->    0x4      <- B10
      // B3 ->    0x8      <- B11
      // B4 ->    0x10     <- B12
      // B5 ->    0x20     <- B13
      // B6 ->    0x40     <- B14
      // B7 ->    0x80     <- B15

      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) 
         << "================= Flock of Birds Status ===============\n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "   * " << "Mode\n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
         << ( (buff[0] & 0x1) != 0 ? "Stream\n" : "Point\n") << vprDEBUG_FLUSH;                                 // B0
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
         << ( (buff[0] & 0x20) != 0 ? "Sleep\n" : "Run\n") << vprDEBUG_FLUSH;                                   // B5
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
         << "Bird is " << ( (buff[1] & 0x10) != 0 ? "Running" : "not Running\n") << vprDEBUG_FLUSH;           // B12
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
         << ( (buff[0] & 0x40) != 0 ? "XOFF\n" : "XON\n") << vprDEBUG_FLUSH;                                    // B6
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
         << ( (buff[1] & 0x1) != 0 ? "No " : "") << "Sync mode enabled\n" << vprDEBUG_FLUSH;                  // B8
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
         << ( (buff[1] & 0x2) != 0 ? "" : "Not ") << "In CRTSYNC mode\n" << vprDEBUG_FLUSH;                   // B9
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
         << ( (buff[1] & 0x2) != 0 ? "Expanded" : "Normal ") << " Adress mode enabled\n" << vprDEBUG_FLUSH;   // B9
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
         << ( (buff[1] & 0x8) != 0 ? "" : "Not ") << "In Host Sync mode\n" << vprDEBUG_FLUSH;                 // B11
         
         // Get Output Mode
      char temp = buff[0] & 0x1E;
      temp >>= 1;
      
      switch ((int)temp)
      {
      case 1:
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
            << "POSITION Outputs selected\n" << vprDEBUG_FLUSH;
         break;
      case 2:
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
            << "ANGLE Outputs selected\n" << vprDEBUG_FLUSH;
         break;
      case 3:
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
            << "MATRIX Outputs selected\n" << vprDEBUG_FLUSH;
         break;
      case 4:
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
            << "POSITION/ANGLE Outputs selected\n" << vprDEBUG_FLUSH;
         break;
      case 5:
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
            << "POSITION/MATRIX Outputs selected\n" << vprDEBUG_FLUSH;
         break;
      case 6:
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
            << "Position Outputs selected\n" << vprDEBUG_FLUSH;
         break;
      case 7:
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
            << "QUATERNION Outputs selected\n" << vprDEBUG_FLUSH;
         break;
      case 8:
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
            << "POSITION/QUATERNION Outputs selected\n" << vprDEBUG_FLUSH;
         break;
      default:
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "     * " 
            << "INVALID/UNKNOWN Output selected\n" << vprDEBUG_FLUSH;
         break;
      }

      
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "   * " 
         << ( (buff[0] & 0x80) != 0 ? "Factory test and" : "") << "Bird commands enabled\n" << vprDEBUG_FLUSH;  // B7
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "   * " 
         << ( (buff[1] & 0x2) != 0 ? "Factory test and" : "") << "Bird commands enabled\n" << vprDEBUG_FLUSH;   // B9
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "   * " 
         << "Bird has" << ( (buff[1] & 0x40) != 0 ? "" : " not") << " been initialized\n" << vprDEBUG_FLUSH;    // B14
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "   * " 
         << "Bird is a " << ( (buff[1] & 0x80) != 0 ? "Master" : "Slave") << " Bird\n" << vprDEBUG_FLUSH;       // B15
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "   * " 
         << ( (buff[1] & 0x20) != 0 ? "" : "NO ") << "Error has been detected!\n" << vprDEBUG_FLUSH;            // B13
         
         // If an ERROR occured
      if ((buff[1] & 0x20) != 0)
      {
         this->checkError();
      }
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "=======================================================\n" << vprDEBUG_FLUSH;

      return(vpr::ReturnStatus::Succeed);
   }
   return(vpr::ReturnStatus::Fail);
}

   // Does not work with older hadware
vpr::ReturnStatus FlockStandalone::readHemisphere()
{
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      vpr::Uint64 timeout=10000;  // How long to wait for data to arrive
      vpr::Uint32 num_read;
      char buff[2];
      
      
      pickBird(1);
      
      buff[0] = 0x4F;
      buff[1] = 0x16;

      vpr::System::msleep(600);
      mSerialPort->write(&buff, sizeof(buff), written);
      
      //mSerialPort->drainOutput();
      //vpr::System::msleep(500);
      
      //mSerialPort->flushQueue(vpr::SerialTypes::INPUT_QUEUE);
      
      vpr::ReturnStatus status = mSerialPort->read(&buff,2,num_read,vpr::Interval(timeout, vpr::Interval::Msec));
      if (!status.success())
      {
         return(status);
      }
      std::cout << "Hemisphere is: " << (int)buff[0] << "." << (int)buff[1] << std::endl;
      
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      //vpr::System::msleep(500);

      return(vpr::ReturnStatus::Succeed);
   }
   else
   {
      return(vpr::ReturnStatus::Fail);
   }
}


void FlockStandalone::pickBird (const int birdID)
{
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff = 0xF0 + birdID;

      vpr::System::msleep(600);
      mSerialPort->write(&buff, sizeof(buff), written);
      // mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

      // vpr::System::usleep(100 * mSleepFactor);
   }
}

// Open the port.
//  give - a serial port
//  give - a baud rate
//  returns portId twice (created by the open function)
//  NOTE: portId is returned from both ends of this function.
//  if portId == -1 then function failed to open the port.
vpr::ReturnStatus FlockStandalone::openPort ()
{
   ///////////////////////////////////////////////////////////////////
   // Open and close the port to reset the tracker, then
   // Open the port
   ///////////////////////////////////////////////////////////////////
   if ( mSerialPort != NULL )
   {
      vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_CONFIG_LVL) << "====== " << "Trying to open serial port: " 
         << mPort << " =====\n" << vprDEBUG_FLUSH;
      
      mSerialPort->setOpenReadWrite();
      
      if (mSerialPort->open().success() )
      {
         vpr::System::msleep(500);
         mSerialPort->close();
         
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Port reset successfully (port was "
                   << "opened then closed)\n" << vprDEBUG_FLUSH;

         if ( mSerialPort->open().success() )
         {
            vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Port opened successfully\n"
               << vprDEBUG_FLUSH;

            mSerialPort->clearAll();
            
            vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting read stuff\n" << vprDEBUG_FLUSH;
            mSerialPort->enableRead();
            
            vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting local attachment\n" << vprDEBUG_FLUSH;
            mSerialPort->enableLocalAttach();

            vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting Break Byte ignore\n" << vprDEBUG_FLUSH;
            mSerialPort->enableBreakByteIgnore();
            
            mSerialPort->setUpdateAction(vpr::SerialTypes::NOW);

            // Setup the port.
            vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Setting new Input baud rate: "
                      << mBaud << " bits per second\n" << vprDEBUG_FLUSH;
            mSerialPort->setInputBaudRate(mBaud);

            vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Setting new Output baud rate: "
                      << mBaud << " bits per second\n" << vprDEBUG_FLUSH;
            mSerialPort->setOutputBaudRate(mBaud);
            
            vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Setting character size\n" << vprDEBUG_FLUSH;
            mSerialPort->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
            
            vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Port setup correctly\n" << vprDEBUG_FLUSH;
            return(vpr::ReturnStatus::Succeed);
         }
         else
         {
            vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "Port open failed\n" << vprDEBUG_FLUSH;
            return(vpr::ReturnStatus::Fail);
         }
      }
      else
      {
         vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "Port reset failed (because port "
            << "open failed)\n" << vprDEBUG_FLUSH;
         return(vpr::ReturnStatus::Fail);
      }
   }
   else
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "ERROR -- No port has been set in FlockStandalone::open_port()!\n" << vprDEBUG_FLUSH;
      return(vpr::ReturnStatus::Fail);
   }
   vprDEBUG_END(vprDBG_ALL,vprDBG_CONFIG_LVL) << "====================================================\n" << vprDEBUG_FLUSH;
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

void FlockStandalone::sendSync ()
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
      unsigned char buff[2];
      buff[0] = 'A';
      buff[1] = mSyncStyle;

      vpr::System::msleep(600);
      mSerialPort->write(buff, 2, written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   }
}

void FlockStandalone::sendHemisphere()
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
      
      int loop_count = mNumBirds;
      if (mExtendedRange)
      {
         loop_count++;
      }    

      // This will loop over all the birds and the transmitter.  When it
      // encounters the transmitter, it skips it.
      for ( int i = 1; i < loop_count; i++ )
      {
         // Skip the transmitter.
         if (! isTransmitter(i))
         {
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
   
            vpr::System::msleep(600);
            mSerialPort->write(buff, sizeof(buff), written);
            // mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   
            // vpr::System::usleep(500 * mSleepFactor);
         }
      }
   }
}

void FlockStandalone::sendRepAndStream ()
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
      vpr::System::msleep(600);
      mSerialPort->write(buff, sizeof(buff), written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      //vpr::System::usleep(2000 * mSleepFactor);

      ////////////////////////////////////////////////////////////////
      // set stream mode
      ////////////////////////////////////////////////////////////////
      buff[0] = '@';
      vpr::System::msleep(600);
      mSerialPort->write(buff, sizeof(buff), written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      //vpr::System::usleep(500 * mSleepFactor);
   }
}

void FlockStandalone::sendPosAngles ()
{
   //////////////////////////////////////////////////////////////////
   // Set Position Angles
   /////////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[1];
      
      int loop_count = mNumBirds;
      if (mExtendedRange)
      {
         loop_count++;
      }    
      
      for ( int bird_id = 1; bird_id < loop_count; bird_id++ )
      {
         // Skip the transmitter unit.
         if (! isTransmitter(bird_id))
         {
            pickBird(bird_id);
            buff[0] = 'Y';
            
            vpr::System::msleep(600);
            mSerialPort->write(buff, sizeof(buff), written);
            //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   
            //vpr::System::usleep(500 * mSleepFactor);
         }
      }
   }
}

void FlockStandalone::sendFilter ()
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

      vpr::System::msleep(600);
      mSerialPort->write(buff, sizeof(buff), written);
      // mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

      //TODO: Do I need to sleep here?
      // vpr::System::usleep(12000 * mSleepFactor);
   }
}

void FlockStandalone::sendTransmitter ()
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

      vpr::System::msleep(600);
      mSerialPort->write(buff, sizeof(buff), written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

      //vpr::System::usleep(12000 * FlockStandalone::mSleepFactor);
   }
}

void FlockStandalone::sendAutoconfig ()
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
      buff[2] = mNumBirds + (mExtendedRange ? 1 : 0); //number of input devices + 1 for transmitter

      vpr::System::msleep(600);
      mSerialPort->write(buff, sizeof(buff), written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      vpr::System::msleep(600);
   }
}

void FlockStandalone::sendGroup ()
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

      vpr::System::msleep(600);
      mSerialPort->write(buff, sizeof(buff), written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      //vpr::System::sleep(2);
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
   
   //if (var < 0)
   //{
   //   std::cout << " (-) ";
   //}
   unsigned bitval;
   for ( int bit=7; bit>=0; bit-- )
   {
      bitval=mask[bit]&var;
      printf("%c",bitval?'1':'0');
   }
}

void FlockStandalone::showbits16(float var)
{
   static int mask[16]={  1,  2,   4,   8,  16,  32,   64,  128,
                             256,512,1024,2048,4096,8192,16384,32768};

   if (var < 0)
   {
      std::cout << " (-) ";
   }
   unsigned bitval;
   for ( int bit=15; bit>=0; bit-- )
   {
      bitval=mask[bit]&(int)var;
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


void FlockStandalone::printError( unsigned char ErrCode, unsigned char ExpandedErrCode )
{
  /*
    Display a message describing the Error
    */
  switch (ErrCode) {
  case 0:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** No Errors Have Occurred ****\n" << vprDEBUG_FLUSH;
    break;
  case 1:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** System RAM Test Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 2:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Non-Volatile Storage Write Failure ****\n" << vprDEBUG_FLUSH;
    break;
  case 3:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** System EEPROM Configuration Corrupt ****\n" << vprDEBUG_FLUSH;
    break;
  case 4:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Transmitter EEPROM Configuration Corrupt ****\n" << vprDEBUG_FLUSH;
    break;
  case 5:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Receiver EEPROM Configuration Corrupt ****\n" << vprDEBUG_FLUSH;
    break;
  case 6:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Invalid RS232 Command ****\n" << vprDEBUG_FLUSH;
    break;
  case 7:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Not an FBB Master ****\n" << vprDEBUG_FLUSH;
    break;
  case 8:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** No 6DFOBs are Active ****\n" << vprDEBUG_FLUSH;
    break;
  case 9:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** 6DFOB has not been Initialized ****\n" << vprDEBUG_FLUSH;
    break;
  case 10:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** FBB Receive Error - Intra Bird Bus ****\n" << vprDEBUG_FLUSH;
    break;
  case 11:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** RS232 Overrun and/or Framing Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 12:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** FBB Receive Error - FBB Host Bus ****\n" << vprDEBUG_FLUSH;
    break;
  case 13:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << 
	    "**** No FBB Command Response from Device at address " 
       << (ExpandedErrCode & 0x0f) << "(decimal) ****\n" << vprDEBUG_FLUSH;
    break;
  case 14:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Invalid FBB Host Command ****\n" << vprDEBUG_FLUSH;
    break;
  case 15:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** FBB Run Time Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 16:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Invalid CPU Speed ****\n" << vprDEBUG_FLUSH;
    break;
  case 17:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Slave No Data Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 18:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Illegal Baud Rate ****\n" << vprDEBUG_FLUSH;
    break;
  case 19:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Slave Acknowledge Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 20:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** CPU Overflow Error - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 21:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Array Bounds Error - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 22:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Unused Opcode Error - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 23:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Escape Opcode Error - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 24:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Reserved Int 9 - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 25:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Reserved Int 10 - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 26:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Reserved Int 11 - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 27:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Numeric CPU Error - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 28:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** CRT Syncronization Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 29:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Transmitter Not Active Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 30:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** ERC Extended Range Transmitter Not Attached Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 31:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** CPU Time Overflow Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 32:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Receiver Saturated Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 33:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Slave Configuration Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 34:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** ERC Watchdog Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 35:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** ERC Overtemp Error ****\n" << vprDEBUG_FLUSH;
    break;
  default:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** UNKNOWN ERROR... check user manual ****\n" << vprDEBUG_FLUSH;
    break;
  }
}

// check for flock error, return err number if there is an error
// zero if no error
int FlockStandalone::checkError()
{
   vpr::Uint32 bytes_read=0;
   unsigned char bird_command[2];
   
   // put the flock to sleep (B to get out of stream mode, G to sleep)
   bird_command[0] = 'B';
   bird_command[1] = 'G';
   if (mSerialPort->write(&bird_command,2,bytes_read) != vpr::ReturnStatus::Succeed)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] Failed writing cmds to tracker.\n" << vprDEBUG_FLUSH;
      return -1;
   }
   
   mSerialPort->drainOutput();
   vpr::System::msleep(500);
   
   // now get error code and clear error status
   // we want error code 16, not 10 -- we want the expanded error code
   // prepare error status query (expanded error codes)

   mSerialPort->flushQueue(vpr::SerialTypes::INPUT_QUEUE);
   
   bird_command[0] = 'O';
   bird_command[1] = 16;
   
   if (mSerialPort->write(&bird_command,2,bytes_read) != vpr::ReturnStatus::Succeed)
   {
     vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] Failed writing cmds to tracker.\n" << vprDEBUG_FLUSH;
     return -1;
   }
   
   // make sure the command is sent out
   mSerialPort->drainOutput();
   vpr::System::msleep(500);
   
   // read response (2 char response to error query 16),
   // 1 char response to 10
   
   mSerialPort->read(&bird_command,2,bytes_read);
   if ( bytes_read !=2)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] Received only " 
         << bytes_read << " of 2 chars for error code\n" << vprDEBUG_FLUSH;
      return -1;
   }
   
   printError( bird_command[0], bird_command[1] );
   
   return bird_command[0];
}

bool FlockStandalone::isTransmitter(int birdID) const
{
   // This flock is configured for extended range and thus has a transmitter on
   // the FBB.
   if (mExtendedRange)
   {
      // Check if the given ID is the ID for the transmitter
      if (birdID == mXmitterUnitNumber)
      {
         // Yay ... this is a transmitter. God help us all.
         return true;
      }
   }

   // Must not be a transmitter
   return false;
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
