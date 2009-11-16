/*********** <VRJ VNC auto-copyright.pl BEGIN do not edit this line> **********
 *
 * VRJ VNC is (C) Copyright 2003-2009 by Iowa State University
 *
 * Original Authors:
 *   Patrick Hartling, Allen Bierbaum
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this application; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 ************ <VRJ VNC auto-copyright.pl END do not edit this line> **********/

/* ------------------------------------------------------------------------
 * VNCInterface.cc
 *
 * This file is part of 3Dwm: The Three-Dimensional User Environment.
 *
 * 3Dwm: The Three-Dimensional User Environment:
 *	<http://www.3dwm.org>
 *
 * Chalmers Medialab
 * 	<http://www.medialab.chalmers.se>
 *
 * ------------------------------------------------------------------------
 * File created 2000-09-23 by Niklas Elmqvist.
 *
 * Copyright (c) 2000 Niklas Elmqvist <elm@3dwm.org>.
 * Copyright (c) 2000 Steve Houston <shouston@programmer.net>.
 * ------------------------------------------------------------------------
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 * ------------------------------------------------------------------------
 */

#if defined(WIN32) || defined(WIN64)
#  include <windows.h>
#endif

// -- Standard Includes
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <cmath>
#include <list>

// -- X11 Includes
#if defined(WIN32) || defined(WIN64)
#  include <WinCompat.h>
#else
#  include <X11/Xmd.h>
#endif

// -- VNC Includes
extern "C" {
#include <rfbproto.h>
#include <vncauth.h>
}

#include <vpr/Sync/Guard.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

#include <VNCDebug.h>
#include <VNCInterface.h>

// -- Defines

// @@@ Need to make this portable!
int endianTest = 1;

#if ! defined(Swap16IfLE)
#define Swap16IfLE(s) \
    (*(char *)&endianTest ? ((((s) & 0xff) << 8) | (((s) >> 8) & 0xff)) : (s))
#endif

#if ! defined(Swap32IfLE)
#define Swap32IfLE(l) \
    (*(char *)&endianTest ? ((((l) & 0xff000000) >> 24) | \
			     (((l) & 0x00ff0000) >> 8)  | \
			     (((l) & 0x0000ff00) << 8)  | \
			     (((l) & 0x000000ff) << 24))  : (l))
#endif

//#define DEBUG

namespace vrjvnc
{

static const unsigned int MAX_ENCODINGS = 10;

// -- Local Function Prototypes
static int countBits(int mask);

// -- Code Segment

VNCInterface::VNCInterface(const std::string& host, const vpr::Uint16 port,
                           const std::string& password)
   : mHost(host), mPort(port), mIncremental(false), mWidth(-1), mHeight(-1),
     mFramebuffer(NULL), mReadBuffer(NULL), mRunning(false)
{
   // Set up pixel format (in VNC, we get to define it ourselves)
   mPf.depth      = 24; mPf.size       = 32;
   mPf.red_bits   = 8;  mPf.red_mask   = 0xff;     mPf.red_shift   = 0;
   mPf.green_bits = 8;  mPf.green_mask = 0xff00;   mPf.green_shift = 8;
   mPf.blue_bits  = 8;  mPf.blue_mask  = 0xff0000; mPf.blue_shift  = 16;

   // Connect to the VNC server
   connectToVNCServer(password);

   // connectToVNCServer should fill in the width and height.
   vprASSERT(mWidth > 0 && "Invalid width");
   vprASSERT(mHeight > 0 && "Invalid height");

   // Create framebuffer
   const int framebuffer_size = mWidth * mHeight * (mPf.size / 8);

   vprDEBUG(vrjDBG_VNC, vprDBG_CONFIG_LVL)
      << "VNCInterface(): framebuffer_size == " << framebuffer_size
      << std::endl << vprDEBUG_FLUSH;

   mFramebuffer = new char[framebuffer_size];
   memset(mFramebuffer, 0, framebuffer_size);

   // Define the read buffer to be able to read then entire frame buffer at once
   mReadBufferSize = framebuffer_size;

   vprDEBUG(vrjDBG_VNC, vprDBG_CONFIG_LVL)
      << "VNCInterface(): mReadBufferSize (row size) == " << mReadBufferSize
      << std::endl << vprDEBUG_FLUSH;

   mReadBuffer = new char[mReadBufferSize];
   memset(mReadBuffer, 0, mReadBufferSize);

   // Inform the VNC server of pixel formats and encodings
   setVNCPixelFormat();
   setVNCEncodings();
}

VNCInterface::~VNCInterface()
{
   if ( NULL != mReadBuffer )
   {
      delete[] mReadBuffer;
   }

   if ( NULL != mFramebuffer )
   {
      delete[] mFramebuffer;
   }

   // Shutdown and close the VNC socket
   mSock.close();
}

void VNCInterface::run()
{
   mRunning = true;

   // Loop forever
   while ( mRunning )
   {
      try
      {
         // Handle a server message
         handleVNCServerMessage();
      }
      catch (std::exception& ex)
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "ERROR")
            << ": Caught an exception from handleVNCServerMessage: "
            << ex.what() << std::endl << vprDEBUG_FLUSH;
         mRunning = false;
         break;
      }
   }
}

void VNCInterface::sendFramebufferUpdateRequest(int x, int y, int w, int h)
{
    rfbFramebufferUpdateRequestMsg fur;
    // Initialize the message
    fur.type = rfbFramebufferUpdateRequest;
    fur.incremental = mIncremental ? 1 : 0;
    fur.x = Swap16IfLE(x);
    fur.y = Swap16IfLE(y);
    fur.w = Swap16IfLE(w);
    fur.h = Swap16IfLE(h);

    // Send the message
    writeData(&fur, sz_rfbFramebufferUpdateRequestMsg);

    // This is no longer the first time we update, we want incremental
    // framebuffer updates
    // I think this means that the first time we are asking for a full update
    // and from then on we only want incremental (non-full) updates.
    mIncremental = true;
}

void VNCInterface::pointerEvent(int x, int y, int button_mask)
{
    rfbPointerEventMsg pe;

    /*
    vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
       << "VNCInterface::pointerEvent(): Pointer event at ("
       << x << ", " << y << ")\n" << vprDEBUG_FLUSH;
       */

    // Initialize the message
    pe.type = rfbPointerEvent;
    pe.buttonMask = button_mask;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    pe.x = Swap16IfLE(x);
    pe.y = Swap16IfLE(y);

    // Send the message
    writeData(&pe, sz_rfbPointerEventMsg);
}

void VNCInterface::keyEvent(unsigned int key, bool pressed)
{
    rfbKeyEventMsg ke;

    // Initialize the message
    ke.type = rfbKeyEvent;
    ke.down = pressed ? 1 : 0;
    ke.key = Swap32IfLE(key);

    // Send the message
    writeData(&ke, sz_rfbKeyEventMsg);
}

bool VNCInterface::getFramebufferUpdate(Rectangle &r)
{
   vpr::Guard<vpr::Mutex> guard(mMutex);

   // Maybe there is no update?
   if ( mRectQueue.empty() == true )
   {
      return false;
   }

   // Yes, there is, update argument and then remove it
   r = mRectQueue.front();
   mRectQueue.pop_front();
   return true;
}

// -- VNC utility methods (protected) -------------------------------------

void VNCInterface::addUpdate(int x, int y, int w, int h)
{
   vpr::Guard<vpr::Mutex> guard(mMutex);

   // Construct a new rectangle
   Rectangle r;
   r.x = x; r.y = y; r.width = w; r.height = h;

   /*
   // Let's move through the rectangle queue looking for potential
   // merges to eliminate some updates
   std::list<Rectangle>::iterator i = mRectQueue.begin();
   while (i != mRectQueue.end())
   {
      // Get a temporary iterator and move the main iterator forward
      std::list<Rectangle>::iterator temp = i++;

      // Can we merge these two rectangles?
      if (isMergeable(r, *temp) == true)
      {
         // Then merge and remove the original rectangle
         r = merge(r, *temp);
         mRectQueue.erase(temp);
      }
   }
   */

   // Finally, add the new rectangle to the end of the queue
   mRectQueue.push_back(r);
}

bool VNCInterface::isMergeable(const Rectangle &r1, const Rectangle &r2)
{
   // @@@ NOTE: The implementation of this function is quite
   // arbitrary, we just need a heuristic that yields good
   // performance when merging rectangles.
   // XXX: May be interesting to add a slack buffer on these tests to take into account rectangles
   //      that are very close to each other from things like mouse movements

   int x1 = r1.x + r1.width;  int x2 = r2.x + r2.width;
   int y1 = r1.y + r1.height; int y2 = r2.y + r2.height;

   // Do the rectangles coincide horizontally?
   // - If r1 x within range of r2 OR r2.x within range of r1
   if ( (r1.x >= r2.x && r1.x <= x2) || (r1.x <= r2.x && x1 >= r2.x) )
   {
      // Yes? Then what about vertically?
      // - If r1.y within range of r2 OR r2.y withing range of r1
      if ((r1.y >= r2.y && r1.y <= y2) || (r1.y <= r2.y && y1 >= r2.y))
      {
         return true;
      }
   }

   // Not mergeable
   return false;
}

Rectangle VNCInterface::merge(const Rectangle &r1, const Rectangle &r2)
{
   Rectangle r;

   // Find the resulting rectangle that encloses both rectangles
#if defined(WIN32) || defined(WIN64)
   r.x = min(r1.x, r2.x);
   r.y = min(r1.y, r2.y);
#else
   r.x = std::min(r1.x, r2.x);
   r.y = std::min(r1.y, r2.y);
#endif
   int x1 = r1.x + r1.width;  int x2 = r2.x + r2.width;
   int y1 = r1.y + r1.height; int y2 = r2.y + r2.height;
   r.width = (x1 > x2) ? r1.width : r2.width;
   r.height = (y1 > y2) ? r1.height : r2.height;

   return r;
}

/** Copy the rectange buffer at buf into the frame buffer.
* @param buf - The buffer to read from
* @param x,y - The origin of the target rect in the framebuffer
* @param w,h - Width and height of the source and target rectangle buffers
*/
void VNCInterface::copyRectToFramebuffer(char *buf, int x, int y, int w, int h)
{
   int bytes_per_pixel = (mPf.size / 8);

   // Find start of the target area in the frame buffer
   char* fbptr = mFramebuffer + (((y * mWidth) + x) * bytes_per_pixel);

   // Copy the contents of the rectangle buffer
   // - Copy one horizontal line at a time
   for(int cur_line=0; cur_line != h; ++cur_line)
   {
      // Copy memory
      memcpy(fbptr, buf, w * bytes_per_pixel);

      // Update buffer positions
      fbptr += mWidth * bytes_per_pixel;     // Add size of fb line to get to next line in rect
      buf   += w * bytes_per_pixel;          // Get to next start line in src
   }
}

void VNCInterface::readData(std::string& data, vpr::Uint32 len)
{
   vprDEBUG_OutputGuard(vrjDBG_VNC, vprDBG_STATE_LVL,
                        "readData(std::string, vpr::Uint32) entered\n",
                        "readData(std::string, vpr::Uint32) done.\n");

   vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
      << "Reading " << len << " bytes from socket\n" << vprDEBUG_FLUSH;

   try
   {
      vpr::Uint32 bytes_read;
      mSock.recvn(data, len, bytes_read);

      vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
         << "Got " << bytes_read << " bytes from socket\n" << vprDEBUG_FLUSH;

      if ( len != bytes_read )
      {
         std::stringstream msg_stream;
         msg_stream << "Failed to read " << len << " bytes";
         throw NetReadException(msg_stream.str());
      }
   }
   catch (vpr::IOException& ex)
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to read " << len << " bytes: " << ex.what();
      // XXX: setCause(ex) ?
      throw NetReadException(msg_stream.str());
   }
}

void VNCInterface::readData(void* data, vpr::Uint32 len)
{
   vprDEBUG_OutputGuard(vrjDBG_VNC, vprDBG_STATE_LVL,
                        "readData(void*, vpr::Uint32) entered\n",
                        "readData(void*, vpr::Uint32) done.\n");

   vpr::Uint32 bytes_read;

   vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
      << "Reading " << len << " bytes from socket\n" << vprDEBUG_FLUSH;

   try
   {
      mSock.recvn(data, len, bytes_read);
      vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
         << "Got " << bytes_read << " bytes from socket\n" << vprDEBUG_FLUSH;

      if ( len != bytes_read )
      {
         std::ostringstream msg_stream;
         msg_stream << "Failed to read " << len << " bytes";
         throw NetReadException(msg_stream.str());
      }
   }
   catch (vpr::IOException& ex)
   {
      throw NetReadException(ex.what());
   }
}

void VNCInterface::writeData(const void* data, vpr::Uint32 len)
{
   try
   {
      vpr::Uint32 bytes_written;
      mSock.write(data, len, bytes_written);

      if ( len != bytes_written )
      {
         std::stringstream msg_stream;
         msg_stream << "Failed to write " << len << " bytes";
         throw NetWriteException(msg_stream.str());
      }
   }
   catch (vpr::IOException& ex)
   {
      std::cerr << ex.what() << std::endl;
      std::stringstream msg_stream;
      msg_stream << "Failed to write " << len << " bytes";
      // XXX: setCause(ex) ?
      throw NetWriteException(msg_stream.str());
   }
}

void VNCInterface::handleVNCVersion()
{
   rfbProtocolVersionMsg pv;
   int msglen = sz_rfbProtocolVersionMsg;

   // Read the message and zero-terminate the result
   readData(pv, msglen);
   pv[msglen] = '\0';

   // Extract server version from protocol version message
   if ( sscanf(pv, rfbProtocolVersionFormat, &mServerMajor, &mServerMinor)!=2 )
   {
      throw VNCProtoException("Not a valid VNC server!");
   }

   // Form and write the client protocol version message
   sprintf(pv, rfbProtocolVersionFormat,
           rfbProtocolMajorVersion, rfbProtocolMinorVersion);
   writeData(pv, msglen);

   vprDEBUG(vrjDBG_VNC, vprDBG_CRITICAL_LVL)
      << "Server uses protocol version " << mServerMajor << "."
      << mServerMinor << std::endl << vprDEBUG_FLUSH;
}

bool VNCInterface::handleVNCAuthentication(const std::string& pwd)
{
   static const vpr::Uint32 MAX_PASSWORD_SIZE = 64;
   vpr::Uint32 scheme, result, reasonLen;
   vpr::Uint8 challenge[CHALLENGESIZE];
   char password[MAX_PASSWORD_SIZE];
   char* reason;

   // Copy the password into mutable array
   strncpy(password, pwd.c_str(), MAX_PASSWORD_SIZE);

   // Which authentication scheme to use?
   readData(&scheme, 4);
   scheme = Swap32IfLE(scheme);

#ifdef DEBUG
    std::cerr << "Authentication scheme " << scheme << std::endl;
#endif

   // What do we do with it?
   switch (scheme)
   {
      case rfbConnFailed:
         // Why did it fail?
         readData((void*) &reasonLen, 4);
         reason = (char*) malloc(reasonLen);
         readData(reason, reasonLen);
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "Connection failed: " << reason << std::endl << vprDEBUG_FLUSH;
         free(reason);
         return false;

      case rfbNoAuth:
         // No authentication needed
         break;

      case rfbVncAuth:
         // VNC password authentication
         readData(challenge, CHALLENGESIZE);

         // Take care of the password
         if ( password == NULL || strlen(password) == 0 )
         {
            return false;
         }

         // Truncate passwords that are too long.
         if ( strlen(password) > 8 )
         {
            password[8] = '\0';
         }

         // Encrypt it! (using the vncauth lib)
         vncEncryptBytes(challenge, password);

         // @@@ Lose it from memory like in vncviewer?

         // Send it back to the server and get the response
         writeData(challenge, CHALLENGESIZE);
         readData(&result, 4);
         result = Swap32IfLE(result);

         // How'd it go?
         switch (result)
         {
            case rfbVncAuthOK:
               // Everything is fine, return with success
               return true;

            case rfbVncAuthFailed:
            case rfbVncAuthTooMany:
            default:
               // Failed to authenticate, return with failure
               return false;
         }

         break;

       default:
         return false;
   }

   // We should never reach this position...
   return true;
}

void VNCInterface::handleVNCInitialization()
{
   rfbClientInitMsg ci;
   rfbServerInitMsg si;

   // Send the client initialization message
   ci.shared = 1;
   writeData(&ci, sz_rfbClientInitMsg);

   // Read the server initialization message
   readData(&si, sz_rfbServerInitMsg);

   // Extract the information we need
   mWidth        = Swap16IfLE(si.framebufferWidth);
   mHeight       = Swap16IfLE(si.framebufferHeight);

   int red_max   = Swap16IfLE(si.format.redMax);
   int green_max = Swap16IfLE(si.format.greenMax);
   int blue_max  = Swap16IfLE(si.format.blueMax);

   si.nameLength = Swap32IfLE(si.nameLength);

   // Set up pixel format
   mNativePf.depth       = si.format.depth;
   mNativePf.size        = si.format.bitsPerPixel;

   mNativePf.red_shift   = si.format.redShift;
   mNativePf.red_mask    = red_max << mNativePf.red_shift;
   mNativePf.red_bits    = countBits(red_max);

   mNativePf.green_shift = si.format.greenShift;
   mNativePf.green_mask  = green_max << mNativePf.green_shift;
   mNativePf.green_bits  = countBits(green_max);

   mNativePf.blue_shift  = si.format.blueShift;
   mNativePf.blue_mask   = blue_max << mNativePf.blue_shift;
   mNativePf.blue_bits   = countBits(blue_max);

   vprDEBUG(vrjDBG_VNC, vprDBG_CRITICAL_LVL)
      << "Desktop size: " << mWidth << "x" << mHeight << "x"
      << mNativePf.depth << "." << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_VNC, vprDBG_CRITICAL_LVL)
      << "Pixel format: red (" << mNativePf.red_bits << ", "
      << mNativePf.red_shift << "), green (" << mNativePf.green_bits
      << ", " << mNativePf.green_shift  << "), blue ("
      << mNativePf.blue_bits << ", " << mNativePf.blue_shift << ")."
      << std::endl << vprDEBUG_FLUSH;

    // Read the name of the desktop
    readData(mName, si.nameLength);

    vprDEBUG(vrjDBG_VNC, vprDBG_CRITICAL_LVL) << "Desktop name: " << mName
                                              << std::endl << vprDEBUG_FLUSH;
}

void VNCInterface::connectToVNCServer(const std::string& password)
{
   vpr::InetAddr server_addr;
   server_addr.setAddress(mHost, mPort);
   mSock.setRemoteAddr(server_addr);

   try
   {
      mSock.open();
      mSock.connect();

      // Read and write version information
      handleVNCVersion();

      // Authenticate client
      if ( handleVNCAuthentication(password) == false )
      {
         throw VNCProtoException("Authentication failed.");
      }

      // Initialize the connection
      handleVNCInitialization();
   }
   catch (std::exception& ex)
   {
      std::cerr << ex.what() << std::endl;
      throw VNCProtoException("Failed to connect.");
   }
}

void VNCInterface::setVNCPixelFormat()
{
    rfbSetPixelFormatMsg spf;

    // Set up the message
    spf.type                = rfbSetPixelFormat;

    // Pixel format: 24 depth, 32 bpp, true color, RGB 8:8:8
    // @@@ Need to add support for bigendian architectures!
    spf.format.bitsPerPixel = mPf.size;
    spf.format.depth        = mPf.depth;
    spf.format.bigEndian    = false;
    spf.format.trueColour   = true;

    spf.format.redMax     = Swap16IfLE((1 << mPf.red_bits) - 1);
    spf.format.redShift   = mPf.red_shift;

    spf.format.greenMax   = Swap16IfLE((1 << mPf.green_bits) - 1);
    spf.format.greenShift = mPf.green_shift;

    spf.format.blueMax    = Swap16IfLE((1 << mPf.blue_bits) - 1);
    spf.format.blueShift  = mPf.blue_shift;

    // Send it to the server
    writeData(&spf, sz_rfbSetPixelFormatMsg);
}

void VNCInterface::setVNCEncodings()
{
    char buf[sz_rfbSetEncodingsMsg + MAX_ENCODINGS * 4];
    rfbSetEncodingsMsg *se = (rfbSetEncodingsMsg *) buf;
    CARD32 *encs = (CARD32 *) (&buf[sz_rfbSetEncodingsMsg]);
    int len = 0;

    // Initialize message
    se->type = rfbSetEncodings;
    se->nEncodings = 0;

    // @@@ We just want raw encoding for now!
    encs[se->nEncodings++] = Swap32IfLE(rfbEncodingRaw);

    /*
    if (mSock.isSameHost() == true) {
	fprintf(stderr,"Same machine: preferring raw encoding\n");
	encs[se->nEncodings++] = Swap32IfLE(rfbEncodingRaw);
    }

    // Set up the encodings we support
    encs[se->nEncodings++] = Swap32IfLE(rfbEncodingCopyRect);
    encs[se->nEncodings++] = Swap32IfLE(rfbEncodingHextile);
    encs[se->nEncodings++] = Swap32IfLE(rfbEncodingCoRRE);
    encs[se->nEncodings++] = Swap32IfLE(rfbEncodingRRE);
    */

    len = sz_rfbSetEncodingsMsg + se->nEncodings * 4;
    se->nEncodings = Swap16IfLE(se->nEncodings);

    // Send the message to the server
    writeData(buf, len);
}

void VNCInterface::handleVNCFramebufferUpdate()
{
   vpr::DebugOutputGuard guard(vrjDBG_VNC, vprDBG_STATE_LVL,
                               "VNCInterface::handleVNCFramebufferUpdate()\n",
                               "VNCInterface::handleVNCFramebufferUpdate() done\n");

   rfbFramebufferUpdateMsg fu;

   // Read the message header (bar the initial type byte)
   readData((char *) &fu + 1, sz_rfbFramebufferUpdateMsg - 1);
   int num_rectangles = Swap16IfLE(fu.nRects);

   vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
      << "fu.nRects == " << fu.nRects << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
      << "num_rectangles == " << num_rectangles << std::endl << vprDEBUG_FLUSH;

   // Read the rectangles and update the framebuffer
   for ( int i = 0; i < num_rectangles; ++i )
   {
      rfbFramebufferUpdateRectHeader rect_u;      // The rectangle header info

      // Read a rectangle header from the server
      readData(&rect_u, sz_rfbFramebufferUpdateRectHeader);

      // Convert results to something we can use
      rect_u.r.x = Swap16IfLE(rect_u.r.x);
      rect_u.r.y = Swap16IfLE(rect_u.r.y);
      rect_u.r.w = Swap16IfLE(rect_u.r.w);
      rect_u.r.h = Swap16IfLE(rect_u.r.h);

      rect_u.encoding = Swap32IfLE(rect_u.encoding);

      vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
         << "Read rectangle information: (" << rect_u.r.x << ", " << rect_u.r.y
         << ") " << rect_u.r.w << "x" << rect_u.r.h << std::endl
         << vprDEBUG_FLUSH;

      // Sanity checking
      if (rect_u.r.x + rect_u.r.w > mWidth || rect_u.r.y + rect_u.r.h > mHeight)
      {
         throw VNCException("Rectangle too large.");
      }

      if ((rect_u.r.h * rect_u.r.w) == 0)
      {
         continue;
      }

      // Now, do different things depending on the encoding
      switch (rect_u.encoding)
      {
      case rfbEncodingRaw:
         {
            // How many lines can we copy?
            size_t bytes_in_update = rect_u.r.w * rect_u.r.h * (mPf.size /8);
            vprASSERT(bytes_in_update <= mReadBufferSize);     // Assert read buffer is large enough

            // Read the entire rectangle from the server
            vprDEBUG(vrjDBG_VNC, vprDBG_HVERB_LVL)
               << "Reading " << bytes_in_update
               << " bytes into the read buffer\n" << vprDEBUG_FLUSH;

            // Read data from the server
            readData(mReadBuffer, bytes_in_update);

            // Copy the rectangle buffer into the framebuffer
            copyRectToFramebuffer(mReadBuffer, rect_u.r.x, rect_u.r.y,
                                  rect_u.r.w, rect_u.r.h);
         }
         break;

      default:
         throw VNCEncodingException("Unknown rectangle encoding.");
      }

      // Add the rectangle to the update rectangle queue
      addUpdate(rect_u.r.x, rect_u.r.y, rect_u.r.w, rect_u.r.h);
   }

   // Finally, we send a framebuffer update request (incremental)
   // so that the server knows to keep sending them.
   sendFramebufferUpdateRequest(0, 0, mWidth, mHeight);
}

void VNCInterface::handleVNCServerCutText()
{
    rfbServerCutTextMsg sct;

    // Read the message (bar the initial type byte)
    readData((char *) &sct + 1, sz_rfbServerCutTextMsg - 1);
    sct.length = Swap32IfLE(sct.length);

    // Retrieve the actual text
    char *text = (char *) malloc(sct.length + 1);
    readData(text, sct.length);
    text[sct.length] = '\0';
    free(text);
}

void VNCInterface::handleVNCServerMessage()
{
   vpr::Uint8 msg;

   // Retrieve the message type
   readData(&msg, 1);

   // What do we do with it?
   switch (msg)
   {
      case rfbFramebufferUpdate:
         vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
            << "handleVNCServerMessage(): Handling framebuffer update\n"
            << vprDEBUG_FLUSH;
         handleVNCFramebufferUpdate();
         break;
      case rfbBell:
         vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
            << "handleVNCServerMessage(): Dropping rfbBell\n"
            << vprDEBUG_FLUSH;
         break;
      case rfbServerCutText:
         vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
            << "handleVNCServerMessage(): Handling server cut text\n"
            << vprDEBUG_FLUSH;
         handleVNCServerCutText();
         break;
      default:
         vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
            << "handleVNCServerMessage(): Unexpected message type\n"
            << vprDEBUG_FLUSH;
         throw VNCProtoException("Unknown message type from server.");
   }
}

int countBits(int mask)
{
    int bits = 0;
    for (int i = 1; mask & i; i <<= 1, bits++);
    return bits;
}

} // End of vrjvnc namespace
