#include <vjConfig.h>
#include <sys/time.h>
#include <iostream.h>

#include <Input/vjInput/vjIbox.h>
#include <Threads/vjThread.h>
#include <Kernel/vjDebug.h>

/**********************************************************
  vjIBox::vjIBox(vjConfigChunk *c)

  IBox Constructor, to be called from vjInputManager

*********************************************** ahimberg */
vjIBox::vjIBox(vjConfigChunk *c) : vjAnalog(c), vjDigital(c), vjInput(c)
{
  cerr << "	vjIBox::vjIbox(vjConfigChunk*)" << endl;
  port_id = c->getProperty("portNum");

  active = 0;
  baudRate = c->getProperty("baud");

  vjDEBUG(5) << "   Creating an IBox.. params: " << endl
             << "	   portnum: " << port_id << endl
             << "        baud   : " << baudRate << endl
             << "   instanceName: " << instName << endl << endl << vjDEBUG_FLUSH;
}

/**********************************************************
  vjIBox::~vjIBox()

  IBox Destructor, just stop sampling, let other destructors
  do the rest.

*********************************************** ahimberg */
vjIBox::~vjIBox()
{
  StopSampling();
}

/**********************************************************
  int vjIBox::StartSampling()

  Starts the Ibox sampling, spawns a thread to query for
  data.

  returns 1 if successful, 0 if it failed or was already sampling

*********************************************** ahimberg */
int vjIBox::StartSampling()
{
  ibox_result result;

  if (myThread == NULL)
  {
      resetIndexes();      // Reset the indexes for triple buffering

      result = ibox_connect(&thingie, port_id, baudRate);
      if (result == SUCCESS)
      {
  	    active = 1;
	    vjDEBUG(4) << "     Connected to IBox.\n" << flush << vjDEBUG_FLUSH;
      }
      else
      {
	    active = 0;
	    vjDEBUG(5) << "   FAILED TO CONNECT to the Ibox named " << instName << endl
	     << "     Ibox settings were: " << endl
	     << "	     port : " << port_id << endl
	     << "	  baudRate: " << baudRate << endl << endl << vjDEBUG_FLUSH;
	    return 0;
      }
      hci_std_cmd(&thingie, 0,0,0);


      vjIBox* devicePtr = this;
      void SampleBox(void*);
      myThread = new vjThread(SampleBox, (void*)devicePtr, 0);
      if (!myThread->valid())
	      return 0; //fail
      else {
	      return 1;
      }
  }
  else return 0; // already sampling

}

/**********************************************************
  void SampleBox(void*)

  The spawned thread just loops from here

*********************************************** ahimberg */
void SampleBox(void* pointer)
{
   vjIBox* devPointer = (vjIBox*) pointer;

   for (;;)
     devPointer->Sample();

}

/**********************************************************
  int vjIBox::Sample()

  IBox Sampler function, tries to get a packet of new stuff,
  when it does it fills up the data and swaps swaps the
  progess/valid array indicies.

  Each call to this function is not guaranteed to result in new data.

*********************************************** ahimberg */
int vjIBox::Sample()
{
     struct timeval tv;
     // double start_time, stop_time;
     ibox_result result;
     int tmp;
     static int c = 0;

     result = hci_check_packet(&thingie);
     if (result == NO_PACKET_YET)
	     ;
     else if (result == SUCCESS)
     {
        hci_std_cmd(&thingie, 0,0,0);
	if (c == 0) {
	  gettimeofday(&tv,0);
	  // start_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;
	}
	c++;
	if (c == 60) {
	  gettimeofday(&tv,0);
	  // stop_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;
	  //cout << 1/((stop_time-start_time) / 60)
	  //     << "  " << endl;
	  c = 0;
	}

	theData[progress].button[0] = thingie.button[0];
	theData[progress].button[1] = thingie.button[1];
	theData[progress].button[2] = thingie.button[2];
	theData[progress].button[3] = thingie.button[3];
	theData[progress].analog[0] = thingie.analog[0];
	theData[progress].analog[1] = thingie.analog[1];
	theData[progress].analog[2] = thingie.analog[2];
	theData[progress].analog[3] = thingie.analog[3];

	swapValidIndices();     // Swap the buffers since we just read in a complete value
     }
     return 1;
}

/**********************************************************
  int vjIBox::StopSampling()

  Kill the sampling thread if it exists, disconnect from
  the ibox.

*********************************************** ahimberg */
int vjIBox::StopSampling()
{
  if (myThread != NULL)
  {
    myThread->kill();
    delete(myThread);
    myThread = NULL;

    sginap(1);

    hci_disconnect(&thingie);
    cout << "stopping the ibox.." << endl;

  }
  return 1;
}

/**********************************************************
  int vjIBox::GetAnalogData(int)

*********************************************** ahimberg */
int vjIBox::GetAnalogData(int d)
{
 return  (theData+current*sizeof(vjIBOX_DATA))->analog[d];
}

/**********************************************************
  int vjIBox::GetDigitalData(int)

*********************************************** ahimberg */
int vjIBox::GetDigitalData(int d)
{
 return  (theData+current*sizeof(vjIBOX_DATA))->button[d];
}

/**********************************************************
  void vjIBox::UpdateData()

  Swap the current/valid array indicies

*********************************************** ahimberg */
void vjIBox::UpdateData()
{
  // swap the indicies for the pointers
   swapCurrentIndexes();

  return;
}

