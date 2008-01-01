/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vpr/vpr.h>

#include <drivers/noDNA/X-IST/X-ISTStandalone.h>

#ifndef GENERIC_READ
#  define GENERIC_READ 0
#endif
#ifndef GENERIC_WRITE
#  define GENERIC_WRITE 0
#endif
#ifndef OPEN_EXISTING
#  define OPEN_EXISTING 0
#endif

#if ! defined(VPR_OS_Windows)
#  define PURGE_TXCLEAR 0x0004
#  define PURGE_RXCLEAR 0x0008
#endif

///////////////////////////////////////////////////////////
// Public Methods
///////////////////////////////////////////////////////////

// Constructor
X_ISTStandalone::X_ISTStandalone()
   : mComm(NULL)
{
}

// Destructor
X_ISTStandalone::~X_ISTStandalone()
{
   closeGlove();
}

// Connect to the X-IST hardware
bool X_ISTStandalone::connectToHardware(const int gloveNumber)
{
   int num_devs;
   FT_STATUS ftstatus = FT_ListDevices(&num_devs, NULL, FT_LIST_NUMBER_ONLY); 

   if ( ftstatus != FT_OK )
   {
      return false;
   }

   if ( gloveNumber >= num_devs )
   {
      return false;
   }

   char str_serial[256];
   
   ftstatus = FT_ListDevices((char*) gloveNumber, str_serial,
                             FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);

   if ( ftstatus != FT_OK )
   {
      return false;
   }

   mComm = FT_W32_CreateFile(str_serial, GENERIC_READ|GENERIC_WRITE, 0, 0,
                             OPEN_EXISTING, FT_OPEN_BY_SERIAL_NUMBER, 0);

#if defined(VPR_OS_Windows)
   if ( mComm == (void*) INVALID_HANDLE_VALUE )
#else
   if ( mComm == (void*) NULL )
#endif
   {
      closeGlove();
      return false;
   }

   FTTIMEOUTS ctmoNew = {0};
   FTDCB dcbUSB;

   FT_W32_GetCommTimeouts(mComm, &mCtmoOld);
   ctmoNew.ReadIntervalTimeout = 50;
   ctmoNew.ReadTotalTimeoutConstant = 200;
   ctmoNew.ReadTotalTimeoutMultiplier = 0;
   ctmoNew.WriteTotalTimeoutMultiplier = 0;
   ctmoNew.WriteTotalTimeoutConstant = 0;
   FT_W32_SetCommTimeouts(mComm, &ctmoNew);
   dcbUSB.DCBlength = sizeof(dcbUSB);
   FT_W32_GetCommState(mComm, &dcbUSB);
   dcbUSB.BaudRate = 1000000;
   dcbUSB.fOutxCtsFlow = false;
   dcbUSB.fRtsControl = 0;
   dcbUSB.fAbortOnError = false;
   FT_W32_SetCommState(mComm, &dcbUSB);
   FT_W32_PurgeComm(mComm, PURGE_TXCLEAR | PURGE_RXCLEAR);

   char buffer[256];

// Get firmware version
   buffer[0] = 0x11;
   write(buffer, 1);
   memset(buffer, 0, sizeof(buffer));
   read(buffer, 40);
   FT_W32_PurgeComm(mComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
   mFirmwareVersion = WORD (((buffer[0] - 48) * 100) + ((buffer[2] - 48) * 10) + (buffer[4] - 48));
//   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
//      << "[X-IST] firmware version : " << mFirmwareVersion << std::endl
//      << vprDEBUG_FLUSH;

// Get hardware version
   buffer[0] = 0x12;
   write(buffer, 1);
   memset(buffer, 0, sizeof(buffer));
   read(buffer, 40);
   FT_W32_PurgeComm(mComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
//   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
//      << "[X-IST] hardware version : " << buffer << std::endl
//      << vprDEBUG_FLUSH;

// Get product id
   buffer[0] = 0x13;
   write(buffer, 1);
   memset(buffer, 0, sizeof(buffer));
   read(buffer, 40);
   FT_W32_PurgeComm(mComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
//   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
//      << "[X-IST] product id : " << buffer << std::endl << vprDEBUG_FLUSH;

   return true;
}

bool X_ISTStandalone::readRecordsFromHardware(WORD values[24])
{
   unsigned char buffer[256];

// Get all channels
   buffer[0] = 0x35;
   if ( ! write((char*) buffer, 1) )
   {
      return false;
   }
   if ( ! read((char*) buffer, (mFirmwareVersion < 27 ? 31 : 38)) )
   {
      return false;
   }

   FT_W32_PurgeComm(mComm, PURGE_TXCLEAR | PURGE_RXCLEAR);

   values[0] = WORD (buffer[0] + (short (buffer[1] / 16) * 256));
   values[1] = WORD (buffer[2] + (short (buffer[1] % 16) * 256));
   values[2] = WORD (buffer[3] + (short (buffer[4] / 16) * 256));
   values[3] = WORD (buffer[5] + (short (buffer[4] % 16) * 256));
   values[4] = WORD (buffer[6] + (short (buffer[7] / 16) * 256));
   values[5] = WORD (buffer[8] + (short (buffer[7] % 16) * 256));
   values[6] = WORD (buffer[9] + (short (buffer[10] / 16) * 256));
   values[7] = WORD (buffer[11] + (short (buffer[10] % 16) * 256));
   values[8] = WORD (buffer[12] + (short (buffer[13] / 16) * 256));
   values[9] = WORD (buffer[14] + (short (buffer[13] % 16) * 256));
   values[10] = WORD (buffer[15] + (short (buffer[16] / 16) * 256));
   values[11] = WORD (buffer[17] + (short (buffer[16] % 16) * 256));
   values[12] = WORD (buffer[18] + (short (buffer[19] / 16) * 256));
   values[13] = WORD (buffer[20] + (short (buffer[19] % 16) * 256));
   values[14] = WORD (buffer[21] + (short (buffer[22] / 16) * 256));
   values[15] = WORD (buffer[23] + (short (buffer[22] % 16) * 256));
   values[16] = WORD (buffer[24] + (short (buffer[25] / 16) * 256));
   values[17] = WORD (buffer[26] + (short (buffer[25] % 16) * 256));
   values[18] = WORD (buffer[27] + (short (buffer[28] / 16) * 256));
   values[19] = WORD (buffer[29] + (short (buffer[28] % 16) * 256));

   if ( mFirmwareVersion > 26 )
   {
      values[20] = WORD (buffer[30] + (short (buffer[31] / 16) * 256));
      values[21] = WORD (buffer[32] + (short (buffer[31] % 16) * 256));
      values[22] = WORD (buffer[33] + (short (buffer[34] / 16) * 256));
      values[23] = WORD (buffer[35] + (short (buffer[34] % 16) * 256));
   }

   return true;
}

void X_ISTStandalone::closeGlove()
{
   if ( mComm != NULL )
   {
      FT_W32_SetCommTimeouts(mComm, &mCtmoOld);
      FT_W32_CloseHandle(mComm);
   }

   mComm = NULL;
}

bool X_ISTStandalone::read(char* buffer, const unsigned long int size)
{
   DWORD dwBytesRead;
   FT_W32_ReadFile(mComm, buffer, size, &dwBytesRead, NULL);
   return (size == dwBytesRead);
}

bool X_ISTStandalone::write(char *buffer, const unsigned long int size)
{
   DWORD dwBytesWrite;
   FT_W32_WriteFile(mComm, buffer, size, &dwBytesWrite, NULL);
   return (size == dwBytesWrite);
}
