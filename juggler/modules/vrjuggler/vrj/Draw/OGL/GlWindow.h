#ifndef _VJ_GL_WINDOW_H_
#define _VJ_GL_WINDOW_H_

/* vjGlWindow.h
 *
 */

#include <config.h>
#include <stdio.h>
#include <GL/gl.h>

#include <Kernel/vjDisplay.h>

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

//-------------------------------------------------------
//: Represent cross-platform interface to OpenGL windows
//-------------------------------------------------------   
// This interface is used by the OpenGL draw manager
// in order to keep all platform specific code in this
// one class.
//-------------------------------------------------------
class vjGlWindow
{
public:
   vjGlWindow()
   {
      mWindowId = getNextWindowId();
      in_stereo = false;
      window_is_open = false;
   }

public:

   //: Open the OpenGL window
   //! PRE: this has been configured
   virtual int open(){ return 1;}

   //: Close the OpenGL window
   virtual int close(){return 1;}

   //: Sets the current OpenGL context to this window
   //! POST: this.context is active context
   virtual bool makeCurrent(){return false;}

   //: Sets the projection matrix for this window to draw the left eye frame
   void setLeftEye();

   //: Sets the projection matrix for this window to draw the right eye frame
   void setRightEye();

   //: Sets the projection matrix for this window to the one for simulator
   void setCameraEye();

   //: Configure the window
   //! POST: this' is configured based on the data in display
   virtual void config(vjDisplay* _display)
   {
      display = _display;
      try_stereo = false;
      /* ^--------- shouldn't there be something in vjDisplay about this? */
      display->originAndSize( origin_x, origin_y, window_width, window_height);
      border = display->drawBorder();

      /// Other stuff
   }

   //: Performs an OpenGL swap buffers command
   virtual void swapBuffers(){;}

   //: Query wether the window is open
   //! RETURNS: true - If window is open
   bool isOpen() {
      return window_is_open;
   }

   //: Query wether the window is in stereo
   //! RETURNS: true - If window is in stereo
   bool isStereo()
   { return in_stereo;}

   vjDisplay* getDisplay()
   { return display;}

   //!RETURNS: A unique window id
   int getId()
   { return mWindowId; }

   friend ostream& operator<<(ostream& out, vjGlWindow& win);

protected:
	      // we store a pointer to the display that we're 
        // created from, to config & to get the viewing
        // transforms from.
   vjDisplay* display;  
	
	   // reflects stereo settings in config information;
        // tells us wether or not to try for a stereo display
   bool try_stereo;     
        
      // when the window is open, this tells us whether the
        // display opened actually is in stereo - if we wanted
        // a stereo display but couldn't open it we fall back
        // to mono, and this will be false.
   bool in_stereo;      
   bool border;
   char* display_name;
   bool window_is_open;
   int  window_width, window_height;
   int  origin_x, origin_y;		    // lower-left corner of window
   int   mWindowId;                  // A unique window id to identify us

private:
   static int mCurMaxWinId;             // The current maximum window id

   static int getNextWindowId()
   {
      return mCurMaxWinId++;
   }
};

// ostream& operator<<(ostream& out, vjGlWindow& win);

#endif







