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

#include <sys/types.h>

#include <vrj/Math/Coord.h>
#include <vrj/Config/ConfigChunk.h>
#include <vrj/Util/Debug.h>
#include <vrj/Input/Devices/Keyboard/KeyboardWin32.h>

// The message loop
void samplem_keys(void*);

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))
#endif
namespace vrj
{
   
bool KeyboardWin32::config(ConfigChunk *c)
{
    vjDEBUG_BEGIN(vjDBG_INPUT_MGR, vjDBG_STATE_LVL)
                     << "vjKeyboardWin32::config " << std::endl
                     << vjDEBUG_FLUSH;

    // Call base class config function first
    if(! (Input::config(c) && Keyboard::config(c)))
      return false;

    int i;
    for(i =0; i < 256; i++)
         m_realkeys[i] = m_keys[i] = 0;
    m_realkeys[0] = m_keys[0] = 1;

    // Get size and position
    m_width = (int)c->getProperty("width");
    m_height = (int)c->getProperty("height");

    if (m_width == 0) m_width = 400;
    if (m_height == 0) m_height = 400;

    m_x = c->getProperty("origin", 0);
    m_y = c->getProperty("origin", 1);

    newx = oldx = 0xfffff;
    newy = oldy = 0xfffff;

    m_mouse_sensitivity = c->getProperty("msens");
    if (0 == m_mouse_sensitivity) m_mouse_sensitivity = 0.5;

    vjDEBUG(vjDBG_INPUT_MGR, vjDBG_STATE_LVL) << "Mouse Sensititivty: "
               << m_mouse_sensitivity << std::endl << vjDEBUG_FLUSH;
    vjDEBUG_END(vjDBG_INPUT_MGR, vjDBG_STATE_LVL) << std::endl
                                                  << vjDEBUG_FLUSH;

    return true;
}


// Create a win32 window and start a thread
// processing it's messages
int KeyboardWin32::startSampling()
{
   if (myThread == NULL) {
      resetIndexes();

      vjDEBUG(vjDBG_INPUT_MGR, vjDBG_CONFIG_LVL)
                 << "vjWin32Keyboard::startSampling() : ready to go.."
                 << std::endl << vjDEBUG_FLUSH;

      KeyboardWin32* devicePtr = this;

      if (0 == (myThread = new vpr::Thread(samplem_keys,(void*)devicePtr)))
         return 0; //fail
      else {
         return 1;
      }
   }
   else return 0; // already sampling
}

void samplem_keys(void* devPtr)
{
    MSG msg;
        // hack:
        // The Window has to be created in the same thread that
        // we run the message pump because all window messages
        // dispatched are dispatched to the thread that created
        // the window.  (And we want to receive the messages
        // in the spawned thread)
    KeyboardWin32* keyboard = (KeyboardWin32*)devPtr;
    keyboard->createWindowWin32 ();

    // When there are messages, process them all.  Otherwise,
    // sleep for a while...
    while(1)
    {
      //while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
      while(PeekMessage(&msg,keyboard->m_hWnd,0,0,PM_REMOVE))
      {
         // Since we have no accelerators, no need to call
         // TranslateAccelerator here.
         TranslateMessage (&msg);
         DispatchMessage (&msg);
      }

      Sleep(100);    // Sleep 1/10 sec
    }

}

int KeyboardWin32::onlyModifier(int mod)
{
  switch (mod) {
     case VJKEY_NONE:
        return (!m_keys[VJKEY_SHIFT] && !m_keys[VJKEY_CTRL] && !m_keys[VJKEY_ALT]);
     case VJKEY_SHIFT:
        return (m_keys[VJKEY_SHIFT] && !m_keys[VJKEY_CTRL] && !m_keys[VJKEY_ALT]);
     case VJKEY_CTRL:
        return (!m_keys[VJKEY_SHIFT] && m_keys[VJKEY_CTRL] && !m_keys[VJKEY_ALT]);
     case VJKEY_ALT:
        return (!m_keys[VJKEY_SHIFT] && !m_keys[VJKEY_CTRL] && m_keys[VJKEY_ALT]);
     default:
       vjDEBUG(vjDBG_INPUT_MGR, vjDBG_CONFIG_LVL) << instName << ": OnlyModifier: bad modifier key" << vjDEBUG_FLUSH;
       return 0;
  }
}

void KeyboardWin32::updateData()
{
   //  UpdKeys();
   // update what the last mouse x/y values were.
   oldx = newx;
   oldy = newy;

/*
   m_keys[VJKEY_CTRL] = GetAsyncKeyState(VK_CONTROL);
   m_keys[VJKEY_ALT] = GetAsyncKeyState(VK_TAB);  // (no alt key is defined for VK_ keys)
   m_keys[VJKEY_SHIFT] = GetAsyncKeyState(VK_SHIFT);

   if (m_keys[VJKEY_CTRL] || m_keys[VJKEY_ALT] || m_keys[VJKEY_SHIFT])
   {
      vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL) << instName << ": Modifier key is down\n" << vjDEBUG_FLUSH;
   }
*/

   // end of updatedata, reset the keydata to the actual keystate (0/1)
   m_realkeys[VJKEY_NONE] = 0; // key-none should always be 0
   for(int q = 0; q < 256; q++)
   {
      m_keys[q] = m_framekeys[q];      // Set the keys from the previous frame of keys
      m_framekeys[q] = m_realkeys[q];
   }

   // InvalidateRect(m_hWnd,NULL,TRUE);
}

// UpdKeys: translates windows message into key updates
// The WNDPROC uses its USERDATA pointer to the keyboard
// to forward on messages to be handled from in the keyboard object.
void KeyboardWin32::updKeys(UINT message, UINT wParam, LONG lParam)
{
   //vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL) << instName << ": KeyWin32::updKeys: Processing keys.\n" << vjDEBUG_FLUSH;

   int key;
   //static HWND lCapture;
   //static int  fHaveCapture = FALSE;

   switch (message)
   {
        case WM_ACTIVATE:
//                if (!fHaveCapture)
//                {
                        // Whenever the window is activated capture the mouse
                        // to receive mousemove messages when its not in the windows client area.
//                        lCapture = SetCapture(m_hWnd);
//                        fHaveCapture = TRUE;
//                }
                break;
        case WM_CAPTURECHANGED:
                // Another window was activated or another process called
                // releasecapture.  We're no longer getting mousemove messages
//                fHaveCapture = FALSE;

                break;
        case  WM_KEYDOWN:
                key = VKKeyToKey(wParam);
                m_realkeys[key] = 1;
                m_framekeys[key] += 1;
                vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL) << instName
                           << ": WM_KEYDOWN: " << key << ": "
                           << getKeyName(key).c_str() << std::endl
                           << vjDEBUG_FLUSH;
                break;

        case WM_KEYUP:
                key = VKKeyToKey(wParam);
                m_realkeys[key] = 0;
                vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL) << instName
                           << ": WM_KEYUP: " << key << std::endl
                     << vjDEBUG_FLUSH;
                break;

      // mouse buttons
      // When we hit a key, then set the frame_key and real
      // When release, only set real so that we don't lose a press of the
      // button then when the updateData happens, the framekeys will be set to
      // non-pressed from real_keys
        case WM_LBUTTONDOWN:
                m_framekeys[VJMBUTTON1] = 1;
                m_realkeys[VJMBUTTON1] = 1;
                vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL) << "LeftButtonDown\n"
                           << vjDEBUG_FLUSH;
                break;
        case WM_LBUTTONUP:
                m_realkeys[VJMBUTTON1] = 0;
                vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL) << "LeftButtonUp\n"
                           << vjDEBUG_FLUSH;
                break;

        case WM_MBUTTONDOWN:
                m_framekeys[VJMBUTTON2] = 1;
                m_realkeys[VJMBUTTON2] = 1;
                vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL)
                           << "MiddleButtonDown\n" << vjDEBUG_FLUSH;
                break;
        case WM_MBUTTONUP:
                m_realkeys[VJMBUTTON2] = 0;
                vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL) << "MiddleButtonUp\n"
                           << vjDEBUG_FLUSH;
                break;

        case WM_RBUTTONDOWN:
                m_framekeys[VJMBUTTON3] = 1;
                m_realkeys[VJMBUTTON3] = 1;
                vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL)
                           << "RightButtonDown\n" << vjDEBUG_FLUSH;
                break;
        case WM_RBUTTONUP:
                m_realkeys[VJMBUTTON3] = 0;
                vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL) << "RightButtonUp\n"
                           << vjDEBUG_FLUSH;
                break;

        // mouse movement
        case WM_MOUSEMOVE:
                // We can receive mouse messages when moving OVER our window,
                // but when our window is not the active window.  We will
                // only process the move messages when we're the active
                // window.  (allows multiple keyboard sims to use mousemove)
//                if (!fHaveCapture)
//                        break;
                newx = GET_X_LPARAM(lParam);
                newy = GET_Y_LPARAM(lParam);

                if (oldx == 0xfffff || oldy == 0xfffff)
                {      // the first message is lost to init the center
                        oldx = newx;
                        oldy = newy;
                }
                else {
                   // We can only have one of pos/neg actually have a value,
                   // because we don't want negative values
                   if ((newx - oldx) >= 0)       // Moving right
                      m_framekeys[VJMOUSE_POSX] += ((newx - oldx) * m_mouse_sensitivity);
                   else
                      m_framekeys[VJMOUSE_POSX] = 0;

                   if ((oldx - newx) >= 0)       // Moving left
                      m_framekeys[VJMOUSE_NEGX] += ((oldx - newx) * m_mouse_sensitivity);
                   else
                      m_framekeys[VJMOUSE_NEGX] = 0;

                   if ((newy - oldy) >= 0)        // Moving up
                      m_framekeys[VJMOUSE_POSY] += ((newy - oldy) * m_mouse_sensitivity);
                   else
                      m_framekeys[VJMOUSE_POSY] = 0;

                   if ((oldy - newy) > 0)        // Moving down
                      m_framekeys[VJMOUSE_NEGY] += ((oldy - newy) * m_mouse_sensitivity);
                   else
                      m_framekeys[VJMOUSE_NEGY] = 0;

                        // reset the oldx oldy values in UpdateData
                        //  oldx = newx; oldy = newy;

                    vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL) << "move PosX: "
                               << m_framekeys[VJMOUSE_POSX] << std::endl
                               << vjDEBUG_FLUSH;
                    vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL) << "move NegX: "
                               << m_framekeys[VJMOUSE_NEGX] << std::endl
                               << vjDEBUG_FLUSH;
                    vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL) << "move PosY: "
                               << m_framekeys[VJMOUSE_POSY] << std::endl
                               << vjDEBUG_FLUSH;
                    vjDEBUG(vjDBG_INPUT_MGR, vjDBG_HVERB_LVL) << "move NegY: "
                               << m_framekeys[VJMOUSE_NEGY] << std::endl
                               << vjDEBUG_FLUSH;
                }

                break;
        }
}


int KeyboardWin32::stopSampling()
{
   if (myThread != NULL)
   {
      myThread->kill();
      myThread = NULL;
      std::cout << "Stoppping Keyboard.." << std::endl;
   }
   return 1;

}

int KeyboardWin32::VKKeyToKey(int vkKey)
{
   switch (vkKey)
   {
     case VK_UP       : return VJKEY_UP;
     case VK_DOWN     : return VJKEY_DOWN;
     case VK_LEFT     : return VJKEY_LEFT;
     case VK_RIGHT    : return VJKEY_RIGHT;
     case VK_CONTROL  : return VJKEY_CTRL;
     case VK_SHIFT    : return VJKEY_SHIFT;
     case VK_TAB      : return VJKEY_ALT;
//     case VK_ALT      : return KEY_ALT;
     case /*VK_1*/0x31  : return VJKEY_1;
     case VK_NUMPAD1    : return VJKEY_1;
     case /*VK_2*/0x32  : return VJKEY_2;
     case VK_NUMPAD2    : return VJKEY_2;
     case /*VK_3*/0x33 : return VJKEY_3;
     case VK_NUMPAD3    : return VJKEY_3;
     case /*VK_4*/0x34  : return VJKEY_4;
     case VK_NUMPAD4    : return VJKEY_4;
     case /*VK_5*/0x35 : return VJKEY_5;
     case VK_NUMPAD5    : return VJKEY_5;
     case /*VK_6*/0x36  : return VJKEY_6;
     case VK_NUMPAD6    : return VJKEY_6;
     case /*VK_7*/0x37  : return VJKEY_7;
     case VK_NUMPAD7    : return VJKEY_7;
     case /*VK_8*/0x38  : return VJKEY_8;
     case VK_NUMPAD8    : return VJKEY_8;
     case /*VK_9*/0x39  : return VJKEY_9;
     case VK_NUMPAD9    : return VJKEY_9;
     case /*VK_0*/0x30  : return VJKEY_0;
     case VK_NUMPAD0    : return VJKEY_0;
     case /*VK_A*/0x41    : return VJKEY_A;
     case /*VK_B*/0x42    : return VJKEY_B;
     case /*VK_C*/0x43    : return VJKEY_C;
     case /*VK_D*/0x44    : return VJKEY_D;
     case /*VK_E*/0x45    : return VJKEY_E;
     case /*VK_F*/0x46    : return VJKEY_F;
     case /*VK_G*/0x47    : return VJKEY_G;
     case /*VK_H*/0x48    : return VJKEY_H;
     case /*VK_I*/0x49    : return VJKEY_I;
     case /*VK_J*/0x4a  : return VJKEY_J;
     case /*VK_K*/0x4b    : return VJKEY_K;
     case /*VK_L*/0x4c    : return VJKEY_L;
     case /*VK_M*/0x4d    : return VJKEY_M;
     case /*VK_N*/0x4e    : return VJKEY_N;
     case /*VK_O*/0x4f    : return VJKEY_O;
     case /*VK_P*/0x50    : return VJKEY_P;
     case /*VK_Q*/0x51    : return VJKEY_Q;
     case /*VK_R*/0x52    : return VJKEY_R;
     case /*VK_S*/0x53    : return VJKEY_S;
     case /*VK_T*/0x54    : return VJKEY_T;
     case /*VK_U*/0x55    : return VJKEY_U;
     case /*VK_V*/0x56    : return VJKEY_V;
     case /*VK_W*/0x57    : return VJKEY_W;
     case /*VK_X*/0x58    : return VJKEY_X;
     case /*VK_Y*/0x59    : return VJKEY_Y;
     case /*VK_Z*/0x5a    : return VJKEY_Z;
     case VK_ESCAPE   : return VJKEY_ESC;
     default: return 255;
   }

}

/*****************************************************************/
/*****************************************************************/
/***********************WINDOW STUFF****************************/
/*****************************************************************/
/*****************************************************************/

//#ifdef DEBUG
#define NEGADD1(x,y)  ((x < 0) ? x+1 : x)

void ThreeDouble2String(char* sz, double f1, double f2, double f3)
{
        wsprintf(sz,"%s%03d.%3.3d %s%03d.%3.3d %s%03d.%3.3d",
                (f1<0?"-":""),abs((int)f1),
                        abs((int)(f1*1000)-(((int)(f1))*1000)),
                (f2<0?"-":""),abs((int)f2),
                        abs((int)(f2*1000)-(((int)(f2))*1000)),
                (f3<0?"-":""),abs((int)f3),
                        abs((int)(f3*1000)-(((int)(f3))*1000)));
//        OutputDebugString(sz);
//        OutputDebugString("\n");
}



/****************************************************************************
 *                                                                          *
 *  FUNCTION   : HandlePaint ( hwnd )                                       *
 *                                                                          *
 *  PURPOSE    : Handles the repainting of window client area.              *
 *               (Outputs the data current as of the last UpdateData call   *
 ****************************************************************************/
VOID APIENTRY HandlePaint (HWND hwnd)
{
    HDC         hdc;
    PAINTSTRUCT ps;
    Keyboard* devPtr = (Keyboard*)GetWindowLong(hwnd,GWL_USERDATA);

    if (!devPtr)
        return;

    char ln1[255] = "line1";
    char ln2[255] = "line2";
    char ln3[255] = "line3";
    char ln4[255] = "line4";
    char ln5[255] = "line5";
    char ln6[255] = "line6";
    //vjMatrix* McurrData = devPtr->GetPosData();

    //vjCoord coord(*McurrData);
    //ThreeDouble2String(ln1, coord.pos[0], coord.pos[1], coord.pos[2]);
    //ThreeDouble2String(ln2, coord.orient[0], coord.orient[1], coord.orient[2]);

    //vjMcurrData = devPtr->GetPosData(1);
    //vjCoord coord2(*McurrData);
    //ThreeDouble2String(ln3, coord2.pos[0], coord2.pos[1], coord2.pos[2]);
    //ThreeDouble2String(ln4, coord2.orient[0], coord2.orient[1], coord2.orient[2]);

    //wsprintf(ln5,"%li %d %d %d", devPtr->GetDigitalData(0), devPtr->GetDigitalData(1),devPtr->GetDigitalData(2),devPtr->GetDigitalData(3));
    //wsprintf(ln6,"%li %d %d %d", devPtr->GetAnalogData(0), devPtr->GetAnalogData(1),devPtr->GetAnalogData(2),devPtr->GetAnalogData(3));

    hdc = BeginPaint (hwnd, (LPPAINTSTRUCT)&ps);

    TextOut(hdc,0 ,0, "Pos0:", 5);
    TextOut(hdc,50,0,ln1, lstrlen(ln1));
    TextOut(hdc,50,20,ln2, lstrlen(ln2));

    /*
    TextOut(hdc,0 ,40, "Pos1:", 5);
    TextOut(hdc,50,40,ln3, lstrlen(ln3));
    TextOut(hdc,50,60,ln4, lstrlen(ln4));

    TextOut(hdc,0 ,90, "Digital:", 8);
    TextOut(hdc,50,90,ln5, lstrlen(ln5));

    TextOut(hdc,0 ,110, "Analog:", 7);
    TextOut(hdc,50,110,ln6, lstrlen(ln6));
   */

   EndPaint(hwnd, (LPPAINTSTRUCT)&ps);
}
//#endif // DEBUG

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : MenuWndProc (hWnd, message, wParam, lParam)                *
 *                                                                          *
 *  PURPOSE    : Window function for the main app. window. Processes all the*
 *               menu selections and oter messages.                         *
 *                                                                          *
 ****************************************************************************/
LONG APIENTRY MenuWndProc (HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
    switch (message) {
        case WM_SYSCOMMAND:
              return DefWindowProc (hWnd, message, wParam, lParam);

//        case WM_COMMAND:
//            break;

        case WM_SIZE:
            if (lParam){
                InvalidateRect (hWnd, NULL, TRUE);
            }
            break;

//#ifdef DEBUG  // Show the pos/dig/analog data in the window in debug
                // (really inefficient display code)
        case WM_PAINT:
            HandlePaint (hWnd);
            break;
//#endif

        case WM_DESTROY:
            PostQuitMessage (0);
            break;

        default:
            KeyboardWin32* devPtr = (KeyboardWin32*)GetWindowLong(hWnd,GWL_USERDATA);
            if (devPtr)
                devPtr->updKeys(message, wParam,lParam);
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return(0);
}


void KeyboardWin32::createWindowWin32 ()
{
   int root_height;

   InitCommonControls();

   //m_hInst = g_hInst;
   m_hInst = GetModuleHandle(NULL);    // Just try to get the application's handle
   MenuInit(m_hInst);

   root_height = GetSystemMetrics(SM_CYSCREEN);

   /* Create the app. window */
   m_hWnd = CreateWindow(("Juggler Keyboard"), instName.c_str(), 
                         WS_OVERLAPPEDWINDOW, m_x, 
                         root_height - m_y - m_height, m_width, m_height,
                         (HWND) NULL, NULL, m_hInst, (LPSTR) NULL);
   ShowWindow(m_hWnd,SW_SHOW);
   UpdateWindow(m_hWnd);

   // Attach a pointer to the device for use from the WNDPROC
   SetWindowLong(m_hWnd, GWL_USERDATA, (LPARAM)this);

   DWORD dwErr = GetLastError();
   if (!m_hWnd)
      return;

} /*CreateWindow*/

BOOL KeyboardWin32::MenuInit (HINSTANCE hInstance)
{
    HANDLE    hMemory;
    PWNDCLASS pWndClass;
    BOOL      bSuccess;

    /* Initialize the menu window class */
    hMemory   = LocalAlloc(LPTR, sizeof(WNDCLASS));
    if(!hMemory){
        MessageBox(NULL, ("<MenuInit> Not enough memory."), NULL, MB_OK | MB_ICONHAND);
        return(FALSE);
    }

    pWndClass = (PWNDCLASS) LocalLock(hMemory);

    pWndClass->style         = 0;
    pWndClass->lpfnWndProc   = (WNDPROC) MenuWndProc;
    pWndClass->hInstance     = hInstance;
//    pyrate_x = (HICON)LoadIcon(hInstance, _T("prof"));
    pWndClass->hIcon         = (HICON)LoadIcon(hInstance,TEXT("Juggler"));
    pWndClass->hCursor       = (HCURSOR)LoadCursor (NULL, IDC_ARROW);
    pWndClass->hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
    pWndClass->lpszMenuName  = ( "MenuMenu"),
    pWndClass->lpszClassName = ("Juggler Keyboard");

    bSuccess = RegisterClass (pWndClass);
    LocalUnlock (hMemory);
    LocalFree (hMemory);

    return bSuccess;
}


};
