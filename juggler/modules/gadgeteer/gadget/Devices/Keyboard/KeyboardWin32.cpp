#include <Input\vjInput\vjKeyboardWin32.h>
#include <sys\types.h>
#include <Math\vjCoord.h>

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))
#endif

vjKeyboard::vjKeyboard(vjConfigChunk *c) : vjPosition(c), vjDigital(c),
				vjAnalog(c), vjInput(c)
{
    cout << "     vjKeyboard::vjKeyboard(vjConfigChunk*c) " << endl;
    myThreadID = 0;
    int i;
    for(i =0; i < 256; i++)
         m_realkeys[i] = m_keys[i] = 0;
    //m_realkeys[0] = m_keys[0] = 1;
    
    m_width = (int)c->getProperty("width");
    m_height = (int)c->getProperty("height");
    
    if (m_width == 0) m_width = 400;
    if (m_height == 0) m_height = 400;
	newx = oldx = 0xfffff;
    newy = oldy = 0xfffff;

    m_toggleoff = c->getProperty("toggleoff");
    m_anastep   = c->getProperty("anastep");
    if (m_anastep == 0) m_anastep = 2;
    
    m_dtrans = c->getProperty("dtrans");
    if (0 == m_dtrans) m_dtrans = 0.1;
    m_drot   = c->getProperty("drot");
    if (0 == m_drot) m_drot = 1;
    m_mouse_sensitivity = c->getProperty("msens");
    if (0 == m_mouse_sensitivity) m_mouse_sensitivity = 0.5;
    
    cout << "Mouse Sensititivty: " << m_mouse_sensitivity << endl;
      
    for (i = 0; i < NUM_POS_CONTROLS; i++)
    { 
      m_pos0key[i] = c->getProperty("pos0keys",i);       
      m_pos0mod[i] = c->getProperty("pos0mods",i);        
      m_pos1key[i] = c->getProperty("pos1keys",i);       
      m_pos1mod[i] = c->getProperty("pos1mods",i);    
    }
    
    for(int z = 0; z < 4; z++)
    {
      m_digkeys[z] = c->getProperty("digkeys",z);
      m_digmods[z] = c->getProperty("digmods",z);
      m_anakeysup[z] = c->getProperty("anakeysup",z);
      m_anamodsup[z] = c->getProperty("anamodsup",z);
      m_anakeysdn[z] = c->getProperty("anakeysdn",z);
      m_anamodsdn[z] = c->getProperty("anamodsdn",z);
      
      // still needed this.  Got rid of this hack
      if (m_digmods[z] != 0) m_digmods[z] += 4;
      if (m_anamodsup[z] != 0) m_anamodsup[z] += 4;
      if (m_anamodsdn[z] != 0) m_anamodsdn[z] += 4; 

	  m_digdata[z] = 0;
	  m_anadata[z] = 0;
    }
    
    // still needed (?) Got rid of this hack
    for(int q = 0; q < NUM_POS_CONTROLS; q++)
    {  
       if (m_pos0mod[q] != 0)  // if the key is not NONE, then add 4 to get it
          m_pos0mod[q] += 4;   // to the SHIFT/CTRL/ALT key
       if (m_pos1mod[q] != 0)
          m_pos1mod[q] += 4;
    }

    for (int n = 0; n < 2; n++)
      m_posdata[n].makeIdent();
     
    cout << "        Keyboard config: " << endl
         << "           Forward :" << m_pos0key[FORWARD] << "  " << m_pos0mod[FORWARD] << endl
	      << "           Backward:" << m_pos0key[BACK] << "  " << m_pos0mod[BACK] << endl
	      << "           Left    :" << m_pos0key[LEFT] << "  " << m_pos0mod[LEFT] << endl
	      << "           Right   :" << m_pos0key[RIGHT] << "  " << m_pos0mod[RIGHT] << endl
	      << "           UpKey   :" << m_pos0key[UP]    << "  " << m_pos0mod[UP]    << endl
	      << "           DownKey :" << m_pos0key[DOWN]  << "  " << m_pos0mod[DOWN]  << endl
	      << "           Rot R   :" << m_pos0key[ROTR]<<"  "<<m_pos0mod[ROTR] << endl
	      << "           Rot L   :" << m_pos0key[ROTL] <<"  "<<m_pos0mod[ROTL]  << endl
	      << "           Rot U   :" << m_pos0key[ROTU]   <<"  "<<m_pos0mod[ROTU]    << endl
	      << "           Rot D   :" << m_pos0key[ROTD] <<"  "<<m_pos0mod[ROTD]  << endl
         << "           Rot CCW :" << m_pos0key[ROT_ROLL_CCW] << "   " << m_pos0mod[ROT_ROLL_CCW] << endl
         << "           Rot CW  :" << m_pos0key[ROT_ROLL_CW] << "   " << m_pos0mod[ROT_ROLL_CW] << endl
           << endl;}

int vjKeyboard::StartSampling()
{ 

if (myThreadID == 0) {
  int processID;
  int i;
  
  current = 0;
  valid = 1;
  progress = 2;
  
  m_x = 100; m_y = 200; 
  
     

  

  cerr << "vjKeyboard::StartSampling() : ready to go.." << endl;
  
  vjKeyboard* devicePtr = this;
  void Samplem_keys(void*);
  
    if (0 == (myThreadID = vjThread::spawn(Samplem_keys,(void*)devicePtr,0))) 
     return 0; //fail
  else {
    //samplePID = processID;
    return 1;
    } 

     
  }else return 0; // already sampling


}

void Samplem_keys(void* devPtr)
{
	MSG msg;
	// hack:
	// The Window has to be created in the same thread that
	// we run the message pump because all window messages
	// dispatched are dispatched to the thread that created
	// the window.  (And we want to receive the messages
	// in the spawned thread)
    ((vjKeyboard*)devPtr)->CreateWindowWin32 ();

	while (GetMessage (&msg, NULL, 0, 0)){
	/* Since we have no accelerators, no need to call
	 * TranslateAccelerator here.
	 */
	TranslateMessage (&msg);
	DispatchMessage (&msg);
    }



}

int vjKeyboard::OnlyModifier(int mod)
{
  switch (mod) {
     case vjKEY_NONE:
        return (!m_keys[vjKEY_SHIFT] && !m_keys[vjKEY_CTRL] && !m_keys[vjKEY_ALT]);
     case vjKEY_SHIFT:
        return (m_keys[vjKEY_SHIFT] && !m_keys[vjKEY_CTRL] && !m_keys[vjKEY_ALT]);
     case vjKEY_CTRL:
        return (!m_keys[vjKEY_SHIFT] && m_keys[vjKEY_CTRL] && !m_keys[vjKEY_ALT]);
     case vjKEY_ALT:
        return (!m_keys[vjKEY_SHIFT] && !m_keys[vjKEY_CTRL] && m_keys[vjKEY_ALT]);
     default:
       OutputDebugString(instName);
	   OutputDebugString(": OnlyModifier: bad modifier key");
       return 0;
  } 
  

}

void vjKeyboard::UpdateData()
{ 
  int i;
//  UpdKeys();
    // update what the last mouse x/y values were.
	oldx = newx;
	oldy = newy;

	m_keys[vjKEY_CTRL] = GetAsyncKeyState(VK_CONTROL);
	m_keys[vjKEY_ALT] = GetAsyncKeyState(VK_TAB);  // (no alt key is defined for VK_ keys)
	m_keys[vjKEY_SHIFT] = GetAsyncKeyState(VK_SHIFT);

/*	if (m_keys[vjKEY_CTRL] || m_keys[vjKEY_ALT] || m_keys[vjKEY_SHIFT])
	{	OutputDebugString(instName);
		OutputDebugString(": Modifier key is down\n");
	}*/

  // --- Update position 0 --- //
  if (m_keys[m_pos0key[FORWARD]] && OnlyModifier(m_pos0mod[FORWARD]))
      MoveFor( 1 * m_keys[m_pos0key[FORWARD]] , 0);
  if (m_keys[m_pos0key[BACK]] && OnlyModifier(m_pos0mod[BACK]))
      MoveFor( -1 * m_keys[m_pos0key[BACK]] , 0 );
  if (m_keys[m_pos0key[LEFT]] && OnlyModifier(m_pos0mod[LEFT]))
      MoveLeft( 1 * m_keys[m_pos0key[LEFT]] , 0 );
  if (m_keys[m_pos0key[RIGHT]] && OnlyModifier(m_pos0mod[RIGHT]))
      MoveLeft( -1 * m_keys[m_pos0key[RIGHT]] , 0);
  if (m_keys[m_pos0key[UP]] && OnlyModifier(m_pos0mod[UP]))
      MoveUp ( 1 * m_keys[m_pos0key[UP]] , 0);
  if (m_keys[m_pos0key[DOWN]] && OnlyModifier(m_pos0mod[DOWN]))
      MoveUp (-1 * m_keys[m_pos0key[DOWN]] , 0 );
      
  if (m_keys[m_pos0key[ROTR]] && OnlyModifier(m_pos0mod[ROTR]))
      RotLeft( -1 * m_keys[m_pos0key[ROTR]] , 0);
  if (m_keys[m_pos0key[ROTL]] && OnlyModifier(m_pos0mod[ROTL]))
      RotLeft( 1  * m_keys[m_pos0key[ROTL]] , 0);
  if (m_keys[m_pos0key[ROTU]] && OnlyModifier(m_pos0mod[ROTU]))
      RotUp( 1 * m_keys[m_pos0key[ROTU]] , 0);
  if (m_keys[m_pos0key[ROTD]] && OnlyModifier(m_pos0mod[ROTD]))
      RotUp( -1 * m_keys[m_pos0key[ROTD]] , 0);
  if (m_keys[m_pos0key[ROT_ROLL_CCW]] && OnlyModifier(m_pos0mod[ROT_ROLL_CCW]))
      RotRollCCW( 1 * m_keys[m_pos0key[ROT_ROLL_CCW]] , 0);
  if (m_keys[m_pos0key[ROT_ROLL_CW]] && OnlyModifier(m_pos0mod[ROT_ROLL_CW]))
      RotRollCCW( -1 * m_keys[m_pos0key[ROT_ROLL_CW]] , 0); 

  // --- Update position 1 --- //
  if (m_keys[m_pos1key[FORWARD]] && OnlyModifier(m_pos1mod[FORWARD]))
      MoveFor( 1 * m_keys[m_pos1key[FORWARD]] , 1);
  if (m_keys[m_pos1key[BACK]] && OnlyModifier(m_pos1mod[BACK]))
      MoveFor( -1 * m_keys[m_pos1key[BACK]] , 1 );
  if (m_keys[m_pos1key[LEFT]] && OnlyModifier(m_pos1mod[LEFT]))
      MoveLeft( 1 * m_keys[m_pos1key[LEFT]] , 1 );
  if (m_keys[m_pos1key[RIGHT]] && OnlyModifier(m_pos1mod[RIGHT]))
      MoveLeft( -1 * m_keys[m_pos1key[RIGHT]] , 1);
  if (m_keys[m_pos1key[UP]] && OnlyModifier(m_pos1mod[UP]))
      MoveUp ( 1 * m_keys[m_pos1key[UP]] , 1);
  if (m_keys[m_pos1key[DOWN]] && OnlyModifier(m_pos1mod[DOWN]))
      MoveUp (-1 * m_keys[m_pos1key[DOWN]] , 1 );
      
  if (m_keys[m_pos1key[ROTR]] && OnlyModifier(m_pos1mod[ROTR]))
      RotLeft( -1 * m_keys[m_pos1key[ROTR]] , 1);
  if (m_keys[m_pos1key[ROTL]] && OnlyModifier(m_pos1mod[ROTL]))
      RotLeft( 1  * m_keys[m_pos1key[ROTL]] , 1);
  if (m_keys[m_pos1key[ROTU]] && OnlyModifier(m_pos1mod[ROTU]))
      RotUp( 1 * m_keys[m_pos1key[ROTU]] , 1);
  if (m_keys[m_pos1key[ROTD]] && OnlyModifier(m_pos1mod[ROTD]))
      RotUp( -1 * m_keys[m_pos1key[ROTD]] , 1);
  if (m_keys[m_pos1key[ROT_ROLL_CCW]] && OnlyModifier(m_pos1mod[ROT_ROLL_CCW]))
      RotRollCCW( 1 * m_keys[m_pos1key[ROT_ROLL_CCW]] , 1);
  if (m_keys[m_pos1key[ROT_ROLL_CW]] && OnlyModifier(m_pos1mod[ROT_ROLL_CW]))
      RotRollCCW( -1 * m_keys[m_pos1key[ROT_ROLL_CW]] , 1); 

  // -- Update digital data -- //
  if (m_toggleoff)
  {
    for (i = 0; i < 4; i++)
      if (m_keys[m_digkeys[i]] && OnlyModifier(m_digmods[i]))
        m_digdata[i] = 1;
      else
        m_digdata[i] = 0;
  } else
  {
    for (i = 0; i < 4; i++)
      if (m_keys[m_digkeys[i]] && OnlyModifier(m_digmods[i]))
		  m_digdata[i] = !m_digdata[i]; 
  } 
      
  // -- Update analog data --- //
  for (i = 0; i < 4; i++)
  {
      m_anadata[i] += m_keys[m_anakeysup[i]] * OnlyModifier(m_anamodsup[i]) * m_anastep;
      m_anadata[i] -= m_keys[m_anakeysdn[i]] * OnlyModifier(m_anamodsdn[i]) * m_anastep;
  
      if (m_anadata[i] < 0) m_anadata[i] = 0;
      if (m_anadata[i] > 255) m_anadata[i] = 255;
  }
 
  for (i = 0; i < 4; i++)
  {
    cout << "Digital " << i << ": " << m_digdata[i]
               << "\tAnalog  " << i << ": " << m_anadata[i] << endl;
  }


   // end of updatedata, reset the keydata to the actual keystate (0/1)
   m_realkeys[vjKEY_NONE] = 0; // keynone should always be 0
   for(int q = 0; q < 256; q++)
      m_keys[q] = m_realkeys[q];

   InvalidateRect(m_hWnd,NULL,TRUE);
}

// Move forward the given amount on position data n
// Forward is in th -Z direction
void vjKeyboard::MoveFor(float amt, int n)
{
   OutputDebugString(instName);
   OutputDebugString(": MoveFor\n");
   m_posdata[n].postTrans(m_posdata[n], 0.0, 0.0, -amt*m_dtrans);
}

// Move left the given amount on position data n
// Left is -X dir
void vjKeyboard::MoveLeft(float amt, int n)
{
   OutputDebugString(instName);
   OutputDebugString(": MoveLeft\n");
    m_posdata[n].postTrans(m_posdata[n], -amt*m_dtrans, 0.0, 0.0);  
}

// Move up the given amount on position data n
// Up is in th +Y dir
void vjKeyboard::MoveUp(float amt, int n)
{
	   OutputDebugString(instName);
   OutputDebugString(": MoveUp\n");
	m_posdata[n].postTrans(m_posdata[n], 0.0, amt*m_dtrans, 0.0);
}

// Pitch up - rot +x axis
void vjKeyboard::RotUp(float amt, int n)
{
	   OutputDebugString(instName);
   OutputDebugString(": RotUp\n");
   static vjVec3 x_axis(1.0,0.0,0.0);
   m_posdata[n].postRot(m_posdata[n], amt*m_drot, x_axis);
}

// Yaw left - rot +Y axis
void vjKeyboard::RotLeft(float amt, int n)
{
	   OutputDebugString(instName);
   OutputDebugString(": RotLeft\n");
	static vjVec3 y_axis(0.0, 1.0, 0.0);
   m_posdata[n].postRot(m_posdata[n], amt*m_drot, y_axis);
}

// Roll Left - rot -z axis
void vjKeyboard::RotRollCCW(float amt, int n)
{
	   OutputDebugString(instName);
   OutputDebugString(": RotRollCCw\n");
   static vjVec3 neg_z_axis(0.0, 0.0, -1.0);
   m_posdata[n].postRot(m_posdata[n], amt*m_drot, neg_z_axis); 
}

// UpdKeys: translates windows message into key updates
// The WNDPROC uses its USERDATA pointer to the keyboard
// to forward on messages to be handled from in the keyboard object.
void vjKeyboard::UpdKeys(	UINT message,	UINT wParam,
	LONG lParam)
{
	int key;
	static HWND lCapture;
	static int  fHaveCapture = FALSE;
	switch (message)
	{
	case WM_ACTIVATE:
		if (!fHaveCapture)
		{
			// Whenever the window is activated capture the mouse
			// to receive mousemove messages when its not in the windows client area.
//			lCapture = SetCapture(m_hWnd);
//			fHaveCapture = TRUE;
		}
		break;
	case WM_CAPTURECHANGED:
		// Another window was activated or another process called
		// releasecapture.  We're no longer getting mousemove messages
//		fHaveCapture = FALSE;
			
		break;
	case  WM_KEYDOWN:
		key = VKKeyTovjKey(wParam);
        m_realkeys[key] = 1;
        m_keys[key] += 1;	
		break;

	case WM_KEYUP:
		key = VKKeyTovjKey(wParam);
        m_realkeys[key] = 0;
		break;

		// mouse buttons
	case WM_LBUTTONDOWN:
		m_keys[VJMBUTTON1] = 1;
		break;
	case WM_LBUTTONUP:
		m_keys[VJMBUTTON1] = 0;
		break;

	case WM_MBUTTONDOWN:
		m_keys[VJMBUTTON2] = 1;
		break;
	case WM_MBUTTONUP:
		m_keys[VJMBUTTON2] = 0;
		break;
	
	case WM_RBUTTONDOWN:
		m_keys[VJMBUTTON3] = 1;
		break;
	case WM_RBUTTONUP:
		m_keys[VJMBUTTON3] = 0;
		break;

		// mouse movement
	case WM_MOUSEMOVE:

		// We can receive mouse messages when moving OVER our window, but when our window
		// is not the active window.  We will only process the move messages when we're the
		// active window.  (allows multiple keyboard sims to use mousemove)
//		if (!fHaveCapture)
//			break;
		newx = GET_X_LPARAM(lParam);
		newy = GET_Y_LPARAM(lParam);
		
		if (oldx == 0xfffff || oldy == 0xfffff)
		{	// the first message is lost to init the center
			oldx = newx;
			oldy = newy;
		} 
		else {

		   if ((newx - oldx) > 0) 
		      m_keys[VJMOUSE_POSX] = (newx - oldx) * m_mouse_sensitivity;
		   else
		      m_keys[VJMOUSE_POSX] = 0;
   
		   if ((newx - oldx) < 0) 
		      m_keys[VJMOUSE_NEGX] = -(newx - oldx) * m_mouse_sensitivity;
		   else
		      m_keys[VJMOUSE_NEGX] = 0;
      
		   if ((newy - oldy) > 0) 
		      m_keys[VJMOUSE_POSY] = (newy - oldy) * m_mouse_sensitivity;
		   else
		      m_keys[VJMOUSE_POSY] = 0;
   
		   if ((newy - oldy) > 0) 
		      m_keys[VJMOUSE_NEGY] = -(newy - oldy) * m_mouse_sensitivity;
		   else
		      m_keys[VJMOUSE_NEGY] = 0;

			// reset the oldx oldy values in UpdateData
			//  oldx = newx; oldy = newy;
			}
		break;

	}
}


int vjKeyboard::StopSampling()
{
  if (myThreadID != 0)
  {
    //vjThread::kill(myThreadID);
    myThreadID = 0;
    cout << "Stoppping Keyboard.." << endl;
//    SleepEx(100);
    
    
 // XDestroyWindow(m_display,m_window);
 // XFree(m_visual);
//  XCloseDisplay((Display*) m_display);
  }


  return 1;

}

int vjKeyboard::VKKeyTovjKey(int vkKey)
{
   switch (vkKey)
   {
     case VK_UP       : return vjKEY_UP;
     case VK_DOWN     : return vjKEY_DOWN;
     case VK_LEFT     : return vjKEY_LEFT;
     case VK_RIGHT    : return vjKEY_RIGHT;
     case VK_CONTROL  : return vjKEY_CTRL;
     case VK_SHIFT    : return vjKEY_SHIFT;
//     case VK_ALT      : return vjKEY_ALT;
     case /*VK_1*/0x31  : return vjKEY_1;
     case /*VK_2*/0x32  : return vjKEY_2;
     case /*VK_3*/0x33 : return vjKEY_3;
     case /*VK_4*/0x34  : return vjKEY_4;
     case /*VK_5*/0x35 : return vjKEY_5;
     case /*VK_6*/0x36  : return vjKEY_6;
     case /*VK_7*/0x37  : return vjKEY_7;
     case /*VK_8*/0x38  : return vjKEY_8;
     case /*VK_9*/0x39  : return vjKEY_9;
     case /*VK_0*/0x30  : return vjKEY_0;
     case /*VK_A*/0x41    : return vjKEY_A;
     case /*VK_B*/0x42    : return vjKEY_B;
     case /*VK_C*/0x43    : return vjKEY_C;
     case /*VK_D*/0x44    : return vjKEY_D;
     case /*VK_E*/0x45    : return vjKEY_E;
     case /*VK_F*/0x46    : return vjKEY_F;
     case /*VK_G*/0x47    : return vjKEY_G;
     case /*VK_H*/0x48    : return vjKEY_H;
     case /*VK_I*/0x49    : return vjKEY_I;
     case /*VK_J*/0x4a  : return vjKEY_J;
     case /*VK_K*/0x4b    : return vjKEY_K;
     case /*VK_L*/0x4c    : return vjKEY_L;
     case /*VK_M*/0x4d    : return vjKEY_M;
     case /*VK_N*/0x4e    : return vjKEY_N;
     case /*VK_O*/0x4f    : return vjKEY_O;
     case /*VK_P*/0x50    : return vjKEY_P;
     case /*VK_Q*/0x51    : return vjKEY_Q;
     case /*VK_R*/0x52    : return vjKEY_R;
     case /*VK_S*/0x53    : return vjKEY_S;
     case /*VK_T*/0x54    : return vjKEY_T;
     case /*VK_U*/0x55    : return vjKEY_U;
     case /*VK_V*/0x56    : return vjKEY_V;
     case /*VK_W*/0x57    : return vjKEY_W;
     case /*VK_X*/0x58    : return vjKEY_X;
     case /*VK_Y*/0x59    : return vjKEY_Y;
     case /*VK_Z*/0x5a    : return vjKEY_Z;
     case VK_ESCAPE   : return vjKEY_ESC;
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
//	OutputDebugString(sz);
//	OutputDebugString("\n");
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
	vjKeyboard* devPtr = (vjKeyboard*)GetWindowLong(hwnd,GWL_USERDATA);
	if (!devPtr)
		return;

	char ln1[255];
	char ln2[255];
	char ln3[255];
	char ln4[255];

	char ln5[255];
	char ln6[255];
	vjMatrix* vjMcurrData = devPtr->GetPosData();

	vjCoord coord(*vjMcurrData);
	ThreeDouble2String(ln1, coord.pos[0], coord.pos[1], coord.pos[2]);
	ThreeDouble2String(ln2, coord.orient[0], coord.orient[1], coord.orient[2]);

	vjMcurrData = devPtr->GetPosData(1);
	vjCoord coord2(*vjMcurrData);
	ThreeDouble2String(ln3, coord2.pos[0], coord2.pos[1], coord2.pos[2]);
	ThreeDouble2String(ln4, coord2.orient[0], coord2.orient[1], coord2.orient[2]);

	wsprintf(ln5,"%li %d %d %d", devPtr->GetDigitalData(0), devPtr->GetDigitalData(1),devPtr->GetDigitalData(2),devPtr->GetDigitalData(3));
	wsprintf(ln6,"%li %d %d %d", devPtr->GetAnalogData(0), devPtr->GetAnalogData(1),devPtr->GetAnalogData(2),devPtr->GetAnalogData(3));
	
	hdc = BeginPaint (hwnd, (LPPAINTSTRUCT)&ps);

	TextOut(hdc,0 ,0, "Pos0:", 5);
	TextOut(hdc,50,0,ln1, lstrlen(ln1));
	TextOut(hdc,50,20,ln2, lstrlen(ln2));
	
	TextOut(hdc,0 ,40, "Pos1:", 5);
	TextOut(hdc,50,40,ln3, lstrlen(ln3));
	TextOut(hdc,50,60,ln4, lstrlen(ln4));
	
	TextOut(hdc,0 ,90, "Digital:", 8);
	TextOut(hdc,50,90,ln5, lstrlen(ln5));
	
	TextOut(hdc,0 ,110, "Analog:", 7);
	TextOut(hdc,50,110,ln6, lstrlen(ln6));

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
LONG APIENTRY MenuWndProc (
	HWND hWnd,
	UINT message,
	UINT wParam,
	LONG lParam)

{

    switch (message){
	case WM_SYSCOMMAND:
	      return DefWindowProc (hWnd, message, wParam, lParam);

//	case WM_COMMAND:
		

//	    break;
		
	case WM_SIZE:
	    if (lParam){
		InvalidateRect (hWnd, NULL, TRUE);
	    }
	    break;

//		#ifdef DEBUG  // Show the pos/dig/analog data in the window in debug
					  // (really inefficient display code)
	case WM_PAINT:	
	    HandlePaint (hWnd);
		break;
//		#endif

	case WM_DESTROY:
	    PostQuitMessage (0);
	    break;

	default:
		vjKeyboard* devPtr = (vjKeyboard*)GetWindowLong(hWnd,GWL_USERDATA);
		if (devPtr)
			devPtr->UpdKeys(message, wParam,lParam);
	    return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return(0);
}


void vjKeyboard::CreateWindowWin32 ()
{
	InitCommonControls();

    m_hInst = g_hInst;
	MenuInit(m_hInst);

    /* Create the app. window */
    m_hWnd = CreateWindow (("Juggler Keyboard"),
			 (instName),
			 WS_OVERLAPPEDWINDOW,
			 CW_USEDEFAULT,
			 CW_USEDEFAULT,
			 m_width,//CW_USEDEFAULT,
			 m_height,//CW_USEDEFAULT,
			 (HWND) NULL,
			 NULL,
			 m_hInst,
			 (LPSTR) NULL);
	ShowWindow(m_hWnd,SW_SHOW);
	UpdateWindow(m_hWnd);

	// Attach a pointer to the device for use from the WNDPROC
	SetWindowLong(m_hWnd, GWL_USERDATA, (LPARAM)this);

	DWORD dwErr = GetLastError();
	if (!m_hWnd)
		return;

} /*CreateWindow*/

BOOL vjKeyboard::MenuInit (HINSTANCE hInstance)
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
//      pyrate_x = (HICON)LoadIcon(hInstance, _T("prof"));
	pWndClass->hIcon = (HICON)LoadIcon(hInstance,TEXT("Juggler"));
    pWndClass->hCursor       = (HCURSOR)LoadCursor (NULL, IDC_ARROW);
    pWndClass->hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
    pWndClass->lpszMenuName  = ( "MenuMenu"),
    pWndClass->lpszClassName = ("Juggler Keyboard");

    bSuccess = RegisterClass (pWndClass);
    LocalUnlock (hMemory);
    LocalFree (hMemory);

    return bSuccess;
}
