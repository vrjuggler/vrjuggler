#ifndef VT_INIT_H
#define VT_INIT_H
/*  FILE: vt_init.h                                           */
/*                                                                    */
/*  header to vt_init.c                                       */
/*                                                                    */
/*  ================================================================  */
/*  --  Copyright (C) 1991, 1992, 1993 Virtual Technologies       --  */
/*  --                                                            --  */
/*  --    Original author:  Jim Helman                            --  */
/*  --    Modified by:      William L. Chapin                     --  */
/*  ================================================================  */


#include <gadget/gadgetConfig.h>
#include <gadget/Devices/VirtualTechnologies/vt_types.h> /* Included for Boolean declaration */

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
  Boolean   hand_visible[VT_MAX_GLOVES];
  Boolean   forearms_visible;
  Boolean   verbose;
  Boolean   debug;
  Boolean       cal_panel;
  int           cal_panel_glove;
  int       slider_resolution;
  float     min_gain;
  float     max_gain;
  int       min_offset;
  int       max_offset;
  Boolean       flat_shading;
  Boolean       wire_frame;
  int           hand_model;
  Boolean       walls_on;
  char          *hand_model_str;
} AppDataStruct, *AppData;

#endif              /* VT_APP_RESOURCE_H */

