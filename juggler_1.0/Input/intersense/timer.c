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

#include "isense.h"
#include "timer.h"

#if defined UNIX 
#include <sys/timeb.h>
#else
#include <sys\timeb.h>
#endif

static struct timeb t;
static unsigned long initialTime;


/************************************************************************/
void initTimer()
{
    ftime(&t);
    initialTime = t.time;
}


/************************************************************************/
/* Returns current time in seconds */
float timeNow()
{
    ftime(&t);
    return ((float)(t.time - initialTime) + (float)t.millitm/1000.0f);
}


/************************************************************************/
void tdelay(float tsecs)
{
    float tstart;

    if(tsecs==0.0) return;
    tstart = timeNow();
    while(timeNow() - tstart < tsecs);
}

