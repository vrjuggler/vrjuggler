
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
#ifndef _GADGET_INPUT_MIXER_H_
#define _GADGET_INPUT_MIXER_H_

#include <gadget/gadgetConfig.h>
#include <vpr/vpr.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>

namespace gadget
{

template <class ParentType>
class InputPlaceHolder	: public ParentType
{
public:
   virtual int sample()
	{return(0);}

   virtual int startSampling()
	{return(0);}

   virtual int stopSampling()
	{return(0);}

   virtual void updateData()
	{;}
};		


/**
* Class for mixin in base classes of input devices.
* All devices should derive from a mixed type like below based on their types.
*
* This allows us to implement any custom functions that are needed
* such as writeObject and readObject.
*
* InputMixer<InputMixer<InputMixer<Input,Digital>,Analog>,Position> 
*/
template <class ComposedParent, class NewParent>
class InputMixer : public ComposedParent, public NewParent
{
public:
   typedef InputPlaceHolder< InputMixer<ComposedParent, NewParent> > MixedPlaceholderType;
   
   vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer)
   {
     ComposedParent::writeObject(writer);
     NewParent::writeObject(writer);
     return(vpr::ReturnStatus::Succeed);
   }
   
   vpr::ReturnStatus readObject(vpr::ObjectReader* reader)
   {
     ComposedParent::readObject(reader);
     NewParent::readObject(reader);
     return(vpr::ReturnStatus::Succeed);
   }
   
   std::string getBaseType()
   {
     return(ComposedParent::getBaseType() + NewParent::getBaseType());	  //Input,Digital,Analog,Position, NEED THIS TOO
   }
   
   void setDelta(vpr::Uint64 delta)
   {
      ComposedParent::setDelta(delta);
      NewParent::setDelta(delta);
   }


};

} // end namespace

#endif   /* _GADGET_INPUT_MIXER_H_ */

