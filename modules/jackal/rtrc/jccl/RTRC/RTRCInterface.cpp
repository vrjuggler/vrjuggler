/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <jccl/jcclConfig.h>

#include <stdlib.h>
#include <vpr/vpr.h>
#include <vpr/Util/Debug.h>

#include <jccl/RTRC/RTRCInterfaceSubjectImpl.h>
#include <jccl/RTRC/RTRCInterface.h>

namespace jccl
{

RTRCInterface::RTRCInterface()
   : mCorbaManager(NULL), mInterface(NULL), mInterfaceName("RTRCInterface")
{}

RTRCInterface::~RTRCInterface()
{
   //Clean up interface by disconnecting first
   if (mInterface != NULL)
   {
      disable(); 
      delete mInterface;
      mInterface = NULL;
   }

   //Clean up the corba manager and delete it
   if (mCorbaManager != NULL)
   {
      //Corba manager has its own destructor cleanup
      delete mCorbaManager;
      mCorbaManager = NULL;
   }
}

void RTRCInterface::init()
{
   //Create new corba manager and initialize it
   //Create new RTRCInterfaceSubjectImpl object

   mCorbaManager = new tweek::CorbaManager;

   //TODO: How do we get a unique instance name?
   mInterfaceName = "RTRCInterface";

   vpr::ReturnStatus status;

   try
   {
      int dummy_int = 0 ;

      //Attempt to initialize the corba manager
      status = mCorbaManager->init("corba_test", dummy_int, NULL);

   }
   catch (...)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::init: Caught an unknown exception while initializing CorbaManager\n" 
         << vprDEBUG_FLUSH ;

      delete mCorbaManager; 
      mCorbaManager = NULL;
      return;
   }

   //Test to see if init succeeded
   if ( !status.success() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::init: Could not initialize CorbaManager\n" 
         << vprDEBUG_FLUSH ;

      delete mCorbaManager; 
      mCorbaManager = NULL;
      return;
   }

   try
   {
      //Attempt to create the subject manager
      status = mCorbaManager->createSubjectManager();
   }
   catch (CORBA::Exception& ex)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::init: Caught an unknown CORBA exception while creating the subject manager\n" 
         << vprDEBUG_FLUSH ;
   }
      
   if ( !status.success() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::init: CORBA subject manager failed to initialize\n" 
         << vprDEBUG_FLUSH ;

      delete mCorbaManager; 
      mCorbaManager = NULL;
      return;
   }

   //Create an intstance of our interface subject
   mInterface = new RTRCInterfaceSubjectImpl();

}

void RTRCInterface::enable()
{
   //Make sure the corba manager has been initialized
   if ((mInterface == NULL) || (mCorbaManager == NULL))
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::enable: Cannot enable interface until it has been initialized\n" 
         << vprDEBUG_FLUSH ;

      return;
   }

   //Register the subject with the corba manager
   try
   {
      //Attempt to register the subject
      mCorbaManager->getSubjectManager()->registerSubject(mInterface, mInterfaceName.c_str());
   }
   catch (...)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::enable: Caught an exception while trying to register subject\n" 
         << vprDEBUG_FLUSH ;
   }
}

void RTRCInterface::disable()
{
   //Make sure the corba manager has been initialized
   if ((mInterface == NULL) || (mCorbaManager == NULL))
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::disable: Cannot disable interface until it has been initialized\n" 
         << vprDEBUG_FLUSH ;

      return;
   }

   //Unregister the subject (note that observers must handle this disconnection)
   vpr::ReturnStatus status;

   try
   {
      //Attempt to register the subject
      status = mCorbaManager->getSubjectManager()->unregisterSubject(mInterfaceName.c_str());
   }
   catch (...)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::disable: Caught an exception while trying to unregister subject\n" 
         << vprDEBUG_FLUSH ;
   }

   if (!status.success())
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::disable: Could not unregister subject\n" 
         << vprDEBUG_FLUSH ;
   }

}

}
