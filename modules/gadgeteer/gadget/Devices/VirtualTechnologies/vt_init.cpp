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
/*                                                                    */
/*  routines to read X resources and initialize application data      */
/*                                                                    */
/*  ================================================================  */
/*  --  Copyright (C) 1991, 1992, 1993 Virtual Technologies       --  */
/*  --                                                            --  */
/*  --    Original author: Jim Helman                             --  */
/*  --    Amended and modified by William Chapin                  --  */
/*  --    Further modified by Larry Edwards                       --  */
/*  ================================================================  */

#include <vjConfig.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Input/vjGlove/vjCyberGlove.h>
#include <Input/vjGlove/vt_globals.h>
#include <Input/vjGlove/vt_init.h>

#define True 1
#define False 0

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*  VT_APP_RESOURCE_INIT --                                     */
/*                                                              */
/*  Creates the toplevel shell widget for this application.     */
/*  Also, initialize the global AppData structure (the variable */
/*  name is app), both from command line arguments and defaults */
/*  in the VirtualHand file.                                    */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


void
CyberGloveBasic::vt_init(char *indir, AppDataStruct &app)
{
  int i;
  char defaultcalfile[100];

  /* fills in app's fields based on the application resources */
  /* app is a global variable declared in vt_globals.c */

  /* CJ - we no longer have any need for the x-style configurable resources,
   * so I've hacked all that out and initialized the app structure using the
   * old defaults.  Note that the main things we ever want to change, baud
   * and port, are passed to us when we open the glove, not read form
   * and config info.
   */

  app.all_gloves_on = 0;
  app.glove[0].on = True;
  app.glove[0].devname = "/dev/ttyd2";
  app.glove[0].baud = 38400;
  app.glove[0].tracker = 1;
  app.glove[0].receiver = 1;
  app.glove[1].on = False;
  app.glove[1].devname = "/dev/ttyd2";
  app.glove[1].baud = 38400;
  app.glove[1].tracker = 1;
  app.glove[1].receiver = 1;
  app.hand_model_str = "highres";
  app.hand_visible[0] = True;
  app.forearms_visible = True;
  app.flat_shading = False;
  app.wire_frame = False;
  app.walls_on = False;
  app.cal_panel = False;

  /* Must add the following two lines since initializing these fields with  */
  /* a string constant in the above resources[] array will not work. This   */
  /* is because we realloc these fields in file_read() in "panel.c" and     */
  /* reallocing a pointer pointing to the DATA section rather than the heap */
  /* is bad news. So as it stands these fields are initialized to NULL in   */
  /* resources[] and pointed to strings alloced on the heap here.	    */

  sprintf( defaultcalfile, "%s/%s", indir, "default.cal");
  app.glove[0].incalfile  = strdup(defaultcalfile);
  app.glove[1].incalfile  = strdup(defaultcalfile);
  for (i=0; i < VT_MAX_GLOVES; i++)
    app.glove[i].outcalfile = strdup("untitled.cal");

  /* have to do the following because a single option can't set more than */
  /* one field at a time */
  if (app.all_gloves_on)
    for (i=0; i < VT_MAX_GLOVES; i++)
      app.glove[i].on = (app.all_gloves_on > 0);

  app.hand_model = 0; /* highres -- change to 0 for lowres */

  app.max_offset = 0xFF;
}
