/* vjPinchGlove */
#include <vjConfig.h>
#include <sys/time.h>
#include <stdio.h>                      // need stdio for sprintf
#include <Input/vjGlove/fsPinchGlove.h> //fakespace pinch driver
#include <Input/vjGlove/vjPinchGlove.h> //vrjuggler pinch driver


bool vjPinchGlove::mLookupInitialized = false;
float vjPinchGlove::mOnLookupTable[4][11];
float vjPinchGlove::mOffLookupTable[4][11];

bool vjPinchGlove::config(vjConfigChunk *c)				
{
   if(!vjGlove::config(c))
      return false;

   // init the lookup table if it hasn't been
    if (vjPinchGlove::mLookupInitialized == false)
    	vjPinchGlove::mInitLookupTable();

    char* home_dir = c->getProperty("homedir");
    if (home_dir != NULL)
    {
        mCalDir = new char [strlen(home_dir) + 1];
	     strcpy(mCalDir,home_dir);
    }

    myThread = NULL;
    mGlove = new fsPinchGlove();

    // these params are available, i only need sPort:
    // mCalDir, sPort, baudRate
    vjASSERT( mGlove != NULL );
}

vjPinchGlove::~vjPinchGlove ()
{
   StopSampling();      // Stop the glove
   delete mGlove;       // Delete the glove
}

int vjPinchGlove::StartSampling()
{
   if (myThread == NULL)
   {
      resetIndexes();

      if (mGlove->connectToHardware( sPort ) == false)
      {
         vjDEBUG(vjDBG_ALL,0) << "ERROR: Can't open Pinchglove or it is already opened." << vjDEBUG_FLUSH;
         return 0;
      }

      // Create a new thread to handle the control
      vjThreadMemberFunctor<vjPinchGlove>* memberFunctor =
         new vjThreadMemberFunctor<vjPinchGlove>(this, &vjPinchGlove::controlLoop, NULL);

      mControlThread = new vjThread(memberFunctor, 0);

      if (!myThread->valid())
      {
         return 0;
      }
      else
      {
         vjDEBUG(vjDBG_ALL,1) << "vjPinchGlove is active " << endl;
         active = 1;
         return 1;
      }
  }
  else
     return 0; // already sampling
}

void vjPinchGlove::controlLoop(void* nullParam)
{
   while(1)
	Sample();
}

int vjPinchGlove::Sample()
{
    // Tell the glove to resample
    mGlove->reSampleStringFromHardware();

    // Copy the data from the fsPinchGlove to myself.
    copyDataFromGlove();

    // Update the xform data
    mTheData[progress][0].calcXforms();
    mTheData[progress][1].calcXforms();

    swapValidIndexes();
    return 1;
}

void vjPinchGlove::UpdateData()
{
vjGuard<vjMutex> updateGuard(lock);

   // Copy the valid data to the current data so that both are valid
   mTheData[0][current] = mTheData[0][valid];   // first hand
   mTheData[1][current] = mTheData[1][valid];   // other hand


   // swap the indicies for the pointers
   swapCurrentIndexes();

    return;
}

int vjPinchGlove::StopSampling()
{
   if (mControlThread != NULL)
   {
      mControlThread->kill();
      delete mControlThread;
      mControlThread = NULL;
      sginap(1);

    // there is no "close"
      //mGlove->Close();
      vjDEBUG(vjDBG_ALL,0) << "stopping vjPinchGlove.." << endl;
   }
   return 1;
}

void vjPinchGlove::mInitLookupTable()
{
    //  DIJ = Distal Interphalangeal Joint  --- finger tip
    //  PIJ = Proximal "              "     --- Middle joint
    //  MPJ = Metacarpo "             "     --- closest to hand
    //  ABDUCT = spread of fingers
    //
    // YAW and PITCH apply only to WRIST

    // LPINKY = 0, LRING = 1, LMIDDLE = 2, LINDEX = 3, LTHUMB = 4, : RTHUMB = 6, RINDEX = 7, RMIDDLE = 8, RRING = 9, RPINKY = 10;

    // Initialize the lookup tables
    mOnLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::LPINKY ] = -15;
    mOnLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::LPINKY ] = -45;
    mOnLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::LPINKY ] = -45;
    mOnLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::LPINKY ] = 0;

    mOnLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::LRING ] = -15;
    mOnLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::LRING ] = -45;
    mOnLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::LRING ] = -45;
    mOnLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::LRING ] = 0;

    mOnLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::LMIDDLE ] = -15;
    mOnLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::LMIDDLE ] = -45;
    mOnLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::LMIDDLE ] = -45;
    mOnLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::LMIDDLE ] = 0;

    mOnLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::LINDEX ] = -15;
    mOnLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::LINDEX ] = -45;
    mOnLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::LINDEX ] = -45;
    mOnLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::LINDEX ] = 0;

    mOnLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::LTHUMB ] = -15;
    mOnLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::LTHUMB ] = -45;
    mOnLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::LTHUMB ] = -45;
    mOnLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::LTHUMB ] = 0;

    // Initialize the lookup tables
    mOffLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::RPINKY ] = -15;
    mOffLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::RPINKY ] = -45;
    mOffLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::RPINKY ] = -45;
    mOffLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::RPINKY ] = 0;

    mOffLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::RRING ] = -15;
    mOffLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::RRING ] = -45;
    mOffLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::RRING ] = -45;
    mOffLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::RRING ] = 0;

    mOffLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::RMIDDLE ] = -15;
    mOffLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::RMIDDLE ] = -45;
    mOffLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::RMIDDLE ] = -45;
    mOffLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::RMIDDLE ] = 0;

    mOffLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::RINDEX ] = -15;
    mOffLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::RINDEX ] = -45;
    mOffLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::RINDEX ] = -45;
    mOffLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::RINDEX ] = 0;

    mOffLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::RTHUMB ] = -15;
    mOffLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::RTHUMB ] = -45;
    mOffLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::RTHUMB ] = -45;
    mOffLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::RTHUMB ] = 0;
}

void vjPinchGlove::copyDataFromGlove()
{
    char gesture[12];
    // get fakespace's idea of a "pinch gesture"
    mGlove->getSampledString( gesture );

    const int RIGHT_HAND = 0;
    const int LEFT_HAND = 1;

    // use the digital data to lookup the expected angles for each joint.

    // Right Pinky
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::PINKY][vjGloveData::MPJ]
	= (gesture[fsPinchGlove::RPINKY] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::RPINKY] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::RPINKY];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::PINKY][vjGloveData::PIJ]
	= (gesture[fsPinchGlove::RPINKY] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::RPINKY] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::RPINKY];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::PINKY][vjGloveData::DIJ]
	= (gesture[fsPinchGlove::RPINKY] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::RPINKY] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::RPINKY];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
	= (gesture[fsPinchGlove::RPINKY] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RPINKY] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RPINKY];


    // Right Ring
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::RING][vjGloveData::MPJ]
	= (gesture[fsPinchGlove::RRING] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::RRING] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::RRING];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::RING][vjGloveData::PIJ]
	= (gesture[fsPinchGlove::RRING] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::RRING] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::RRING];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::RING][vjGloveData::DIJ]
	= (gesture[fsPinchGlove::RRING] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::RRING] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::RRING];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
	= (gesture[fsPinchGlove::RRING] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RRING] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RRING];


    // Right Middle
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::MIDDLE][vjGloveData::MPJ]
	= (gesture[fsPinchGlove::RMIDDLE] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::RMIDDLE] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::RMIDDLE];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::MIDDLE][vjGloveData::PIJ]
	= (gesture[fsPinchGlove::RMIDDLE] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::RMIDDLE] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::RMIDDLE];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::MIDDLE][vjGloveData::DIJ]
	= (gesture[fsPinchGlove::RMIDDLE] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::RMIDDLE] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::RMIDDLE];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
	= (gesture[fsPinchGlove::RMIDDLE] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RMIDDLE] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RMIDDLE];

    // Right Index
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::INDEX][vjGloveData::MPJ]
	= (gesture[fsPinchGlove::RINDEX] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::RINDEX] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::RINDEX];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::INDEX][vjGloveData::PIJ]
	= (gesture[fsPinchGlove::RINDEX] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::RINDEX] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::RINDEX];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::INDEX][vjGloveData::DIJ]
	= (gesture[fsPinchGlove::RINDEX] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::RINDEX] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::RINDEX];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
	= (gesture[fsPinchGlove::RINDEX] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RINDEX] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RINDEX];

    // Right Thumb
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::THUMB][vjGloveData::MPJ]
	= (gesture[fsPinchGlove::RTHUMB] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::RTHUMB] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::RTHUMB];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::THUMB][vjGloveData::PIJ]
	= (gesture[fsPinchGlove::RTHUMB] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::RTHUMB] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::RTHUMB];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::THUMB][vjGloveData::DIJ]
	= (gesture[fsPinchGlove::RTHUMB] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::RTHUMB] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::RTHUMB];
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
	= (gesture[fsPinchGlove::RTHUMB] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RTHUMB] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RTHUMB];


    // Left Pinky
    mTheData[progress][LEFT_HAND].angles[vjGloveData::PINKY][vjGloveData::MPJ]
	= (gesture[fsPinchGlove::LPINKY] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::LPINKY] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::LPINKY];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::PINKY][vjGloveData::PIJ]
	= (gesture[fsPinchGlove::LPINKY] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::LPINKY] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::LPINKY];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::PINKY][vjGloveData::DIJ]
	= (gesture[fsPinchGlove::LPINKY] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::LPINKY] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::LPINKY];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
	= (gesture[fsPinchGlove::LPINKY] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LPINKY] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LPINKY];

    // Left Ring
    mTheData[progress][LEFT_HAND].angles[vjGloveData::RING][vjGloveData::MPJ]
	= (gesture[fsPinchGlove::LRING] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::LRING] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::LRING];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::RING][vjGloveData::PIJ]
	= (gesture[fsPinchGlove::LRING] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::LRING] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::LRING];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::RING][vjGloveData::DIJ]
	= (gesture[fsPinchGlove::LRING] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::LRING] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::LRING];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
	= (gesture[fsPinchGlove::LRING] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LRING] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LRING];

    // Left Middle
    mTheData[progress][LEFT_HAND].angles[vjGloveData::MIDDLE][vjGloveData::MPJ]
	= (gesture[fsPinchGlove::LMIDDLE] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::LMIDDLE] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::LMIDDLE];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::MIDDLE][vjGloveData::PIJ]
	= (gesture[fsPinchGlove::LMIDDLE] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::LMIDDLE] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::LMIDDLE];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::MIDDLE][vjGloveData::DIJ]
	= (gesture[fsPinchGlove::LMIDDLE] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::LMIDDLE] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::LMIDDLE];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
	= (gesture[fsPinchGlove::LMIDDLE] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LMIDDLE] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LMIDDLE];

    // Left Index
    mTheData[progress][LEFT_HAND].angles[vjGloveData::INDEX][vjGloveData::MPJ]
	= (gesture[fsPinchGlove::LINDEX] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::LINDEX] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::LINDEX];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::INDEX][vjGloveData::PIJ]
	= (gesture[fsPinchGlove::LINDEX] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::LINDEX] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::LINDEX];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::INDEX][vjGloveData::DIJ]
	= (gesture[fsPinchGlove::LINDEX] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::LINDEX] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::LINDEX];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
	= (gesture[fsPinchGlove::LINDEX] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LINDEX] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LINDEX];

    // Left Thumb
    mTheData[progress][LEFT_HAND].angles[vjGloveData::THUMB][vjGloveData::MPJ]
	= (gesture[fsPinchGlove::LTHUMB] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::LTHUMB] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::LTHUMB];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::THUMB][vjGloveData::PIJ]
	= (gesture[fsPinchGlove::LTHUMB] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::LTHUMB] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::LTHUMB];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::THUMB][vjGloveData::DIJ]
	= (gesture[fsPinchGlove::LTHUMB] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::LTHUMB] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::LTHUMB];
    mTheData[progress][LEFT_HAND].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
	= (gesture[fsPinchGlove::LTHUMB] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LTHUMB] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LTHUMB];

    // Left and Right Wrist
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::WRIST][vjGloveData::YAW] = 0;
    mTheData[progress][RIGHT_HAND].angles[vjGloveData::WRIST][vjGloveData::PITCH] = 0;
    mTheData[progress][LEFT_HAND].angles[vjGloveData::WRIST][vjGloveData::YAW] = 0;
    mTheData[progress][LEFT_HAND].angles[vjGloveData::WRIST][vjGloveData::PITCH] = 0;
}
