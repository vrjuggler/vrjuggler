#include <vjConfig.h>

#include <Kernel/vjUser.h>
#include <Kernel/vjDebug.h>


int vjUser::mNextUserId = 0;


int vjUser::getId()
{ return mUserId; }

string vjUser::getName()
{ return mName; }

bool vjUser::config(vjConfigChunk* chunk)
{
   vjASSERT(chunk != NULL);
   vjASSERT((string)(char*)chunk->getType() == string("JugglerUser"));

   vjDEBUG_BEGIN(1) << "vjUser::config: Creating a new user\n" << vjDEBUG_FLUSH;

   // Assign user id
   mUserId = mNextUserId++;

   // Setup user name
   mName = (string)(char*)chunk->getProperty("Name");

   // Initialize the head stuff
   string head_alias = (string)(char*)chunk->getProperty("headPos");
   mHead.init(head_alias);

   vjDEBUG(1) << "id: " << mUserId << "   Name:" << mName << "   headPos:" << head_alias << endl << vjDEBUG_FLUSH;
   vjDEBUG_END(1) << "\n" << vjDEBUG_FLUSH;
   return true;
}
