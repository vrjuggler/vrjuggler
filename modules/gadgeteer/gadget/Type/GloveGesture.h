/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _GADGET_GLOVE_GESTURE_H_
#define _GADGET_GLOVE_GESTURE_H_
//#pragma once

#include <gadget/gadgetConfig.h>
#include <vector>
#include <gadget/Type/Input.h>
#include <gadget/Type/Gesture.h>
#include <gadget/Type/Glove.h>
#include <gadget/Type/GloveProxy.h>
#include <jccl/Config/ConfigElementPtr.h>

namespace gadget
{

/** \class GloveGesture GloveGesture.h gadget/Type/GloveGesture.h
 *
 * Abstract base class for all glove gesture recognition.
 */
class GloveGesture : public Gesture
{
public:
   GloveGesture()
   {
      mGloveProxy = GloveProxyPtr();
   }

   virtual bool config(jccl::ConfigElementPtr e)
   {
      return Gesture::config(e);
   }

public:
   /**
    * Gets a gesture name.
    * @return Name of gesture with the given id (gestureId).
    * @note if gestureId = -1, returns name of current gesture.
    */
   virtual std::string getGestureString(const int gestureId) const;

   /**
    * Creates a new gesture.
    * @param gestureName  Name of the new gesture.
    * @return int id of the new gesture.
    */
   virtual int createGesture(std::string gestureName);

   /**
    * Loads the header of a glove data file.
    * This is both for the samples and for the trained files.
    *
    * The header format is:
    *
    *   -Comments -- # starting<br>
    *   -num gestures<br>
    *   -Gesture names<br>
    *   -Gesture samples<br>
    *
    * \p infile is a file that is already open and ready for reading.
    *
    * @post After running, this routines will leave the file pointer
    *       immedately after the header.  mGestureNames & mGestureExamples
    *       will be filled with correct data.
    */
   virtual void loadFileHeader(std::ifstream& infile);

   /**
    * Saves a file header.
    * @note The client of this routine may add their own initial lines to the
    *       header as long as they remove them before calling loadFileHeader.
    */
   virtual void saveFileHeader(std::ofstream& outFile);

   /**
    * Returns the identifier of the string gesture.
    * @return -1 if not found.
    */
   virtual int getGestureIndex(const std::string& gestureName) const;

   /** Return the number of gestures in system. */
   virtual int getNumGestures() const
   {
      return mGestureNames.size();
   }

protected:
   // mGestureNames and mGestureExamples MUST always be same size
   std::vector<std::string>     mGestureNames;    /**<  List of all gesture names */
   std::vector<GloveData>       mGestureExamples;   /**< Examples of all gestures */
   GloveProxyPtr                mGloveProxy;        /**< Proxy to the glove */
};

} // End of gadget namespace

#endif
