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

#include <iostream>

#include <VPR/SharedMem/vjMemPool.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjChunkFactory.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjPosInterface.h>
#include <Math/vjCoord.h>
#include <vpr/System.h>


#define CHUNK_DESC_LOCATION "/home/users/allenb/Source/juggler/Data/chunksDesc"
#define CONFIG_LOCATION "/home/users/allenb/.vjconfig/activeConfig"

int main()
{
   // --- Load chunk database -- //
   MemPool* shared_pool = new SharedPool(1024*1024);
   ChunkDescDB desc;
   bool load_worked = desc.load(CHUNK_DESC_LOCATION);
   ChunkFactory::setChunkDescDB(&desc);

   if(!load_worked)
      std::cerr << "Could not load chunkDesc's\n" << std::flush;

   std::cout << desc << "\n----------------------------------------"
             << std::endl;

   // -- Load config -- //
   ConfigChunkDB *chunkdb = new ConfigChunkDB();
   load_worked = chunkdb->load(CONFIG_LOCATION);
   if(!load_worked)
      std::cerr << "Could not load config file\n" << std::flush;

   std::cout << "Printing Chunk DB:" << std::endl;
   std::cout << (*chunkdb);
   std::cout << "endochunks" << std::endl;

   InputManager *input_manager = new(shared_pool)InputManager;
   std::cout << "vjInputManager created" << std::endl;

   // --- configure the input manager -- //
   input_manager->configureInitial(chunkdb);
   std::cout << "new devices have been added..\n\n";

   std::cout << input_manager << std::endl;

   std::cout << "Sleeping..." << std::flush;
   vpr::System::sleep(2);
   std::cout << "awake." << std::endl << std::flush;

      // get the indices for the devices
   int head_index = input_manager->getProxyIndex("VJHead");
   int wand_index = input_manager->getProxyIndex("VJWand");

   for (int l = 0; l <30; l++)
   {
      vpr::System::sleep (1);
      std::cout << "Updating All Data .. " << std::endl;
      input_manager->updateAllData();


      Matrix* pd_head = input_manager->getPosProxy(head_index)->getData();
      Matrix* pd_wand = input_manager->getPosProxy(wand_index)->getData();

      std::cout << "-------------------------------------\n";
      std::cout << "head:\n" << *pd_head << std::endl;
      Coord head_coord(*pd_head);
      std::cout << "\tpos:" << head_coord.pos << std::endl;
      std::cout << "\tor:" << head_coord.orient << std::endl;

      std::cout << "wand:\n" << *pd_wand << std::endl << std::flush;
      Coord wand_coord(*pd_wand);
      std::cout << "\tpos:" << wand_coord.pos << std::endl;
      std::cout << "\tor:" << wand_coord.orient << std::endl;
      std::cout << "-------------------------------------\n\n";
   }


   // -- Delete the garabage --- //
   delete input_manager;
   delete shared_pool;

   return 0;
}
