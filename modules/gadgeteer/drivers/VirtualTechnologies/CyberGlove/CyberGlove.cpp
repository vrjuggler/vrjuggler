/* vjCyberGlove */
#include <vjConfig.h>
#include <sys/time.h>

// need stdio for sprintf
#include <stdio.h>

#include <Input/vjGlove/vt_types.h>
#include <Input/vjGlove/vjCyberGlove.h>
#include <Input/vjGlove/vt_globals.h>
#include <Input/vjGlove/vt_types.h>
#include <Input/vjGlove/vt_error.h>
#include <Kernel/vjKernel.h>


bool vjCyberGlove::config(vjConfigChunk *c)
{
   if(!vjGlove::config(c))
      return false;

   vjASSERT(myThread == NULL);      // This should have been set by vjInput(c)

    char* home_dir = c->getProperty("calDir").cstring();
    if (home_dir != NULL)
    {
        mCalDir = new char [strlen(home_dir) + 1];
	     strcpy(mCalDir,home_dir);
    }

    std::string glove_pos_proxy = c->getProperty("glovePos");    // Get the name of the pos_proxy
    if(glove_pos_proxy == std::string(""))
    {
       vjDEBUG(vjDBG_ALL,0) << "ERROR: vjCyberglove has no posProxy." << endl << vjDEBUG_FLUSH;
       return false;
    }

    // init glove proxy interface
    int proxy_index = vjKernel::instance()->getInputManager()->GetProxyIndex(glove_pos_proxy);
    if(proxy_index != -1)
       mGlovePos[0] = vjKernel::instance()->getInputManager()->GetPosProxy(proxy_index);
    else
       vjDEBUG(vjDBG_ALL,0) << "ERROR: vjCyberGlove::vjCyberGlove: Can't find posProxy." << endl << vjDEBUG_FLUSH << endl;

    mGlove = new CyberGloveBasic(mCalDir, sPort, baudRate);

    return true;
};

int
vjCyberGlove::StartSampling()
{
   if (myThread == NULL)
   {
      resetIndexes();

      // Create a new thread to handle the control
      vjThreadMemberFunctor<vjCyberGlove>* memberFunctor =
         new vjThreadMemberFunctor<vjCyberGlove>(this, &vjCyberGlove::controlLoop, NULL);

      myThread = new vjThread(memberFunctor, 0);

      if (!myThread->valid())
      {
         return 0;
      }
      else
      {
         vjDEBUG(vjDBG_ALL,1) << "vjCyberGlove is active " << endl;
         active = 1;
         return 1;
      }
  }
  else
     return 0; // already sampling
}

// -Opens the glove port
// -Starts sampling the glove
void vjCyberGlove::controlLoop(void* nullParam)
{
   // Open the port and run with it
   if(mGlove->Open() == 0)
   {
      vjDEBUG(vjDBG_ALL,0) << "ERROR: Can't open Cyberglove or it is already opened." << vjDEBUG_FLUSH;
      return;
   }

   // Spin in the sampling
   while(1)
 	   Sample();
}

int vjCyberGlove::Sample()
{
   mGlove->Sample();       // Tell the glove to sample

   copyDataFromGlove();                   // Copy the data across
   mTheData[0][progress].calcXforms();    // Update the xform data

   swapValidIndexes();
   return 1;
}

void vjCyberGlove::UpdateData()
{
   // swap the indicies for the pointers
   swapCurrentIndexes();

  return;
}

int vjCyberGlove::StopSampling()
{
   if (myThread != NULL)
   {
      myThread->kill();
      delete myThread;
      myThread = NULL;
      sginap(1);

      mGlove->Close();
      vjDEBUG(vjDBG_ALL,0) << "stopping vjCyberGlove.." << endl;
   }
   return 1;
}

vjCyberGlove::~vjCyberGlove ()
{
   StopSampling();      // Stop the glove
   delete mGlove;       // Delete the glove
}



void vjCyberGlove::copyDataFromGlove()
{
   CYBER_GLOVE_DATA* glove_data = mGlove->GetData();     // Get ptr to data

   for(int i=0;i<vjGloveData::NUM_COMPONENTS;i++)
      for(int j=0;j<vjGloveData::NUM_JOINTS;j++)
         mTheData[0][progress].angles[i][j] = glove_data->joints[i][j];

   vjASSERT(mTheData[0][progress].angles[vjGloveData::MIDDLE][vjGloveData::MPJ]
             == glove_data->joints[MIDDLE][MCP]);
}

