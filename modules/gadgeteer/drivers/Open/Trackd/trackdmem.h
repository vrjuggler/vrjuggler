// Copied from public information at: http://www.evl.uic.edu/pape/CAVE/prog/
//
// Thanks Dave.
//
#ifndef _TRACKD_MEM_H_
#define _TRACKD_MEM_H_

#include <vjDefines.h>
#include <sys/types.h>

#if defined(VJ_OS_Linux)
#   include <stdint.h>
#endif

struct CAVE_SENSOR_ST
   {
   float       x, y, z;
   float       azim, elev, roll;
   uint32_t    timestamp[2];
   int32_t     calibrated;
   int32_t     frame;
   };


void * trackd_attach_tracker_mem(int shmKey);
void trackd_release_tracker_mem(void *mem);
int trackd_num_sensors(void *mem);
CAVE_SENSOR_ST * trackd_sensor(void *mem,int sensorNum);
void * trackd_attach_controller_mem(int shmKey);
void trackd_release_controller_mem(void *mem);
int trackd_num_buttons(void *mem);
int trackd_num_valuators(void *mem);
int trackd_button(void *mem,int buttonNum);
float trackd_valuator(void *mem,int valuatorNum);

#endif
