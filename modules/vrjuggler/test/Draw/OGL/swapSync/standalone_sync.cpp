// Hacked up test program for testing GLX_NV_swap_group
//
// Tries to:
//   - Create graphics window
//   - Load extensions
//   - Query swap_group max values
//   - Turn on swap group and swap barrier
//   - Each frame: clear screen, wait for a while, swap buffers
//
#include <iostream>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <unistd.h>

bool master = true;
bool stereo = false;

typedef void (*VoidExtFunc)(void);

// NVIDIA swap control
   typedef Bool ( * PFNGLXJOINSWAPGROUPNVPROC) (::Display *dpy,
                                                GLXDrawable drawable,
                                                GLuint group);

   typedef Bool ( * PFNGLXBINDSWAPBARRIERNVPROC) (::Display *dpy,
                                                  GLuint group,
                                                  GLuint barrier);

   typedef Bool ( * PFNGLXQUERYSWAPGROUPNVPROC) (::Display *dpy,
                                                 GLXDrawable drawable,
                                                 GLuint *group,
                                                 GLuint *barrier);

   typedef Bool ( * PFNGLXQUERYMAXSWAPGROUPSNVPROC) (::Display *dpy,
                                                     int screen,
                                                     GLuint *maxGroups,
                                                     GLuint *maxBarriers);

   typedef Bool ( * PFNGLXQUERYFRAMECOUNTNVPROC) (::Display *dpy,
                                                  int screen,
                                                  GLuint *count);

   typedef Bool ( * PFNGLXRESETFRAMECOUNTNVPROC) (::Display *dpy, int screen);

struct GlxFuncs
{
   GlxFuncs()
   {
      glXJoinSwapGroupNV = NULL;
      glXBindSwapBarrierNV = NULL;
      glXQuerySwapGroupNV = NULL;
      glXQueryMaxSwapGroupsNV = NULL;
      glXQueryFrameCountNV = NULL;
      glXResetFrameCountNV = NULL;
   }

   PFNGLXJOINSWAPGROUPNVPROC        glXJoinSwapGroupNV;
   PFNGLXBINDSWAPBARRIERNVPROC      glXBindSwapBarrierNV;
   PFNGLXQUERYSWAPGROUPNVPROC       glXQuerySwapGroupNV;
   PFNGLXQUERYMAXSWAPGROUPSNVPROC   glXQueryMaxSwapGroupsNV;
   PFNGLXQUERYFRAMECOUNTNVPROC      glXQueryFrameCountNV;
   PFNGLXRESETFRAMECOUNTNVPROC      glXResetFrameCountNV;
};

GlxFuncs mGlxFuncs;
unsigned mFrameNum;


VoidExtFunc getFunctionByName(const char* name)
{
   VoidExtFunc found_func(NULL);

   return glXGetProcAddressARB((const GLubyte*)name);
}

void initExtensions()
{
   mGlxFuncs.glXJoinSwapGroupNV       = (PFNGLXJOINSWAPGROUPNVPROC)       (getFunctionByName("glXJoinSwapGroupNV"));
   mGlxFuncs.glXBindSwapBarrierNV     = (PFNGLXBINDSWAPBARRIERNVPROC)     (getFunctionByName("glXBindSwapBarrierNV"));
   mGlxFuncs.glXQuerySwapGroupNV      = (PFNGLXQUERYSWAPGROUPNVPROC)      (getFunctionByName("glXQuerySwapGroupNV"));
   mGlxFuncs.glXQueryMaxSwapGroupsNV  = (PFNGLXQUERYMAXSWAPGROUPSNVPROC)  (getFunctionByName("glXQueryMaxSwapGroupsNV"));
   mGlxFuncs.glXQueryFrameCountNV     = (PFNGLXQUERYFRAMECOUNTNVPROC)     (getFunctionByName("glXQueryFrameCountNV"));
   mGlxFuncs.glXResetFrameCountNV     = (PFNGLXRESETFRAMECOUNTNVPROC)     (getFunctionByName("glXResetFrameCountNV"));
}


// Display method
void display(void)
{
   //Display* display = glXGetCurrentDisplay();
   //GLuint frame_num;
   //mGlxFuncs.glXQueryFrameCountNV(display, 1, &frame_num);
   mFrameNum += 1;

   //std::cout << "App Frame Count [" << mFrameNum << "] [" << frame_num << "]" << std::endl;
   std::cout << "App Frame Count [" << mFrameNum << "]" << std::endl;

   glDrawBuffer(GL_BACK);
   unsigned clr_choice = (mFrameNum % 5);

   if(0 == clr_choice)
   { glClearColor(0.0, 0.0, 0.0, 0.0); }     // Black
   else if(1==clr_choice)
   { glClearColor(1.0, 1.0, 1.0, 0.0); }     // White
   else if(2==clr_choice)
   { glClearColor(1.0, 0.0, 0.0, 0.0); }     // Red
   else if(3==clr_choice)
   { glClearColor(0.0, 1.0, 0.0, 0.0); }     // Green
   else if(4==clr_choice)
   { glClearColor(0.0, 0.0, 1.0, 0.0); }     // Blue

   glClear(GL_COLOR_BUFFER_BIT);

   if(stereo)
   {
      glDrawBuffer(GL_BACK_LEFT);

      unsigned clr_choice = ((mFrameNum + 2) % 5);

      if(0 == clr_choice)
      { glClearColor(0.0, 0.0, 0.0, 0.0); }     // Black
      else if(1==clr_choice)
      { glClearColor(1.0, 1.0, 1.0, 0.0); }     // White
      else if(2==clr_choice)
      { glClearColor(1.0, 0.0, 0.0, 0.0); }     // Red
      else if(3==clr_choice)
      { glClearColor(0.0, 1.0, 0.0, 0.0); }     // Green
      else if(4==clr_choice)
      { glClearColor(0.0, 0.0, 1.0, 0.0); }     // Blue

      glClear(GL_COLOR_BUFFER_BIT);
   }


   // Sleep for one of a predetermined times.
   // - Use this instead of random because we want to generate worst case
   //   on some of these sleeps (ie. one node 0, one node 2 seconds)
   unsigned sleep_choice = rand() % 3;
   unsigned long sleep_ms;

   if (master)
   {
      if(0 == sleep_choice) { sleep_ms = 0; }
      else if(1 == sleep_choice) { sleep_ms = 1000; }
      else if(2 == sleep_choice) { sleep_ms = 4000; }

      std::cout << "sleep: " << sleep_ms << "ms" << std::endl;

      usleep(sleep_ms * 1000);
   }

   glutSwapBuffers();

}

void initGraphics()
{
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glShadeModel(GL_SMOOTH);

   initExtensions();

   Display* display = glXGetCurrentDisplay();
   GLXDrawable drawable = glXGetCurrentDrawable();
   int screen = DefaultScreen(display);


   GLuint max_groups, max_barriers;
   mGlxFuncs.glXQueryMaxSwapGroupsNV(display, screen, &max_groups, &max_barriers);
   std::cout << "Screen: " << screen << std::endl
             << "   Max groups: " << max_groups
             << "   Max Barriers:" << max_barriers << std::endl;
   std::cout << "Setting up NV swap group and barrier group. Group: 1, Barrier: 1\n";

   // For now, just assume both groups are group 1
   mGlxFuncs.glXJoinSwapGroupNV(display, drawable, 1);
   mGlxFuncs.glXBindSwapBarrierNV(display, 1, 1);
}

void reshape(int w, int h)
{ glutPostRedisplay(); }

// react to mouse button presses
void mouse(int button, int state, int x, int y)
{;}

// react to mouse motions with pressed buttons
void motion(int x, int y)
{;}

// react to keys
void keyboard(unsigned char k, int x, int y)
{
   switch(k)
   {
   case 27:
   {
      exit(0);
   }
   break;
   }
}


int main(int argc, char* argv[])
{
   glutInit(&argc, argv);
   if( stereo )
   {
      glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STEREO);
   }
   else
   {
      glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   }

   int winid = glutCreateWindow("Test");
   mFrameNum = 0;

   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutKeyboardFunc(keyboard);

   if (argc > 1)
   {
      std::cout << "ARG:" << argv[1] << std::endl;
   }
   if (argc > 1 && (0 == strcmp(argv[1], "-s")))
   {
      master = false;
      std::cout << "We are a slave" << std::endl;
   }
   initGraphics();

   glutMainLoop();

   return 0;
}
