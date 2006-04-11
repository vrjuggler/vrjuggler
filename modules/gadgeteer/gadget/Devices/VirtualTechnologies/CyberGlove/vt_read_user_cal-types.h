/*  FILE: vt_read_user_cal-types.h                              */
/*                                                              */
/*  Type declarations for vt_read_user_cal.c                    */
/*                                                              */
/*  ==========================================================  */
/*  --  Copyright (C) Virtual Technologies 1990,91,92,93    --  */
/*  --                                                      --  */
/*  --    Author:  William L. Chapin                        --  */
/*  ==========================================================  */

#ifndef VT_READ_USER_CAL_TYPES_H
#define VT_READ_USER_CAL_TYPES_H

#include <gadget/Devices/DriverConfig.h>
#include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_math3d.h>
#include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_glove-types.h>

typedef struct
{
  int fingers,joints;
  Boolean right_hand;
  vec3d geom[MAX_SENSOR_GROUPS][MAX_GROUP_VALUES];
  vec3d pisiform;
  float knuckle_radius[MAX_SENSOR_GROUPS][MAX_GROUP_VALUES];
  vec3d radi_ulna[8], forearm[8];
  vec3d thumb_roll_axis;
  vec3d thumb_mcp_axis;
  float thumb_roll;
  float hand_roll;
  vec3d wrist_offset;       /* offset of wrist from tracker receiver */
} UserGeometryStruct,*UserGeometry;

#endif /* VT_READ_USER_CAL_TYPES_H */
