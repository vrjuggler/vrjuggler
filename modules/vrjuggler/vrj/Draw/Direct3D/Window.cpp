/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#include <vrj/Draw/Direct3D/Config.h>

#include <vpr/vpr.h>
#include <vpr/System.h>

#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/Projection.h>
#include <vrj/Display/Frustum.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Viewport.h>

#include <jccl/Config/ConfigElement.h>

#include <vrj/Draw/Direct3D/SimInterface.h>
#include <vrj/Draw/Direct3D/Window.h>

// The following is used below in vrj::direct3dWindow::registerWindowClass().
// The key parts are VJ_LIB_RT_OPT and VJ_VERSION_STR.
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

// This variable determines which matrix stack we put the viewing transformation
// If it is on the proj matrix, then lighting and env maps work but fog breaks.
#define USE_PROJECTION_MATRIX 1  /* Should we put the camera transforms on the
                                   Projection or modelview matrix */


static const char* D3D_WINDOW_WIN32_CLASSNAME("vrj::direct3d::Window");

namespace vrj
{

namespace direct3d
{

int Window::mCurMaxWinId = 0;
vpr::Mutex Window::mWinIdMutex;

// Complete any setup that is needed after open.
void Window::finishSetup()
{
   vprASSERT(mWindowIsOpen && "Pre-condition of being open failed");
}

void Window::updateViewport()
{
   D3DVIEWPORT9 viewport;

   viewport.X = 0;
   viewport.Y = 0;
   viewport.Width = mWindowWidth;
   viewport.Height = mWindowHeight;
   viewport.MinZ = 0.0f;
   viewport.MaxZ = 1.0f;

   HRESULT r = mRenderDevice->SetViewport(&viewport);
   setDirtyViewport(false);
}

void Window::setViewport(const float xo, const float yo, const float xSize,
                         const float ySize)
{
   vprASSERT( ((xo+xSize) <= 1.0f) && "X viewport sizes are out of range");
   vprASSERT( ((yo+ySize) <= 1.0f) && "Y viewport sizes are out of range");

   unsigned int ll_x = static_cast<unsigned int>(xo * float(mWindowWidth));
   unsigned int ll_y = static_cast<unsigned int>(yo * float(mWindowHeight));
   unsigned int x_size =
      static_cast<unsigned int>(xSize * float(mWindowWidth));
   unsigned int y_size =
      static_cast<unsigned int>(ySize * float(mWindowHeight));

   D3DVIEWPORT9 viewport;

   viewport.X = ll_x;
   viewport.Y =ll_y;
   viewport.Width = x_size;
   viewport.Height = y_size;
   viewport.MinZ = 0.0f;
   viewport.MaxZ = 1.0f;

   HRESULT r = mRenderDevice->SetViewport(&viewport);
}

void Window::setViewBuffer(vrj::Viewport::View view)
{
   // XXX: Not implemented
}

void Window::setProjection(vrj::ProjectionPtr proj)
{
   if (!mWindowIsOpen)
   {
      return;
   }

   const std::vector<float>& frust = proj->getFrustum().getValues();

   D3DXMATRIX proj_matrix;
   ZeroMemory( &proj_matrix, sizeof( D3DXMATRIX ) );

   D3DXMatrixPerspectiveOffCenterLH(&proj_matrix,
      frust[Frustum::VJ_LEFT], frust[Frustum::VJ_RIGHT],
      frust[Frustum::VJ_BOTTOM], frust[Frustum::VJ_TOP],
      frust[Frustum::VJ_NEAR], frust[Frustum::VJ_FAR]);

   mRenderDevice->SetTransform( D3DTS_PROJECTION, &proj_matrix );
#ifdef USE_PROJECTION_MATRIX
   mRenderDevice->MultiplyTransform( D3DTS_PROJECTION, (const D3DMATRIX*)&proj->getViewMatrix().mData);
#endif

#ifndef USE_PROJECTION_MATRIX
   D3DXMATRIX view_matrix;
   D3DXMatrixIdentity(view_matrix);
   mRenderDevice->SetTransform(D3DTS_VIEW, &view_matrix);
   mRenderDevice->MultiplyTransform( D3DTS_VIEW, &proj->getViewMatrix().mData);
#endif
}

int Window::getNextWindowId()
{
   vpr::Guard<vpr::Mutex> guard(mWinIdMutex);      // Protect the id
   return mCurMaxWinId++;
}

// Open the window
// - Creates a window
// - Creates a rendering context
// - Registers new window with the window list
bool Window::open()
{
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
      << "[vrj::direct3d::Window::open()]" << std::endl << vprDEBUG_FLUSH;

   if ( false == registerWindowClass() )
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
         << "[vrj::direct3d::Window::open()] Attempting to give window a "
         << "border.\n" << vprDEBUG_FLUSH;
      style |= WS_OVERLAPPEDWINDOW;
   }
   // Otherwise, come as close as possible to having no border by using
   // the thin-line border.
   else
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_HVERB_LVL)
         << "[vrj::direct3d::Window::open()] Attempting to make window "
         << "borderless." << vprDEBUG_FLUSH;
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
   mWinHandle = CreateWindowEx(ex_style, D3D_WINDOW_WIN32_CLASSNAME,
                               mWindowName.c_str(), style, mOriginX,
                               root_height - mOriginY - mWindowHeight,
                               rc.right - rc.left, rc.bottom - rc.top,
                               NULL, NULL, hMod, NULL);

   // If window was not created, quit
   if ( NULL == mWinHandle )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " [vrj::direct3d::Window::open()] Could not create window.\n"
         << vprDEBUG_FLUSH;
      return false;
   }

   HRESULT r = 0;

   // Acquire a pointer to IDirect3D9
   mDirect3D = Direct3DCreate9( D3D_SDK_VERSION );
   if (NULL == mDirect3D)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:")
         << " [vrj::direct3d::Window::open()] Could not create IDirect3D8 "
         << "object.\n" << vprDEBUG_FLUSH;
      return false;
   }

   D3DPRESENT_PARAMETERS d3dpp; 
   ZeroMemory( &d3dpp, sizeof(d3dpp) );
   d3dpp.Windowed = TRUE;
   d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
   d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

   // Create the device
   if( FAILED( mDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mWinHandle,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &mRenderDevice ) ) )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:")
         << " [vrj::direct3d::Window::open()] Initialization of the device "
         << "failed.\n" << vprDEBUG_FLUSH;
      return false;
   }

   // Attach a pointer to the device for use from the WNDPROC
   SetWindowLongPtr(mWinHandle, GWLP_USERDATA, (LPARAM) this);

   // Register the window with the window list
   addWindow(mWinHandle, this);

   // Display the window
   ShowWindow(mWinHandle, SW_SHOW);
   UpdateWindow(mWinHandle);             // Tell the window to paint
   mWindowIsOpen = true;

   // If mHideMouse is true we must pass false to ShowCursor
   ShowCursor(! mHideMouse);

   return true;
}

// Closes the OpenGL window.
bool Window::close()
{
   // if not open, then don't bother.
   if ( !mWindowIsOpen )
   {
      return false;
   }

   // Remove window from window list
   removeWindow(mWinHandle);

   mDirect3D->Release();
   mRenderDevice->Release();

   mWindowIsOpen = false;

   // destroy the win32 window
   return(1 == DestroyWindow(mWinHandle));
}

// Sets the current Direct3D context to this window.
bool Window::makeCurrent()
{
   // XXX: Don't know if we need to do this.
   return true;
}

// Swap the front and back buffers
// Process events here
void Window::swapBuffers()
{
   vprASSERT(NULL != mRenderDevice);
   mSwapCount++;
   // Present the back buffer to the primary surface
   HRESULT r = mRenderDevice->Present( NULL, NULL, NULL, NULL );
   //vprASSERT(SUCCESS(r));
}

void Window::checkEvents()
{
   handleEvents();
}

void Window::configWindow(vrj::DisplayPtr disp)
{
   const char neg_one_STRING[] = "-1";
   vprASSERT(disp.get() != NULL);

   mVrjDisplay = disp;
   mVrjDisplay->getOriginAndSize(mOriginX, mOriginY, mWindowWidth,
                                 mWindowHeight);
   mHasBorder = mVrjDisplay->shouldDrawBorder();
   mIsFullScreen = mVrjDisplay->isFullScreen();
   mAlwaysOnTop = mVrjDisplay->isAlwaysOnTop();
   mHideMouse = mVrjDisplay->shouldHideMouse();

   // Get the vector of display chunks
   jccl::ConfigElementPtr disp_sys_elt =
      DisplayManager::instance()->getDisplaySystemElement();
   jccl::ConfigElementPtr display_elt = disp->getConfigElement();

   // Get the lock KeyboardMouseDevice information.
   gadget::InputArea::config(display_elt);

   mWindowName = disp->getName();
}

// WindowProcedure to deal with the generated messages.
// Called only for the window that we are controlling.
LRESULT Window::handleEvent(HWND hWnd, UINT message, WPARAM wParam,
                            LPARAM lParam)
{
   switch ( message )
   {
      // ---- Window creation, setup for OpenGL ---- //
      case WM_CREATE:
         vprASSERT(false);                               // Should never get called because
         break;

         // ---- Window is being destroyed, cleanup ---- //
      case WM_DESTROY:
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
bool Window::setPixelFormat(HDC hDC)
{
   return true;
}

// The user has changed the size of the window
void Window::sizeChanged(long width, long height)
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

// Global Window event handler.
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam,
                                 LPARAM lParam)
{
   vrj::direct3d::Window* d3dWin = getD3dWin(hWnd);

   // If we can find a GLWindow, make sure that
   // it handles the event.
   if ( d3dWin != NULL )
   {
      LRESULT result = d3dWin->handleEvent(hWnd, message, wParam, lParam);
      return result;
   }
   else
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
         << "Could not find vrj::direct3d::Window to process event."
         << std::endl << vprDEBUG_FLUSH;

      LRESULT result = DefWindowProc(hWnd, message, wParam, lParam);
      return result;
   }
}

// Window registration.
bool Window::mWinRegisteredClass = false;
WNDCLASS Window::mWinClass;           // The window class to register
std::map<HWND, Window*> Window::mD3dWinMap;

bool Window::registerWindowClass()
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
   mWinClass.lpfnWndProc = (WNDPROC) Window::WndProc;
   mWinClass.cbClsExtra  = 0;
   mWinClass.cbWndExtra  = 0;
   mWinClass.hInstance   = hInstance;            // Get handle to the module that created current process
   mWinClass.hIcon       = LoadIcon(hInstance, "VRJUGGLER_ICON");
   mWinClass.hCursor     = LoadCursor(NULL, IDC_ARROW);

   // No need for background brush for OpenGL window
   //mWinClass.hbrBackground  = NULL;
   mWinClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

   mWinClass.lpszMenuName   = NULL;
   mWinClass.lpszClassName  = D3D_WINDOW_WIN32_CLASSNAME;

   if (mWinClass.hIcon == NULL)
   {
      HINSTANCE hDLLInstance =
         LoadLibrary("vrj_d3d" VJ_LIB_RT_OPT "-" VJ_VERSION_STR ".dll");
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

void Window::addWindow(HWND handle, vrj::direct3d::Window* d3dWin)
{
   vprASSERT(d3dWin != NULL);
   
   if ( mD3dWinMap.find(handle) == mD3dWinMap.end() )     // Not already there
   {
      mD3dWinMap[handle] = d3dWin;
   }
}

void Window::removeWindow(HWND handle)
{
   mD3dWinMap.erase(handle);     // Erase the entry in the list
}

vrj::direct3d::Window* Window::getD3dWin(HWND handle)
{
   std::map<HWND, Window*>::iterator d3dWinIter;

   d3dWinIter = mD3dWinMap.find(handle);
   if ( d3dWinIter == mD3dWinMap.end() ) // Not found
   {
      return NULL;
   }
   else
   {
      return(*d3dWinIter).second;			// Return the found window
   }
}

std::ostream& operator<<(std::ostream& out, vrj::direct3d::Window& win)
{
   vprASSERT(win.mVrjDisplay != NULL);

   out << "-------- vrj::direct3d::Window --------" << std::endl;
   out << "Open: " << (win.mWindowIsOpen ? "Yes" : "No") << std::endl;
   out << "Stereo: " << (win.mInStereo ? "Yes" : "No") << std::endl;
   out << "Swap Count: " << win.mSwapCount << std::endl;
   out << "Display Info:\n" << *(win.mVrjDisplay) << std::endl;
   return out;
}

} // end direct3d namespace

} // end vrj namespace
