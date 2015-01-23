// Copied from public information at: http://www.evl.uic.edu/pape/CAVE/prog/
//
// Thanks Dave.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <drivers/Open/Trackd/trackdmem.h>

#if !defined(VPR_OS_Windows)
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

// ===========================================================================
// trackd tracker API

struct TrackerConnection
{
#if defined(VPR_OS_Windows)
   HANDLE                     file_mapping;
   CAVE_TRACKDTRACKER_HEADER* tracker;
#else
   int
   CAVE_TRACKDTRACKER_HEADER* tracker;
#endif
};

TrackerConnection*
trackd_tracker_attach(int shmKey)
{
   TrackerConnection* t = new TrackerConnection();

#if defined(VPR_OS_Windows)
   char shmfile[256];
   sprintf(shmfile, "%d", shmKey);
   t->file_mapping = OpenFileMapping(FILE_MAP_READ, FALSE, shmfile);

   if(!t->file_mapping)
   {
      fprintf(stderr, "ERROR: cannot get tracker shared memory\n");
      perror("OpenFileMapping");
      return NULL;
   }

   t->tracker =
      reinterpret_cast<CAVE_TRACKDTRACKER_HEADER*>(
         MapViewOfFile(t->file_mapping, FILE_MAP_READ, 0, 0, 0));

   if(!t->tracker)
   {
      fprintf(stderr, "ERROR: cannot attach tracker shared memory\n");
      perror("MapViewOfFile");
      return NULL;
   }
#else
   t->shmid = shmget(shmKey, sizeof(CAVE_TRACKDTRACKER_HEADER), 0);

   if(t->shmid < 0)
   {
      fprintf(stderr,"ERROR: can't get tracker shared memory\n");
      perror("shmget");
      return NULL;
   }

   t->tracker =
      reinterpret_cast<CAVE_TRACKDTRACKER_HEADER*>(
         shmat(t->shmid, (void *) 0, 0));

   if(t->tracker == reinterpret_cast<CAVE_TRACKDTRACKER_HEADER*>(-1))
   {
      fprintf(stderr,"ERROR: can't attach tracker shared memory\n");
      perror("shmat");
      return NULL;
   }
#endif

   return t;
}

void
trackd_tracker_release(TrackerConnection* t)
{
#if defined(VPR_OS_Windows)
   if(t && t->tracker)
   {
      UnmapViewOfFile(t->tracker);
      CloseHandle(t->file_mapping);
   }
#else
   if(t && t->tracker)
   {
#  ifdef __sun__
#     if defined(_XOPEN_SOURCE) && (_XOPEN_VERSION - 0 >= 4)
      shmdt(t->tracker);
#     else
      shmdt((char*) t->tracker);
#     endif
#  else
      shmdt(t->tracker);
#  endif
   }
#endif

   delete t;
}

int
trackd_tracker_num_sensors(TrackerConnection* t)
{
   return t->tracker->numSensors;
}

CAVE_SENSOR_ST*
trackd_tracker_sensor(TrackerConnection* t, int sensorNum)
{
   unsigned char* pointer =
      reinterpret_cast<unsigned char*>(t->tracker);

   pointer += t->tracker->sensorOffset + sensorNum * t->tracker->sensorSize;

   return reinterpret_cast<CAVE_SENSOR_ST*>(pointer);
}

// ===========================================================================
// trackd controller API

struct ControllerConnection
{
#if defined(VPR_OS_Windows)
   HANDLE                        file_mapping;
   CAVE_TRACKDCONTROLLER_HEADER* controller;
#else
   int                           shmid;
   CAVE_TRACKDCONTROLLER_HEADER* controller;
#endif
};

ControllerConnection*
trackd_controller_attach(int shmKey)
{
   ControllerConnection* c = new ControllerConnection();

#if defined(VPR_OS_Windows)
   char shmfile[256];
   sprintf(shmfile, "%d", shmKey);
   c->file_mapping = OpenFileMapping(FILE_MAP_READ, FALSE, shmfile);

   if(!c->file_mapping)
   {
      fprintf(stderr, "ERROR: cannot get controller shared memory\n");
      perror("OpenFileMapping");
      return NULL;
   }

   c->controller =
      reinterpret_cast<CAVE_TRACKDCONTROLLER_HEADER*>(
         MapViewOfFile(c->file_mapping, FILE_MAP_READ, 0, 0, 0));

   if(!c->controller)
   {
      fprintf(stderr, "ERROR: cannot attach controller shared memory\n");
      perror("MapViewOfFile");
      return NULL;
   }
#else
   c->shmid = shmget(shmKey, sizeof(CAVE_TRACKDCONTROLLER_HEADER), 0);

   if(c->shmid < 0)
   {
      fprintf(stderr,"ERROR: can't get controller shared memory\n");
      perror("shmget");
      return NULL;
   }

   c->controller =
      reinterpret_cast<CAVE_TRACKDCONTROLLER_HEADER*>(
         shmat(controller_shmid, (void *) 0, 0));

   if(c->controller == (CAVE_TRACKDCONTROLLER_HEADER *) -1)
   {
      fprintf(stderr,"ERROR: can't attach controller shared memory\n");
      perror("shmat");
      return NULL;
   }
#endif

   return c;
}

void
trackd_controller_release(ControllerConnection* c)
{
#if defined(VPR_OS_Windows)
   if(c && c->controller)
   {
      UnmapViewOfFile(c->controller);
      CloseHandle(c->file_mapping);
   }
#else
   if(c && c->controller)
   {
#  ifdef __sun__
#     if defined(_XOPEN_SOURCE) && (_XOPEN_VERSION - 0 >= 4)
      shmdt(c->controller);
#     else
      shmdt((char*) c->controller);
#     endif
#  else
      shmdt(c->controller);
#  endif
   }
#endif

   delete c;
}

int
trackd_controller_num_buttons(ControllerConnection* c)
{
   return c->controller->numButtons;
}

int
trackd_controller_num_valuators(ControllerConnection* c)
{
   return c->controller->numValuators;
}

int
trackd_controller_button(ControllerConnection* c, int buttonNum)
{
   unsigned char* pointer =
      reinterpret_cast<unsigned char*>(c->controller)
      + c->controller->buttonOffset;
   int* buttons = reinterpret_cast<int*>(pointer);

   return buttons[buttonNum];
}

float
trackd_controller_valuator(ControllerConnection* c, int valuatorNum)
{
   unsigned char* pointer =
      reinterpret_cast<unsigned char*>(c->controller)
      + c->controller->valuatorOffset;
   float* valuators = reinterpret_cast<float*>(pointer);

   return valuators[valuatorNum];
}
