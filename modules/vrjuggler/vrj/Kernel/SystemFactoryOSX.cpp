/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vjConfig.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <vrj/Kernel/OSXSystemFactory.h>
#include <vpr/Thread/Thread.h>
#include <vpr/System.h>
#include <vrj/Util/Debug.h>
#include <CoreFoundation/CFString.h>

namespace vrj
{
   
vprSingletonImp(OSXSystemFactory);

// This function comes from Carbon SetupGL 1.5 distributed by Apple
// Corporation.  Its use is here is permitted by the license.
static Boolean PreflightGL (Boolean checkFullscreen)
{
   // ensure Mac OS X flag is set
   if ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) aglChoosePixelFormat) // check for existance of OpenGL
      return false;

   // if on Mac OS X then this check is not required
   if (checkFullscreen &&
       ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) DSpStartup)) // check for existance of DSp
      return false;
   return true;
}

void OSXSystemFactory::CarbonApplicationThread(void* nullData)
{
    Initialize();
    PreflightGL (false);

    InitComplete = true;
    EventLoop(); // This is where we will put the main loop to retrieve events from carbon
}

OSXSystemFactory::OSXSystemFactory()
{
    InitComplete = false; //We haven't initialized yet!

    vpr::ThreadMemberFunctor<OSXSystemFactory>* memberFunctor = new vpr::ThreadMemberFunctor<OSXSystemFactory>(this, &OSXSystemFactory::CarbonApplicationThread, NULL);
    vpr::Thread* new_thread;
    new_thread = new vpr::Thread(memberFunctor);

    while(!InitComplete); //Spin here until the application is initialized!
}

/*******************************************************************/
//
// Menubar Definitions
//
/*******************************************************************/
#define	rMenuBar	128	/* menu bar */

#define	mApple		128	/* Apple menu */
#define	iAbout		1

#define	mFile		129	/* File menu */
#define	iNew		1
#define	iClose		4
#define	iQuitSeparator	10
#define	iQuit		11

#define	mEdit		130	/* Edit menu */

#define kAboutBox	200	/* Dialog resource for About box */

/*******************************************************************/
//
// Carbon Mac OS X Event handling
//
/*******************************************************************/

static OSErr QuitAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon )
{
    OSXSystemFactory::instance()->setQuitFlag(true);
    return noErr;
}

void OSXSystemFactory::Initialize()
{
    OSErr	err;


    InitCursor();

    err = AEInstallEventHandler( kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP((AEEventHandlerProcPtr)QuitAppleEventHandler), 0, false );
    if (err != noErr)
        ExitToShell();

    /*/ Doesn't work right quite yet...
    char	bundle_path[1024];
    char	root_path[] = "\0";
    CFStringRef bundle_path_cfstr;
    CFURLRef bundleURL;
    CFBundleRef myBundle;
    IBNibRef mainmenu;

    char* vj_base_dir = getenv("VJ_BASE_DIR");
    if(vj_base_dir == NULL) vj_base_dir = root_path;

    sprintf(bundle_path,"%s/share/Data/OSXData.bundle",vj_base_dir);
    bundle_path_cfstr = CFStringCreateWithCString(NULL, (const char*)bundle_path, kCFStringEncodingMacRoman);
    bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, bundle_path_cfstr, kCFURLPOSIXPathStyle, true);
    myBundle = CFBundleCreate(kCFAllocatorDefault, bundleURL);

    if( CreateNibReferenceWithCFBundle(myBundle, CFSTR("mainmenu"), &mainmenu) != 0 )
        vjDEBUG(vjDBG_INPUT_MGR,0) << "vjOSXSystemFactory::Initialize():  Error finding mainmenu in the bundle!" << std::endl << vjDEBUG_FLUSH;
    else if( SetMenuBarFromNib(mainmenu, CFSTR("MainMenu") ) != 0 )
        vjDEBUG(vjDBG_INPUT_MGR,0) << "vjOSXSystemFactory::Initialize():  Error setting the menubar!" << std::endl << vjDEBUG_FLUSH;;

    CFRelease(bundleURL);
    CFRelease(bundle_path_cfstr); */

    Handle	menuBar;
    MenuRef	menu;
    long	response;

    menuBar = GetNewMBar(rMenuBar);	/* read menus into menu bar */
    if ( menuBar != nil )
    {
        SetMenuBar(menuBar);	/* install menus */

        err = Gestalt(gestaltMenuMgrAttr, &response);
	if ((err == noErr) && (response & gestaltMenuMgrAquaLayoutMask))
        {
            menu = GetMenuHandle( mFile );
            DeleteMenuItem( menu, iQuit );
            DeleteMenuItem( menu, iQuitSeparator );
        }
    }

    // This has to be done no matter what!
    DrawMenuBar();
}

void OSXSystemFactory::EventLoop()
{
    Boolean	gotEvent;
    EventRecord	event;

    gQuitFlag = false;
    //vjDEBUG(vjDBG_INPUT_MGR,0) << "vjOSXSystemFactory::EventLoop()" << std::endl << vjDEBUG_FLUSH;
    do
    {
        gotEvent = WaitNextEvent(everyEvent,&event,36000,nil);
        if (gotEvent)
            DoEvent(&event);
    } while (!gQuitFlag);


    // This should kill the application completely.
    // The nice thing about this is that when you put the application into a bundle
    // Cmd + Q works for killing the application.
    ExitToShell();					
}

void OSXSystemFactory::DoEvent(EventRecord *event)
{
    char	key;
    short	part;
    WindowRef	whichWindow;

    switch (event->what)
    {
        case mouseDown:
            part = FindWindow(event->where, &whichWindow);
            switch (part)
            {
                case inMenuBar:  /* process a moused menu command */
                    DoMenuCommand(MenuSelect(event->where));
                    break;
                default: break;
            }
            break;
        case keyDown:
        case autoKey:
            key = event->message & charCodeMask;
            if (event->modifiers & cmdKey)
                if (event->what == keyDown)
                    DoMenuCommand(MenuKey(key));
            break;
        case kHighLevelEvent:
                AEProcessAppleEvent( event );
                break;
    }
}

void OSXSystemFactory::DoMenuCommand(long menuResult)
{
    short	menuID;		/* the resource ID of the selected menu */
    short	menuItem;	/* the item number of the selected menu */
	
    menuID = HiWord(menuResult);    /* use macros to get item & menu number */
    menuItem = LoWord(menuResult);

    switch (menuID)
    {
        case mApple:
            switch (menuItem)
            {
                case iAbout:
                    vjDEBUG(vjDBG_INPUT_MGR,0) << "vjOSXSystemFactory::DoMenuCommand()" << "   AboutBox Menu selected" << std::endl << vjDEBUG_FLUSH;
                    break;

                case iQuit:
                    ExitToShell();
                    break;
				
                default:
                    break;
            }
            break;

        case mFile:
            break;
		
        case mEdit:
            break;
    }

    HiliteMenu(0);	/* unhighlight what MenuSelect (or MenuKey) hilited */
}

};
