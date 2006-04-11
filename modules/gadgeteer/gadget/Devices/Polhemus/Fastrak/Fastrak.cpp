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

// ----------------------------------------------------------------------------
// Author:
//     Nicolas Tarrin <nicolas.tarrin@inria.fr>
//     February 2001
// ----------------------------------------------------------------------------

#include <gadget/Devices/DriverConfig.h>

#include <gmtl/EulerAngle.h>
#include <gmtl/Vec.h>
#include <gmtl/Generate.h>
#include <vpr/vpr.h>
#include <vpr/System.h>

#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Devices/Polhemus/Fastrak/Fastrak.h>

void initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::Fastrak>(inputMgr);
}

static void printError(std::string errorMsg)
{
#ifdef _FASTRAK_DEBUG_
   std::cout << errorMsg << std:endl;
#endif
}

namespace gadget
{

Fastrak::Fastrak() : mSampleThread(NULL)
{
   ;
}

Fastrak::~Fastrak()
{
   this->stopSampling();
   mFastrakDev.trackerFinish();
}

bool Fastrak::config(jccl::ConfigChunkPtr fastrakChunk)
{
   if ( !gadget::Digital::config(fastrakChunk) )
   {
      return false;
   }

   // configFile = "/home/kruger/VRjuggler/juggler/Input/vjPosition/ft_config.dat";

   // *************************************************************************
   // mFastrakDev.trackerInit(configFile);

   FastrakConfig conf;

   // *************************************************************************
   // readconfig(configfile);

   //  FILE *fp;
   // char configline[100];
   int station;
   struct perstation *psp;

   // port
   strcpy(conf.port, (fastrakChunk->getProperty<std::string>("port")).c_str());
   conf.found |= 1<<DEV;

   // baud
   conf.baud = fastrakChunk->getProperty<int>("baud");
   conf.found |= 1<<BAUD;

   // button
   switch ( fastrakChunk->getProperty<int>("button") )
   {
      case 0: conf.button = '1'; conf.cont='C'; break;
      case 1: conf.button = '1'; conf.cont='c'; break;
      case 2: conf.button = '0'; conf.cont='C'; break;
      case 3: conf.button = '0'; conf.cont='c'; break;
      default: printError( "ERROR: vjFastrak::config : not a valid configuration"); break;
   }
   conf.found |= 1<<BUTTON;

   // Rec Pos
   if ( fastrakChunk->getProperty<bool>("Rec1", 0) )
   {
      conf.perstation[0].rec |= 1<<Pos;
   }
   else
   {
      conf.perstation[0].rec &= ~(1<<Pos);
   }
   conf.found |= 1<<REC;

   if ( fastrakChunk->getProperty<bool>("Rec2", 0) )
   {
      conf.perstation[1].rec |= 1<<Pos;
   }
   else
   {
      conf.perstation[1].rec &= ~(1<<Pos);
   }
   conf.found |= 1<<REC1;

   if ( fastrakChunk->getProperty<bool>("Rec3", 0) )
   {
      conf.perstation[2].rec |= 1<<Pos;
   }
   else
   {
      conf.perstation[2].rec &= ~(1<<Pos);
   }
   conf.found |= 1<<REC2;

   if ( fastrakChunk->getProperty<bool>("Rec4", 0) )
   {
      conf.perstation[3].rec |= 1<<Pos;
   }
   else
   {
      conf.perstation[3].rec &= ~(1<<Pos);
   }
   conf.found |= 1<<REC3;

   // Rec Ang
   if ( fastrakChunk->getProperty<bool>("Rec1", 1) )
   {
      conf.perstation[0].rec |= 1<<Ang;
   }
   else
   {
      conf.perstation[0].rec &= ~(1<<Ang);
   }

   if ( fastrakChunk->getProperty<bool>("Rec2", 1) )
   {
      conf.perstation[1].rec |= 1<<Ang;
   }
   else
   {
      conf.perstation[1].rec &= ~(1<<Ang);
   }

   if ( fastrakChunk->getProperty<bool>("Rec3", 1) )
   {
      conf.perstation[2].rec |= 1<<Ang;
   }
   else
   {
      conf.perstation[2].rec &= ~(1<<Ang);
   }

   if ( fastrakChunk->getProperty<bool>("Rec4", 1) )
   {
      conf.perstation[3].rec |= 1<<Ang;
   }
   else
   {
      conf.perstation[3].rec &= ~(1<<Ang);
   }

   // Rec Quat
   if ( fastrakChunk->getProperty<bool>("Rec1", 2) )
   {
      conf.perstation[0].rec |= 1<<Quat;
   }
   else
   {
      conf.perstation[0].rec &= ~(1<<Quat);
   }

   if ( fastrakChunk->getProperty<bool>("Rec2", 2) )
   {
      conf.perstation[1].rec |= 1<<Quat;
   }
   else
   {
      conf.perstation[1].rec &= ~(1<<Quat);
   }

   if ( fastrakChunk->getProperty<bool>("Rec3", 2) )
   {
      conf.perstation[2].rec |= 1<<Quat;
   }
   else
   {
      conf.perstation[2].rec &= ~(1<<Quat);
   }

   if ( fastrakChunk->getProperty<bool>("Rec4", 2) )
   {
      conf.perstation[3].rec |= 1<<Quat;
   }
   else
   {
      conf.perstation[3].rec &= ~(1<<Quat);
   }

   // Rec But
   if ( fastrakChunk->getProperty<bool>("Rec1", 3) )
   {
      conf.perstation[0].rec |= 1<<But;
   }
   else
   {
      conf.perstation[0].rec &= ~(1<<But);
   }

   // TIP
   conf.perstation[0].tip[0] = fastrakChunk->getProperty<float>("Tip1", 0);
   conf.perstation[0].tip[1] = fastrakChunk->getProperty<float>("Tip1", 1);
   conf.perstation[0].tip[2] = fastrakChunk->getProperty<float>("Tip1", 2);
   conf.found |= 1<<TIP;

   conf.perstation[1].tip[0] = fastrakChunk->getProperty<float>("Tip2", 0);
   conf.perstation[1].tip[1] = fastrakChunk->getProperty<float>("Tip2", 1);
   conf.perstation[1].tip[2] = fastrakChunk->getProperty<float>("Tip2", 2);
   conf.found |= 1<<TIP1;

   conf.perstation[2].tip[0] = fastrakChunk->getProperty<float>("Tip3", 0);
   conf.perstation[2].tip[1] = fastrakChunk->getProperty<float>("Tip3", 1);
   conf.perstation[2].tip[2] = fastrakChunk->getProperty<float>("Tip3", 2);
   conf.found |= 1<<TIP2;

   conf.perstation[3].tip[0] = fastrakChunk->getProperty<float>("Tip4", 0);
   conf.perstation[3].tip[1] = fastrakChunk->getProperty<float>("Tip4", 1);
   conf.perstation[3].tip[2] = fastrakChunk->getProperty<float>("Tip4", 2);
   conf.found |= 1<<TIP3;

   // INC
   conf.perstation[0].inc = fastrakChunk->getProperty<float>("Inc1");
   conf.found |= 1<<INC;
   conf.perstation[1].inc = fastrakChunk->getProperty<float>("Inc2");
   conf.found |= 1<<INC1;
   conf.perstation[2].inc = fastrakChunk->getProperty<float>("Inc3");
   conf.found |= 1<<INC2;
   conf.perstation[3].inc = fastrakChunk->getProperty<float>("Inc4");
   conf.found |= 1<<INC3;

   // HEM
   conf.perstation[0].hem[0] = fastrakChunk->getProperty<float>("Hem1", 0);
   conf.perstation[0].hem[1] = fastrakChunk->getProperty<float>("Hem1", 1);
   conf.perstation[0].hem[2] = fastrakChunk->getProperty<float>("Hem1", 2);
   conf.found |= 1<<HEM;

   conf.perstation[1].hem[0] = fastrakChunk->getProperty<float>("Hem2", 0);
   conf.perstation[1].hem[1] = fastrakChunk->getProperty<float>("Hem2", 1);
   conf.perstation[1].hem[2] = fastrakChunk->getProperty<float>("Hem2", 2);
   conf.found |= 1<<HEM1;

   conf.perstation[2].hem[0] = fastrakChunk->getProperty<float>("Hem3", 0);
   conf.perstation[2].hem[1] = fastrakChunk->getProperty<float>("Hem3", 1);
   conf.perstation[2].hem[2] = fastrakChunk->getProperty<float>("Hem3", 2);
   conf.found |= 1<<HEM2;

   conf.perstation[3].hem[0] = fastrakChunk->getProperty<float>("Hem4", 0);
   conf.perstation[3].hem[1] = fastrakChunk->getProperty<float>("Hem4", 1);
   conf.perstation[3].hem[2] = fastrakChunk->getProperty<float>("Hem4", 2);
   conf.found |= 1<<HEM3;

   // ARF
   conf.perstation[0].arf[0] = fastrakChunk->getProperty<float>("ARF1", 0);
   conf.perstation[0].arf[1] = fastrakChunk->getProperty<float>("ARF1", 1);
   conf.perstation[0].arf[2] = fastrakChunk->getProperty<float>("ARF1", 2);
   conf.perstation[0].arf[3] = fastrakChunk->getProperty<float>("ARF1", 3);
   conf.perstation[0].arf[4] = fastrakChunk->getProperty<float>("ARF1", 4);
   conf.perstation[0].arf[5] = fastrakChunk->getProperty<float>("ARF1", 5);
   conf.perstation[0].arf[6] = fastrakChunk->getProperty<float>("ARF1", 6);
   conf.perstation[0].arf[7] = fastrakChunk->getProperty<float>("ARF1", 7);
   conf.perstation[0].arf[8] = fastrakChunk->getProperty<float>("ARF1", 8);
   conf.found |= 1<<ARF;

   conf.perstation[1].arf[0] = fastrakChunk->getProperty<float>("ARF2", 0);
   conf.perstation[1].arf[1] = fastrakChunk->getProperty<float>("ARF2", 1);
   conf.perstation[1].arf[2] = fastrakChunk->getProperty<float>("ARF2", 2);
   conf.perstation[1].arf[3] = fastrakChunk->getProperty<float>("ARF2", 3);
   conf.perstation[1].arf[4] = fastrakChunk->getProperty<float>("ARF2", 4);
   conf.perstation[1].arf[5] = fastrakChunk->getProperty<float>("ARF2", 5);
   conf.perstation[1].arf[6] = fastrakChunk->getProperty<float>("ARF2", 6);
   conf.perstation[1].arf[7] = fastrakChunk->getProperty<float>("ARF2", 7);
   conf.perstation[1].arf[8] = fastrakChunk->getProperty<float>("ARF2", 8);
   conf.found |= 1<<ARF1;

   conf.perstation[2].arf[0] = fastrakChunk->getProperty<float>("ARF3", 0);
   conf.perstation[2].arf[1] = fastrakChunk->getProperty<float>("ARF3", 1);
   conf.perstation[2].arf[2] = fastrakChunk->getProperty<float>("ARF3", 2);
   conf.perstation[2].arf[3] = fastrakChunk->getProperty<float>("ARF3", 3);
   conf.perstation[2].arf[4] = fastrakChunk->getProperty<float>("ARF3", 4);
   conf.perstation[2].arf[5] = fastrakChunk->getProperty<float>("ARF3", 5);
   conf.perstation[2].arf[6] = fastrakChunk->getProperty<float>("ARF3", 6);
   conf.perstation[2].arf[7] = fastrakChunk->getProperty<float>("ARF3", 7);
   conf.perstation[2].arf[8] = fastrakChunk->getProperty<float>("ARF3", 8);
   conf.found |= 1<<ARF2;

   conf.perstation[3].arf[0] = fastrakChunk->getProperty<float>("ARF4", 0);
   conf.perstation[3].arf[1] = fastrakChunk->getProperty<float>("ARF4", 1);
   conf.perstation[3].arf[2] = fastrakChunk->getProperty<float>("ARF4", 2);
   conf.perstation[3].arf[3] = fastrakChunk->getProperty<float>("ARF4", 3);
   conf.perstation[3].arf[4] = fastrakChunk->getProperty<float>("ARF4", 4);
   conf.perstation[3].arf[5] = fastrakChunk->getProperty<float>("ARF4", 5);
   conf.perstation[3].arf[6] = fastrakChunk->getProperty<float>("ARF4", 6);
   conf.perstation[3].arf[7] = fastrakChunk->getProperty<float>("ARF4", 7);
   conf.perstation[3].arf[8] = fastrakChunk->getProperty<float>("ARF4", 8);
   conf.found |= 1<<ARF3;

   // TMF
   conf.perstation[0].tmf[0] = fastrakChunk->getProperty<float>("TMF1", 0);
   conf.perstation[0].tmf[1] = fastrakChunk->getProperty<float>("TMF1", 1);
   conf.perstation[0].tmf[2] = fastrakChunk->getProperty<float>("TMF1", 2);
   conf.found |= 1<<TMF;

   conf.perstation[1].tmf[0] = fastrakChunk->getProperty<float>("TMF2", 0);
   conf.perstation[1].tmf[1] = fastrakChunk->getProperty<float>("TMF2", 1);
   conf.perstation[1].tmf[2] = fastrakChunk->getProperty<float>("TMF2", 2);
   conf.found |= 1<<TMF1;

   conf.perstation[2].tmf[0] = fastrakChunk->getProperty<float>("TMF3", 0);
   conf.perstation[2].tmf[1] = fastrakChunk->getProperty<float>("TMF3", 1);
   conf.perstation[2].tmf[2] = fastrakChunk->getProperty<float>("TMF3", 2);
   conf.found |= 1<<TMF2;

   conf.perstation[3].tmf[0] = fastrakChunk->getProperty<float>("TMF4", 0);
   conf.perstation[3].tmf[1] = fastrakChunk->getProperty<float>("TMF4", 1);
   conf.perstation[3].tmf[2] = fastrakChunk->getProperty<float>("TMF4", 2);
   conf.found |= 1<<TMF3;

   conf.len = 3;
   for ( station = 0; station < NSTATION; ++station )
   {
      psp = &conf.perstation[station];
      if ( psp->rec )
      {
         psp->begin = conf.len - 3;
      }

      if ( psp->rec & (1<<Pos) )
      {
         psp->posoff = conf.len;
         conf.len +=  XYZ*sizeof (int);
      }
      if ( psp->rec & (1<<Ang) )
      {
         psp->angoff = conf.len;
         conf.len +=  XYZ*sizeof (int);
      }
      if ( psp->rec & (1<<Quat) )
      {
         psp->quatoff = conf.len;
         conf.len +=  4*sizeof (int);
      }
      if ( psp->rec & (1<<But) )
      {
         psp->butoff = conf.len + 1;
         conf.len +=  2;
      }
      if ( psp->rec )
      {
         conf.len += 5;
      }
   }
   conf.len -= 3;

   // end readconfig(configfile);
   // *************************************************************************

   mFastrakDev.setConfig(conf);

   return true;
}

int Fastrak::startSampling()
{
   int status(0);

   if ( mFastrakDev.open().success() )
   {
      mFastrakDev.trackerInit();

      mSampleThread = new vpr::Thread(threadedSampleFunction, (void*)this);

      if ( mSampleThread->valid() )
      {
         status = 1; // thread creation succeded
      }
   }

   return status;
}

// Record (or sample) the current data
// this is called repeatedly by the sample thread created by startSampling()
int Fastrak::sample()
{
   int status(1);

   // XXX: This should not be hard-coded to four.
   std::vector<gadget::PositionData> cur_pos_samples(4);
   std::vector<gadget::DigitalData>  cur_dig_samples(1);

   // get an initial timestamp for this entire sample. we'll copy it into
   // each PositionData for this sample.
   if ( ! cur_pos_samples.empty() )
   {
      cur_pos_samples[0].setTime();
   }

   // Fill the position and orientation arrays using the current values from
   // the Fastrak stations.  This also has the side effect of getting the
   // button state.  Weird...
   //NB: 15 = 1111b = the 4 stations
   mButtonState = mFastrakDev.getCoords(15, &mTrackersPosition[0][0],
                                         &mTrackersOrientation[0][0]);
   cur_dig_samples[0].setTime(cur_pos_samples[0].getTime());
   cur_dig_samples[0].setDigital(mButtonState);

   addDigitalSample(cur_dig_samples);
      
   for ( int i = 0; i < 4; ++i )         // for each station
   {
      cur_pos_samples[i].setTime(cur_pos_samples[i].getTime());
      cur_pos_samples[i].mPosData = getPosData(i);
   }

   addPositionSample(cur_pos_samples);
   
   return status;
}

void Fastrak::updateData()
{
   // swap the buffered sample data
   swapPositionBuffers();
}

// kill sample thread
int Fastrak::stopSampling()
{
   if ( mSampleThread != NULL )
   {
      mSampleThread->kill();
      delete mSampleThread;
      mSampleThread = NULL;
   }
   return 1;
}

// function for users to get the digital data.
// overload gadget::Digital::getDigitalData
// digital data only on the first station
int Fastrak::getDigitalData( int station )
{
   if ( station != 0 )
   {
      std::cout << "error in vjFastrak::getDigitalData: invalid station:" << station << std::endl;
      return 0;
   }
   else
   {
      return mButtonState;
   }
}

// function for users to get the position data
// overload gadget::Position::getPosData
gmtl::Matrix44f Fastrak::getPosData( int station )
{
   gmtl::Matrix44f position_matrix;

   // vprASSERT( axis >= 0 && axis <= 2 && "3 (x,y and z) axes available");

   if ( !( (0<=station) && (station<=3) ) )
   {
      std::cout << "error in vjFastrak::getPosData: invalid station:" << station << std::endl;
   }
   else
   {
      gmtl::EulerAngleXYZf tracker_orient(gmtl::Math::deg2Rad(mTrackersOrientation[station][0]),
                                          gmtl::Math::deg2Rad(mTrackersOrientation[station][1]),
                                          gmtl::Math::deg2Rad(mTrackersOrientation[station][2]));
      gmtl::Vec3f tracker_pos(mTrackersPosition[station][0],
                              mTrackersPosition[station][1],
                              mTrackersPosition[station][2]);

      gmtl::setRot(position_matrix, tracker_orient);
      gmtl::setTrans(position_matrix, tracker_pos);
   }

   return position_matrix;
}

void Fastrak::threadedSampleFunction( void* classPointer )
{
   Fastrak* this_ptr = static_cast<Fastrak*>( classPointer );

   while ( 1 )
   {
      this_ptr->sample();
      if ( this_ptr->mFastrakDev.getConfig().cont != 'C' )
      {
         vpr::System::sleep(1);
      }
   }
}

} // End of gadget namespace
