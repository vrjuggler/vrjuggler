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
#include <sys/time.h>
#else
#include <sys\timeb.h>
#endif

#ifdef UNIX
static struct timeval t;
#else
static struct timeb t;
#endif
static unsigned long initialTime;


/************************************************************************/
void initTimer()
{
#ifdef UNIX
    gettimeofday(&t, NULL);
    initialTime = t.tv_sec;
#else
    ftime(&t);
    initialTime = t.time;
#endif
}


/************************************************************************/
/* Returns current time in seconds */
float timeNow()
{
#ifdef UNIX
    gettimeofday(&t, NULL);
    return ((float)(t.tv_sec - initialTime) + (float)t.tv_usec);
#else
    ftime(&t);
    return ((float)(t.time - initialTime) + (float)t.millitm/1000.0f);
#endif
}


/************************************************************************/
void tdelay(float tsecs)
{
    float tstart;

    if(tsecs==0.0) return;
    tstart = timeNow();
    while(timeNow() - tstart < tsecs);
}
