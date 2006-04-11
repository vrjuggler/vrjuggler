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

#include <string>
#include <string.h>
#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Util/Assert.h>

#include <gadget/Devices/Polhemus/Fastrak/FastrakStandalone.h>

vpr::ReturnStatus FastrakStandalone::open()
{
   vpr::ReturnStatus status;

   mSerialPort = new vpr::SerialPort(std::string(mConf.port));
   mSerialPort->setOpenNonBlocking();
   mSerialPort->setOpenReadWrite();

   status = mSerialPort->open();

   if ( status.success() )
   {
      vpr::Uint32 baud;

      mSerialPort->clearAll();

      if ( mConf.found & (1 << BAUD) )
      {
         baud = mConf.baud;
         baud = mConf.baud;
      }
      else
      {
         baud = 9600;
      }

      mSerialPort->setOutputBaudRate(baud);
      mSerialPort->setInputBaudRate(baud);

//      tcsetattr(mTrackerFD, TCSAFLUSH, &tc);
//      fcntl(mTrackerFD, F_SETFL, 0);
   }

   return status;
}

void FastrakStandalone::trackerFinish()
{
   vpr::Uint32 bytes_written;
   mSerialPort->write("c", 1, bytes_written);
   vpr::System::sleep(1);

   mSerialPort->flushQueue(vpr::SerialTypes::INPUT_QUEUE);
   mSerialPort->close();

   if ( NULL != mReadThread )
   {
      mReadThread->kill();
   }
}

int FastrakStandalone::Read(int len)
{
   vpr::Uint32 bytes_read;
   vpr::ReturnStatus status;

   int nread;
   int rem = len;
   char *cp = (char *) mTrackerBuf;
   for ( ;; )
   {
      status = mSerialPort->read(cp, rem, bytes_read);

      if ( bytes_read <= 0 || status.failure() )
      {
         return len - rem;
      }
      if ( (rem -= nread) == 0 )
      {
         return len;
      }

      cp += nread;
   }
}


void FastrakStandalone::readloop(void *unused)
{
   vpr::Uint32 bytes_written;
   vpr::Uint32 sleep_time(10000000/mConf.baud);

/*
   // XXX: Maybe this could be replaced with VPR's signal abstraction?
#ifndef VPR_OS_Win32
   sigignore(SIGINT);
#endif
*/

   for ( ;; )
   {
      if ( mDoFlush )
      {
         mSerialPort->write("c", 1, bytes_written);
         vpr::System::usleep(sleep_time);
         mSerialPort->flushQueue(vpr::SerialTypes::INPUT_QUEUE);
         vpr::System::usleep(sleep_time);
         mSerialPort->write("C", 1, bytes_written);
      }

      this->Read(mConf.len);
      mDoFlush = false;
//      if ( getppid() == 1 )
//      {
//         trackerFinish();
//         _exit(0);
//      }
   }
}

vpr::ReturnStatus FastrakStandalone::trackerInit()
{
   vprASSERT(mSerialPort->isOpen() && "Port must be open before initializing");
   vpr::Uint32 bytes_written;

//   static struct termios tc;
   char buf[256];
   char c;

   mSerialPort->write("uf", 3, bytes_written); // unites: cm, mode binaire

   for ( unsigned int station = 0; station < NSTATION; ++station )
   {
      struct perstation *psp = &(mConf.perstation[station]);
      int len;
      c = '1' + station;

      mSerialPort->write("e", 1, bytes_written);
      mSerialPort->write(&c, 1, bytes_written);
      mSerialPort->write(",",1, bytes_written);
      mSerialPort->write(&mConf.button,1, bytes_written);
      mSerialPort->write("\r",1, bytes_written);
      mSerialPort->write("l",1, bytes_written);
      mSerialPort->write(&c, 1, bytes_written);

      if ( mConf.found&(1<<(REC+station)) )
      {
         if ( psp->rec == 0 )
         {
            mSerialPort->write(",0\r", 3, bytes_written);
         }
         else
         {
            mSerialPort->write(",1\rO", 4, bytes_written);
            mSerialPort->write(&c, 1, bytes_written);

            if ( psp->rec & (1<<Pos) )
            {
               mSerialPort->write(",2", 2, bytes_written);
            }
            if ( psp->rec & (1<<Ang) )
            {
               mSerialPort->write(",4", 2, bytes_written);
            }
            if ( psp->rec & (1<<Quat) )
            {
               mSerialPort->write(",11", 3, bytes_written);
            }
            if ( psp->rec & (1<<But) )
            {
               mSerialPort->write(",16", 3, bytes_written);
            }
            mSerialPort->write(",1\r", 3, bytes_written);
         }
      }

      if ( mConf.found & (1<<(TIP+station)) )
      {
         len = sprintf(buf, "N%c,%.2f,%.2f,%.2f\r", c,
                       psp->tip[0], psp->tip[1], psp->tip[2]);
         mSerialPort->write(buf, len, bytes_written);
      }

      if ( mConf.found & (1<<(INC+station)) )
      {
         len = sprintf(buf, "I%c,%.2f\r", c, psp->inc);
         mSerialPort->write(buf, len, bytes_written);
      }
      if ( mConf.found & (1<<(HEM+station)) )
      {
         len = sprintf(buf, "H%c,%.2f,%.2f,%.2f\r", c,
                       psp->hem[0], psp->hem[1], psp->hem[2]);
         mSerialPort->write(buf, len, bytes_written);
      }
      if ( mConf.found & (1<<(TMF+station)) )
      {
         len = sprintf(buf, "r%c,%.2f,%.2f,%.2f\r", c,
                       psp->tmf[0],
                       psp->tmf[1],
                       psp->tmf[2]);
         mSerialPort->write(buf, len, bytes_written);
      }
      if ( mConf.found & (1<<(ARF+station)) )
      {
         len = sprintf(buf, "A%c,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r",
                       c,
                       psp->arf[0], psp->arf[1], psp->arf[2], psp->arf[3],
                       psp->arf[4], psp->arf[5], psp->arf[6], psp->arf[7],
                       psp->arf[8]);
         mSerialPort->write(buf, len, bytes_written);
      }
   }
   /*
   if (mConf.cont == 'C')
   {
      ReadPid = sproc(readloop, PR_SALL);
      if (ReadPid == -1)
      {
         perror("Can't creat read process");
         exit(1);
      }
   }
   */
   mSerialPort->write(&mConf.cont, 1, bytes_written);

   return vpr::ReturnStatus();
}

static int littlendian(unsigned char *src)
{
   return(src[3]<<24)|(src[2]<<16)|(src[1]<<8)|src[0];
}

void FastrakStandalone::checkchild()
{
   // XXX: This is commented out because I do not know how useful it is.
/*
   if ( NULL != mReadThread )
   {
      if ( waitpid(mReadPid, 0, WNOHANG) == 0 )
      {
         return;
      }
      else
      {
         mReadThread->kill(9);
      }
   }
   vpr::ThreadMemberFunctor<FastrakStandalone>* read_func =
      new vpr::ThreadMemberFunctor<FastrakStandalone>(this,
                                                       &FastrakStandalone::readloop,
                                                       NULL);
   mReadThread = new vpr::Thread(read_func);
*/
   if ( NULL == mReadThread )
   {
      vpr::ThreadMemberFunctor<FastrakStandalone>* read_func =
         new vpr::ThreadMemberFunctor<FastrakStandalone>(this,
                                                          &FastrakStandalone::readloop,
                                                          NULL);
      mReadThread = new vpr::Thread(read_func);
   }
}

void FastrakStandalone::getTrackerBuf()
{
   if ( (mConf.cont != 'C') && (mConf.button == '0') )
   {
      vpr::Uint32 bytes_written;
      mSerialPort->write("P", 1, bytes_written);
   }

   this->Read(mConf.len);
}

void FastrakStandalone::getTrackerInfo(struct perstation* psp, unsigned char c)
{
   unsigned char *cp;
   retry:
   getTrackerBuf();
   cp = (unsigned char *)mTrackerBuf + psp->begin;
   if ( cp[0] != '0' || cp[1] != c )
   {
      if ( mConf.cont != 'C' )
      {
         mSerialPort->flushQueue(vpr::SerialTypes::INPUT_QUEUE);
      }
      else
      {
         mDoFlush = true;
         while ( mDoFlush )
         {
            checkchild();
         }
      }
      goto retry;
   }
}

int FastrakStandalone::getCoords(unsigned int stations, float *vecXYZ,
                                  float *vecAER)
{
   unsigned char *cp;
   int i, station, button = 0;
   struct perstation *psp;
   unsigned char c = '1';
   getTrackerBuf();
   for ( station = 0; station < NSTATION; ++station, ++c )
   {
      if ( (stations&(1<<station)) == 0 ) continue;
      psp  = &mConf.perstation[station];
      if ( psp->rec )
      {
         cp = (unsigned char *)mTrackerBuf + psp->begin;
         if ( cp[0] != '0' || cp[1] != c )
         {
            if ( mConf.cont != 'C' )
            {
               mSerialPort->flushQueue(vpr::SerialTypes::INPUT_QUEUE);
            }
            else
            {
               mDoFlush = true;
               while ( mDoFlush )
               {
                  checkchild();
               }
            }
            getTrackerBuf();
         }
         if ( psp->rec & (1<<Pos) )
         {
            cp = (unsigned char *)mTrackerBuf + psp->posoff;
            for ( i = 0; i < 3; ++i )
            {
               *(int *)(vecXYZ+i) = littlendian(cp);
               cp += sizeof (int);
            }
         }
         else
         {
            memset((void *)vecXYZ, 0, 3*sizeof (float));
         }
         vecXYZ += 3;
         if ( psp->rec & (1<<Ang) )
         {
            cp = (unsigned char *)mTrackerBuf + psp->angoff;
            for ( i = 0; i < 3; ++i )
            {
               *(int *)(vecAER+i) = littlendian(cp);
               cp += sizeof (int);
            }
         }
         else
         {
            memset((void *)vecAER, 0, 3*sizeof (float));
         }

         vecAER += 3;
         if ( psp->rec & (1<<But) )
         {
            button = mTrackerBuf[psp->butoff] - '0';
         }
      }
   }
   return button;
}

void FastrakStandalone::getNewCoords(unsigned int station, float *vecXYZ, float *vecAER)
{
   unsigned char *cp;
   int i;
   struct perstation *psp;
   unsigned char c;


   if ( (unsigned)(--station) >= 4 ) return;
   c = '1'+station;
   psp  = &mConf.perstation[station];
   if ( (psp->rec & ((1<<Pos)|(1<<Ang))) == 0 ) return;
   getTrackerInfo(psp, c);
   if ( psp->rec & (1<<Pos) )
   {
      cp = (unsigned char *)mTrackerBuf + psp->posoff;
      for ( i = 0; i < 3; ++i )
      {
         *(int *)(vecXYZ+i) = littlendian(cp);
         cp += sizeof (int);
      }
   }
   if ( psp->rec & (1<<Ang) )
   {
      cp = (unsigned char *)mTrackerBuf + psp->angoff;
      for ( i = 0; i < 3; ++i )
      {
         *(int *)(vecAER+i) = littlendian(cp);
         cp += sizeof (int);
      }
   }
}

int FastrakStandalone::getNewButtonStatus(unsigned int station)
{
   struct perstation *psp;
   char c;

   if ( (unsigned)(--station) >= 4 ) return 0;
   c = '1'+station;
   psp = &mConf.perstation[station];
   if ( psp->rec & (1<<But) == 0 ) return 0;
   getTrackerInfo(psp, c);
   return mTrackerBuf[psp->butoff] - '0';
}

#ifdef TEST
main(int argc, char **argv)
{
   float xyz[XYZ*4], aer[XYZ*4];
   //    int station = 1;
   int but;

   trackerInit(argv[1]);
   //    if (argc > 2) station = argv[2][0] - '0';

   for ( ;; )
   {
      but = getCoords(1, xyz, aer);
      printf("%7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %d\n",
             xyz[0], xyz[1], xyz[2], aer[0], aer[1], aer[2], but);
      but = getCoords(2, xyz, aer);
      printf("%7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %d\n",
             xyz[0], xyz[1], xyz[2], aer[0], aer[1], aer[2], but);
      but = getCoords(4, xyz, aer);
      printf("%7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %d\n",
             xyz[0], xyz[1], xyz[2], aer[0], aer[1], aer[2], but);
      but = getCoords(8, xyz, aer);
      printf("%7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %d\n",
             xyz[0], xyz[1], xyz[2], aer[0], aer[1], aer[2], but);
      fflush(stdout);
   }
}
#endif
