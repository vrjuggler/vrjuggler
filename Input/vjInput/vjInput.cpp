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


#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>


vjInput::vjInput()
{
   deviceAbilities = 0;
   instName = NULL;
   sPort = NULL;
   myThread = NULL;
   active = 0;
}

vjInput::~vjInput()
{
    if (sPort != NULL)
        delete [] sPort;
    if (instName != NULL)
        delete [] instName;

}

bool vjInput::config( vjConfigChunk *c)
{
  sPort = NULL;
  char* t = c->getProperty("port").cstring();
  if (t != NULL)
  {
    sPort = new char[ strlen(t) + 1 ];
    strcpy(sPort,t);
  }
  t = c->getProperty("name").cstring();
  if (t != NULL)
  {
    instName = new char[ strlen(t) + 1];
    strcpy(instName,t);
  }

  baudRate = c->getProperty("baud");

  return true;
}


void vjInput::setPort(const char* serialPort)
{
if (myThread != NULL) {
     cerr << "Cannot change the serial Port while active\n";
     return;
  }
  strncpy(sPort,serialPort,(size_t)30);
}

char* vjInput::getPort()
{
  if (sPort == NULL) return "No port";
  return sPort;
}

void vjInput::setBaudRate(int baud)
{
  if (myThread != NULL)
     baudRate = baud;
}

int vjInput::fDeviceSupport(int devAbility)
{
    return (deviceAbilities & devAbility);
}

//: Reset the Index Holders
// Sets to (0,1,2) in that order
void vjInput::resetIndexes()
{
    current = 0;
    valid = 1;
    progress = 2;
}

//: Swap the current and valid indexes (thread safe)
void vjInput::swapCurrentIndexes()
{
   // Removed the lock acquisition because there
   // is device specific code that must be within the lock as well
   //lock.acquire();
   vjASSERT(lock.test());       // Make sure that we have the lock when we are called
  int tmp = current;
  current = valid;
  valid = tmp;
  //lock.release();
}

//: Swap the valid and progress indexes (thread safe)
void vjInput::swapValidIndexes()
{
   lock.acquire();
	int tmp = valid;
	valid= progress;
	progress = tmp;
	lock.release();
}

