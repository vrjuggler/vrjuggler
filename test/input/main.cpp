#include <iostream.h>

#include <SharedMem/vjMemPool.h>
#include <Config/vjChunkDescDB.h>
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
   if(!load_worked)
      cerr << "Could not load chunkDesc's\n" << flush;

   cout << desc << "\n----------------------------------------" << endl;

   // -- Load config -- //
   vjConfigChunkDB *chunkdb = new vjConfigChunkDB(&desc);
   load_worked = chunkdb->load(CONFIG_LOCATION);
   if(!load_worked)
      cerr << "Could not load config file\n" << flush;

   cout << "Printing Chunk DB:" << endl;
   cout << (*chunkdb);
   cout << "endochunks" << endl;

   vjInputManager *input_manager = new(shared_pool)vjInputManager;
   cout << "vjInputManager created" << endl;

   // --- configure the input manager -- //
   input_manager->ConfigureInitial(chunkdb);
   cout << "new devices have been added.." << endl << endl;

   cout << input_manager << endl;

   cout << "Sleeping..." << flush;
   sleep(2);
   cout << "awake." << endl << flush;

      // Get the indices for the devices
   int head_index = input_manager->GetProxyIndex("VJHead");
   int wand_index = input_manager->GetProxyIndex("VJWand");

   for (int l = 0; l <30; l++)
   {
      sleep (1);
      cout << "Updating All Data .. " << endl;
      input_manager->UpdateAllData();


      vjMatrix* pd_head = input_manager->GetPosProxy(head_index)->GetData();
      vjMatrix* pd_wand = input_manager->GetPosProxy(wand_index)->GetData();

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
