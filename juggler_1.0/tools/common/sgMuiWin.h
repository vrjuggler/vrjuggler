#ifndef _SG_MUI_WIN_H_
#define _SG_MUI_WIN_H_

#include <iostream.h>
#include <stdio.h>
#include <GL/glut.h>

#include <mui/mui.h>

/**
 * sgMuiWin: This is a base class for general mui windows.
 *
 * NOTE: For now this is a singleton.  Only one is allowed to be created at a time.
 */

class sgMuiWin
{
protected:
	sgMuiWin()
	{
		winHandle = -1;
	}

public:
	void setWindow(int handle)
	{
		winHandle = handle;
	}

		// Creates a new Glut window and assigns it to us.
	int createWindow(int xo = 0, int yo = 0, int xs = 400, int ys = 400, char* name = NULL);

public:
		// The interface should be created in this function.
		// It is suggested, that all mui objects should have their id set
		// to allow for easier interaction.
	virtual void constructUI()
	{;}

		// This function is called when there is MUI interaction
		// It is suggested that this is simply a switch on mui id values
	virtual void interact(muiObject* obj, enum muiReturnValue val)
	{;}

		// Helper function for lazy people. (like me)
		// Sets the callback function for the given object to call the class member "interact".
	void setMuiCallback(muiObject* obj)
		{ muiSetCallback(obj, sgMuiWin::muiCallback); }

		// This is the function that is called by mui as a callback
		// It should then call the interact function of the singleton object
	static
	void muiCallback(muiObject* obj, enum muiReturnValue val)
	{
		getInstance()->interact(obj, val);
	}

public:
	int		winHandle;		// The glut window.  If -1, then has not been set.
	int		width, height;

	// ---------- SINGLETON -------- //
public:
		// NOTE: When deriving subclass, override this function to create a
		//        New get instance.
		// NOTE2: This one will be that one that muiCallback uses, but that is fine
		//        since it will never be called with a NULL _instance
	static sgMuiWin* getInstance();

protected:
	static sgMuiWin* _instance;
};



#endif
