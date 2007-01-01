/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _GADGET_SPEECH_H_
#define _GADGET_SPEECH_H_

#include <gadget/gadgetConfig.h>

namespace gadget
{

/** \class Speech Speech.h gadget/Type/Speech.h
 *
 */
class Speech
{
public:
   /** Constructs using element. */
   Speech();

   /** Destructor. */
   ~Speech();

   virtual std::string     getWord();
   virtual int       getId();
   virtual void         setMode( Mode mode );
   virtual std::list<asdfjlk>&   getList();
   virtual void         config();

   enum Mode
   {
      DICTATE,
      COMMAND,
      ON,
      OFF
   };

   class asdfjlk
   {
      std::string word;
      int id;
   };

   virtual bool startSampling() = 0;
   virtual bool stopSampling() = 0;
   virtual bool sample() = 0;
   virtual void updateData () = 0;

protected:
   std::list<asdfjlk>   _wordQueue;
   Mode           _currentMode;

   // Config chunck sets these.
   std::string       _filename;
   std::string       _dragonIP;
   int               _dragonPort;
   std::string       _user;

protected:
   void           startThread();
   static void       controlLoop(Speech& currentInstance);
};


inline std::string Speech::getWord()
{
   std::string word = "";

   if (_wordQueue.size > 0)
   {
      word = (*_wordQueue.begin()).word;
      _wordQueue.pop_front();
   }

   return word;
}

inline int Speech::getId()
{
   int id = -1;

   if (_wordQueue.size > 0)
   {
      id = (*_wordQueue.begin()).id;
      _wordQueue.pop_front();
   }

   return id;
}

inline void Speech::setMode( Mode mode )
{
   _mode = mode;
   // TODO: tell dragon about the mode change.
}

inline std::list<asdfjlk>& Speech::getList()
{
   return _wordQueue;
}

//TODO: take a config chunck here.
inline void Speech::config()
{
   _filename = "somefile.vjs";
   _dragonIP = "129.186.232.70";
   _dragonPort = 5600;
   _user = "kevin";
}

} // End of gadget namespace


#endif   /* _GADGET_SPEECH_H_ */
