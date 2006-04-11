// Copied from public information at: http://www.evl.uic.edu/pape/CAVE/prog/
//
// Thanks Dave.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <drivers/Open/Trackd/trackdmem.h>


/*************************************************************************
 void * trackd_attach_tracker_mem(int)
   Attaches to shared memory for the sensors.  The argument is the key
  # for the shared memory segment.  The returned value points to the chunk.
**************************************************************************/
void * trackd_attach_tracker_mem(int shmKey)
   {
   CAVE_TRACKDTRACKER_HEADER *tracker;
   int tracker_shmid = shmget(shmKey,sizeof(CAVE_TRACKDTRACKER_HEADER),0);
   if (tracker_shmid < 0)
      {
      fprintf(stderr,"ERROR: can't get tracker shared memory\n");
      perror("shmget");
      return NULL;
      }
   tracker = (CAVE_TRACKDTRACKER_HEADER *) shmat(tracker_shmid,
                     (void *) 0, 0);
   if (tracker == (CAVE_TRACKDTRACKER_HEADER *) -1)
      {
      fprintf(stderr,"ERROR: can't attach tracker shared memory\n");
      perror("shmat");
      return NULL;
      }
   return tracker;
   }


void trackd_release_tracker_mem(void *mem)
   {
#ifdef __sun__
#   if defined(_XOPEN_SOURCE) && (_XOPEN_VERSION - 0 >= 4)
   shmdt(mem);
#   else
   shmdt((char*) mem);
#   endif
#else
   shmdt(mem);
#endif
   }


int trackd_num_sensors(void *mem)
   {
   CAVE_TRACKDTRACKER_HEADER *tracker=(CAVE_TRACKDTRACKER_HEADER*)mem;
   return tracker->numSensors;
   }


CAVE_SENSOR_ST * trackd_sensor(void *mem,int sensorNum)
   {
   CAVE_TRACKDTRACKER_HEADER *tracker=(CAVE_TRACKDTRACKER_HEADER*)mem;
   unsigned char *pointer = (unsigned char *)mem;
   pointer += tracker->sensorOffset + sensorNum * tracker->sensorSize;
   return (CAVE_SENSOR_ST *) pointer;
   }


/*************************************************************************
 void * trackd_attach_controller_mem(int)
   Attaches to shared memory for the buttons/joystick.  The argument
  is the key # for the shared memory segment.  The returned value points
  to the chunk.
**************************************************************************/
void * trackd_attach_controller_mem(int shmKey)
   {
   CAVE_TRACKDCONTROLLER_HEADER *controller;
   int controller_shmid = shmget(shmKey,
            sizeof(CAVE_TRACKDCONTROLLER_HEADER), 0);
   if (controller_shmid < 0)
      {
      fprintf(stderr,"ERROR: can't get controller shared memory\n");
      perror("shmget");
      return NULL;
      }
   controller = (CAVE_TRACKDCONTROLLER_HEADER *)
            shmat(controller_shmid,(void *) 0, 0);
   if (controller == (CAVE_TRACKDCONTROLLER_HEADER *) -1)
      {
      fprintf(stderr,"ERROR: can't attach controller shared memory\n");
      perror("shmat");
      return NULL;
      }
   return controller;
   }


void trackd_release_controller_mem(void *mem)
   {
#ifdef __sun__
#   if defined(_XOPEN_SOURCE) && (_XOPEN_VERSION - 0 >= 4)
   shmdt(mem);
#   else
   shmdt((char*) mem);
#   endif
#else
   shmdt(mem);
#endif
   }


int trackd_num_buttons(void *mem)
   {
   CAVE_TRACKDCONTROLLER_HEADER *controller =
            (CAVE_TRACKDCONTROLLER_HEADER*)mem;
   return controller->numButtons;
   }


int trackd_num_valuators(void *mem)
   {
   CAVE_TRACKDCONTROLLER_HEADER *controller =
            (CAVE_TRACKDCONTROLLER_HEADER*)mem;
   return controller->numValuators;
   }


int trackd_button(void *mem,int buttonNum)
   {
   CAVE_TRACKDCONTROLLER_HEADER *controller =
            (CAVE_TRACKDCONTROLLER_HEADER*)mem;
   unsigned char *pointer = (unsigned char *) controller;
   int *buttons = (int *) (pointer + controller->buttonOffset);
   return buttons[buttonNum];
   }


float trackd_valuator(void *mem,int valuatorNum)
   {
   CAVE_TRACKDCONTROLLER_HEADER *controller =
            (CAVE_TRACKDCONTROLLER_HEADER*)mem;
   unsigned char *pointer = (unsigned char *) controller;
   float *valuators = (float *) (pointer + controller->valuatorOffset);
   return valuators[valuatorNum];
   }
