/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifdef __sun__
#define _REENTRANT
#endif

#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <time.h>
#include <netdb.h>
#include <sys/time.h>

#include <Input/vjPosition/aMotionStar.h>


// ----------------------------------------------------------------------------
// Constructor.  This initializes member variables and determines the
// endianness of the host machine.
// ----------------------------------------------------------------------------
aMotionStar::aMotionStar(char* address, const unsigned short port,
                         int hemisphere, unsigned int birdFormat,
                         unsigned int birdsRequired, int runMode,
                         double birdRate, unsigned char reportRate)
{
  union {
    char c[sizeof(short)];
    short value;
  } endian;

  mPort        = port;
  mActive        = false;
  mHemisphere    = hemisphere;
  mBirdFormat    = birdFormat;
  mBirdsRequired = birdsRequired;
  mRunMode       = runMode;
  mBirdRate      = birdRate;
  mReportRate    = reportRate;

  if ( address != NULL ) {
    mAddress = strdup(address);
  } else {
    mAddress = NULL;
  }

  m_xmtr_pos_scale = 288.0;
  m_xmtr_rot_scale = 180.0;
  m_xmtr_divisor   = 32767.0;

  // Determine the endianness of the host platform.  A value of true for
  // m_big_endian means that the host use big endian byte order.  false of
  // course means that it is little endian.  The way this works is that we
  // access the first byte of endian.value directly.  If it is 1, the host
  // treats that as the high-order byte.  Otherwise, it is the low-order
  // byte.
  endian.value = 256;
  m_big_endian = (endian.c[0] ? true : false);
} // end aMotionStar::aMotionStar()

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
aMotionStar::~aMotionStar() {
  this->stop();

  if ( mAddress != NULL ) {
    free(mAddress);
    mAddress = NULL;
  }
} // end aMotionStar::~aMotionStar()

// ----------------------------------------------------------------------------
// Initializes the MotionStar, setting the status for each bird.
// ----------------------------------------------------------------------------
int
aMotionStar::start () {
  struct hostent* host_ent;
  unsigned int format;
  struct sockaddr_in server_addr;
  int rtn;

//  cout << "In start" << endl;
  assert(!mActive);

  mRate[0] = 57; // 48
  mRate[1] = 48; // 50
  mRate[2] = 48;
  mRate[3] = 48;
  mRate[4] = 48;
  mRate[5] = 48;

  switch (mBirdFormat) {
    case 1:
      format = mBirdFormat | 0x30;
      break;

    case 2:
      format = mBirdFormat | 0x30;
      break;

    case 3:
      format = mBirdFormat | 0x90;
      break;

    case 4:
      format = mBirdFormat | 0x60;
      break;

    case 5:
      format = mBirdFormat | 0xC0;
      break;

    case 7:
      format = mBirdFormat | 0x40;
      break;

    case 8:
      format = mBirdFormat | 0x70;
      break;

    default:
      format = 0x64;
      break;
    }

  /*
   * Open a TCP socket.
   */

  printf("\nConnecting to %s ...\n", mAddress);

  if ( (mSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    perror("client: can't open stream socket");
    return -1;
  }

  /* Fill in the structure with the address of the
   * server that we want to connect to.
   */

  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port   = htons(mPort);		// Server port number

  // Try to look up address by name.  This will work for an IP address too,
  // but we fall back on inet_addr(3) below just to be safe.
  host_ent = gethostbyname(mAddress);

  // If host_ent is non-NULL, we found an IP address for the hostname in
  // address.  Move that address into the server struct.
  if ( host_ent != NULL ) {
    memmove((void*) &server_addr.sin_addr.s_addr, (void*) host_ent->h_addr,
            sizeof(server_addr.sin_addr.s_addr));
  }
  // Otherwise, assume that the value in address is already an IP address
  // and use inet_addr(3) on it.
  else {
    server_addr.sin_addr.s_addr = inet_addr(mAddress);
  }

  rtn = connect(mSocket, (struct sockaddr*) &server_addr, sizeof(server_addr));
/*
  printf("connect = %4d, connect error = %4d, ", rtn, errno);
  perror(NULL);
  printf("\n");
*/

  mNewptr     = (char*) &mResponse;
  mLpCommand  = &mCommand;
  mLpResponse = &mResponse;

  /* send the wake up call */
  sendWakeup();

  /* get the system status */
  getSystemStatus();

  /* send the system setup */
  setSystemStatus();

   /* for 1 to n flock boards                    */
  /* get the individual bird status             */
  /* modify the appropriate contents            */
  /* send the individual bird status packet back        */

  for ( unsigned int flock = 1; flock<= mChassisDevices; flock++ ) {
      /* get the status of an individual bird */
      getBirdStatus(flock);

      /* change the data format to something new for all birds */
      if( flock <= mBirdsRequired ) {
          mResponse.buffer[6] = format;
      }
      else {
          mResponse.buffer[6] = 0;
      }

      /* set new report rate for all birds */
      mResponse.buffer[7]  = mReportRate;
      mResponse.buffer[26] = mHemisphere;

      /* set the status of an individual bird */
      setBirdStatus(flock);
  }

  if ( mRunMode == 0 ) {
      runContinuous();
  }
  else {
      singleShot();
  }

  mActive = true;

  return 0;
}

// ----------------------------------------------------------------------------
// Stops the driver.
// ----------------------------------------------------------------------------
void
aMotionStar::stop () {
    mCommand.sequence = htons(mSequenceNumber);
    mCommand.type     = BIRDNET::MSG_STOP_DATA;
    mNumberBytes      = send(mSocket, (void*) mLpCommand, sizeof(mCommand), 0);
} // end void aMotionStar::stop()

// ----------------------------------------------------------------------------
// Based on the current run mode, a single sample is taken (run mode is
// 1), or continuous samples are taken (run mode is 0).
// ----------------------------------------------------------------------------
void
aMotionStar::sample () {
//    cout << "Sampling..." << endl;
    if ( mRunMode == 1 ) {
        singleShot();
    }
//    cout << "After singleShot() " << endl;
/*
    double        x;
    long          p;
    unsigned int  xraw, yraw, zraw;
    int           xint, yint, zint;
    double                xreal, yreal, zreal;
    int           cpos;

*/
    int total_bytes_needed, total_bytes_received;
    char* lp_buffer;
    int header_bytes_received;
    time_t time_seconds;
    char time_char[64];
    struct tm newtime;

       /* receive the header */

    if ( mRunMode == 0 ) {
      header_bytes_received = 0;
      lp_buffer = (char*) mLpResponse;

      while ( header_bytes_received != 16 ) {
        mBytesReceived = recv(mSocket, (void*) lp_buffer, 16, 0);

      /*  if ( mBytesReceived < 0 ) {
          perror("recv1"), exit(1);
          } */

        header_bytes_received = header_bytes_received + mBytesReceived;
        lp_buffer             = (char*) lp_buffer + mBytesReceived;
      }
    }

    mSequenceNumber = ntohs(mResponse.header.sequence);

    //        printf("\n#%6d bytes received:%3d", , mBytesReceived);
    /*        printf("\ntype:%3d seq:%6d #bytes:%4d",mResponse.header.type,
			mResponse.header.sequence,mResponse.header.number_bytes); */

    /*  printf("\n#%6d:",n);
     */

    /*      for (i=0;i<16;++i)
            {
            printf("%3x",*(mNewptr+i));
            }
    */

    time_seconds = (unsigned int)(mResponse.header.time[0]) << 24;
    time_seconds = time_seconds | (unsigned int)(mResponse.header.time[1]) << 16;
    time_seconds = time_seconds | (unsigned int)(mResponse.header.time[2]) <<  8;
    time_seconds = time_seconds | (unsigned int)(mResponse.header.time[3]);
    localtime_r(&time_seconds, &newtime);
#ifdef __sun__
    asctime_r(&newtime, time_char, sizeof(time_char));
#else
    asctime_r(&newtime, time_char);
#endif

    /*  printf(" TIME: %10d ", time_seconds);     */

    if ( ntohs(mResponse.header.milliseconds) > 999 ) {
        short new_ms;

        new_ms = ntohs(mResponse.header.milliseconds) - 1000;
        mResponse.header.milliseconds = htons(new_ms);
    }
    /* printf(" %.19s.%3.3d ", time_char, mResponse.header.milliseconds);
     */
    /* printf(" %3.3d ",  mResponse.header.milliseconds);
     */

    total_bytes_received = 0;
    total_bytes_needed   = ntohs(mResponse.header.number_bytes);
    lp_buffer            = (char*) mLpResponse + 16;

    if ( mRunMode == 0 ) {
      struct timeval first, second;

      while ( total_bytes_received != total_bytes_needed ) {
        gettimeofday(&first, NULL);
        mBytesReceived = recv(mSocket, (void*) lp_buffer,
                              (total_bytes_needed - total_bytes_received), 0);
        gettimeofday(&second, NULL);

        if ( mBytesReceived < 0 ) {
          perror("recv2");
          exit(1);
        }

        if ( second.tv_usec - first.tv_usec > 5000 ) {
            fprintf(stderr,
                    "aMotionStar WARNING: Packet took longer than 5 ms\n");
        }

        total_bytes_received = total_bytes_received + mBytesReceived;
        lp_buffer            = (char*) lp_buffer + mBytesReceived;
      }

      if ( mResponse.header.error_code != 0 ) {
        printError(mResponse.header.error_code);
      }
    }
    //    printf(" .....received data #bytes = %d", totalBytesReceived);

    //    printf("\n");

    int o; // Offset -- 14 * bird number (bnum)
    for ( unsigned int bnum = 0; bnum < mBirdsRequired; bnum++ ) {
      o = 14*bnum;
      mPosinfo[bnum][0] = getXPos(o) / 12.0;	// X translation
      mPosinfo[bnum][1] = getYPos(o) / 12.0;	// Y translation
      mPosinfo[bnum][2] = getZPos(o) / 12.0;	// Z translation
      mPosinfo[bnum][3] = getZRot(o);		// Z rotation
      mPosinfo[bnum][4] = getYRot(o);		// Y rotation
      mPosinfo[bnum][5] = getXRot(o);		// X rotation
    }
}

// ----------------------------------------------------------------------------
// Set the address (either IP address or hostname) for the server.
// ----------------------------------------------------------------------------
void
aMotionStar::setAddress (const char* addr) {
  if ( addr != NULL ) {
    // Free the old memory before duplicating the new address string.
    if ( mAddress != NULL ) {
      free(mAddress);
    }

    mAddress = strdup(addr);
  }
}

// ============================================================================
// Private methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Send a wakeup call to the MotionStar server.
// ----------------------------------------------------------------------------
void
aMotionStar::sendWakeup () {
  /***** send a command to the server wakeup *****/
//  printf("WAKEUP:");

  mCommand.sequence             = htons(mSequenceNumber++);
  mCommand.type                 = BIRDNET::MSG_WAKE_UP;
  mCommand.xtype                = 0;
  mCommand.protocol             = 1;
  mCommand.number_bytes         = 0;
  mCommand.error_code           = 0;
  mCommand.error_code_extension = 0;

  /*n++;*/
  mNumberBytes = send(mSocket, (void*) mLpCommand, sizeof(mCommand), 0);

//  printf(" - number of bytes sent = %3d errorcode = %d", numberBytes,errno);

  mBytesReceived = recv(mSocket, (void*) mLpResponse, sizeof(mResponse), 0);

//  printf("  | WAKEUP ACKNOWLEDGE: - number bytes received = %5d\n", bytesReceived);
}

// ----------------------------------------------------------------------------
// Tell the MotionStar server to sample continuously.
// ----------------------------------------------------------------------------
void
aMotionStar::runContinuous () {
//  cout << "runContinous" << endl;

  mCommand.type     = BIRDNET::MSG_RUN_CONTINUOUS;
  mCommand.xtype    = 0;
  mCommand.sequence = htons(mSequenceNumber++);
//  n++;
  mNumberBytes   = send(mSocket, (void*) mLpCommand, sizeof(mCommand), 0);

//    printf("\n\nSTREAM: - number of bytes sent = %3d errorcode = %d", numberBytes,errno);

  mBytesReceived = recv(mSocket, (void*) mLpResponse, sizeof(mResponse), 0);

//    printf("  | STREAM ACKNOWLEDGE: - number bytes received = %5d\n", bytesReceived);


      /* receive the header */
/*
   int           headerBytesReceived;
   char          *lpBuffer;

    headerBytesReceived = 0;
    lpBuffer = (char*) mLpResponse;
    while(headerBytesReceived != 16) {
      bytesReceived = recv(s, (void*)lpBuffer, 16, 0);

      if (bytesReceived < 0) {
          perror("recv1"), exit(1);
      }

      headerBytesReceived = headerBytesReceived + bytesReceived;
      lpBuffer = (char *)lpBuffer + bytesReceived;
    }
*/
}

// ----------------------------------------------------------------------------
// Request a single sampling of the MotionStar server.
// ----------------------------------------------------------------------------
void
aMotionStar::singleShot () {
  /* send a request for a single shot packet */
  mCommand.type     = BIRDNET::MSG_SINGLE_SHOT;
  mCommand.xtype    = 0;
  mCommand.sequence = htons(mSequenceNumber++);
  mNumberBytes = send(mSocket, (void*) mLpCommand, sizeof(mCommand), 0);

    //printf("bytes sent = %d errno %d\n", numberBytes,errno);

  /* wait for the packet to come back */

  mBytesReceived = recv(mSocket, (void*) mLpResponse, sizeof(mResponse), 0);

    //printf("  >>> %3d; sequence=%4d; type=%4d; bytes received=%4d\n",n,mResponse.header.sequence
	//	,mResponse.header.type, bytesReceived);
}

// ----------------------------------------------------------------------------
// Get the system status.
// ----------------------------------------------------------------------------
void
aMotionStar::getSystemStatus () {
  void* lp_buffer;
  int header_bytes, data_bytes;
  int bytes_received;
  unsigned char all, FBBerror, flock_number;
  char sz_rate[7];
  int rate;

  mCommand.type  = BIRDNET::MSG_GET_STATUS;
  mCommand.xtype = 0;
  mNumberBytes = send(mSocket, (void*) mLpCommand, sizeof(mCommand), 0);

//  printf("\nGET STATUS - number bytes sent = %5d errorno %d", numberBytes,errno);

  /***** Receive packet from the server *****/

  header_bytes = 0;
  lp_buffer    = mLpResponse;

  while( header_bytes < 16 ) {
      bytes_received = recv(mSocket, (void*) lp_buffer, 16, 0);
      header_bytes   = header_bytes + bytes_received;
      lp_buffer      = (char*) lp_buffer + bytes_received;
  }

  data_bytes = 0;

  while ( data_bytes < ntohs(mResponse.header.number_bytes) ) {
      bytes_received = recv(mSocket, (void*) lp_buffer,
                            ntohs(mResponse.header.number_bytes), 0);
      data_bytes     = data_bytes + bytes_received;
      lp_buffer      = (char*) lp_buffer + bytes_received;
  }

  mStatusSize = header_bytes + data_bytes;
//  printf("\nSYSTEM STATUS RECEIVED - number bytes received = %5d,", bytesReceived);
//  printf("  type %d\n",mResponse.header.type);

  printf("\n============================================================\n\n");

  /* now print out the details of the status packet */

  all          = mResponse.buffer[0];
  FBBerror     = mResponse.buffer[1];
  flock_number = mResponse.buffer[2];
  printf("Number of FBB devices in system = %d\n", flock_number);

  mTransmitterNumber = (mResponse.buffer[4]>>4) & 0x0F;
  printf("Transmitter is at FBB address %d\n", mTransmitterNumber);

  sz_rate[0] = mResponse.buffer[5];
  sz_rate[1] = mResponse.buffer[6];
  sz_rate[2] = mResponse.buffer[7];
  sz_rate[3] = mResponse.buffer[8];
  sz_rate[4] = mResponse.buffer[9];
  sz_rate[5] = mResponse.buffer[10];
  sz_rate[6] = 0;

  mServerNumber     = mResponse.buffer[3];
  mChassisNumber    = mResponse.buffer[11];
  mChassisDevices   = mResponse.buffer[12];
  mFirstAddress     = mResponse.buffer[13];
  mSoftwareRevision = toShort(mResponse.buffer[14], mResponse.buffer[15]);
  rate              = atoi(sz_rate);
  mRealRate         = ((double) rate) / 1000.0;

  printf("Measurement rate = %6.1f\n", mRealRate);
  printf("Number of chassis in system = %d\n", mServerNumber);
  printf("Chassis# = %d\n", mChassisNumber);
  printf("Number of FBB devices in this chassis = %d\n", mChassisDevices);
  printf("First FBB device in this chassis = %d\n", mFirstAddress);
  printf("SERVER Software Version = %d\n\n", mSoftwareRevision);
  printf("============================================================\n\n");

/*
  for(i= 0;i<16;i++) printf(" %2x", mResponse.buffer[i]);
  printf("  |  ");
  for(i=16;i<48;i++) printf(" %2x", mResponse.buffer[i]);

  printf("\n\n");
*/

  for ( int i = 0; i < flock_number; i++ ) {
      mBird[i].status.status             = mResponse.buffer[16+i*8];
      mBird[i].status.id                 = mResponse.buffer[17+i*8];
      mBird[i].status.softwareRev        = mResponse.buffer[18+i*8] +
                                               (mResponse.buffer[19+i*8]*256);
      //      mBird[i].status.FBBStatus  = mResponse.buffer[20+i*8];
      mBird[i].status.errorCode  = mResponse.buffer[21+i*8];
      mBird[i].status.setup              = mResponse.buffer[22+i*8] +
        (mResponse.buffer[23+i*8]*256);
      // Lew
      mBird[i].status.hemisphere = mResponse.buffer[26+i*8];
//      printf("Bird hemisphere = %2x \n", mBird[i].status.hemisphere);
  }

//  printf("");

  /*
    for (i=0;i<bytesReceived;++i)
    {
    printf("%4d,",*(mNewptr+i));
    }
    printf("\n");
*/
}

// ----------------------------------------------------------------------------
// Set the system status.
// ----------------------------------------------------------------------------
void
aMotionStar::setSystemStatus () {
     int i;
     mResponse.header.type         = BIRDNET::MSG_SEND_SETUP;
     mResponse.header.xtype        = 0;
     mResponse.header.number_bytes = htons(mStatusSize - 16);

     for ( i = 0; i < 6; i++ ) {
        mResponse.buffer[5+i] = mRate[i];
     }

     mNumberBytes = send(mSocket,(void*) mLpResponse, mStatusSize, 0);

//     printf("\nSEND STATUS - number bytes sent = %5d errorNumber %d", numberBytes,errno);

     /***** Receive packet from the server *****/

     mBytesReceived = recv(mSocket, (void*) mLpResponse, sizeof(mResponse), 0);

//     printf("\nSEND STATUS ACK - number bytes received = %5d,", bytesReceived);
//     printf("  type %d\n",mResponse.header.type);

}

// ----------------------------------------------------------------------------
// Get the status of an individual bird.
// ----------------------------------------------------------------------------
void
aMotionStar::getBirdStatus (unsigned char fbb_addr) {
  int header_bytes, data_bytes;
  void* lp_buffer;

  mCommand.type         = BIRDNET::MSG_GET_STATUS;
  mCommand.xtype        = fbb_addr;
  mCommand.number_bytes = 0;

  mNumberBytes = send(mSocket, (void*) mLpCommand, sizeof(mCommand), 0);

//  printf("\n\nGET STATUS #%d; number bytes sent = %5d errorno %d ",fbb_addr, numberBytes,errno);

  /***** Receive packet from the server *****/

  header_bytes = 0;
  lp_buffer    = mLpResponse;

  while ( header_bytes < 16 ) {
      mBytesReceived = recv(mSocket, (void*) lp_buffer, 16, 0);
      header_bytes   = header_bytes + mBytesReceived;
      lp_buffer      = (char*) lp_buffer + mBytesReceived;
  }

  data_bytes = 0;

  while ( data_bytes < ntohs(mResponse.header.number_bytes) ) {
      mBytesReceived = recv(mSocket, (void*) lp_buffer,
                           ntohs(mResponse.header.number_bytes), 0);
      data_bytes     = data_bytes + mBytesReceived;
      lp_buffer      = (char*) lp_buffer + mBytesReceived;
  }

  mBytesReceived = data_bytes + header_bytes;

/*
  printf("\nSTATUS RECEIVED - number bytes received = %5d ", bytesReceived);
  printf("type %d ",mResponse.header.type);
  for (i=0;i<bytesReceived;++i)
    printf("%4d",*(mNewptr+i));
*/
}

// ----------------------------------------------------------------------------
// Set the status of an individual bird.
// ----------------------------------------------------------------------------
void
aMotionStar::setBirdStatus (unsigned char fbb_addr) {
  mResponse.header.type         = BIRDNET::MSG_SEND_SETUP;
  mResponse.header.xtype        = fbb_addr;
  mResponse.header.number_bytes = htons(70);

  mNumberBytes = send(mSocket, (void*) mLpResponse, 86, 0);

//  printf("\nSEND SETUP #%d; number bytes sent = %5d errorno %d ",fbb_addr, numberBytes,errno);

  /***** Receive packet from the server *****/

  mBytesReceived = recv(mSocket, (void*) mLpResponse, sizeof(mResponse), 0);

/*
  printf("\nSETUP ACKNOWLEDGED - number bytes received = %5d ", bytesReceived);
  printf("type %d ",mResponse.header.type);
  for (i=0;i<bytesReceived;++i)
    printf("%4d",*(mNewptr+i));
*/
}

// ----------------------------------------------------------------------------
// Print out the MotionStar information.
// ----------------------------------------------------------------------------
void
aMotionStar::printInfo () {
  int i;

  for (i = 0; i < 3; i++){
    printf("P = %f, %f, %f  A = %f, %f, %f \n", mPosinfo[i][0], mPosinfo[i][1],
           mPosinfo[i][2], mPosinfo[i][3], mPosinfo[i][4],mPosinfo[i][5]);
  }

  printf ("\n");
}

// ----------------------------------------------------------------------------
// Print out the MotionStar's header information.
// ----------------------------------------------------------------------------
void
aMotionStar::displayHdr () {
  /*printf("sequence=%6d time= %8d milliseconds=%6d type= %4d "
	,mResponse.header.sequence);*/
}

// ----------------------------------------------------------------------------
// Print the error message that corresponds to the given error code.  The
// message is based on the table on page 140 of the MotionStar manual.
// ----------------------------------------------------------------------------
void
aMotionStar::printError (const unsigned char err_code) {
    // Map the error code to a human-readable string.  These messages are
    // based on the table on page 140 of the MotionStar manual.
    switch (err_code) {
      case 1:
        fprintf(stderr, "WARNING: Single packet missing (error 1)\n");
        break;
      case 2:
        fprintf(stderr, "WARNING: Two or more packets missing (error 2)\n");
        break;
      case 3:
        fprintf(stderr, "WARNING: Many, many packets missing (error 3)\n");
        break;
      case 4:
        fprintf(stderr, "WARNING: Packet sequence number repeated (error 4)\n");
        break;
      case 6:
        fprintf(stderr, "WARNING: Unexpected packet type received (error 6)\n");
//        fprintf(stderr, "         Illegal packet sequence number: %hu\n",
//                mResponse.header.error_code_extension);
        fprintf(stderr, "         Re-syncing may be necessary!\n");
        break;
      case 7:
        fprintf(stderr,
                "WARNING: Packet contents could not be determined (error 7)\n");
        break;
      case 8:
        fprintf(stderr,
                "WARNING: Status requested for non-existant FBB device (error 8)\n");
        break;
      case 9:
        fprintf(stderr, "WARNING: Illegal setup data sent (error 9)\n");
        break;
      case 100:
        fprintf(stderr, "WARNING: System not ready (error 100)\n");
        break;
    }
}
