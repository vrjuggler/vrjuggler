#include <config.h>
#include <Input/vjInput/vjKeyboard.h>
#include <Kernel/vjDebug.h>

vjKeyboard::vjKeyboard(vjConfigChunk *c) : vjPosition(c), vjDigital(c),
				vjAnalog(c), vjInput(c)
{
    cout << "     vjKeyboard::vjKeyboard(vjConfigChunk*c) " << endl;
    myThread = NULL;
    int i;
    for(i =0; i < 256; i++)
         m_realkeys[i] = m_keys[i] = 0;
    m_realkeys[0] = m_keys[0] = 1;

    m_width = (int)c->getProperty("width");
    m_height = (int)c->getProperty("height");

    if (m_width == 0) m_width = 400;
    if (m_height == 0) m_height = 400;

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

      /* Got rid of this hack
      if (m_digmods[z] != 0) m_digmods[z] += 4;
      if (m_anamodsup[z] != 0) m_anamodsup[z] += 4;
      if (m_anamodsdn[z] != 0) m_anamodsdn[z] += 4; */
    }

    /* Got rid of this hack
    for(int q = 0; q < NUM_POS_CONTROLS; q++)
    {
       if (m_pos0mod[q] != 0)  // if the key is not NONE, then add 4 to get it
          m_pos0mod[q] += 4;   // to the SHIFT/CTRL/ALT key
       if (m_pos1mod[q] != 0)
          m_pos1mod[q] += 4;
    } */

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
           << endl;

}

int vjKeyboard::StartSampling()
{

if (myThread == NULL) {
  //UnUsed// int processID;
  int i;

  current = 0;
  valid = 1;
  progress = 2;

  m_x = 100; m_y = 200;

  m_display = XOpenDisplay(NULL);
  if (m_display == NULL)
  {
     cerr << "vjKeyboard::StartSampling() : failed to open display" << endl;
     return 0;
  }
  m_screen = DefaultScreen(m_display);

  XVisualInfo vTemplate;
  long vMask = VisualScreenMask;
  vTemplate.screen = m_screen;
  int nVisuals;

  m_visual = XGetVisualInfo( m_display, vMask, &vTemplate, &nVisuals);
  XVisualInfo* p_visinfo;
  for(i = 0, p_visinfo = m_visual; i < nVisuals; i++, p_visinfo++)
  {
    if (p_visinfo->depth > 8)
    {
	m_visual = p_visinfo;
        break;
    }
  }

  if (i == nVisuals)
  {
    cerr << "vjKeyboard::StartSampling() : find visual failed" << endl;
    return 0;
  }

  m_swa.colormap = XCreateColormap(m_display,
                              RootWindow(m_display,m_visual->screen),

				m_visual->visual, AllocNone);
  m_swa.border_pixel = 0;
  m_swa.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask;
  CheckGeometry();

  m_window = CreateWindow ( DefaultRootWindow(m_display) ,
		1, BlackPixel(m_display,m_screen),
                WhitePixel(m_display,m_screen), ExposureMask |
		StructureNotifyMask |
		KeyPressMask | KeyReleaseMask | ButtonPressMask |
		ButtonReleaseMask | ButtonMotionMask | PointerMotionMask);
  SetHints(m_window, instName , "VJm_keys" , "VJKeyboard2", "VJInputD" );
  XSelectInput(m_display,m_window, KeyPressMask | KeyReleaseMask | ButtonPressMask |
		ButtonReleaseMask);
  XMapWindow(m_display, m_window);
  XFlush(m_display);
  XRaiseWindow(m_display,m_window);


  vjDEBUG(0) << "vjKeyboard::StartSampling() : ready to go.." << endl << vjDEBUG_FLUSH;

  //UnUsed// vjKeyboard* devicePtr = this;
  void Samplem_keys(void*);

  myThread = (vjThread *) 1;
  return 1;

  }
  else return 0; // already sampling


}

int vjKeyboard::OnlyModifier(int mod)
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
       vjASSERT(false);
       return 0;
  }


}

void vjKeyboard::UpdateData()
{
  int i;
  UpdKeys();

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
         m_digdata[i] = 1 - m_digdata[i];
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
    vjDEBUG(6) << "Digital " << i << ": " << m_digdata[i]
               << "\tAnalog  " << i << ": " << m_anadata[i] << endl << vjDEBUG_FLUSH;
  }

}

// Move forward the given amount on position data n
// Forward is in th -Z direction
void vjKeyboard::MoveFor(float amt, int n)
{
   m_posdata[n].postTrans(m_posdata[n], 0.0, 0.0, -amt*m_dtrans);
}

// Move left the given amount on position data n
// Left is -X dir
void vjKeyboard::MoveLeft(float amt, int n)
{
    m_posdata[n].postTrans(m_posdata[n], -amt*m_dtrans, 0.0, 0.0);
}

// Move up the given amount on position data n
// Up is in th +Y dir
void vjKeyboard::MoveUp(float amt, int n)
{
  m_posdata[n].postTrans(m_posdata[n], 0.0, amt*m_dtrans, 0.0);
}

// Pitch up - rot +x axis
void vjKeyboard::RotUp(float amt, int n)
{
   static vjVec3 x_axis(1.0,0.0,0.0);
   m_posdata[n].postRot(m_posdata[n], amt*m_drot, x_axis);
}

// Yaw left - rot +Y axis
void vjKeyboard::RotLeft(float amt, int n)
{
   static vjVec3 y_axis(0.0, 1.0, 0.0);
   m_posdata[n].postRot(m_posdata[n], amt*m_drot, y_axis);
}

// Roll Left - rot -z axis
void vjKeyboard::RotRollCCW(float amt, int n)
{
   static vjVec3 neg_z_axis(0.0, 0.0, -1.0);
   m_posdata[n].postRot(m_posdata[n], amt*m_drot, neg_z_axis);
}

void vjKeyboard::UpdKeys()
{
   XEvent event;
   KeySym key;
   static int oldx;
   static int oldy;
   Window win1, win2;
   int wX, wY, rootX, rootY;
   unsigned int mask;

   for(int i = 0; i < 256; i++)
      m_keys[i] = m_realkeys[i];

   while(XCheckWindowEvent(m_display,m_window,KeyPressMask |
		KeyReleaseMask | ButtonPressMask | ButtonReleaseMask, &event))
   {
      switch (event.type)
      {
      case KeyPress:
         key = XLookupKeysym((XKeyEvent*)&event,0);
         m_realkeys[XKeyTovjKey(key)] = 1;
         m_keys[XKeyTovjKey(key)] += 1;
         vjDEBUG(1) << "KeyPress:  " << hex << key
         << " state:" << ((XKeyEvent*)&event)->state << " ==> " << XKeyTovjKey(key) << endl << vjDEBUG_FLUSH;
         break;
      case KeyRelease:
         key = XLookupKeysym((XKeyEvent*)&event,0);
         m_realkeys[XKeyTovjKey(key)] = 0;
         vjDEBUG(1) << "KeyRelease:" << hex << key
         << " state:" << ((XKeyEvent*)&event)->state << " ==> " << XKeyTovjKey(key) << endl << vjDEBUG_FLUSH;
         break;
      }
   }


   XQueryPointer(m_display, m_window, &win1, &win2, &rootX, &rootY,
		   &wX, &wY, &mask);
	
   if ((rootX - oldx) > 0)
      m_keys[VJMOUSE_POSX] = (rootX - oldx) * m_mouse_sensitivity;
   else
      m_keys[VJMOUSE_POSX] = 0;

   if ((rootX - oldx) < 0)
      m_keys[VJMOUSE_NEGX] = -(rootX - oldx) * m_mouse_sensitivity;
   else
      m_keys[VJMOUSE_NEGX] = 0;

   if ((rootY - oldy) > 0)
      m_keys[VJMOUSE_POSY] = (rootY - oldy) * m_mouse_sensitivity;
   else
      m_keys[VJMOUSE_POSY] = 0;

   if ((rootY - oldy) > 0)
      m_keys[VJMOUSE_NEGY] = -(rootY - oldy) * m_mouse_sensitivity;
   else
      m_keys[VJMOUSE_NEGY] = 0;

  oldx = rootX; oldy = rootY;

   m_keys[VJMBUTTON1] = ((mask | Button1Mask) == mask) ? 1 : 0;
   m_keys[VJMBUTTON2] = ((mask | Button2Mask) == mask) ? 1 : 0;
   m_keys[VJMBUTTON3] = ((mask | Button3Mask) == mask) ? 1 : 0;

}

int vjKeyboard::StopSampling()
{
  if (myThread != NULL)
  {
    //vjThread::kill(myThreadID);
    myThread = NULL;
    cout << "Stoppping Keyboard.." << endl;
    sleep(1);


    XDestroyWindow(m_display,m_window);
    XFree(m_visual);
    XCloseDisplay((Display*) m_display);
  }


  return 1;

}

int vjKeyboard::XKeyTovjKey(KeySym xKey)
{
   switch (xKey)
   {
   case XK_Up        : return VJKEY_UP;
   case XK_Down      : return VJKEY_DOWN;
   case XK_Left      : return VJKEY_LEFT;
   case XK_Right     : return VJKEY_RIGHT;
   case XK_Control_L : return VJKEY_CTRL;
   case XK_Control_R : return VJKEY_CTRL;
   case XK_Shift_L   : return VJKEY_SHIFT;
   case XK_Shift_R   : return VJKEY_SHIFT;
   case XK_Alt_L     : return VJKEY_ALT;
   case XK_Alt_R     : return VJKEY_ALT;

      // Map all number keys
      // Note we map keypad and normal keys making no distinction
   case XK_1            : return VJKEY_1;
   case XK_KP_End       : return VJKEY_1;
   case XK_2            : return VJKEY_2;
   case XK_KP_Down      : return VJKEY_2;
   case XK_3            : return VJKEY_3;
   case XK_KP_Page_Down : return VJKEY_3;
   case XK_4            : return VJKEY_4;
   case XK_KP_Left      : return VJKEY_4;
   case XK_5            : return VJKEY_5;
   case XK_KP_Begin     : return VJKEY_5;
   case XK_6            : return VJKEY_6;
   case XK_KP_Right     : return VJKEY_6;
   case XK_7            : return VJKEY_7;
   case XK_KP_Home      : return VJKEY_7;
   case XK_8            : return VJKEY_8;
   case XK_KP_Up        : return VJKEY_8;
   case XK_9            : return VJKEY_9;
   case XK_KP_Page_Up   : return VJKEY_9;
   case XK_0            : return VJKEY_0;
   case XK_KP_Insert    : return VJKEY_0;

   case XK_A         : return VJKEY_A;
   case XK_B         : return VJKEY_B;
   case XK_C         : return VJKEY_C;
   case XK_D         : return VJKEY_D;
   case XK_E         : return VJKEY_E;
   case XK_F         : return VJKEY_F;
   case XK_G         : return VJKEY_G;
   case XK_H         : return VJKEY_H;
   case XK_I         : return VJKEY_I;
   case XK_J         : return VJKEY_J;
   case XK_K         : return VJKEY_K;
   case XK_L         : return VJKEY_L;
   case XK_M         : return VJKEY_M;
   case XK_N         : return VJKEY_N;
   case XK_O         : return VJKEY_O;
   case XK_P         : return VJKEY_P;
   case XK_Q         : return VJKEY_Q;
   case XK_R         : return VJKEY_R;
   case XK_S         : return VJKEY_S;
   case XK_T         : return VJKEY_T;
   case XK_U         : return VJKEY_U;
   case XK_V         : return VJKEY_V;
   case XK_W         : return VJKEY_W;
   case XK_X         : return VJKEY_X;
   case XK_Y         : return VJKEY_Y;
   case XK_Z         : return VJKEY_Z;
   case XK_a         : return VJKEY_A;
   case XK_b         : return VJKEY_B;
   case XK_c         : return VJKEY_C;
   case XK_d         : return VJKEY_D;
   case XK_e         : return VJKEY_E;
   case XK_f         : return VJKEY_F;
   case XK_g         : return VJKEY_G;
   case XK_h         : return VJKEY_H;
   case XK_i         : return VJKEY_I;
   case XK_j         : return VJKEY_J;
   case XK_k         : return VJKEY_K;
   case XK_l         : return VJKEY_L;
   case XK_m         : return VJKEY_M;
   case XK_n         : return VJKEY_N;
   case XK_o         : return VJKEY_O;
   case XK_p         : return VJKEY_P;
   case XK_q         : return VJKEY_Q;
   case XK_r         : return VJKEY_R;
   case XK_s         : return VJKEY_S;
   case XK_t         : return VJKEY_T;
   case XK_u         : return VJKEY_U;
   case XK_v         : return VJKEY_V;
   case XK_w         : return VJKEY_W;
   case XK_x         : return VJKEY_X;
   case XK_y         : return VJKEY_Y;
   case XK_z         : return VJKEY_Z;
   case XK_Escape    : return VJKEY_ESC;
   default: return 255;
   }

}

/*****************************************************************/
/*****************************************************************/
/***********************X WINDOW STUFF****************************/
/*****************************************************************/
/*****************************************************************/
/* Sets basic window manager hints for a window. */
void vjKeyboard::SetHints(Window window,
    char*  window_name,
    char*  icon_name,
    char*  class_name,
    char*  class_type)

{
    XTextProperty  w_name;
    XTextProperty  i_name;
    XSizeHints     sizehints;
    XWMHints       wmhints;
    XClassHint     classhints;
    int            status;

    /*
     * Generate window and icon names.
     */
	status = XStringListToTextProperty(&window_name,
			1,	/* 1 string to convert */
			&w_name);

	status = XStringListToTextProperty(&icon_name,
			1,	/* 1 string to convert */
			&i_name);

    sizehints.x           = m_x;        /* -- Obsolete in R4 */
    sizehints.y           = m_y;        /* -- Obsolete in R4 */
    sizehints.width       = m_width;    /* -- Obsolete in R4 */
    sizehints.height      = m_height;   /* -- Obsolete in R4 */
    sizehints.base_width  = m_width;    /* -- New in R4 */
    sizehints.base_height = m_height;   /* -- New in R4 */


    /* Set up flags for all the fields we've filled in. */
    sizehints.flags = USPosition | USSize | PBaseSize;


    /*   assume the window starts in "normal" (rather than
     *    iconic) state and wants keyboard input.
     */
    wmhints.initial_state = NormalState;
    wmhints.input         = True;
    wmhints.flags         = StateHint | InputHint;


    /* Fill in class name. */
    classhints.res_name  = class_name;
    classhints.res_class = class_type;

    XSetWMProperties(m_display, window,
        &w_name,
        &i_name,
        //argv, argc, /* Note reversed order. */
        NULL,0,
	&sizehints,
        &wmhints,
        &classhints);

	XFree(w_name.value);
	XFree(i_name.value);

}

Window vjKeyboard::CreateWindow (Window parent, unsigned int border, unsigned long
     fore, unsigned long back, unsigned long event_mask)
{
  Window window;
  //UnUsed// XSetWindowAttributes attributes;
  //UnUsed// unsigned long attribute_mask;

  // set up attributes
  //UnUsed// attributes.background_pixel = back;
  //UnUsed// attributes.border_pixel = fore;
  //UnUsed// attributes.event_mask = event_mask;
  //UnUsed// attribute_mask = CWBackPixel | CWBorderPixel | CWEventMask;

  // create it
  window = XCreateWindow( m_display, parent,
           m_x,m_y,m_width,m_height, border,
           m_visual->depth, /*depth*/
           InputOutput, /*window class*/
           m_visual->visual,CWBorderPixel|CWColormap|CWEventMask, &m_swa);

  return window;
} /*CreateWindow*/

void vjKeyboard::CheckGeometry()
{
  char* geo_string;
  int status;
  int screen_width, screen_height;

  geo_string = CheckArgs( "-geom" );
  if (geo_string != (char*) NULL) {
    status = XParseGeometry(geo_string,&m_x,&m_y,&m_width,&m_height);
    if (status & XNegative) {
       screen_width = DisplayWidth(m_display,m_visual->screen);
       m_x = screen_width - m_width + m_x;
    }
    if (status & YNegative) {
       screen_height = DisplayHeight(m_display, m_visual->screen);
       m_y = screen_height - m_height + m_y;
    }
  }
} /*CheckGeometry*/

char* vjKeyboard::CheckArgs(char* look_for)
{
  //Unused//int i, l;

 /* l = strlen(look_for);
  i = 1;
  while (i < argc)
  { if (strncmp(argv[i], look_for, l) == 0)
   {i++;

    if (i < argc) {
       return argv[i];
    } else {
      cerr << "ERROR: Usage is:\n" << look_for << " value\n";
    }
   }
   i++;
  }*/
  return ((char*) NULL);
} /*CheckArgs*/

int vjKeyboard::FilterEvent( XEvent* event, int want_exposes,
		  int width, int height)
{
    int status = 1;
    if (XFilterEvent( event, (Window)NULL ) )
    {
      return 0;
    }
    switch (event->type) {
       case Expose:
         if (!want_exposes ) {
	   if (event->xexpose.count != 0) {
	     status = 0;
	     }
	 }
	 break;
        case ConfigureNotify:
	  if ((width == event->xconfigure.width ) && (height == event->xconfigure.height))
	  {  status = 0; }
	  break;
	case MappingNotify:
	  XRefreshKeyboardMapping ( (XMappingEvent*) event );
	  status = 0;
	  break;
	default: ;
     }
     return status;
}
