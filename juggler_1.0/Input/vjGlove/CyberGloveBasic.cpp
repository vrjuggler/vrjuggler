/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>

// need stdio for sprintf
#include <stdio.h>
#include <sys/time.h>

/* CyberGloveBasic */
#include <Input/vjGlove/CyberGloveBasic.h>
#include <Input/vjGlove/vt_globals.h>
#include <Input/vjGlove/vt_types.h>
#include <Input/vjGlove/vt_error.h>

char version_string[] = "2.0.2";
int vt_error_code = 0;
int vt_error_level = 2;

CyberGloveBasic::CyberGloveBasic (char* calFileDir, char *serport, int baud)
   : mItsOpen(false)
{
   if(calFileDir != NULL)
   {
      mCalFileDir = new char [strlen(calFileDir) + 1];
	   strcpy(mCalFileDir,calFileDir);
   }
   else
      cerr << "Invalid calibration file specified" << endl;

   if(serport != NULL)
   {
      mSerialPort = new char [strlen(serport) + 1];
	   strcpy(mSerialPort,serport);
   }
   else
      cerr << "Invalid calibration file specified" << endl;

   mBaudRate = baud;
};

CyberGloveBasic::~CyberGloveBasic ()
{
   close();
   if (needToClosePorts)
      vt_serial_close_ports();
   if (mCalFileDir != NULL)
      delete [] mCalFileDir;
   if (mSerialPort != NULL)
      delete mSerialPort;
}

int CyberGloveBasic::open()
{
   AppDataStruct app;
   if (!mItsOpen)
   {
      current_glove = NULL;
      current_glove_private = NULL;
      vt_init(mCalFileDir,app);
      hand = vt_create_VirtualHand(mSerialPort,mBaudRate,app);
      current_glove = hand->private_data->glove;
      current_glove_private = hand->private_data->glove->private_data;

      return 1;
   }
   else
      return 0; // already sampling
}

int CyberGloveBasic::close()
{
   if(mItsOpen)
   {
      vt_destroy_VirtualHand(hand);
      cout << "Closing glove" << endl;
   }

   return 1;
}



int CyberGloveBasic::sample()
{
   //struct timeval tv;
   //double start_time, stop_time;
   //static int c = 0;

   if (hand->read_glove)
   {
      /*if (c == 0)
      {
         gettimeofday(&tv,0);
         start_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;
      } c++;

      if (c == 60) {
         gettimeofday(&tv,0);
         stop_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;
         cout << 1/((stop_time-start_time) / 60)
              << "  " << endl;
         c = 0;
      }*/

      vt_send_glove_command(CG_REQUEST_DATA);
   }

   vt_update_hand_state(hand);

  int i,j;
  for(i=0; i < MAX_SENSOR_GROUPS; i++)
     for(j = 0; j < MAX_GROUP_VALUES; j++)
        theData.joints[i][j] = hand->joint_angle[i][j];

     /*
   for(i=0; i < MAX_SENSOR_GROUPS; i++)
     for(j = 0; j < MAX_GROUP_VALUES; j++)
       for(int k = 0; k <= 4; k++)
          for(int l = 0; l <= 4; l++)
	(theData[progress].xforms[i][j])[k][l] = (hand->digit_xform[i][j])[k][l];
 */
     return 1;
}


CYBER_GLOVE_DATA* CyberGloveBasic::getData()
{
   return &theData;
}


//volatile float** CyberGloveBasic::getData () {
//	/* I always find multidimensional arrays confusing to pass
//	 * and return.  For the record, the glove code defines the
// 	 * type of joint angle as:
//	 * volatile float (*joint_angle)[MAX_GROUP_VALUES]
//	 * or float joint_angle[MAX_SENSOR_GROUPS][MAX_GROUP_VALUES]
//	 */
//      int i;
////	hand->joint_angle[THUMB][MCP]=3.0;
//      for (i = 0; i < MAX_SENSOR_GROUPS; i++)
//	joints[i] = (hand->joint_angle[i]);
//      return joints;
//    };


matrix4x4 ** CyberGloveBasic::getDigitXForms ()
{
   int i;
   for (i = 0; i < 5; i++)
	xforms[i] = (hand->digit_xform[i]);
      return xforms;	
};

float CyberGloveBasic::getThumbRoll ()
{
	return (DEG2RAD)*vt_calc_thumb_roll(hand);
};

SurfaceModel CyberGloveBasic::getSurfaceModel()
{
	return (hand->surface);
};

volatile float * CyberGloveBasic::getStateVec()
{
	return hand->state_vec;
}

float CyberGloveBasic::getThumbUnflexedAbduction() {
 	return hand->unflexed_abduction;
}

int CyberGloveBasic::getFingerState (int finger)
{
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


void CyberGloveBasic::readSurfaceModel (int resolution, char *dir, char *name) {
  char filename[256];
  sprintf (filename, "%s/%s", dir, name);
  if (!resolution)  // lowres
    vt_read_lowres_hand_model (filename, hand);
  else  // hires
    vt_read_hand_model (filename, hand, dir);
}
