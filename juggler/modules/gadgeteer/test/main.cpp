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

#include <iostream>

#include <jccl/Config/ChunkDescDB.h>
#include <jccl/Config/ChunkFactory.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <gadget/InputManager.h>
#include <gadget/Type/PositionInterface.h>
#include <vrj/Math/Coord.h>
#include <vpr/System.h>


#define CHUNK_DESC_LOCATION "/home/users/allenb/Source/juggler/Data/chunksDesc"
#define CONFIG_LOCATION "/home/users/allenb/.vjconfig/activeConfig"

int main()
{
   // --- Load chunk database -- //
   jccl::ChunkDescDB desc;
   bool load_worked = desc.load(CHUNK_DESC_LOCATION);
   jccl::ChunkFactory::instance()->addDescs(&desc);

   if(!load_worked)
      std::cerr << "Could not load chunkDesc's\n" << std::flush;

   std::cout << desc << "\n----------------------------------------"
             << std::endl;

   // -- Load config -- //
   jccl::ConfigChunkDB *chunkdb = new jccl::ConfigChunkDB();
   load_worked = chunkdb->load(CONFIG_LOCATION);
   if(!load_worked)
      std::cerr << "Could not load config file\n" << std::flush;

   std::cout << "Printing Chunk DB:" << std::endl;
   std::cout << (*chunkdb);
   std::cout << "endochunks" << std::endl;

   gadget::InputManager *input_manager = new gadget::InputManager;
   std::cout << "vjInputManager created" << std::endl;

   // --- configure the input manager -- //
   for ( jccl::ConfigChunkDB::iterator i = chunkdb->begin();
         i != chunkdb->end();
         i++ )
   {
      input_manager->configAdd(*i);
   }

   std::cout << "new devices have been added..\n\n";

   std::cout << input_manager << std::endl;

   std::cout << "Sleeping..." << std::flush;
   vpr::System::sleep(2);
   std::cout << "awake." << std::endl << std::flush;

      // get the indices for the devices
   gadget::PositionProxy* head_proxy =
      dynamic_cast<gadget::PositionProxy*>(input_manager->getProxy("VJHead"));
   gadget::PositionProxy* wand_proxy =
      dynamic_cast<gadget::PositionProxy*>(input_manager->getProxy("VJWand"));

   for (int l = 0; l <30; l++)
   {
      vpr::System::sleep (1);
      std::cout << "Updating All Data .. " << std::endl;
      input_manager->updateAllData();


      vrj::Matrix* pd_head = head_proxy->getData();
      vrj::Matrix* pd_wand = wand_proxy->getData();

      std::cout << "-------------------------------------\n";
      std::cout << "head:\n" << *pd_head << std::endl;
      vrj::Coord head_coord(*pd_head);
      std::cout << "\tpos:" << head_coord.pos << std::endl;
      std::cout << "\tor:" << head_coord.orient << std::endl;

      std::cout << "wand:\n" << *pd_wand << std::endl << std::flush;
      vrj::Coord wand_coord(*pd_wand);
      std::cout << "\tpos:" << wand_coord.pos << std::endl;
      std::cout << "\tor:" << wand_coord.orient << std::endl;
      std::cout << "-------------------------------------\n\n";
   }


   // -- Delete the garabage --- //
   delete input_manager;

   return 0;
}
