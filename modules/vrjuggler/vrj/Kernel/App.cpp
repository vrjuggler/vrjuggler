/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h>

#include <stdlib.h>
#include <boost/concept_check.hpp>

#include <vpr/vpr.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Sound/SoundManagerFactory.h>
#include <vrj/Kernel/App.h>


namespace vrj
{

App::App(Kernel* kern)
   : mKernel(kern)
   , mHaveFocus(true)
{
   if(NULL == kern)
   {
      mKernel = Kernel::instance();
   }
}

bool App::configCanHandle(jccl::ConfigElementPtr element)
{
   boost::ignore_unused_variable_warning(element);
   return false;
}

bool App::configAdd(jccl::ConfigElementPtr element)
{
   boost::ignore_unused_variable_warning(element);
   vprASSERT(false);
   return false;
}

bool App::configRemove(jccl::ConfigElementPtr element)
{
   boost::ignore_unused_variable_warning(element);
   vprASSERT(false);
   return false;
}

SoundManager* App::getSoundManager()
{
   return &SoundManagerFactory::get();
}
}
