#include <SharedMem/vjMemPool.h>
#include <Input/InputManager/vjInputManager.h>


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
   input_manager->FNewInput(chunkdb);
   cout << "new devices have been added.." << endl << endl;

   input_manager->DumpStatus();

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


      vjMatrix* pd_head = input_manager->GetPosData(head_index);
      vjMatrix* pd_wand = input_manager->GetPosData(wand_index);    
      
      cout << "head:\n" << *pd_head << endl;
      cout << "wand:\n" << *pd_wand << endl << flush;
   } 


   // -- Delete the garabage --- //
   delete input_manager;
   delete shared_pool;

   return 0;
}
