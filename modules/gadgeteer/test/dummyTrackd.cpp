/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <limits.h>
#include <sys/prctl.h>
//#include <sys/schedctl.h>
#include <unistd.h>

#include <gadget/Devices/Open/Trackd/trackdmem.h>

#define TRACKD_MAX_SENSORS  30
#define CAVE_MAX_VALUATORS  20
#define CAVE_MAX_BUTTONS    20

struct TRACKD_TRACKING
{
   CAVE_TRACKDTRACKER_HEADER  header;
   CAVE_SENSOR_ST             sensor[TRACKD_MAX_SENSORS];
};

struct TRACKD_CONTROLLER
{
   CAVE_TRACKDCONTROLLER_HEADER  header;
   int                           button[CAVE_MAX_BUTTONS];
   float                         valuator[CAVE_MAX_VALUATORS];
};

static int tracker_shmid=-1, controller_shmid=-1;
static TRACKD_TRACKING*    tracker =NULL;
static TRACKD_CONTROLLER*  controller=NULL;

#define PERMS 0666

// Allocate tracker mem
void allocateTrackerMem(int trackdKey)
{
 tracker_shmid = shmget(trackdKey, sizeof(TRACKD_TRACKING), PERMS | IPC_CREAT);
 if (tracker_shmid < 0)
        {
        fprintf(stderr,"can't get shared memory\n");
        exit(-1);
        }
 tracker = (struct TRACKD_TRACKING *) shmat(tracker_shmid,(char *) 0, 0);
 if (tracker == (struct TRACKD_TRACKING *) -1)
        {
        fprintf(stderr,"can't attach shared memory\n");
        exit(-1);
        }
}

// Allocate controller
void allocateControllerMem(int controllerKey)
{
 controller_shmid = shmget(controllerKey,
                        sizeof(struct TRACKD_CONTROLLER), PERMS | IPC_CREAT);
 if (controller_shmid < 0)
        {
        fprintf(stderr,"can't get shared memory\n");
        exit(-1);
        }
 controller = (struct TRACKD_CONTROLLER *)
                        shmat(controller_shmid,(char *) 0, 0);
 if (controller == (struct TRACKD_CONTROLLER *) -1)
        {
        fprintf(stderr,"can't attach shared memory\n");
        exit(-1);
        }
}

// Initialize tracker
void initTracker()
{
 CAVE_SENSOR_ST sensor;

 tracker->header.version = CAVELIB_2_6;
 tracker->header.numSensors = 0;
 tracker->header.sensorOffset = ((char *)&tracker->sensor[0]) - ((char *)tracker);
 tracker->header.sensorSize = sizeof(CAVE_SENSOR_ST);
 tracker->header.timestamp[0] = tracker->header.timestamp[1] = 0;
 tracker->header.command = 0;

 sensor.x = 0.0;
 sensor.y = 0.0;
 sensor.z = 0.0;
 sensor.elev = 0.0;
 sensor.azim = 0.0;
 sensor.roll = 0.0;
 sensor.calibrated = 0;

 for (int loop=0; loop < TRACKD_MAX_SENSORS; ++loop)
   tracker->sensor[loop] = sensor;
}


// Intiializae controller data
void initController()
{
 controller->header.version = CAVELIB_2_6;
 controller->header.buttonOffset = ((char *)&controller->button[0]) - ((char *)controller);
 controller->header.valuatorOffset = ((char *)&controller->valuator[0]) - ((char *)controller);
 controller->header.numButtons = 0;
 controller->header.numValuators = 0;
 controller->header.timestamp[0] = controller->header.timestamp[1] = 0;
 controller->header.command = 0;

 int loop;
 for (loop=0; loop < CAVE_MAX_BUTTONS; ++loop)
      controller->button[loop] = 0;
 for (loop=0; loop < CAVE_MAX_VALUATORS; ++loop)
      controller->valuator[loop] = 0.0;
 //controller->num_buttons = 0;
 //controller->num_valuators = 0;
}


void getTimeStaMp(uint32_t stamp[2])
{
 struct timeval curtime;
 gettimeofday(&curtime,NULL);
 stamp[0] = curtime.tv_sec;
 stamp[1] = curtime.tv_usec;
}


void setNewTrackerData(int numSensors)
{
 static int nextSensor=0, first=1;
 float t;
 struct timeval curtime;
 static struct timeval starttime;
 if (first)
 {
   gettimeofday(&starttime,NULL);
   first=0;
 }
 gettimeofday(&curtime,NULL);
 t = curtime.tv_sec-starttime.tv_sec + (curtime.tv_usec-starttime.tv_usec)/1000000.0f;

 // Translate head in circle around y axis radius of 4.0f
 // Also rotate around z axis
 // Head will bob up and down as well
 // Head should also "nod" while spinning
 if (nextSensor==0)
 {
   tracker->sensor[0].x = sin(t) * 2.0f;
   tracker->sensor[0].y = 6 + cos(t/2.0f);
   tracker->sensor[0].z = cos(t) * 3.0f;
   tracker->sensor[0].elev = sin(t)*45.0f; 	// sin(t/10.0f) * 45.0f;
   tracker->sensor[0].azim = t*36.0f;	// sin(t/3.0f) * 120.0f;
   tracker->sensor[0].roll = 0.0f;
   tracker->sensor[0].calibrated = 0;
 }
 else
 {
   tracker->sensor[nextSensor].x = sin(t+nextSensor) + nextSensor*2.0f - 2.0f;
   tracker->sensor[nextSensor].y = cos(t+nextSensor) + 4.0f;
   tracker->sensor[nextSensor].z = -4.0f;
   tracker->sensor[nextSensor].elev = sin(t/4.0f+nextSensor) * 180.0f;
   tracker->sensor[nextSensor].azim = sin(t/2.0f+nextSensor) * 120.0f;
   tracker->sensor[nextSensor].roll = sin(t/7.0f+nextSensor) * 90.0f;
   tracker->sensor[nextSensor].calibrated = 0;
 }
 nextSensor = ((nextSensor+1) % numSensors);
}


int get_new_controller_data(int numButtons,int numValuators)
{
 int i, buttonChange=0, t=time(NULL);
 static int lastButtonChangeTime=-1;
 //controller->num_buttons = numButtons;
 //controller->num_valuators = numValuators;
 if ((numButtons > 0) && (t > lastButtonChangeTime))
 {
   int b = random()%numButtons;
   controller->button[b] = !(controller->button[b]);
   lastButtonChangeTime = t;
   buttonChange=1;
 }

 for (i=0; i < numValuators; i++)
 {
   controller->valuator[i] += (drand48()-0.5f)/20.0f;
   if (controller->valuator[i] < -1.0f)
      controller->valuator[i] = -1.0f;
   else if (controller->valuator[i] > 1.0f)
      controller->valuator[i] = 1.0f;
 }

 return ((numValuators > 0) || (buttonChange));
}



int main(int argc,char **argv)
{
   int numSensors=0, numButtons=0, numValuators=0;
   int trackerKey=0, controllerKey=0;

   int i;      // sensornum;
   //CAVE_CONFIG_ST config;
   //CAVE_SENSOR_ST sensor;
   //CAVE_CONTROLLER_ST controlvals;

   if (argc < 5)
   {
      fprintf(stderr,"Usage: %s [-sensors <numsensors>] [-buttons <numbuttons>]"
              " [-valuators <numvaluators>] [-trackerkey <keynum>] [-controllerkey <keynum>]\n", argv[0]);
      exit(1);
   }

   // Get command line
   for (i=1; i < argc; i++)
   {
      if (!strcmp(argv[i],"-sensors"))
         numSensors = atoi(argv[++i]);
      else if (!strcmp(argv[i],"-buttons"))
         numButtons = atoi(argv[++i]);
      else if (!strcmp(argv[i],"-valuators"))
         numValuators = atoi(argv[++i]);
      else if (!strcmp(argv[i],"-trackerkey"))
         trackerKey = atoi(argv[++i]);
      else if (!strcmp(argv[i],"-controllerkey"))
         controllerKey = atoi(argv[++i]);
      else
         printf("Unknown option \"%s\"\n",argv[i]);
   }

   allocateTrackerMem(trackerKey);
   allocateControllerMem(controllerKey);

   initTracker();
   initController();

   tracker->header.numSensors = numSensors;
   controller->header.numButtons = numButtons;
   //controller->controller.num_buttons = numButtons;
   controller->header.numValuators = numValuators;
   //controller->controller.num_valuators = numValuators;

   while (1)
   {
      setNewTrackerData(numSensors);
      getTimeStaMp(tracker->header.timestamp);

      if (get_new_controller_data(numButtons,numValuators))
      {
         getTimeStaMp(controller->header.timestamp);
      }
      usleep(100);
   }

   return 1;
}
