// Copied from public information at: http://www.evl.uic.edu/pape/CAVE/prog/
//
// Thanks Dave.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "trackdmem.h"

typedef struct
	{
	uint32_t version;		/* CAVElib version (see constants below) */
	uint32_t numSensors;	/* Total number of sensors */
	uint32_t sensorOffset;	/* Byte offset from of header to start of sensor array */
	uint32_t sensorSize;	/* sizeof() of a sensor struct */
	uint32_t timestamp[2];	/* NB: *Not* a struct timeval - that changes */
					/*	size between 32 & 64 bit mode */
	uint32_t command;		/* For sending commands (such as 'reset') to daemon */
	} CAVE_TRACKDTRACKER_HEADER;


/* Command values for header.command */
/*   header.command should be 0 when no command is pending */
#define CAVE_TRACKD_NO_COMMAND		0
#define CAVE_TRACKD_RESET_COMMAND		1


typedef struct
	{
	uint32_t version;		/* CAVElib version (see constants below) */
	uint32_t buttonOffset;	/* Byte offset from start of header to start of button data */
	uint32_t valuatorOffset; /* Byte offset from start of header to start of valuator data */
	uint32_t numButtons;
	uint32_t numValuators;
	uint32_t timestamp[2];
	uint32_t command;		/* For sending commands (such as 'reset') to daemon */
	} CAVE_TRACKDCONTROLLER_HEADER;


/* ID Numbers for CAVElib version */
/*  Increasing values should be assigned for newer library versions whenever
    either the header, sensor, or controller struct definition is expanded */

#define CAVELIB_2_6	1




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
	shmdt(mem);
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
	shmdt(mem);
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
