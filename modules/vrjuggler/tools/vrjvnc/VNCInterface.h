/*********** <VRJ VNC auto-copyright.pl BEGIN do not edit this line> **********
 *
 * VRJ VNC is (C) Copyright 2003-2011 by Iowa State University
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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 ************ <VRJ VNC auto-copyright.pl END do not edit this line> **********/

/* ------------------------------------------------------------------------
 * VNCInterface.hh
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

#ifndef _VRJVNC_VNC_INTERFACE_H_
#define _VRJVNC_VNC_INTERFACE_H_

#include <list>
#include <string>
#include <exception>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Sync/Mutex.h>

#include <Rectangle.h>


namespace vrjvnc
{

class VNCException : public std::exception
{
public:
   VNCException(const std::string& what) : std::exception(), mWhat(what)
   {
   }

   virtual ~VNCException() throw()
   {
   }

   virtual const char* what() const throw()
   {
      return mWhat.c_str();
   }

private:
   std::string mWhat;
};

class VNCProtoException : public VNCException
{
public:
   VNCProtoException(const std::string& what) : VNCException(what)
   {
   }

   virtual ~VNCProtoException() throw()
   {
   }
};

class NetReadException : public VNCException
{
public:
   NetReadException(const std::string& what) : VNCException(what)
   {
   }

   virtual ~NetReadException() throw()
   {
   }
};

class NetWriteException : public VNCException
{
public:
   NetWriteException(const std::string& what) : VNCException(what)
   {
   }

   virtual ~NetWriteException() throw()
   {
   }
};

class VNCEncodingException : public VNCProtoException
{
public:
   VNCEncodingException(const std::string& what) : VNCProtoException(what)
   {
   }

   virtual ~VNCEncodingException() throw()
   {
   }
};


/**
 * VNC implementation of a windowing system interface.
 */
class VNCInterface
{
public:

   /// Window system framebuffer pixel format.
   struct PixelFormat
   {
      vpr::Uint32 depth;        /**< color depth (in bits) */
      vpr::Uint32 size;         /**< pixel element size (in bits) */
      vpr::Uint32 red_bits;     /**< number of red bits */
      vpr::Uint32 red_mask;     /**< bitmask for red component */
      vpr::Uint32 red_shift;    /**< red component shift  */
      vpr::Uint32 green_bits;   /**< number of green bits */
      vpr::Uint32 green_mask;   /**< bitmask for green component */
      vpr::Uint32 green_shift;  /**< green component shift  */
      vpr::Uint32 blue_bits;    /**< number of blue bits */
      vpr::Uint32 blue_mask;    /**< bitmask for blue component */
      vpr::Uint32 blue_shift;   /**< blue component shift  */
   };

   /**
    * Constructor. This will connect to the specified VNC server and
    * setup the session.
    *
    * @param host the name of the host to connect to.
    * @param port the VNC server port (begins at 5900).
    * @param password the password to use for connecting.
    */
   VNCInterface(const std::string& host, const vpr::Uint16 port,
                const std::string& password);

   /**
    * Destructor.
    */
   ~VNCInterface();

   /**
    * Run the VNC interface (called by the Celsius thread).
    */
   void run();

   void stop()
   {
      mRunning = false;
   }

   /** Send request for the server to send updates.
   * This requests that the server tells us anytime there
   * is an update to the framebuffer.
   * @note: I am not sure about this....
   */
   void sendFramebufferUpdateRequest(int x, int y, int w, int h);

   /** Call to generate a pointer event on the desktop.
   * @param x,y - mouse position in vnc desktop coordinates.
   */
   void pointerEvent(int x, int y, int button_mask);
   void keyEvent(unsigned int key, bool pressed = true);

   /**
    * Retrieve the frame buffer pixel format.
    */
   const PixelFormat& getPixelFormat() const
   {  return mPf; }

   /**
    * Retrieve the frame buffer width.
    */
   int getWidth() const
   {  return mWidth; }

   /**
    * Retrieve the frame buffer height.
    */
   int getHeight() const
   {  return mHeight;   }

   /**
    * Retrieve the size, in bits, of each pixel.
    */
   int getPixelSize() const
   {  return mPf.size;  }

   /**
    * Retrieve the frame buffer pointer.
    */
   const char* getFramebuffer() const
   {  return mFramebuffer; }

   /**
    * Retrieve the desktop name.
    */
   const std::string& getName() const
   {  return mName; }

   /**
    * Retrieve a frame buffer update.
    *
    * @param r a rectangle that will be filled in if the call succeeds.
    * @return true if there is an update, false otherwise.
    */
   bool getFramebufferUpdate(Rectangle &r);

private:
   /// Adds a rectangle update to the update queue.
   void addUpdate(int x, int y, int w, int h);

   /** Copies a rectangle buffer to the framebuffer.
   * @param buf - Source buffer.
   * @param x,y - Destination origin.
   * @param w,h - width and height of source and destination rect
   */
   void copyRectToFramebuffer(char* buf, int x, int y, int w, int h);

   /** Should two rectangles be merged into one?
   * @return true - Rectangles should be merged.
   */
   bool isMergeable(const Rectangle& r1, const Rectangle& r2);

   /** Merges two rectangles.
   * Merges the rectangles into a single rectange that covers the
   * same space
   */
   Rectangle merge(const Rectangle& r1, const Rectangle& r2);

   /// Reads data from the VNC server.
   void readData(std::string& data, vpr::Uint32 len);

   /// Reads data from the VNC server.
   void readData(void* data, vpr::Uint32 len);

   /// Writes data to the VNC server.
   void writeData(const void* data, vpr::Uint32 len);

   /// Handles version exchange.
   void handleVNCVersion();

   /// Authenticates VNC session.
   bool handleVNCAuthentication(const std::string& pwd);

   /// Initializes VNC session.
   void handleVNCInitialization();

   /// Handles a VNC server message (called by main loop).
   void handleVNCServerMessage();

   /// Handles server cut text.
   void handleVNCServerCutText();

   /// Handles frame buffer update.
   void handleVNCFramebufferUpdate();

   /// Connects to a VNC server.
   void connectToVNCServer(const std::string& password);

   /// Sets pixel format.
   void setVNCPixelFormat();

   /// Sets encodings.
   void setVNCEncodings();

   // Private member variables
   std::string mName;

   std::list<Rectangle> mRectQueue;    /**< List of pending updates */

   std::string          mHost;
   vpr::Uint16          mPort;
   vpr::SocketStream    mSock;
   int mServerMajor, mServerMinor;
   bool                 mIncremental;

   PixelFormat mPf;
   PixelFormat mNativePf;
   int         mWidth;                /**< Width of frame buffer */
   int         mHeight;               /**< Height of frame buffer */

   /** The frame buffer of data.
   * This is an array of pixels of size mWidth x mHeight
   * with mPf.size() bits per pixel
   */
   char* mFramebuffer;

   // Read buffer (used in framebuffer updates)
   vpr::Uint32 mReadBufferSize;
   char*       mReadBuffer;

   vpr::Mutex mMutex;
   bool       mRunning;
};

}


#endif /* _VRJVNC_VNC_INTERFACE_H_ */
