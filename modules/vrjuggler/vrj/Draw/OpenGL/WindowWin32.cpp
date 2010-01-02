/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/Draw/OpenGL/Config.h>

#include <iomanip>

#include <jccl/Config/ConfigElement.h>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Util/Assert.h>
#include <vpr/Perf/ProfileManager.h>
#include <gadget/InputManager.h>
#include <vrj/Util/Debug.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Display/Display.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Draw/OpenGL/Window.h>
#include <vrj/Draw/OpenGL/WindowWin32.h>

// The following is used below in
// vrj::opengl::WindowWin32::registerWindowClass(). The key parts are
// VJ_LIB_RT_OPT and VJ_VERSION_STR.
#  if defined(VJ_DEBUG)
#     if defined(_DEBUG)
#        define VJ_LIB_RT_OPT "_d"
#     else
#        define VJ_LIB_RT_OPT "_g"
#     endif
#  else
#     define VJ_LIB_RT_OPT ""
#  endif

#  define VJ_STRINGIZE(X) VJ_DO_STRINGIZE(X)
#  define VJ_DO_STRINGIZE(X) #X
#  define VJ_VERSION_STR VJ_STRINGIZE(__VJ_MAJOR__) "_" \
                         VJ_STRINGIZE(__VJ_MINOR__) "_" \
                         VJ_STRINGIZE(__VJ_PATCH__)


static const char* GL_WINDOW_WIN32_CLASSNAME("vrj::opengl::WindowWin32");

namespace vrj
{

namespace opengl
{

WindowWin32::WindowWin32()
   : mRenderContext(NULL)
   , mDeviceContext(NULL)
{
   // Event processing is not blocking.
   mBlocking = false;
}

WindowWin32::~WindowWin32()
{
   this->close();
}

// Open the window
// - Creates a window
// - Creates a rendering context
// - Registers new window with the window list
bool WindowWin32::open()
{
   if ( false == WindowWin32::registerWindowClass() )
   {
      return false;
   }

   if ( mWindowIsOpen )
   {
      return true;
   }

   HMODULE hMod = GetModuleHandle(NULL);
   int root_height;

   // OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS.
   DWORD style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

   if ( mIsFullScreen )
   {
      style |= WS_MAXIMIZE | WS_POPUP;
      mWindowWidth  = GetSystemMetrics(SM_CXSCREEN);
      mWindowHeight = GetSystemMetrics(SM_CYSCREEN);
   }
   // If we want a border, create an overlapped window.  This will have
   // a titlebar and a border.
   else if ( mHasBorder )
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_HVERB_LVL)
         << "[vrj::opengl::WindowWin32::open()] Attempting to give window a "
         << "border." << std::endl << vprDEBUG_FLUSH;
      style |= WS_OVERLAPPEDWINDOW;
   }
   // Otherwise, come as close as possible to having no border by using
   // the thin-line border.
   else
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_HVERB_LVL)
         << "[vrj::opengl::WindowWin32::open()] Attempting to make window "
         << "borderless." << std::endl << vprDEBUG_FLUSH;
      style |= WS_OVERLAPPED | WS_POPUP | WS_VISIBLE;
   }

   DWORD ex_style(0);

   if ( mAlwaysOnTop )
   {
      ex_style = WS_EX_TOPMOST;
   }

   root_height = GetSystemMetrics(SM_CYSCREEN);

   // The desired client rectangle size (left, top, right, bottom).
   RECT rc = { 0, 0, mWindowWidth, mWindowHeight };

   if ( ! AdjustWindowRect(&rc, style, false) )
   {
      doInternalError("Failed to adjust window rectangle");
      return false;
   }

   // Ensure that the input window base class has the right dimension
   // information.
   InputAreaWin32::resize(mWindowWidth, mWindowHeight);

   // Create the main application window
   mWinHandle = CreateWindowEx(ex_style, GL_WINDOW_WIN32_CLASSNAME,
                               mWindowName.c_str(), style, mOriginX,
                               root_height - mOriginY - mWindowHeight,
                               rc.right - rc.left, rc.bottom - rc.top,
                               NULL, NULL, hMod, NULL);

   // If window was not created, quit
   if ( NULL == mWinHandle )
   {
      doInternalError("Could not create window using CreateWindowEx()!");
      return false;
   }

   // Attach a pointer to the device for use from the WNDPROC
   SetWindowLongPtr(mWinHandle, GWLP_USERDATA, (LPARAM) this);

   // We have a valid window, so... Create the context
   mDeviceContext = GetDC(mWinHandle);            // Store the device context
   if ( false == setPixelFormat(mDeviceContext) ) // Select the pixel format
   {
      return false;
   }

   // Create the rendering context and make it current
   mRenderContext = createContext(mDeviceContext);

   if ( NULL == mRenderContext )
   {
      char* msg_buffer(NULL);

      DWORD error_code = GetLastError();
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL, error_code,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR) &msg_buffer, 0, NULL);

      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
         << "ERROR: [vrj::opengl::WindowWin32::open()] wglCreateContext() "
         << "failed with error code " << error_code << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
         << msg_buffer << std::endl << vprDEBUG_FLUSH;

      LocalFree(msg_buffer);

      return false;
   }

   wglMakeCurrent(mDeviceContext, mRenderContext);

   // Register extensions in this window
   mExtensions.registerExtensions();

   // Check on using swap group
   jccl::ConfigElementPtr disp_sys_elt =
      DisplayManager::instance()->getDisplaySystemElement();
   bool use_swap_group = disp_sys_elt->getProperty<bool>("use_swap_group");

   if(use_swap_group)
   {
      vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL,
                           "Attempting to set up WGL swap group.\n", "");

      // Try NV swap group extension
      if(mExtensions.hasSwapGroupNV())
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
            << "SwapGroupNV: " << mExtensions.hasSwapGroupNV() << std::endl
            << vprDEBUG_FLUSH;
         GLuint max_groups, max_barriers;
         mExtensions.wglQueryMaxSwapGroupsNV(mDeviceContext, &max_groups,
                                             &max_barriers);
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
            << "Max groups: " << max_groups << " Max Barriers:" << max_barriers
            << std::endl << vprDEBUG_FLUSH;

         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
            << "Setting up NV swap group and barrier group. "
            << "Group: 1, Barrier: 1\n" << vprDEBUG_FLUSH;
         // For now, just assume both groups are group 1
         // Note: In the future this code may need to be refactored to be
         //       controlled from the vrj::opengl::Pipe class since it is
         //       really the thing that would correspond to the group and
         //       could group the correct windows to a group id.
         mExtensions.wglJoinSwapGroupNV(mDeviceContext, 1);
         mExtensions.wglBindSwapBarrierNV(1, 1);
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Could not detect any WGL extensions that support swap groups.\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Proceeding with no swap locking.\n" << vprDEBUG_FLUSH;
      }
   }

   // Register the window with the window list
   WindowWin32::addWindow(mWinHandle, this);

   // Display the window
   ShowWindow(mWinHandle, SW_SHOW);
   UpdateWindow(mWinHandle);             // Tell the window to paint
   mWindowIsOpen = true;

   // If mHideMouse is true we must pass false to ShowCursor
   ShowCursor(! mHideMouse);

   return true;
}

// Closes the OpenGL window.
bool WindowWin32::close()
{
   // if not open, then don't bother.
   if ( !mWindowIsOpen )
   {
      return false;
   }

   // Remove window from window list
   WindowWin32::removeWindow(mWinHandle);

   mWindowIsOpen = false;

   // destroy the win32 window
   return(1 == DestroyWindow(mWinHandle));
}

// Sets the current OpenGL context to this window.
bool WindowWin32::makeCurrent()
{
   vprASSERT((mDeviceContext != NULL) && (mRenderContext != NULL));
   wglMakeCurrent(mDeviceContext, mRenderContext);  // Make our context current
   return true;
}

// Swap the front and back buffers
// Process events here
void WindowWin32::swapBuffers()
{
   VPR_PROFILE_GUARD_HISTORY("WindowWin32::swapBuffers", 10);
   vprASSERT(mDeviceContext != NULL);
   Window::swapBuffers();
   SwapBuffers(mDeviceContext);
}

void WindowWin32::checkEvents()
{
   handleEvents();
}

void WindowWin32::configWindow(vrj::DisplayPtr disp)
{
   const char neg_one_STRING[] = "-1";
   vprASSERT(disp.get() != NULL);
   vrj::opengl::Window::configWindow(disp);

   // Get the vector of display chunks
   jccl::ConfigElementPtr disp_sys_elt =
      DisplayManager::instance()->getDisplaySystemElement();
   jccl::ConfigElementPtr display_elt = disp->getConfigElement();

   // Get the lock KeyboardMouseDevice information.
   gadget::InputArea::config(display_elt);

   mWindowName = disp->getName();
   mPipe = disp->getPipe();
   vprASSERT(mPipe >= 0);

   mXDisplayName = disp_sys_elt->getProperty<std::string>("pipes", mPipe);
   if (mXDisplayName == neg_one_STRING)    // Use display env
   {
       const std::string DISPLAY_str("DISPLAY");
       std::string d;
       vpr::System::getenv(DISPLAY_str, d);
       if ( ! d.empty() )
       {
          mXDisplayName = std::string( d );
       }
   }
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL)
      << "[vrj::opengl::WindowWin32::configWindow()] display name is: "
      << mXDisplayName << std::endl << vprDEBUG_FLUSH;
}

// WindowProcedure to deal with the generated messages.
// Called only for the window that we are controlling.
LRESULT WindowWin32::handleEvent(HWND hWnd, UINT message, WPARAM wParam,
                                 LPARAM lParam)
{
   switch ( message )
   {
      // ---- Window creation, setup for OpenGL ---- //
      case WM_CREATE:
         vprASSERT(false);                               // Should never get called because
                                                         //we are not registered when this gets called

         mDeviceContext = GetDC(hWnd);                   // Store the device context
         if ( false == setPixelFormat(mDeviceContext) )  // Select the pixel format
         {
            return 0;
         }

         // Create the rendering context and make it current
         mRenderContext = createContext(mDeviceContext);
         wglMakeCurrent(mDeviceContext, mRenderContext);
         break;

         // ---- Window is being destroyed, cleanup ---- //
      case WM_DESTROY:

         // Deselect the current rendering context and delete it
         wglMakeCurrent(mDeviceContext, NULL);
         wglDeleteContext(mRenderContext);

         // Tell the application to terminate after the window
         // is gone.
         PostQuitMessage(0);
         break;

         // --- Window is resized. --- //
      case WM_SIZE:
         // Call our function which modifies the clipping
         // volume and viewport
         sizeChanged(LOWORD(lParam), HIWORD(lParam));
         InputAreaWin32::resize(LOWORD(lParam), HIWORD(lParam));
         break;

         // The painting function.  This message sent by Windows
         // whenever the screen needs updating.
      case WM_PAINT:
         {
            PAINTSTRUCT ps;         // Paint structure
            BeginPaint(hWnd, &ps);  // Validate the drawing of the window
            EndPaint(hWnd, &ps);
         }
         break;
      // Catch the ALT key so that it does not open the system menu.
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
         break;
      default:   // Passes it on if unproccessed
         return DefWindowProc(hWnd, message, wParam, lParam);
   }

   return(0L);
}

// Set the pixel format for the given window
bool WindowWin32::setPixelFormat(HDC hDC)
{
   PIXELFORMATDESCRIPTOR pfd;

   memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   pfd.nSize = (sizeof(PIXELFORMATDESCRIPTOR));
   pfd.nVersion = 1;

   int visual_id(-1);
   int red_size(8), green_size(8), blue_size(8), alpha_size(8), db_size(32),
       accum_red_size(1), accum_green_size(1), accum_blue_size(1),
       accum_alpha_size(1), stencil_size(1);
   int num_aux_bufs(0);

   jccl::ConfigElementPtr gl_fb_elt = mVrjDisplay->getGlFrameBufferConfig();

   if ( gl_fb_elt.get() != NULL )
   {
      if ( gl_fb_elt->getVersion() < 2 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING") << ": Display window '"
            << mVrjDisplay->getName() << "'" << std::endl;
         vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "         has an out of date OpenGL frame buffer "
            << "configuration.\n";
         vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "         Expected version 3 but found version "
            << gl_fb_elt->getVersion() << ".  Default values\n";
         vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "         will be used for some frame buffer settings.\n"
            << vprDEBUG_FLUSH;
      }

      visual_id        = gl_fb_elt->getProperty<int>("visual_id");
      red_size         = gl_fb_elt->getProperty<int>("red_size");
      green_size       = gl_fb_elt->getProperty<int>("green_size");
      blue_size        = gl_fb_elt->getProperty<int>("blue_size");
      alpha_size       = gl_fb_elt->getProperty<int>("alpha_size");
      num_aux_bufs     = gl_fb_elt->getProperty<int>("auxiliary_buffer_count");
      db_size          = gl_fb_elt->getProperty<int>("depth_buffer_size");
      stencil_size     = gl_fb_elt->getProperty<int>("stencil_buffer_size");
      accum_red_size   = gl_fb_elt->getProperty<int>("accum_red_size");
      accum_green_size = gl_fb_elt->getProperty<int>("accum_green_size");
      accum_blue_size  = gl_fb_elt->getProperty<int>("accum_blue_size");
      accum_alpha_size = gl_fb_elt->getProperty<int>("accum_alpha_size");
   }

   int pixel_format;

   if ( visual_id != -1 )
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << "Requesting visual 0x" << std::hex << visual_id << std::dec
         << " from WGL." << std::endl << vprDEBUG_FLUSH;

      int result = DescribePixelFormat(hDC, visual_id,
                                       sizeof(PIXELFORMATDESCRIPTOR), &pfd);

      if ( result == 0 )
      {
         std::stringstream error;
         error << "Failed to get requested visual (ID 0x" << std::hex
               << visual_id << std::dec << ")" << std::flush;

         doInternalError(error.str());
         return false;
      }

      mInStereo = (mVrjDisplay->isStereoRequested() &&
                   (pfd.dwFlags & PFD_STEREO));
      pixel_format = visual_id;
   }
   else
   {
      if ( red_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Color buffer red channel size was negative ("
            << red_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         red_size = 1;
      }

      if ( green_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Color buffer green channel size was negative ("
            << green_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         green_size = 1;
      }

      if ( blue_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Color buffer blue channel size was negative ("
            << blue_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         blue_size = 1;
      }

      if ( alpha_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Color buffer alpha channel size was negative ("
            << alpha_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         alpha_size = 1;
      }

      if ( num_aux_bufs < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Auxiliary buffer count was negative (" << num_aux_bufs
            << ").  Setting to 0.\n" << vprDEBUG_FLUSH;
         num_aux_bufs = 0;
      }

      if ( db_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Depth buffer size was negative (" << db_size
            << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         db_size = 1;
      }

      if ( stencil_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Stencil buffer size was negative (" << stencil_size
            << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         stencil_size = 1;
      }

      if ( accum_red_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Accumulation buffer red channel size was negative ("
            << accum_red_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_red_size = 1;
      }

      if ( accum_green_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Accumulation buffer green channel size was negative ("
            << accum_green_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_green_size = 1;
      }

      if ( accum_blue_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Accumulation buffer blue channel size was negative ("
            << accum_blue_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_blue_size = 1;
      }

      if ( accum_alpha_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Accumulation buffer alpha channel size was negative ("
            << accum_alpha_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_alpha_size = 1;
      }

      /* Defaults. */
      pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;

      if ( mVrjDisplay->isStereoRequested() )
      {
         mInStereo = true;
         pfd.dwFlags |= PFD_STEREO;
      }
      else
      {
         mInStereo = false;
      }

      pfd.iPixelType        = PFD_TYPE_RGBA;
      pfd.cColorBits        = red_size + green_size + blue_size + alpha_size;
      pfd.cRedBits          = red_size;       // Ignored by ChoosePixelFormat()
      pfd.cGreenBits        = green_size;     // Ignored by ChoosePixelFormat()
      pfd.cBlueBits         = blue_size;      // Ignored by ChoosePixelFormat()
      pfd.cAlphaBits        = alpha_size;     // Ignored by ChoosePixelFormat()
      pfd.cAccumBits        = accum_red_size + accum_green_size +
                                 accum_blue_size + accum_alpha_size;
      pfd.cAccumRedBits     = accum_red_size;   // Ignored by ChoosePixelFormat()
      pfd.cAccumGreenBits   = accum_green_size; // Ignored by ChoosePixelFormat()
      pfd.cAccumBlueBits    = accum_blue_size;  // Ignored by ChoosePixelFormat()
      pfd.cAccumAlphaBits   = accum_alpha_size; // Ignored by ChoosePixelFormat()
      pfd.cDepthBits        = db_size;
      pfd.cStencilBits      = stencil_size;
      pfd.cAuxBuffers       = num_aux_bufs;

      const unsigned int indent_level(2);
      const std::string indent_text(indent_level, ' ');
      const int pad_width_dot(40 - indent_level);
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << "OpenGL visual request settings for " << mVrjDisplay->getName()
         << ":\n";
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Color bit size " << " " << (int) pfd.cColorBits << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Auxiliary buffer count " << " " << (int) pfd.cAuxBuffers
         << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Depth buffer size " << " " << (int) pfd.cDepthBits << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Stencil buffer size " << " " << (int) pfd.cStencilBits
         << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Accumulation buffer bit size " << " " << (int) pfd.cAccumBits
         << std::endl;
      vprDEBUG_CONTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << vprDEBUG_FLUSH;

      // Let Win32 choose one for us
      pixel_format = ChoosePixelFormat(hDC, &pfd);

      if ( pixel_format > 0 )
      {
         PIXELFORMATDESCRIPTOR match;
         DescribePixelFormat(hDC, pixel_format, sizeof(PIXELFORMATDESCRIPTOR),
                             &match);

         // ChoosePixelFormat is dumb in that it will return a pixel format
         // that doesn't have stereo even if it was requested so we need to
         // make sure that if stereo was selected, we got it.
         if ( mVrjDisplay->isStereoRequested() )
         {
            if ( !(match.dwFlags & PFD_STEREO) )
            {
               doInternalError("Could not get a stereo pixel format.");
               return false;
            }
         }
      }
   }

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << "Visual ID: 0x" << std::setw(2) << std::hex << pixel_format
      << std::dec << std::endl << vprDEBUG_FLUSH;

   // Set the pixel format for the device context
   SetPixelFormat(hDC, pixel_format, &pfd);
   return true;
}

// The user has changed the size of the window
void WindowWin32::sizeChanged(long width, long height)
{
   // Make sure we don't have window of 1 size (divide by zero would follow).
   if ( width == 0 )
   {
      width = 1;
   }

   if ( height == 0 )
   {
      height = 1;
   }

   updateOriginSize(mOriginX, mOriginY, width, height);
   setDirtyViewport(true);
}

// Global context creator.
HGLRC WindowWin32::createContext(HDC hdc)
{
   jccl::ConfigElementPtr gl_fb_elt = mVrjDisplay->getGlFrameBufferConfig();

   const bool use_create_context_attribs =
      gl_fb_elt->getProperty<bool>("use_create_context_attribs");

   if ( use_create_context_attribs )  // We want to try to use the ARB function
   {
      // First create a traditional context so we can get extensions
      HGLRC context = wglCreateContext(hdc);

      if ( NULL == context )
      {
         DWORD error_code = GetLastError();
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
            << "ERROR: [vrj::opengl::WindowWin32::createContext()] "
            << "wglCreateContext() failed with error code " << error_code
            << std::endl << vprDEBUG_FLUSH;
         return NULL;
      }

      // Make the original context current
      wglMakeCurrent(hdc, context);

      // load WGL extensions
      mExtensions.registerExtensions();

      // Delete the context
      wglDeleteContext(context);

      if ( mExtensions.hasCreateContextARB() )
      {
         // use environment variables to construct attribute array
         int attribList[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB,
            gl_fb_elt->getProperty<int>("gl_context_major_version"),
            WGL_CONTEXT_MINOR_VERSION_ARB,
            gl_fb_elt->getProperty<int>("gl_context_minor_version"),
            WGL_CONTEXT_FLAGS_ARB,
            gl_fb_elt->getProperty<int>("gl_context_flags"),
            0
         };

         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
            << "Creating WGL Context, attributes = {"
            << attribList[0] << ", " << attribList[1] << ", "
            << attribList[2] << ", " << attribList[3] << ", "
            << attribList[4] << ", " << attribList[5] << ", "
            << attribList[6] << "}"
            <<  std::endl << vprDEBUG_FLUSH;

         return mExtensions.wglCreateContextAttribsARB(hdc, 0, attribList);
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Could not detect WGL_ARB_create_context extension.\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Falling back on traditional wglCreateContext.\n"
            << vprDEBUG_FLUSH;
      }
   }

   return wglCreateContext(hdc);
}

// Global Window event handler.
LRESULT CALLBACK WindowWin32::WndProc(HWND hWnd, UINT message, WPARAM wParam,
                                      LPARAM lParam)
{
   WindowWin32* glWin = getGlWin(hWnd);

   // If we can find a GLWindowWin32, make sure that it handles the event.
   if ( glWin != NULL )
   {
      return glWin->handleEvent(hWnd, message, wParam, lParam);
   }
   else
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
         << "Could not find vrj::opengl::WindowWin32 to process event."
         << std::endl << vprDEBUG_FLUSH;

      return DefWindowProc(hWnd, message, wParam, lParam);
   }
}

// Window registration.
bool WindowWin32::mWinRegisteredClass = false;
WNDCLASS WindowWin32::mWinClass;           // The window class to register
std::map<HWND, WindowWin32*> WindowWin32::mGlWinMap;

bool WindowWin32::registerWindowClass()
{
   if ( mWinRegisteredClass )
   {
      return true;
   }

   char lpszAppName[1024];
   GetModuleFileName(NULL,lpszAppName,sizeof(lpszAppName));

   mWinRegisteredClass = true;     // We have registered now

   HINSTANCE hInstance = GetModuleHandle(NULL);

   // Register Window style
   mWinClass.style       = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
   mWinClass.lpfnWndProc = (WNDPROC) WindowWin32::WndProc;
   mWinClass.cbClsExtra  = 0;
   mWinClass.cbWndExtra  = 0;
   mWinClass.hInstance   = hInstance;            // Get handle to the module that created current process
   mWinClass.hIcon       = LoadIcon(hInstance, "VRJUGGLER_ICON");
   mWinClass.hCursor     = LoadCursor(NULL, IDC_ARROW);

   // No need for background brush for OpenGL window
   //mWinClass.hbrBackground  = NULL;
   mWinClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

   mWinClass.lpszMenuName   = NULL;
   mWinClass.lpszClassName  = GL_WINDOW_WIN32_CLASSNAME;

   if (mWinClass.hIcon == NULL)
   {
      HINSTANCE hDLLInstance =
         LoadLibrary("vrj_ogl" VJ_LIB_RT_OPT "-" VJ_VERSION_STR ".dll");
      if (hDLLInstance != NULL)
      {
         mWinClass.hIcon = LoadIcon(hDLLInstance, "VRJUGGLER_ICON");
      }
   }

   // Register the window class
   if ( RegisterClass(&mWinClass) == 0 )
   {
      return false;
   }
   else
   {
      return true;
   }
}

void WindowWin32::addWindow(HWND handle, vrj::opengl::WindowWin32* glWin)
{
   vprASSERT(glWin != NULL);

   if ( mGlWinMap.find(handle) == mGlWinMap.end() )     // Not already there
   {
      mGlWinMap[handle] = glWin;
   }
}

void WindowWin32::removeWindow(HWND handle)
{
   mGlWinMap.erase(handle);     // Erase the entry in the list
}

vrj::opengl::WindowWin32* WindowWin32::getGlWin(HWND handle)
{
   std::map<HWND, WindowWin32*>::iterator glWinIter;

   glWinIter = mGlWinMap.find(handle);
   if ( glWinIter == mGlWinMap.end() ) // Not found
   {
      return NULL;
   }
   else
   {
      return (*glWinIter).second;       // Return the found window
   }
}

} // End of opengl namespace

} // End of vrj namespace
