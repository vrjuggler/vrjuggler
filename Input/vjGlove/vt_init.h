#ifndef VT_INIT_H
#define VT_INIT_H
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
/*  header to vt_init.c                                       */
/*                                                                    */
/*  ================================================================  */
/*  --  Copyright (C) 1991, 1992, 1993 Virtual Technologies       --  */
/*  --                                                            --  */
/*  --    Original author:  Jim Helman                            --  */
/*  --    Modified by:      William L. Chapin                     --  */
/*  ================================================================  */


#include <vjConfig.h>
#include <Input/vjGlove/vt_types.h>	/* Included for Boolean declaration */

enum
{
  VT_MAX_GLOVES = 2,
  VT_MAX_SETUP_ROTS = 4,
  VT_MAX_TRACKERS = 1
};

enum
{
  VT_HIGHRES,VT_LOWRES
};
  
/* Data needed to open serial communications for a glove ... also includes */
/* some info linking a particular glove with a tracker receiver            */
typedef struct
{
  Boolean on;
  char *devname;
  int baud;
  char *incalfile;
  char *outcalfile;
  int tracker;
  int receiver;
} CbGloveOpenStruct,*CbGloveOpen;


typedef struct
{
  CbGloveOpenStruct glove[VT_MAX_GLOVES];
  int           all_gloves_on;
  Boolean	hand_visible[VT_MAX_GLOVES];
  Boolean	forearms_visible;
  Boolean	verbose;
  Boolean	debug;
  Boolean       cal_panel;
  int           cal_panel_glove;
  int		slider_resolution;
  float		min_gain;
  float		max_gain;
  int		min_offset;
  int		max_offset;
  Boolean       flat_shading;
  Boolean       wire_frame;
  int           hand_model;
  Boolean       walls_on;
  char          *hand_model_str;
} AppDataStruct, *AppData;

#endif				/* VT_APP_RESOURCE_H */

