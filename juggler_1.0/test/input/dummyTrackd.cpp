#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <limits.h>
#include <sys/prctl.h>
#include <sys/schedctl.h>
#include "trackd.h"

#define PERMS 0666

static int tracker_shmid=-1, controller_shmid=-1;
static struct TRACKD_TRACKING	*tracker=NULL;
static struct TRACKD_CONTROLLER	*controller=NULL;

		
// Initialize tracker
void initTracker()
{
 TrackdSensor sensor;

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
 controller->header.buttonOffset = ((char *)&controller->controller.button[0]) -
					((char *)controller);
 controller->header.valuatorOffset = ((char *)&controller->controller.valuator[0]) -
					((char *)controller);
 controller->header.numButtons = 0;
 controller->header.numValuators = 0;
 controller->header.timestamp[0] = controller->header.timestamp[1] = 0;
 controller->header.command = 0;

 int loop;
 for (loop=0; loop < CAVE_MAX_BUTTONS; ++loop) 
		controller->controller.button[loop] = 0;
 for (loop=0; loop < CAVE_MAX_VALUATORS; ++loop) 
		controller->controller.valuator[loop] = 0.0;
 controller->controller.num_buttons = 0;
 controller->controller.num_valuators = 0;
}


void get_timestamp(uint32_t stamp[2])
{
 struct timeval curtime;
 gettimeofday(&curtime,NULL);
 stamp[0] = curtime.tv_sec;
 stamp[1] = curtime.tv_usec;
}


int get_new_tracker_data(int numSensors,CAVE_SENSOR_ST *sensor,int *sensorNum)
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
 if (nextSensor==0)	/* Head */
	{
	sensor->x = fsin(t/2.0f) * 2.0f;
	sensor->y = 6;
	sensor->z = fcos(t) * 3.0f;
	sensor->elev = fsin(t/10.0f) * 45.0f;
	sensor->azim = fsin(t/3.0f) * 120.0f;
	sensor->roll = 0;
	sensor->calibrated = 0;
	}
 else
	{
	sensor->x = fsin(t+nextSensor) + nextSensor*2.0f - 2.0f;
	sensor->y = fcos(t+nextSensor) + 4.0f;
	sensor->z = -4.0f;
	sensor->elev = fsin(t/4.0f+nextSensor) * 180.0f;
	sensor->azim = fsin(t/2.0f+nextSensor) * 120.0f;
	sensor->roll = fsin(t/7.0f+nextSensor) * 90.0f;
	sensor->calibrated = 0;
	}
 *sensorNum = nextSensor;
 nextSensor = (nextSensor+1) % numSensors;
 return 1;
}


int get_new_controller_data(int numButtons,int numValuators,CAVE_CONTROLLER_ST *controller)
{
 int i, buttonChange=0, t=time(NULL);
 static int lastButtonChangeTime=-1;
 controller->num_buttons = numButtons;
 controller->num_valuators = numValuators;
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



main(int argc,char **argv)
{
 int numSensors=0, numButtons=0, numValuators=0;
 int i, sensornum;
 CAVE_CONFIG_ST config;
 CAVE_SENSOR_ST sensor;
 CAVE_CONTROLLER_ST controlvals;

 if (argc < 3)
	{
	fprintf(stderr,"Usage: %s [-sensors <numsensors>] [-buttons <numbuttons>]"
		" [-valuators <numvaluators>]\n", argv[0]);
	exit(1);
	}


 if (!getenv("CAVEDEBUGCONFIG"))
        putenv("CAVEDEBUGCONFIG=OFF");
 CAVEConfigurationInit(&argc,argv,NULL,&config);
	
 get_tracker_mem(&config);
 get_controller_mem(&config);

 init_tracker_data(&config);
 init_controller_data(&config);

 for (i=1; i < argc; i++)
	{
	if (!strcmp(argv[i],"-sensors"))
		numSensors = atoi(argv[++i]);
	else if (!strcmp(argv[i],"-buttons"))
		numButtons = atoi(argv[++i]);
	else if (!strcmp(argv[i],"-valuators"))
		numValuators = atoi(argv[++i]);
	else
		printf("Unknown option \"%s\"\n",argv[i]);
	}
 tracker->header.numSensors = numSensors;
 controller->header.numButtons = numButtons;
 controller->controller.num_buttons = numButtons;
 controller->header.numValuators = numValuators;
 controller->controller.num_valuators = numValuators;

 while (1)
	{
	if (get_new_tracker_data(numSensors,&sensor,&sensornum))
		{
		tracker->sensor[sensornum] = sensor;
		get_timestamp(tracker->header.timestamp);
		}
	if (get_new_controller_data(numButtons,numValuators,&controlvals))
		{
		controller->controller = controlvals;
		get_timestamp(controller->header.timestamp);
		}
	sginap(1);
	}
}
