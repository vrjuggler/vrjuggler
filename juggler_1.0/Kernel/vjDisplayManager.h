#ifndef _VJ_DISPLAY_MANAGER_
#define _VJ_DISPLAY_MANAGER_
#pragma once

#include <config.h>

class vjDrawManager;
class vjDisplay;

#ifdef VJ_OS_HPUX
#   include <vector>
#else
    // This needs to be done for vector.h if pthread.h has not already been
    // included.
#   if defined(VJ_USE_PTHREADS) && ! defined(_PTHREADS)
#       define _PTHREADS
#   endif

#   include <vector.h>
#endif	/* VJ_OS_HPUX */

#include <Input/vjPosition/vjPosition.h>
#include <Kernel/vjDebug.h>
#include <Input/InputManager/vjPosInterface.h>

//-----------------------------------------------------------------------------------
//: Singleton Container class for all vjDisplays.
// 
// PURPOSE:
//	This class is responsible for holding the data about display aspects of
//  the application.  The displays themselves may be machine/OS specific,  but this
//  class should be able to run for all configurations.  This includes all machine/OS 
//  specific aspects.  For now that basically means simply tracking window data. 
//
// @author Allen Bierbaum
//  Date: 9-7-97
//------------------------------------------------------------------------------------
class vjDisplayManager
{
public:
   // 
   virtual int addDisplay(vjDisplay* disp, int notifyDrawMgr = 0);

   virtual int closeDisplay(int dispId);

   virtual vjDisplay* getDisplay(int dispId);

   //: This function updates the projections for all contained displays.
   virtual void updateProjections();

   void setDrawManager(vjDrawManager* drawMgr);

   //: Setup the head index data
   //!NOTE: should be called by Kernel when head should be setup
   void setupHeadIndices();
   
public:
   vector<vjDisplay*>  displays;    //: List of displays currently operating    

protected:
   vjDrawManager*  drawManager;     //: The current drawManager to communicate with

   // --- Information about the head --- //
   vjPosInterface    mHeadInterface;   //: The proxy index for the head
   vjMatrix          curHeadPos;       //: Keep cached pos here


   // ---- Singleton stuff ---- //
public:
   static vjDisplayManager* instance()
   {
      if (_instance == NULL)
         _instance = new vjDisplayManager();
      return _instance;
   }

protected:
   vjDisplayManager()
   {;}

private:
   static vjDisplayManager* _instance;
};

#endif
