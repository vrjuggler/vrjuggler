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


#ifndef _VJ_API_
#define _VJ_API_
//#pragma once

#include <vjConfig.h>

namespace vrj
{
   
//---------------------------------------------------------
// API: Encapulates interfaces to graphics APIs.
//
// PURPOSE:
//	 This class is used for the application to tell the
//  Kernel what API it is using.
//
// @author Allen Bierbaum
//  Date: 9-7-97
//--------------------------------------------------------
class API
{
public:
   enum
   {UNKNOWN = 0, PERFORMER = 1, OPENGL = 2};

   //: Constructor
   //!ARGS: api - enum of the API to set too
   API(int api = UNKNOWN)
   { mApi = api;}

   // ---  API QUERY  --- //
public:
   //: Sets API to Performer
   void setPerformer()
   { mApi = PERFORMER;}

   //: Sets API to OpenGL
   void setOpenGL()
   { mApi = OPENGL;}

public:
   //: Test if API is Performer
   //! RETURNS: True - API is Performer
   //! RETURNS: False - API is not Performer
   int Performer()
   { return (mApi == PERFORMER);}

   //: Test if API is OpenGL
   //! RETURNS: True - API is OpenGL
   //! RETURNS: False - API is not OpenGL
   int OpenGL()
   { return (mApi == OPENGL);}

private:
   int mApi;    //: The API we are using.

};

};

#endif
