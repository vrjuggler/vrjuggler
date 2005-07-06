/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#include <vjConfig.h>

#include <Input/vjPosition/vjFastrack.h>

void
printError(std::string errorMsg) {
#ifdef _FASTRACK_DEBUG_
  std::cout << errorMsg << std:endl;
#endif
}

vjFastrack::vjFastrack() : sampleThread( NULL ) {
  ;
}

vjFastrack::~vjFastrack() {
  this->stopSampling();
  trackerFinish();
}

bool
vjFastrack::config( vjConfigChunk *fastrackChunk ) {
  if (!vjDigital::config(fastrackChunk))
    return FALSE;

  // configFile = "/home/kruger/VRjuggler/juggler/Input/vjPosition/ft_config.dat";

  // *****************************************************************************************
  // trackerInit(configFile);

  static struct termios tc;
  char buf[256];
  char c;

  // *****************************************************************************************
  // readconfig(configfile);

  //  FILE *fp;
  // char configline[100];
  int station;
  struct perstation *psp;

  // port
  strcpy(conf.port, ( (static_cast<std::string>(fastrackChunk->getProperty("port"))).c_str()));
  conf.found |= 1<<DEV;

  // baud
  conf.baud = fastrackChunk->getProperty("baud");
  conf.found |= 1<<BAUD;

  // button
  switch( static_cast<int>(fastrackChunk->getProperty("button")) ) {
  case 0: conf.button = '1'; conf.cont='C'; break;
  case 1: conf.button = '1'; conf.cont='c'; break;
  case 2: conf.button = '0'; conf.cont='C'; break;
  case 3: conf.button = '0'; conf.cont='c'; break;
  default: printError( "ERROR: vjFastrack::config : not a valid configuration"); break;
  }
  conf.found |= 1<<BUTTON;

  // Rec Pos
  if ( fastrackChunk->getProperty("Rec1", 0) ) {
    conf.perstation[0].rec |= 1<<Pos;
  }
  else {
    conf.perstation[0].rec &= ~(1<<Pos);
  }
  conf.found |= 1<<REC;

  if ( fastrackChunk->getProperty("Rec2", 0) ) {
    conf.perstation[1].rec |= 1<<Pos;
  }
  else {
    conf.perstation[1].rec &= ~(1<<Pos);
  }
  conf.found |= 1<<REC1;

  if ( fastrackChunk->getProperty("Rec3", 0) ) {
    conf.perstation[2].rec |= 1<<Pos;
  }
  else {
    conf.perstation[2].rec &= ~(1<<Pos);
  }
  conf.found |= 1<<REC2;

  if ( fastrackChunk->getProperty("Rec4", 0) ) {
    conf.perstation[3].rec |= 1<<Pos;
  }
  else {
    conf.perstation[3].rec &= ~(1<<Pos);
  }
  conf.found |= 1<<REC3;

  // Rec Ang
  if ( fastrackChunk->getProperty("Rec1", 1) ) conf.perstation[0].rec |= 1<<Ang;
  else conf.perstation[0].rec &= ~(1<<Ang);
  if ( fastrackChunk->getProperty("Rec2", 1) ) conf.perstation[1].rec |= 1<<Ang;
  else conf.perstation[1].rec &= ~(1<<Ang);
  if ( fastrackChunk->getProperty("Rec3", 1) ) conf.perstation[2].rec |= 1<<Ang;
  else conf.perstation[2].rec &= ~(1<<Ang);
  if ( fastrackChunk->getProperty("Rec4", 1) ) conf.perstation[3].rec |= 1<<Ang;
  else conf.perstation[3].rec &= ~(1<<Ang);

  // Rec Quat
  if ( fastrackChunk->getProperty("Rec1", 2) ) conf.perstation[0].rec |= 1<<Quat;
  else conf.perstation[0].rec &= ~(1<<Quat);
  if ( fastrackChunk->getProperty("Rec2", 2) ) conf.perstation[1].rec |= 1<<Quat;
  else conf.perstation[1].rec &= ~(1<<Quat);
  if ( fastrackChunk->getProperty("Rec3", 2) ) conf.perstation[2].rec |= 1<<Quat;
  else conf.perstation[2].rec &= ~(1<<Quat);
  if ( fastrackChunk->getProperty("Rec4", 2) ) conf.perstation[3].rec |= 1<<Quat;
  else conf.perstation[3].rec &= ~(1<<Quat);

  // Rec But
  if ( fastrackChunk->getProperty("Rec1", 3) ) conf.perstation[0].rec |= 1<<But;
  else conf.perstation[0].rec &= ~(1<<But);

  // TIP
  conf.perstation[0].tip[0] = fastrackChunk->getProperty("Tip1", 0);
  conf.perstation[0].tip[1] = fastrackChunk->getProperty("Tip1", 1);
  conf.perstation[0].tip[2] = fastrackChunk->getProperty("Tip1", 2);
  conf.found |= 1<<TIP;

  conf.perstation[1].tip[0] = fastrackChunk->getProperty("Tip2", 0);
  conf.perstation[1].tip[1] = fastrackChunk->getProperty("Tip2", 1);
  conf.perstation[1].tip[2] = fastrackChunk->getProperty("Tip2", 2);
  conf.found |= 1<<TIP1;

  conf.perstation[2].tip[0] = fastrackChunk->getProperty("Tip3", 0);
  conf.perstation[2].tip[1] = fastrackChunk->getProperty("Tip3", 1);
  conf.perstation[2].tip[2] = fastrackChunk->getProperty("Tip3", 2);
  conf.found |= 1<<TIP2;

  conf.perstation[3].tip[0] = fastrackChunk->getProperty("Tip4", 0);
  conf.perstation[3].tip[1] = fastrackChunk->getProperty("Tip4", 1);
  conf.perstation[3].tip[2] = fastrackChunk->getProperty("Tip4", 2);
  conf.found |= 1<<TIP3;

  // INC
  conf.perstation[0].inc = fastrackChunk->getProperty("Inc");
  conf.found |= 1<<INC;
  conf.perstation[1].inc = fastrackChunk->getProperty("Inc");
  conf.found |= 1<<INC1;
  conf.perstation[2].inc = fastrackChunk->getProperty("Inc");
  conf.found |= 1<<INC2;
  conf.perstation[3].inc = fastrackChunk->getProperty("Inc");
  conf.found |= 1<<INC3;

  // HEM
  conf.perstation[0].hem[0] = fastrackChunk->getProperty("Hem1", 0);
  conf.perstation[0].hem[1] = fastrackChunk->getProperty("Hem1", 1);
  conf.perstation[0].hem[2] = fastrackChunk->getProperty("Hem1", 2);
  conf.found |= 1<<HEM;

  conf.perstation[1].hem[0] = fastrackChunk->getProperty("Hem2", 0);
  conf.perstation[1].hem[1] = fastrackChunk->getProperty("Hem2", 1);
  conf.perstation[1].hem[2] = fastrackChunk->getProperty("Hem2", 2);
  conf.found |= 1<<HEM1;

  conf.perstation[2].hem[0] = fastrackChunk->getProperty("Hem3", 0);
  conf.perstation[2].hem[1] = fastrackChunk->getProperty("Hem3", 1);
  conf.perstation[2].hem[2] = fastrackChunk->getProperty("Hem3", 2);
  conf.found |= 1<<HEM2;

  conf.perstation[3].hem[0] = fastrackChunk->getProperty("Hem4", 0);
  conf.perstation[3].hem[1] = fastrackChunk->getProperty("Hem4", 1);
  conf.perstation[3].hem[2] = fastrackChunk->getProperty("Hem4", 2);
  conf.found |= 1<<HEM3;

  // ARF
  conf.perstation[0].arf[0] = fastrackChunk->getProperty("ARF1", 0);
  conf.perstation[0].arf[1] = fastrackChunk->getProperty("ARF1", 1);
  conf.perstation[0].arf[2] = fastrackChunk->getProperty("ARF1", 2);
  conf.perstation[0].arf[3] = fastrackChunk->getProperty("ARF1", 3);
  conf.perstation[0].arf[4] = fastrackChunk->getProperty("ARF1", 4);
  conf.perstation[0].arf[5] = fastrackChunk->getProperty("ARF1", 5);
  conf.perstation[0].arf[6] = fastrackChunk->getProperty("ARF1", 6);
  conf.perstation[0].arf[7] = fastrackChunk->getProperty("ARF1", 7);
  conf.perstation[0].arf[8] = fastrackChunk->getProperty("ARF1", 8);
  conf.found |= 1<<ARF;

  conf.perstation[1].arf[0] = fastrackChunk->getProperty("ARF2", 0);
  conf.perstation[1].arf[1] = fastrackChunk->getProperty("ARF2", 1);
  conf.perstation[1].arf[2] = fastrackChunk->getProperty("ARF2", 2);
  conf.perstation[1].arf[3] = fastrackChunk->getProperty("ARF2", 3);
  conf.perstation[1].arf[4] = fastrackChunk->getProperty("ARF2", 4);
  conf.perstation[1].arf[5] = fastrackChunk->getProperty("ARF2", 5);
  conf.perstation[1].arf[6] = fastrackChunk->getProperty("ARF2", 6);
  conf.perstation[1].arf[7] = fastrackChunk->getProperty("ARF2", 7);
  conf.perstation[1].arf[8] = fastrackChunk->getProperty("ARF2", 8);
  conf.found |= 1<<ARF1;

  conf.perstation[2].arf[0] = fastrackChunk->getProperty("ARF3", 0);
  conf.perstation[2].arf[1] = fastrackChunk->getProperty("ARF3", 1);
  conf.perstation[2].arf[2] = fastrackChunk->getProperty("ARF3", 2);
  conf.perstation[2].arf[3] = fastrackChunk->getProperty("ARF3", 3);
  conf.perstation[2].arf[4] = fastrackChunk->getProperty("ARF3", 4);
  conf.perstation[2].arf[5] = fastrackChunk->getProperty("ARF3", 5);
  conf.perstation[2].arf[6] = fastrackChunk->getProperty("ARF3", 6);
  conf.perstation[2].arf[7] = fastrackChunk->getProperty("ARF3", 7);
  conf.perstation[2].arf[8] = fastrackChunk->getProperty("ARF3", 8);
  conf.found |= 1<<ARF2;

  conf.perstation[3].arf[0] = fastrackChunk->getProperty("ARF4", 0);
  conf.perstation[3].arf[1] = fastrackChunk->getProperty("ARF4", 1);
  conf.perstation[3].arf[2] = fastrackChunk->getProperty("ARF4", 2);
  conf.perstation[3].arf[3] = fastrackChunk->getProperty("ARF4", 3);
  conf.perstation[3].arf[4] = fastrackChunk->getProperty("ARF4", 4);
  conf.perstation[3].arf[5] = fastrackChunk->getProperty("ARF4", 5);
  conf.perstation[3].arf[6] = fastrackChunk->getProperty("ARF4", 6);
  conf.perstation[3].arf[7] = fastrackChunk->getProperty("ARF4", 7);
  conf.perstation[3].arf[8] = fastrackChunk->getProperty("ARF4", 8);
  conf.found |= 1<<ARF3;

  // TMF
  conf.perstation[0].tmf[0] = fastrackChunk->getProperty("TMF1", 0);
  conf.perstation[0].tmf[1] = fastrackChunk->getProperty("TMF1", 1);
  conf.perstation[0].tmf[2] = fastrackChunk->getProperty("TMF1", 2);
  conf.found |= 1<<TMF;

  conf.perstation[1].tmf[0] = fastrackChunk->getProperty("TMF2", 0);
  conf.perstation[1].tmf[1] = fastrackChunk->getProperty("TMF2", 1);
  conf.perstation[1].tmf[2] = fastrackChunk->getProperty("TMF2", 2);
  conf.found |= 1<<TMF1;

  conf.perstation[2].tmf[0] = fastrackChunk->getProperty("TMF3", 0);
  conf.perstation[2].tmf[1] = fastrackChunk->getProperty("TMF3", 1);
  conf.perstation[2].tmf[2] = fastrackChunk->getProperty("TMF3", 2);
  conf.found |= 1<<TMF2;

  conf.perstation[3].tmf[0] = fastrackChunk->getProperty("TMF4", 0);
  conf.perstation[3].tmf[1] = fastrackChunk->getProperty("TMF4", 1);
  conf.perstation[3].tmf[2] = fastrackChunk->getProperty("TMF4", 2);
  conf.found |= 1<<TMF3;

  conf.len = 3;
  for (station = 0; station < NSTATION; ++station) {
    psp = &conf.perstation[station];
    if (psp->rec)
      psp->begin = conf.len - 3;
    if (psp->rec & (1<<Pos)) {
      psp->posoff = conf.len;
      conf.len +=  XYZ*sizeof (int);
    }
    if (psp->rec & (1<<Ang)) {
      psp->angoff = conf.len;
      conf.len +=  XYZ*sizeof (int);
    }
    if (psp->rec & (1<<Quat)) {
      psp->quatoff = conf.len;
      conf.len +=  4*sizeof (int);
    }
    if (psp->rec & (1<<But)) {
      psp->butoff = conf.len + 1;
      conf.len +=  2;
    }
    if (psp->rec)
      conf.len += 5;
  }
  conf.len -= 3;

  // end readconfig(configfile);
  // *****************************************************************************************


  trackerfd = open(conf.port, O_RDWR|O_NONBLOCK, 0);
  tc.c_cc[VMIN] = conf.len;
  tc.c_cc[VTIME] = 1;
  tc.c_cflag = CS8|CREAD|CLOCAL;
  tc.c_ospeed = B9600;
  if (conf.found & (1<<BAUD))
    switch (conf.baud) {
    case 38400: tc.c_ospeed = B38400; break;
    case 19200: tc.c_ospeed = B19200; break;
    case 9600: tc.c_ospeed = B9600; break;
    case 4800: tc.c_ospeed = B4800; break;
    case 2400: tc.c_ospeed = B2400; break;
    case 1200: tc.c_ospeed = B1200; break;
    case 600: tc.c_ospeed = B600; break;
    case 300: tc.c_ospeed = B300; break;
    }
  tc.c_ispeed = tc.c_ospeed;
  tcsetattr(trackerfd, TCSAFLUSH, &tc);
  fcntl(trackerfd, F_SETFL, 0);
  write(trackerfd, "uf", 3);	/* unites: cm, mode binaire */
  for (station = 0; station < NSTATION; ++station) {
    struct perstation *psp = &(conf.perstation[station]);
    int len;
    c = '1' + station;
    write(trackerfd, "e", 1);
    write(trackerfd, &c, 1);
    write(trackerfd, ",",1);
    write(trackerfd, &conf.button,1);
    write(trackerfd, "\r",1);
    write(trackerfd, "l",1);
    write(trackerfd, &c, 1);
    if (conf.found&(1<<(REC+station)))
      if (psp->rec == 0)
	write(trackerfd, ",0\r", 3);
      else {
	write(trackerfd, ",1\rO", 4);
	write(trackerfd, &c, 1);
	if (psp->rec & (1<<Pos))
	  write(trackerfd, ",2", 2);
	if (psp->rec & (1<<Ang))
	  write(trackerfd, ",4", 2);
	if (psp->rec & (1<<Quat))
	  write(trackerfd, ",11", 3);
	if (psp->rec & (1<<But))
	  write(trackerfd, ",16", 3);
	write(trackerfd, ",1\r", 3);
      }
    if (conf.found&(1<<(TIP+station))) {
      len = sprintf(buf, "N%c,%.2f,%.2f,%.2f\r", c,
		    psp->tip[0],
		    psp->tip[1],
		    psp->tip[2]);
      write(trackerfd, buf, len);
    }
    if (conf.found&(1<<(INC+station))) {
      len = sprintf(buf, "I%c,%.2f\r", c, psp->inc);
      write(trackerfd, buf, len);
    }
    if (conf.found&(1<<(HEM+station))) {
      len = sprintf(buf, "H%c,%.2f,%.2f,%.2f\r", c,
		    psp->hem[0],
		    psp->hem[1],
		    psp->hem[2]);
      write(trackerfd, buf, len);
    }
    if (conf.found&(1<<(TMF+station))) {
      len = sprintf(buf, "r%c,%.2f,%.2f,%.2f\r", c,
		    psp->tmf[0],
		    psp->tmf[1],
		    psp->tmf[2]);
      write(trackerfd, buf, len);
    }
    if (conf.found&(1<<(ARF+station))) {
      len = sprintf(buf, "A%c,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r",
		    c,
		    psp->arf[0],
		    psp->arf[1],
		    psp->arf[2],
		    psp->arf[3],
		    psp->arf[4],
		    psp->arf[5],
		    psp->arf[6],
		    psp->arf[7],
		    psp->arf[8]);
      write(trackerfd, buf, len);
    }
  }
  /*
  if (conf.cont == 'C') {
    ReadPid = sproc(readloop, PR_SALL);
    if (ReadPid == -1) {
      perror("Can't creat read process");
      exit(1);
    }
  }
  */
  write(trackerfd, &conf.cont, 1);
  // end trackerInit
  // *****************************************************************************************

  return TRUE;
}

int
vjFastrack::startSampling() {
  sampleThread = new vjThread( threadedSampleFunction, (void*)this, 0 );
  if (!sampleThread->valid())
    return 0; // thread creation failed
  else
    return 1; // thread creation succeded
}

// Record (or sample) the current data
// this is called repeatedly by the sample thread created by startSampling()
int
vjFastrack::sample() {
  buttonState[progress] = ::getCoords(15,
				      &trackersPosition[0][0][progress],
				      &trackersOrientation[0][0][progress]);  //NB: 15 = 1111b = the 4 stations
  return 1;
}

void
vjFastrack::updateData() {
  vjGuard<vjMutex> updateGuard(lock);

  // copy the valid data to the current data so that both are valid
  buttonState[current] = buttonState[valid];
  for (int i=0; i<4; i++) {    // for each station
    for (int j=0; j<3; j++) {      // for each coordinate
      trackersPosition[i][j][current] = trackersPosition[i][j][valid];
      trackersOrientation[i][j][current] = trackersOrientation[i][j][valid];
    }
  }

  // swap the indicies for the tri-buffer pointers
  vjInput::swapCurrentIndexes();
}

// kill sample thread
int
vjFastrack::stopSampling() {
  if (sampleThread != NULL)
    {
      sampleThread->kill();
      delete sampleThread;
      sampleThread = NULL;
    }
  return 1;
}

// function for users to get the digital data.
// overload vjDigital::getDigitalData
// digital data only on the first station
int
vjFastrack::getDigitalData( int station ) {
  if ( station != 0 ) {
    std::cout << "error in vjFastrack::getDigitalData: invalid station:" << station << std::endl;
    return NULL;
  }
  else
    return buttonState[current];
}

// function for users to get the position data
// overload vjPosition::getPosData
vjMatrix *
vjFastrack::getPosData( int station ) {
  // see vjAnalog.h
  //! NOTE: TO ALL ANALOG DEVICE DRIVER WRITERS, you *must* normalize your data using vjAnalog::normalizeMinToMax()

  // assert( axis >= 0 && axis <= 2 && "3 (x,y and z) axes available");
  if ( !( (0<=station) && (station<=3) ) ) {
    std::cout << "error in vjFastrack::getPosData: invalid station:" << station << std::endl;
    return NULL;
  }
  else {
    vjMatrix *positionMatrix = new vjMatrix();

    positionMatrix->makeXYZEuler(trackersOrientation[station][0][current],
				 trackersOrientation[station][1][current],
				 trackersOrientation[station][2][current]);
    positionMatrix->setTrans(trackersPosition[station][0][current],
			     trackersPosition[station][1][current],
			     trackersPosition[station][2][current]);
    return positionMatrix;
  }
}

void
vjFastrack::threadedSampleFunction( void* classPointer ) {
  vjFastrack* this_ptr = static_cast<vjFastrack*>( classPointer );

  while (1)   
    {
      this_ptr->sample();
      if (conf.cont != 'C')
	sleep(1);
    }
}

// ************************************************************************************************
// driver R.E.

void
trackerFinish()
{
  pid_t pid;

  pid = ReadPid;
  ReadPid = 0;
  write(trackerfd, "c",1);
  sleep(1);
  tcflush(trackerfd, TCIFLUSH);
  close(trackerfd);
  if (pid)
    kill(pid, SIGKILL);
}

static int
Read(int fd, unsigned char *buf, int len)
{
  int nread;
  int rem = len;
  char *cp = (char *)buf;
  for(;;) {
    if ((nread = read(fd, cp, rem)) <= 0)
      return len - rem;
    if ((rem -= nread) == 0) return len;
    cp += nread;
  }
}


static void
readloop(void *unused)
{
  sigignore(SIGINT);
  for(;;) {
    if (DoFlush) {
      write(trackerfd, "c", 1);
      usleep(10000000/conf.baud);
      tcflush(trackerfd, TCIFLUSH);
      usleep(10000000/conf.baud);
      write(trackerfd, "C", 1);
    }
    Read(trackerfd, (unsigned char *)TrackerBuf, conf.len);
    DoFlush = 0;
    if (getppid() == 1) {
      trackerFinish();
      _exit(0);
    }
  }
}

void
trackerInit(char *configfile)
{
}

static int
littlendian(unsigned char *src)
{
  return (src[3]<<24)|(src[2]<<16)|(src[1]<<8)|src[0];
}

static void
checkchild()
{
  if (ReadPid)
    if (waitpid(ReadPid, 0, WNOHANG) == 0)
      return;
    else
      kill(ReadPid, 9);
  ReadPid = sproc(readloop, PR_SALL);
}

void
getTrackerBuf()
{
  if ((conf.cont != 'C') && (conf.button == '0'))
    write(trackerfd, "P", 1);
  Read(trackerfd, (unsigned char *)TrackerBuf, conf.len);
}

void
getTrackerInfo(struct perstation *psp, unsigned char c)
{
  unsigned char *cp;
retry:
  getTrackerBuf();
  cp = (unsigned char *)TrackerBuf + psp->begin;
  if (cp[0] != '0' || cp[1] != c) {
    if (conf.cont != 'C')
      tcflush(trackerfd, TCIFLUSH);
    else {
      DoFlush = 1;
      while(DoFlush)
	checkchild();
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
  for (station = 0; station < NSTATION; ++station, ++c) {
    if ((stations&(1<<station)) == 0) continue;
    psp  = &conf.perstation[station];
    if (psp->rec) {
      cp = (unsigned char *)TrackerBuf + psp->begin;
      if (cp[0] != '0' || cp[1] != c) {
	if (conf.cont != 'C')
	  tcflush(trackerfd, TCIFLUSH);
	else {
	  DoFlush = 1;
	  while(DoFlush)
	    checkchild();
	}
	getTrackerBuf();
      }
      if (psp->rec & (1<<Pos)) {
	cp = (unsigned char *)TrackerBuf + psp->posoff;
	for (i = 0; i < 3; ++i) {
	  *(int *)(vecXYZ+i) = littlendian(cp);
	  cp += sizeof (int);
	}
      } else
	memset((void *)vecXYZ, 0, 3*sizeof (float));
      vecXYZ += 3;
      if (psp->rec & (1<<Ang)) {
	cp = (unsigned char *)TrackerBuf + psp->angoff;
	for (i = 0; i < 3; ++i) {
	  *(int *)(vecAER+i) = littlendian(cp);
	  cp += sizeof (int);
	}
      } else
	memset((void *)vecAER, 0, 3*sizeof (float));
      vecAER += 3;
      if (psp->rec & (1<<But))
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

	
  if ((unsigned)(--station) >= 4) return;
  c = '1'+station;
  psp  = &conf.perstation[station];
  if ((psp->rec & ((1<<Pos)|(1<<Ang))) == 0) return;
  getTrackerInfo(psp, c);
  if (psp->rec & (1<<Pos)) {
    cp = (unsigned char *)TrackerBuf + psp->posoff;
    for (i = 0; i < 3; ++i) {
      *(int *)(vecXYZ+i) = littlendian(cp);
      cp += sizeof (int);
    }
  }
  if (psp->rec & (1<<Ang)) {
    cp = (unsigned char *)TrackerBuf + psp->angoff;
    for (i = 0; i < 3; ++i) {
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

  if ((unsigned)(--station) >= 4) return 0;
  c = '1'+station;
  psp = &conf.perstation[station];
  if (psp->rec & (1<<But) == 0) return 0;
  getTrackerInfo(psp, c);
  return TrackerBuf[psp->butoff] - '0';
}

#ifdef TEST
main(int argc, char **argv)
{
  float xyz[XYZ*4], aer[XYZ*4];
  //	int station = 1;
  int but;

  trackerInit(argv[1]);
  //	if (argc > 2) station = argv[2][0] - '0';

  for (;;) {
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


// end driver R.E.
// ************************************************************************************************

