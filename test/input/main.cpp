/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#include <iostream.h>

#include <SharedMem/vjMemPool.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjChunkFactory.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjPosInterface.h>
#include <Math/vjCoord.h>


#define CHUNK_DESC_LOCATION "/home/users/allenb/Source/juggler/Data/chunksDesc"
#define CONFIG_LOCATION "/home/users/allenb/.vjconfig/activeConfig"

int main()
{
   // --- Load chunk database -- //
   vjMemPool* shared_pool = new vjSharedPool(1024*1024);
   vjChunkDescDB desc;
   bool load_worked = desc.load(CHUNK_DESC_LOCATION);
   vjChunkFactory::setChunkDescDB(&desc);

   if(!load_worked)
      cerr << "Could not load chunkDesc's\n" << flush;

   cout << desc << "\n----------------------------------------" << endl;

   // -- Load config -- //
   vjConfigChunkDB *chunkdb = new vjConfigChunkDB();
   load_worked = chunkdb->load(CONFIG_LOCATION);
   if(!load_worked)
      cerr << "Could not load config file\n" << flush;

   cout << "Printing Chunk DB:" << endl;
   cout << (*chunkdb);
   cout << "endochunks" << endl;

   vjInputManager *input_manager = new(shared_pool)vjInputManager;
   cout << "vjInputManager created" << endl;

   // --- configure the input manager -- //
   input_manager->configureInitial(chunkdb);
   cout << "new devices have been added.." << endl << endl;

   cout << input_manager << endl;

   cout << "Sleeping..." << flush;
   sleep(2);
   cout << "awake." << endl << flush;

      // get the indices for the devices
   int head_index = input_manager->getProxyIndex("VJHead");
   int wand_index = input_manager->getProxyIndex("VJWand");

   for (int l = 0; l <30; l++)
   {
      sleep (1);
      cout << "Updating All Data .. " << endl;
      input_manager->updateAllData();


      vjMatrix* pd_head = input_manager->getPosProxy(head_index)->getData();
      vjMatrix* pd_wand = input_manager->getPosProxy(wand_index)->getData();

      cout << "-------------------------------------\n";
      cout << "head:\n" << *pd_head << endl;
      vjCoord head_coord(*pd_head);
      cout << "\tpos:" << head_coord.pos << endl;
      cout << "\tor:" << head_coord.orient << endl;

      cout << "wand:\n" << *pd_wand << endl << flush;
      vjCoord wand_coord(*pd_wand);
      cout << "\tpos:" << wand_coord.pos << endl;
      cout << "\tor:" << wand_coord.orient << endl;
      cout << "-------------------------------------\n\n";
   }


   // -- Delete the garabage --- //
   delete input_manager;
   delete shared_pool;

   return 0;
}
