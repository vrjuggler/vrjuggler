/***********************************************************************
*
*    File Name:      timer.c
*    Description:    timer interface program for UNIX and Win32
*    Created:        9/15/97
*    Author:         Yury Altshuler
*
*    Copyright:      InterSense 1999 - All rights Reserved.
*
************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vpr/vpr.h>
#include <vpr/Util/Interval.h>

#include "isense.h"
#include "timer.h"

#ifdef VPR_OS_Win32
#include <sys\timeb.h>
#else
#include <sys/timeb.h>
#endif

/* Old InterSense Code
static struct timeb t;
static unsigned long initialTime;
*/
static vpr::Interval initialTime;


/************************************************************************/
void initTimer()
{
   /* Old InterSense Code
   ftime(&t);
   initialTime = t.time;
   */
   initialTime.setNow();
}


/************************************************************************/
/* Returns current time in seconds */
float timeNow()
{
   /* Old InterSense Code
   ftime(&t);
   return ((float)(t.time - initialTime) + (float)t.millitm/1000.0f);
   */
   vpr::Interval currentTime;
   currentTime.setNow();
   return((float)(currentTime.getBaseVal()-initialTime.getBaseVal())/1000000);
}


/************************************************************************/
void tdelay(float tsecs)
{
    float tstart;

    if(tsecs==0.0) return;
    tstart = timeNow();
    while(timeNow() - tstart < tsecs);
}

