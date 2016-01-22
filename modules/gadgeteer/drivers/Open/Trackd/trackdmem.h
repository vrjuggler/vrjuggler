// Copied from public information at: http://www.evl.uic.edu/pape/CAVE/prog/
//
// Thanks Dave.
//
#ifndef _TRACKD_MEM_H_
#define _TRACKD_MEM_H_

#include <sys/types.h>
#include <vpr/vpr.h>

#if defined(VPR_OS_Linux)
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

struct CAVE_TRACKDTRACKER_HEADER
{
   uint32_t version;       /* CAVElib version (see constants below) */
   uint32_t numSensors;    /* Total number of sensors */
   uint32_t sensorOffset;  /* Byte offset from of header to start of sensor array */
   uint32_t sensorSize;    /* sizeof() of a sensor struct */
   uint32_t timestamp[2];  /* NB: *Not* a struct timeval - that changes */
                           /* size between 32 & 64 bit mode */
   uint32_t command;       /* For sending commands (such as 'reset') to daemon */
} ;

struct CAVE_TRACKDCONTROLLER_HEADER
{
   uint32_t version;          /* CAVElib version (see constants below) */
   uint32_t buttonOffset;     /* Byte offset from start of header to start of button data */
   uint32_t valuatorOffset;   /* Byte offset from start of header to start of valuator data */
   uint32_t numButtons;
   uint32_t numValuators;
   uint32_t timestamp[2];
   uint32_t command;          /* For sending commands (such as 'reset') to daemon */
};

/* Command values for header.command */
/*   header.command should be 0 when no command is pending */
#define CAVE_TRACKD_NO_COMMAND      0
#define CAVE_TRACKD_RESET_COMMAND      1

/* ID Numbers for CAVElib version */
/*  Increasing values should be assigned for newer library versions whenever
    either the header, sensor, or controller struct definition is expanded */
#define CAVELIB_2_6  1

struct TrackerConnection;
struct ControllerConnection;

TrackerConnection*
trackd_tracker_attach(int shmKey);
void
trackd_tracker_release(TrackerConnection* t);
int
trackd_tracker_num_sensors(TrackerConnection* t);
CAVE_SENSOR_ST*
trackd_tracker_sensor(TrackerConnection* t, int sensorNum);

ControllerConnection*
trackd_controller_attach(int shmKey);
void
trackd_controller_release(ControllerConnection* c);
int
trackd_controller_num_buttons(ControllerConnection* c);
int
trackd_controller_num_valuators(ControllerConnection* c);
int
trackd_controller_button(ControllerConnection* c, int buttonNum);
float
trackd_controller_valuator(ControllerConnection* c, int valuatorNum);

#endif
