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

char version_string[] = "2.0.2";
int vt_error_code = 0;
int vt_error_level = 2;

vjCyberGlove::vjCyberGlove(vjConfigChunk *c) : vjGlove(c)
{
    char* home_dir = c->getProperty("homedir");
    if (home_dir != NULL)
    {
        mCalDir = new char [strlen(home_dir) + 1];
	     strcpy(mCalDir,home_dir);
    }

    myThread = NULL;

    mGlove = new CyberGloveBasic(mCalDir, sPort, baudRate);
};

int
vjCyberGlove::StartSampling()
{
   if (myThread == NULL)
   {
      resetIndexes();

      if(mGlove->Open() == 0)
      {
         vjDEBUG(0) << "ERROR: Can't open Cyberglove or it is already opened." << vjDEBUG_FLUSH;
         return 0;
      }

      // Create a new thread to handle the control
      vjThreadMemberFunctor<vjCyberGlove>* memberFunctor =
         new vjThreadMemberFunctor<vjCyberGlove>(this, &vjCyberGlove::controlLoop, NULL);

      mControlThread = new vjThread(memberFunctor, 0);

      if (!myThread->valid())
      {
         return 0;
      }
      else
      {
         vjDEBUG(1) << "vjCyberGlove is active " << endl;
         active = 1;
         return 1;
      }
  }
  else
     return 0; // already sampling
}

void vjCyberGlove::controlLoop(void* nullParam)
{
   while(1)
 	   Sample();
}

int vjCyberGlove::Sample()
{
   mGlove->Sample();       // Tell the glove to sample

   copyDataFromGlove();                   // Copy the data across
   mTheData[progress][0].calcXforms();    // Update the xform data
   swapValidIndices();
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
   if (mControlThread != NULL)
   {
      mControlThread->kill();
      delete mControlThread;
      mControlThread = NULL;
      sginap(1);

      mGlove->Close();
      vjDEBUG(0) << "stopping vjCyberGlove.." << endl;
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
         mTheData[progress][0].angles[i][j] = glove_data->joints[i][j];

   vjASSERT(mTheData[progress][0].angles[vjGloveData::MIDDLE][vjGloveData::MPJ]
             == glove_data->joints[MIDDLE][MCP]);
}
