/* vjCyberGlove */
#include <config.h>
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

//vjCyberGlove::vjCyberGlove (char *homedir, char *serport, int baud) {
vjCyberGlove::vjCyberGlove(vjConfigChunk *c) : vjInput(c) {
    cerr << "	vjCyberGlove::vjCyberGlove(vjConfigChunk*) " << endl;
    char* t = c->getProperty("homedir");
    if (t != NULL)
    {
        home = new char [strlen(t) + 1];
	strcpy(home,t);
    }

    myThreadID = 0;
};

int
vjCyberGlove::StartSampling() {
   AppDataStruct app; 
   if (myThreadID == 0) {
   //int i;
   current = 0; valid = 1; progress = 2;

      current_glove = NULL;
      current_glove_private = NULL;
      vt_init(home,app);
      hand = vt_create_VirtualHand(sPort,baudRate,app);
      current_glove = hand->private_data->glove;
      current_glove_private = hand->private_data->glove->private_data;
   vjCyberGlove* devicePtr = this;
   void SampleGlove(void*);

   if ( (myThreadID = vjThread::spawn(SampleGlove, (void *) devicePtr, 0)) ) {
      return 0;
   } else {
      cout << "     vjCyberGlove is active " << endl;
      active = 1;
      return 1;
   }
  }
   else return 0; // already sampling


}

void SampleGlove(void* pointer) {

    vjCyberGlove* devPointer = (vjCyberGlove*)pointer;
    for(;;) 
 	devPointer->Sample();

}

int vjCyberGlove::Sample() {
   struct timeval tv;
  double start_time, stop_time;
 static int c = 0;
 
    if (hand->read_glove) {
       if (c == 0) {
     gettimeofday(&tv,0);
     start_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;
    } c++;
        if (c == 60) {
     gettimeofday(&tv,0);
     stop_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;
     cout << 1/((stop_time-start_time) / 60)
          << "  " << endl;
     c = 0;
     }
      vt_send_glove_command(CG_REQUEST_DATA); }
    vt_update_hand_state(hand);

  int i,j;
  for(i=0; i < MAX_SENSOR_GROUPS; i++)
     for(j = 0; j < MAX_GROUP_VALUES; j++) 
        theData[progress].joints[i][j] = hand->joint_angle[i][j];
/*  for(i=0; i < MAX_SENSOR_GROUPS; i++)
     for(j = 0; j < MAX_GROUP_VALUES; j++) 
       for(int k = 0; k <= 4; k++)
          for(int l = 0; l <= 4; l++)
	(theData[progress].xforms[i][j])[k][l] = (hand->digit_xform[i][j])[k][l];
 */

  lock.acquire();
  int tmp = valid;
  valid = progress;
  progress = tmp;
  lock.release();
  return 1;
  
}


int vjCyberGlove::StopSampling() {
  if (myThreadID != 0) {
    vjThread::kill(myThreadID,SIGKILL);
    delete(myThreadID);
    myThreadID = 0;
    sginap(1);
      vt_destroy_VirtualHand(hand);
    cout << "stopping vjCyberGlove.." << endl;

   }
   return 1;
}

vjCyberGlove::~vjCyberGlove () {
        StopSampling();
      if (needToClosePorts)
        vt_serial_close_ports();
      if (home != NULL)
        delete [] home;
    }

void vjCyberGlove::UpdateData () {

  lock.acquire();
  int tmp = valid;
  valid = current;
  current = tmp;
  lock.release();

    }

void vjCyberGlove::GetData(vjGLOVE_DATA* &data)
{  
   data = &theData[current];
}

volatile float** vjCyberGlove::GetData () {
	/* I always find multidimensional arrays confusing to pass 
	 * and return.  For the record, the glove code defines the
 	 * type of joint angle as:
	 * volatile float (*joint_angle)[MAX_GROUP_VALUES]
	 * or float joint_angle[MAX_SENSOR_GROUPS][MAX_GROUP_VALUES]
	 */
      int i;
//	hand->joint_angle[THUMB][MCP]=3.0;
      for (i = 0; i < MAX_SENSOR_GROUPS; i++)
	joints[i] = (hand->joint_angle[i]);
      return joints;
    };

matrix4x4 ** vjCyberGlove::GetDigitXForms () {
      int i;
      for (i = 0; i < 5; i++)
	xforms[i] = (hand->digit_xform[i]);
      return xforms;	
    };

float vjCyberGlove::GetThumbRoll () {
	return (DEG2RAD)*vt_calc_thumb_roll(hand);
    };

SurfaceModel vjCyberGlove::GetSurfaceModel() {
	return (hand->surface);
};

volatile float * vjCyberGlove::GetStateVec() {
	return hand->state_vec;
}

float vjCyberGlove::GetThumbUnflexedAbduction() {
 	return hand->unflexed_abduction;
}

int vjCyberGlove::GetFingerState (int finger) {
     /* a quick function to determine the position of a finger.
	return values:
	0 finger open/pointing
	1 finger closed
	2 finger inbetween/relaxed (whatever isn't zero or one
      */
     /* Still need to double check these values...
        I'm taking a hint from the other gesture recognition code &
        using mainly the metacarpal(?) joints to figure out finger
        positions.  Makes it easier for those of us with screwy 
        figures, and the 3rd knuckle reading isn't likely to be 
        to accurate (just because of the way the glove fits)
      */
      if (hand->joint_angle[finger][0] > -0.75 &&
	  hand->joint_angle[finger][1] > -0.75 )
	return 0;
      if (hand->joint_angle[finger][0] < -0.35 /*&&
	  hand->joint_angle[finger][1] < -0.35 */)
	return 1;
      return 0;
    };


void vjCyberGlove::ReadSurfaceModel (int resolution, char *dir, char *name) {
  char filename[256];
  sprintf (filename, "%s/%s", dir, name);
  if (!resolution)  // lowres
    vt_read_lowres_hand_model (filename, hand);
  else  // hires
    vt_read_hand_model (filename, hand, dir);
}

