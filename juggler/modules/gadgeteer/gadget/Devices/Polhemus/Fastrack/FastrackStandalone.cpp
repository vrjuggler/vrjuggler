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

#include <gadget/Devices/Polhemus/FastrackStandalone.h>

void trackerFinish()
{
   pid_t pid;

   pid = ReadPid;
   ReadPid = 0;
   write(trackerfd, "c",1);
   sleep(1);
   tcflush(trackerfd, TCIFLUSH);
   close(trackerfd);
   if ( pid )
   {
      kill(pid, SIGKILL);
   }
}

static int Read(int fd, unsigned char *buf, int len)
{
   int nread;
   int rem = len;
   char *cp = (char *)buf;
   for ( ;; )
   {
      if ( (nread = read(fd, cp, rem)) <= 0 )
         return len - rem;
      if ( (rem -= nread) == 0 ) return len;
      cp += nread;
   }
}


static void readloop(void *unused)
{
   sigignore(SIGINT);
   for ( ;; )
   {
      if ( DoFlush )
      {
         write(trackerfd, "c", 1);
         usleep(10000000/conf.baud);
         tcflush(trackerfd, TCIFLUSH);
         usleep(10000000/conf.baud);
         write(trackerfd, "C", 1);
      }
      Read(trackerfd, (unsigned char *)TrackerBuf, conf.len);
      DoFlush = 0;
      if ( getppid() == 1 )
      {
         trackerFinish();
         _exit(0);
      }
   }
}

void trackerInit(char *configfile)
{
}

static int littlendian(unsigned char *src)
{
   return(src[3]<<24)|(src[2]<<16)|(src[1]<<8)|src[0];
}

static void checkchild()
{
   if ( ReadPid )
   {
      if ( waitpid(ReadPid, 0, WNOHANG) == 0 )
      {
         return;
      }
      else
      {
         kill(ReadPid, 9);
      }
   }
   ReadPid = sproc(readloop, PR_SALL);
}

void getTrackerBuf()
{
   if ( (conf.cont != 'C') && (conf.button == '0') )
   {
      write(trackerfd, "P", 1);
   }
   Read(trackerfd, (unsigned char *)TrackerBuf, conf.len);
}

void
getTrackerInfo(struct perstation *psp, unsigned char c)
{
   unsigned char *cp;
   retry:
   getTrackerBuf();
   cp = (unsigned char *)TrackerBuf + psp->begin;
   if ( cp[0] != '0' || cp[1] != c )
   {
      if ( conf.cont != 'C' )
      {
         tcflush(trackerfd, TCIFLUSH);
      }
      else
      {
         DoFlush = 1;
         while ( DoFlush )
         {
            checkchild();
         }
      }
      goto retry;
   }
}

int
getCoords(unsigned int stations, float *vecXYZ, float *vecAER)
{
   unsigned char *cp;
   int i, station, button = 0;
   struct perstation *psp;
   unsigned char c = '1';
   getTrackerBuf();
   for ( station = 0; station < NSTATION; ++station, ++c )
   {
      if ( (stations&(1<<station)) == 0 ) continue;
      psp  = &conf.perstation[station];
      if ( psp->rec )
      {
         cp = (unsigned char *)TrackerBuf + psp->begin;
         if ( cp[0] != '0' || cp[1] != c )
         {
            if ( conf.cont != 'C' )
               tcflush(trackerfd, TCIFLUSH);
            else
            {
               DoFlush = 1;
               while ( DoFlush )
                  checkchild();
            }
            getTrackerBuf();
         }
         if ( psp->rec & (1<<Pos) )
         {
            cp = (unsigned char *)TrackerBuf + psp->posoff;
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
            cp = (unsigned char *)TrackerBuf + psp->angoff;
            for ( i = 0; i < 3; ++i )
            {
               *(int *)(vecAER+i) = littlendian(cp);
               cp += sizeof (int);
            }
         }
         else
            memset((void *)vecAER, 0, 3*sizeof (float));
         vecAER += 3;
         if ( psp->rec & (1<<But) )
            button = TrackerBuf[psp->butoff] - '0';
      }
   }
   return button;
}

void
getNewCoords(unsigned int station, float *vecXYZ, float *vecAER)
{
   unsigned char *cp;
   int i;
   struct perstation *psp;
   unsigned char c;


   if ( (unsigned)(--station) >= 4 ) return;
   c = '1'+station;
   psp  = &conf.perstation[station];
   if ( (psp->rec & ((1<<Pos)|(1<<Ang))) == 0 ) return;
   getTrackerInfo(psp, c);
   if ( psp->rec & (1<<Pos) )
   {
      cp = (unsigned char *)TrackerBuf + psp->posoff;
      for ( i = 0; i < 3; ++i )
      {
         *(int *)(vecXYZ+i) = littlendian(cp);
         cp += sizeof (int);
      }
   }
   if ( psp->rec & (1<<Ang) )
   {
      cp = (unsigned char *)TrackerBuf + psp->angoff;
      for ( i = 0; i < 3; ++i )
      {
         *(int *)(vecAER+i) = littlendian(cp);
         cp += sizeof (int);
      }
   }
}

int
getNewButtonStatus(unsigned int station)
{
   struct perstation *psp;
   char c;

   if ( (unsigned)(--station) >= 4 ) return 0;
   c = '1'+station;
   psp = &conf.perstation[station];
   if ( psp->rec & (1<<But) == 0 ) return 0;
   getTrackerInfo(psp, c);
   return TrackerBuf[psp->butoff] - '0';
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
