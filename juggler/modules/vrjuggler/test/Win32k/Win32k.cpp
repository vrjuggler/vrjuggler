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

// Win32k.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include <VPR/SharedMem/vjMemPool.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjPosInterface.h>
#include <Math/vjCoord.h>

#include <Input/vjInput/vjKeyboardWin32.h>


#define CHUNK_DESC_LOCATION "\\\\endless2\\c$\\juggler\\test\\win32k\\chunksDesc.txt"
#define CONFIG_LOCATION "\\\\endless2\\c$\\juggler\\test\\win32k\\vjconfig.txt"

// you may need to uncomment this if g_hInst is not found in link
//HINSTANCE g_hInst = NULL;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	g_hInst = hInstance;
	Keyboard keys;

	   // --- Load chunk database -- //
   MemPool* shared_pool = new SharedPool(1024*1024);
   ChunkDescDB desc;
   bool load_worked = desc.load(CHUNK_DESC_LOCATION);
   if(!load_worked)
      cerr << "Could not load chunkDesc's\n" << flush;

   cout << desc << "\n----------------------------------------" << endl;

   // -- Load config -- //
   ConfigChunkDB *chunkdb = new ConfigChunkDB(&desc);
   load_worked = chunkdb->load(CONFIG_LOCATION);
   if(!load_worked)
      cerr << "Could not load config file\n" << flush;

   cout << "Printing Chunk DB:" << endl;
   cout << (*chunkdb);
   cout << "endochunks" << endl;

   InputManager *input_manager = new(shared_pool)InputManager;
   cout << "vjInputManager created" << endl;

   // --- configure the input manager -- //
   input_manager->FNewInput(chunkdb);
   cout << "new devices have been added.." << endl << endl;

   input_manager->DumpStatus();

   cout << "Sleeping..." << flush;
   Sleep(2);
   cout << "awake." << endl << flush;

      // Get the indices for the devices
   int head_index = input_manager->GetProxyIndex("VJHead");
   int wand_index = input_manager->GetProxyIndex("VJWand");

   for (int l = 0; l <30; l++)
   {
      Sleep (1);
      cout << "Updating All Data .. " << endl;
      input_manager->UpdateAllData();


      Matrix* pd_head = input_manager->GetPosProxy(head_index)->GetData();
      Matrix* pd_wand = input_manager->GetPosProxy(wand_index)->GetData();

      cout << "-------------------------------------\n";
      cout << "head:\n" << *pd_head << endl;
      Coord head_coord(*pd_head);
      cout << "\tpos:" << head_coord.pos << endl;
      cout << "\tor:" << head_coord.orient << endl;

      cout << "wand:\n" << *pd_wand << endl << flush;
      Coord wand_coord(*pd_wand);
      cout << "\tpos:" << wand_coord.pos << endl;
      cout << "\tor:" << wand_coord.orient << endl;
      cout << "-------------------------------------\n\n";
   }

   while (1)
   {
	Sleep(500);  //(milliseconds)
	input_manager->UpdateAllData();
   }
   // -- Delete the garabage --- //
   delete input_manager;
   delete shared_pool;


	return 0;
}



