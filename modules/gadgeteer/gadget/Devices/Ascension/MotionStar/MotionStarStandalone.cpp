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
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <time.h>
#include <netdb.h>

#include <Input/vjPosition/aMotionStar.h>

aMotionStar::aMotionStar(char* _address,
			 int _hemisphere,
			 unsigned int _birdFormat,
			 unsigned int _birdsRequired,
			 int _runMode,
			 double _birdRate,
			 unsigned char _reportRate) :
				active(0),
				hemisphere(_hemisphere),
				birdFormat(_birdFormat),
				birdsRequired(_birdsRequired),
				runMode(_runMode),
				birdRate(_birdRate),
				reportRate(_reportRate)
{
  if ( _address != NULL ) {
    address = strdup(_address);
  } else {
    address = NULL;
  }

  m_xmtr_pos_scale = 288.0;
  m_xmtr_rot_scale = 180.0;
  m_xmtr_divisor   = 32767.0;
} // end aMotionStar::aMotionStar()



aMotionStar::~aMotionStar() {

  this->stop();

  if ( address != NULL ) {
    free(address);
    address = NULL;
  }

} // end aMotionStar::~aMotionStar()


void aMotionStar::start () {

  struct hostent* host_ent;

  cout << "In start" << endl;
  assert(!active);

  rate[0] = 57; // 48
  rate[1] = 48; // 50
  rate[2] = 48;
  rate[3] = 48;
  rate[4] = 48;
  rate[5] = 48;

  switch(birdFormat)
    {
    case 1:
      format = birdFormat | 0x30;
      break;

    case 2:
      format = birdFormat | 0x30;
      break;

    case 3:
      format = birdFormat | 0x90;
      break;

    case 4:
      format = birdFormat | 0x60;
      break;

    case 5:
      format = birdFormat | 0xC0;
      break;

    case 7:
      format = birdFormat | 0x40;
      break;

    case 8:
      format = birdFormat | 0x70;
      break;

    default:
      format = 0x64;
      break;
    }

  /*
   * Open a TCP socket.
   */

  if (( s = socket(AF_INET,SOCK_STREAM,0))<0)
    printf("client: can't open stream socket");

  /* Fill in the structure with the address of the
   * server that we want to connect to.
   */

  bzero((char *)&server, sizeof(server));
  server.sin_family=AF_INET;
  server.sin_port=htons(TCP_PORT);                              /* Server port number */

printf("Looking up %s\n", address);

  // Try to look up address by name.  This will work for an IP address too,
  // but we fall back on inet_addr(3) below just to be safe.
  host_ent = gethostbyname(address);

  // If host_ent is non-NULL, we found an IP address for the hostname in
  // address.  Move that address into the server struct.
  if ( host_ent != NULL ) {
    memmove((void*) &server.sin_addr.s_addr, (void*) host_ent->h_addr,
            sizeof(server.sin_addr.s_addr));
  }
  // Otherwise, assume that the value in address is already an IP address
  // and use inet_addr(3) on it.
  else {
    server.sin_addr.s_addr = inet_addr(address);
  }

  cout << "connecting..." << endl;
  rtn = connect(s, (struct sockaddr *) &server, sizeof(server));
  printf("connect = %4d, connect error = %4d, ", rtn, errno);
  perror(NULL);
  printf("\n");

  newptr =(char*)(&response);
  lpCommand = &command;
  lpRspGetStatAll=(struct RSP_GET_STATUS_all*)(newptr);
  lpResponse = &response;
  Size = sizeof(server);
  lpSize = &Size;

  /* send the wake up call */
  send_wakeup();

  /* get the system status */
  get_status_all();

  /* send the system setup */
  set_status_all();

   /* for 1 to n flock boards                    */
  /* get the individual bird status             */
  /* modify the appropriate contents            */
  /* send the individual bird status packet back        */

  for(int flock=1;flock<=chassisDevices;flock++)
    {
      /* get the status of an individual bird */
      get_status_fbb(flock);

                /* change the data format to something new for all birds */
      if(flock<=birdsRequired)
        {
          response.buffer[6] = format;
        }
      else
        {
          response.buffer[6] = 0;
        }

      /* set new report rate for all birds */
      response.buffer[7] = reportRate;
      response.buffer[26] = hemisphere;
      /* set the status of an individual bird */
      set_status_fbb(flock);
    }

  if (runMode == 0) {
      runContinuous();
  }
  else {
      singleShot();
  }
  active = true;

} // end void aMotionStar::start()


void aMotionStar::stop() {
    /* put in a STOP COMMAND here */

    command.sequence      = sequenceNumber;
    command.type          = 105;
    numberBytes           = sendto(s, (void*)lpCommand,
                                   sizeof(command),
                                   0,
                                   (struct sockaddr *)&server,
                                   sizeof(server));

} // end void aMotionStar::stop()


void aMotionStar::send_wakeup()
{
  /***** send a command to the server wakeup *****/
  printf("WAKEUP:");

  command.sequence              = sequenceNumber++;
  command.type                  = 10;
  command.xtype                 = 0;
  command.protocol              = 1;
  command.number_bytes          = 0;
  command.error_code            = 0;
  command.error_code_extension  = 0;

  /*n++;*/
  numberBytes = send(s,(void*)lpCommand, sizeof(command), 0);

  printf(" - number of bytes sent = %3d errorcode = %d", numberBytes,errno);

  bytesReceived = recv( s,
                        (void*)lpResponse,
                        sizeof(response),
                        0);

  printf("  | WAKEUP ACKNOWLEDGE: - number bytes received = %5d\n", bytesReceived);

} // end void aMotionStar::send_wakeup ()


void aMotionStar::runContinuous() {
  cout << "runContinous" << endl;

  command.type=104;
// MSG_RUN CONTINIOUS - request server to send packets continuously

  command.xtype = 0;
  command.sequence = sequenceNumber++;
  n++;
  numberBytes   = send(s,
                       (void*)lpCommand,
                       sizeof(command),
                       0);

    printf("\n\nSTREAM: - number of bytes sent = %3d errorcode = %d", numberBytes,errno);

  bytesReceived = recv( s,
                        (void*)lpResponse,
                        sizeof(response),
                        0);

    printf("  | STREAM ACKNOWLEDGE: - number bytes received = %5d\n", bytesReceived);


      /* receive the header */
/*
   int           headerBytesReceived;
   char          *lpBuffer;

    headerBytesReceived = 0;
    lpBuffer = (char*)lpResponse;
    while(headerBytesReceived != 16) {
      bytesReceived = recvfrom(s,
                               (void*)lpBuffer,
                               16,
                               0,
                               (struct sockaddr *)&server,
                               lpSize);
      if (bytesReceived < 0) {
          perror("recv1"), exit(1);
      }

      headerBytesReceived = headerBytesReceived + bytesReceived;
      lpBuffer = (char *)lpBuffer + bytesReceived;
    }
*/
} // end aMotionStar::runContinuous()


void aMotionStar::singleShot()
{
  /* send a request for a single shot packet */
  command.type=103; /* MSG_SINGLE_SHOT - request server to send single packet of data */
  command.xtype = 0;
  command.sequence = sequenceNumber++;
  numberBytes = sendto(s,(void*)lpCommand, sizeof(command), 0,
                       (struct sockaddr *)&server, sizeof(server));

    //printf("bytes sent = %d errno %d\n", numberBytes,errno);

  /* wait for the packet to come back */

  bytesReceived = recvfrom(s, (void*)lpResponse, sizeof(response), 0,
                           (struct sockaddr *)&server, lpSize);
    //printf("  >>> %3d; sequence=%4d; type=%4d; bytes received=%4d\n",n,response.header.sequence
	//	,response.header.type, bytesReceived);

} // end void aMotionStar::singleShot()


void aMotionStar::sample() {
//    cout << "Sampling..." << endl;
    if (runMode == 1) {singleShot();}
//    cout << "After singleShot() " << endl;
/*
    double        x;
    long          p;
    unsigned int  xraw, yraw, zraw;
    int           xint, yint, zint;
    double                xreal, yreal, zreal;
    int           cpos;

*/
    int           totalBytesNeeded, totalBytesReceived;
    char          *lpBuffer;
    int           headerBytesReceived;

       /* receive the header */

    if (runMode == 0) {

      headerBytesReceived = 0;
      lpBuffer = (char*)lpResponse;
      while(headerBytesReceived != 16) {
        bytesReceived = recvfrom(s,
                               (void*)lpBuffer,
                               16,
                               0,
                               (struct sockaddr *)&server,
                               lpSize);
      /*  if (bytesReceived < 0){
          perror("recv1"), exit(1);
          } */

        headerBytesReceived = headerBytesReceived + bytesReceived;
        lpBuffer = (char *)lpBuffer + bytesReceived;
      }
    }
    sequenceNumber = response.header.sequence;

    //        printf("\n#%6d bytes received:%3d",n ,bytesReceived);
    /*        printf("\ntype:%3d seq:%6d #bytes:%4d",response.header.type,
			response.header.sequence,response.header.number_bytes); */

    /*  printf("\n#%6d:",n);
     */

    /*      for (i=0;i<16;++i)
            {
            printf("%3x",*(newptr+i));
            }
    */

    timeSeconds = (unsigned int)(response.header.time[0]) << 24;
    timeSeconds = timeSeconds | (unsigned int)(response.header.time[1]) << 16;
    timeSeconds = timeSeconds | (unsigned int)(response.header.time[2]) <<  8;
    timeSeconds = timeSeconds | (unsigned int)(response.header.time[3]);
    localtime_r(&timeSeconds, &newtime);
#ifdef __sun__
    asctime_r(&newtime, timeChar, sizeof(timeChar));
#else
    asctime_r(&newtime, timeChar);
#endif

    /*  printf(" TIME: %10d ", timeSeconds);     */
    if(response.header.milliseconds>999)
      {
        response.header.milliseconds = response.header.milliseconds - 1000;
      }
    /* printf(" %.19s.%3.3d ", timeChar, response.header.milliseconds);
     */
    /* printf(" %3.3d ",  response.header.milliseconds);
     */

    totalBytesReceived = 0;
    totalBytesNeeded = response.header.number_bytes;
    lpBuffer = (char*)lpResponse + 16;

    if (runMode == 0) {
      while(totalBytesReceived != totalBytesNeeded){
        bytesReceived = recvfrom(s,
                               (void*)lpBuffer,
                               (totalBytesNeeded-totalBytesReceived),
                               0,
                               (struct sockaddr *)&server,
                               lpSize);
        if (bytesReceived < 0)
          perror("recv2"), exit(1);

        totalBytesReceived = totalBytesReceived + bytesReceived;
        lpBuffer = (char*)lpBuffer + bytesReceived;

      }
    }
    //    printf(" .....received data #bytes = %d", totalBytesReceived);

    //    printf("\n");

    int o; // Offset -- 14 * bird number (bnum)
    for (int bnum = 0; bnum < birdsRequired; bnum++)
    {
      o = 14*bnum;
      posinfo[bnum][0] = getXPos(o) / 12.0;	// X translation
      posinfo[bnum][1] = getYPos(o) / 12.0;	// Y translation
      posinfo[bnum][2] = getZPos(o) / 12.0;	// Z translation
      posinfo[bnum][3] = getRoll(o);		// Z rotation (roll)
      posinfo[bnum][4] = getAzimuth(o);		// Y rotation (azimuth)
      posinfo[bnum][5] = getElevation(o);	// X rotatoin (elevation)
    } // end for loop


} // end void aMotionStar::sample()


void aMotionStar::printInfo(){
  int i;

  for (i = 0; i < 3; i++){
    printf("P = %f, %f, %f  A = %f, %f, %f \n", posinfo[i][0], posinfo[i][1],
           posinfo[i][2], posinfo[i][3], posinfo[i][4],posinfo[i][5]);
  }
  printf ("\n");

} // end void aMotionStar::printinfo()

void aMotionStar::get_status_all()
{
  void                  *lpBuffer;
  int                   headerBytes, dataBytes;
  int                   bytesReceived;

  command.type=101;
  command.xtype=0;
  numberBytes = sendto(s,(void*)lpCommand, sizeof(command), 0,
                       (struct sockaddr *)&server, sizeof(server));
  printf("\nGET STATUS - number bytes sent = %5d errorno %d", numberBytes,errno)
;

  /***** Receive packet from the server *****/

  headerBytes = 0;
  lpBuffer = lpResponse;
  while(headerBytes<16)
    {
      bytesReceived = recvfrom(s, (void*)lpBuffer, 16, 0, (struct sockaddr *)&server, lpSize);
      headerBytes = headerBytes + bytesReceived;
      lpBuffer = (char *)lpBuffer + bytesReceived;
    }

  dataBytes = 0;
  while(dataBytes<(response.header.number_bytes))
    {
      bytesReceived = recvfrom(s, (void*)lpBuffer, response.header.number_bytes,

                               0, (struct sockaddr *)&server, lpSize);
      dataBytes = dataBytes + bytesReceived;
      lpBuffer = (char *)lpBuffer + bytesReceived;
    }

  statusSize = headerBytes + dataBytes;
  printf("\nSYSTEM STATUS RECEIVED - number bytes received = %5d,", bytesReceived);
  printf("  type %d\n",response.header.type);

  printf("\n\n============================================================\n\n");

  /* now print out the details of the status packet */

  all = response.buffer[0];

  FBBerror = response.buffer[1];

  flockNumber = response.buffer[2];
  printf("Number of FBB devices in system = %d,  ", flockNumber);

  transmitterNumber = (response.buffer[4]>>4) & 0x0F;
  printf("Transmitter is at FBB address %d,  ", transmitterNumber);

  szRate[0] = response.buffer[5];
  szRate[1] = response.buffer[6];
  szRate[2] = response.buffer[7];
  szRate[3] = response.buffer[8];
  szRate[4] = response.buffer[9];
  szRate[5] = response.buffer[10];
  szRate[6] = 0;

 serverNumber = response.buffer[3];
  chassisNumber = response.buffer[11];
  chassisDevices = response.buffer[12];
  firstAddress = response.buffer[13];
  softwareRevision = (((unsigned short)(response.buffer[14])<<8) & 0xFF00)
    | ((unsigned short)(response.buffer[15]));


  mRate = atoi(szRate);
  realRate = (double)(mRate)/1000;
  printf("Measurement rate = %6.1f\n", realRate);

  printf("Number of chassis in system = %d, \n", serverNumber);
  printf("Chassis# = %d, ", chassisNumber);
  printf("Number of FBB devices in this chassis = %d, ", chassisDevices);
  printf("First FBB device in this chassis = %d, \n", firstAddress);
  printf("SERVER Software Version = %d\n\n============================================================\n\n",
         softwareRevision);
  printf("\n\n");


  for(i= 0;i<16;i++) printf(" %2x", response.buffer[i]);
  printf("  |  ");
  for(i=16;i<48;i++) printf(" %2x", response.buffer[i]);

  printf("\n\n");


  for(i=0;i<flockNumber;i++)
    {
      Bird[i].status.status             = response.buffer[16+i*8];
      Bird[i].status.id                 = response.buffer[17+i*8];
      Bird[i].status.softwareRev        = response.buffer[18+i*8] +
        (response.buffer[19+i*8]*256);
      //      Bird[i].status.FBBStatus  = response.buffer[20+i*8];
      Bird[i].status.errorCode  = response.buffer[21+i*8];
      Bird[i].status.setup              = response.buffer[22+i*8] +
        (response.buffer[23+i*8]*256);
      // Lew
      Bird[i].status.hemisphere = response.buffer[26+i*8];
      printf("Bird hemisphere = %2x \n", Bird[i].status.hemisphere);
    }

  printf("");

  /*
    for (i=0;i<bytesReceived;++i)
    {
    printf("%4d,",*(newptr+i));
    }
    printf("\n");
*/

} // end void aMotionStar::get_status_all()



void aMotionStar::set_status_all()
{
     int i;
     response.header.type=102;
     response.header.xtype=0;
     response.header.number_bytes = statusSize-16;

     for(i=0;i<6;i++)
        {
        response.buffer[5+i] = rate[i];
        }

     numberBytes = sendto(s,(void*)lpResponse, statusSize, 0,
                 (struct sockaddr *)&server, sizeof(server));

     printf("\nSEND STATUS - number bytes sent = %5d errorNumber %d", numberBytes,errno);

     /***** Receive packet from the server *****/

     bytesReceived = recvfrom(s, (void*)lpResponse, sizeof(response), 0,
                              (struct sockaddr *)&server, lpSize);
     printf("\nSEND STATUS ACK - number bytes received = %5d,", bytesReceived);
     printf("  type %d\n",response.header.type);

} // end void aMotionStar::set_status_all()



void aMotionStar::get_status_fbb(unsigned char fbb_addr)
{
  int headerBytes, dataBytes;
  void * lpBuffer;


  command.type=101;
  command.xtype=fbb_addr;
  command.number_bytes = 0;

  numberBytes = sendto(s,(void*)lpCommand, sizeof(command), 0,
                       (struct sockaddr *)&server, sizeof(server));
  printf("\n\nGET STATUS #%d; number bytes sent = %5d errorno %d ",fbb_addr, numberBytes,errno);

  /***** Receive packet from the server *****/

  headerBytes = 0;
  lpBuffer = lpResponse;
  while(headerBytes<16)
    {
      bytesReceived = recvfrom(s, (void*)lpBuffer, 16, 0, (struct sockaddr *)&server, lpSize);
      headerBytes = headerBytes + bytesReceived;
      lpBuffer = (char *)lpBuffer + bytesReceived;
    }

  dataBytes = 0;
  while(dataBytes<(response.header.number_bytes))
    {
      bytesReceived = recvfrom(s, (void*)lpBuffer, response.header.number_bytes,

                               0, (struct sockaddr *)&server, lpSize);
      dataBytes = dataBytes + bytesReceived;
      lpBuffer = (char *)lpBuffer + bytesReceived;
    }

  bytesReceived = dataBytes + headerBytes;

  printf("\nSTATUS RECEIVED - number bytes received = %5d ", bytesReceived);
  printf("type %d ",response.header.type);
  for (i=0;i<bytesReceived;++i)
    printf("%4d",*(newptr+i));

} // end void aMotionStar::get_status_fbb()



void aMotionStar::display_hdr()
{


  /*printf("sequence=%6d time= %8d milliseconds=%6d type= %4d "
	,response.header.sequence);*/

} // end void aMotionStar::display_hdr()



void aMotionStar::set_status_fbb(unsigned char fbb_addr)
{
  response.header.type = 102;
  response.header.xtype = fbb_addr;
  response.header.number_bytes = 70;

  numberBytes = sendto(s,(void*)lpResponse, 86, 0,
                       (struct sockaddr *)&server, sizeof(server));
  printf("\nSEND SETUP #%d; number bytes sent = %5d errorno %d ",fbb_addr, numberBytes,errno);

  /***** Receive packet from the server *****/

  bytesReceived = recvfrom(s, (void*)lpResponse, sizeof(response), 0,
                           (struct sockaddr *)&server, lpSize);
  printf("\nSETUP ACKNOWLEDGED - number bytes received = %5d ", bytesReceived);
  printf("type %d ",response.header.type);
  for (i=0;i<bytesReceived;++i)
    printf("%4d",*(newptr+i));

} // end void aMotionStar::set_status_fbb()


void aMotionStar::setHemisphere (int n) { hemisphere = n; }
void aMotionStar::setBirdFormat (unsigned int n) { birdFormat = n; }
void aMotionStar::setNumBirds (unsigned int n) { birdsRequired = n; }
void aMotionStar::setBirdRate (double n) { birdRate = n; }
void aMotionStar::setRunMode ( int n ) { runMode = n; }
void aMotionStar::setReportRate (unsigned char n) { reportRate = n; }

void
aMotionStar::setAddress (const char* n) {
  if ( n != NULL ) {
    // Free the old memory before duplicating the new address string.
    if ( address != NULL ) {
      free(address);
    }

    address = strdup(n);
  }
}

float aMotionStar::xPos( int i) {return posinfo[i][0];}
float aMotionStar::yPos( int i) {return posinfo[i][1];}
float aMotionStar::zPos( int i) {return posinfo[i][2];}
float aMotionStar::zRot( int i) {return posinfo[i][3];}
float aMotionStar::yRot( int i) {return posinfo[i][4];}
float aMotionStar::xRot( int i) {return posinfo[i][5];}

bool& aMotionStar::isActive() {return active;}
