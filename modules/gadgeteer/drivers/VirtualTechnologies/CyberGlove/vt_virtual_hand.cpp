/*  FILE: vt_virtual_hand.c  --                                 */
/*                                                              */
/*  Virtual Hand initialization and control routines.           */
/*                                                              */
/*  The VirtualHand data structure encapsulates tracker and     */
/*  glove data.                                                 */
/*                                                              */
/*  ==========================================================  */
/*  -- Copyright (C) 1992,93 Virtual Technologies          --  */
/*  --                                                      --  */
/*  --    Author:  Larry Edwards                            --  */
/*  ==========================================================  */

#include <vrj/vjConfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <vrj/Input/Devices/VirtualTechnologies/CyberGlove.h>
#include <vrj/Input/Devices/VirtualTechnologies/vt_math3d.h>
#include <vrj/Input/Devices/VirtualTechnologies/vt_init.h>
#include <vrj/Input/Devices/VirtualTechnologies/vt_read_user_cal-types.h>
#include <vrj/Input/Devices/VirtualTechnologies/vt_virtual_hand-types.h>
#include <vrj/Input/Devices/VirtualTechnologies/vt_globals.h>
#include <vrj/Input/Devices/VirtualTechnologies/vt_error.h>

typedef float Matrix[4][4];

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                             */
/*  Initialization                                             */
/*                                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*  ALLOCATE_VIRTUALHAND --                                     */
/*                                                              */
/*  allocates and a VirtualHand structure and any subsidiary    */
/*  structures                                                  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

static VirtualHand
allocate_VirtualHand(void)
{
  VirtualHand newhand;
  VirtualHandPrivateStruct *newhand_private;

  if ((newhand = (VirtualHand) calloc(1,sizeof(VirtualHandStruct))) == NULL)
    vt_fatal_unix_error("allocate_VirtualHand","calloc");

  if ((newhand->private_data =
       (VirtualHandPrivateStruct *) calloc(1,sizeof(VirtualHandPrivateStruct))) == NULL)
    vt_fatal_unix_error("allocate_VirtualHand","calloc");

  newhand_private = newhand->private_data;

  if ((newhand_private->user =
       (UserGeometry) calloc(1,sizeof(UserGeometryStruct))) == NULL)
    vt_fatal_unix_error("allocate_VirtualHand","calloc");

  if ((newhand->surface =
       (SurfaceModel) calloc(1,sizeof(SurfaceModelStruct))) == NULL)
    vt_fatal_unix_error("allocate_VirtualHand","calloc");

  newhand->geom = newhand_private->user->geom;

  return (newhand);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*  INIT_VIRTUALHAND --                                         */
/*                                                              */
/*  allocates and initializes a VirtualHand structure           */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void
CyberGloveBasic::init_VirtualHand(VirtualHand hand, int glove_index,
		 char calfile[],
		 Boolean visible)
{
  VirtualHandPrivate hand_private = hand->private_data;

  hand_private->glove_index = glove_index;

  hand_private->glove = vt_create_CbGlove(FALSE);
  hand->timestamp = &(hand_private->glove->timestamp);

  hand->joint_angle = hand_private->glove->joint_angle;
  hand->raw_glove_value = hand_private->glove->raw_value;

  /* read in user's hand geometry and glove/tracker calibration */
  vt_hand_read(hand,calfile);

  vt_unprocess_glove_angles();
}

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*  VT_CREATE_VIRTUALHAND --                                    */
/*                                                              */
/*  creates a VirtualHand structure and reads in associated     */
/*  configuration and data files                                */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

VirtualHand
CyberGloveBasic::vt_create_VirtualHand( char *port, int baud, AppDataStruct &app )
{
  VirtualHand thehand;
  int glove_index;
  Boolean right_hand_glove;

  thehand = allocate_VirtualHand();

  glove_index = 0;
  init_VirtualHand(thehand,glove_index,
		   app.glove[glove_index].incalfile,
		   app.hand_visible[glove_index]);

  thehand->read_glove = app.glove[glove_index].on;
  if (thehand->read_glove)
  {
    if (vt_open_glove_port(port,
			   baud) < Ok)
    {
      vt_print_error("vt_create_VirtualHand");
      thehand->read_glove = FALSE;
    }
    else
    {
      vt_send_glove_query(CG_RIGHT_HAND_QUERY,&right_hand_glove);
      if (thehand->right_hand != NULL)
	if (right_hand_glove != *(thehand->right_hand))
	  fprintf(stderr,"\nWARNING in vt_create_VirtualHand:\n\thandedness of"
		  " calibration file does not match CbGlove.");
    }
  }

  return (thehand);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*  VT_DESTROY_VIRTUALHAND --                                   */
/*                                                              */
/*  frees memory associated with a VirtualHand and closes any   */
/*  associated open ports                                       */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void
CyberGloveBasic::vt_destroy_VirtualHand(VirtualHand hand)
{
  vt_destroy_CbGlove(hand->private_data->glove);
  free(hand->private_data->user);
  free(hand->surface);
  free(hand->private_data);
  free(hand);
}
#if CJGLOVE == 1
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                             */
/*  routines to get/set hand private info                      */
/*                                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*  VT_HAND_USER_GEOM_ADDRESS --                                */
/*                                                              */
/*  Not for general use ... needed for vt_read_user_cal.c       */
/*  and vt_graphics.c                                           */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

UserGeometry
vt_hand_user_geom_address(VirtualHand hand)
{
  VirtualHandPrivate private_data = hand->private_data;

  return(private_data->user);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*  VT_HAND_GLOVE_ADDRESS --                                    */
/*                                                              */
/*  Not for general use ... needed for vt_panel.c               */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

CbGlove
vt_hand_glove_address(VirtualHand hand)
{
  VirtualHandPrivate private_data = hand->private_data;

  return(private_data->glove);
}
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  VT_BUILD_HAND_MATRICES --                                    */
/*                                                               */
/*  Generate the unflexed (i.e. all joint angles == 0) joint     */
/*  transformations.                                             */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void
CyberGloveBasic::vt_build_hand_matrices(VirtualHand hand)
{
  VirtualHandPrivate private_data = hand->private_data;
  UserGeometry user = private_data->user;
  char finger, joint;
  vec3d digit,normal,thumb_mcp;
  static vec3d z_axis = {0.0,0.0,1.0};
  matrix4x4 rotmatrix;
  float oldtheta,theta,length;

  /* this function determines the transform for each joint */
  /* the transform that is stored with each joint is the transform */
  /* that gets you from the previous joints coord system to the local */
  /* coord system of the current joint */

  /* determine the transform for the first joint of each finger */
  for (finger = THUMB; finger < FINGERS; finger++)
  {
    joint = MCP;
    /* the following determines any skewness of the joint axis */
    if (finger != THUMB)
    {
      digit[VX] = user->geom[finger][joint+1][VX] -
	                                user->geom[finger][joint][VX];
      digit[VY] = user->geom[finger][joint+1][VY] -
                                        user->geom[finger][joint][VY];
      theta = atan2(digit[VY], digit[VX]) - M_PI/2.0;
    }
    else			/* for thumb we skew in x-y and y-z planes */
    {
      vt_normalize3(user->thumb_mcp_axis,thumb_mcp);
      if ((hand->right_hand != NULL) && *(hand->right_hand) != TRUE)
	vt_vec_neg3(thumb_mcp,thumb_mcp);

      /* determine the transformation to the unabducted MCP coordinate frame */
      vt_zero_matrix_fill(rotmatrix);
      rotmatrix[3][3] = 1.0;
      vt_normalize3(thumb_mcp,rotmatrix[0]); /* the thumb x-axis */
      vt_cross_prod3(z_axis,rotmatrix[0],normal); /* the thumb y-axis */
      vt_normalize3(normal,rotmatrix[1]);
      vt_cross_prod3(rotmatrix[0],rotmatrix[1],normal);	
      vt_normalize3(normal,rotmatrix[2]); /* the z-axis */

      /* determine the unflexed abduction angle ... we can't include this */
      /* in the digit_xform because we must do the bend about the MCP axis */
      /* before we do the abduction*/
      vt_vec_sub3(user->geom[finger][joint+1],user->geom[finger][joint],digit);
      vt_normalize3(digit,digit);
      hand->unflexed_abduction = -(M_PI_2 - acos(vt_dot_prod3(digit,thumb_mcp)));
    }
    /* translate from the origin to the joint */
    vt_trans_matrix(user->geom[finger][joint],hand->digit_xform[finger][joint]);
    /* rotate by skew angle */
    if (finger != THUMB)
      vt_mult_rot_matrix(theta,'z',Premult,hand->digit_xform[finger][joint]);
    else
      vt_mult_matrix(rotmatrix,hand->digit_xform[finger][joint],
		     hand->digit_xform[finger][joint]);
  }

  /* for thumb we just assume that the PIP and DIP axes are parallel since */
  /* we have no info to the contrary ... we just translate out along y-axis  */
  finger = THUMB;
  for (joint = PIP; joint < ABDUCT; joint++)
  {
    digit[VX] = user->geom[finger][joint][VX] - user->geom[finger][joint-1][VX];
    digit[VY] = user->geom[finger][joint][VY] - user->geom[finger][joint-1][VY];
    digit[VZ] = user->geom[finger][joint][VZ] - user->geom[finger][joint-1][VZ];
    length = sqrt(digit[VX]*digit[VX]+digit[VY]*digit[VY]+digit[VZ]*digit[VZ]);

    digit[VX] = 0.0;
    digit[VY] = length;
    digit[VZ] = 0.0;
    vt_trans_matrix(digit,hand->digit_xform[finger][joint]);
  }

  /* for fingers we translate out for each segment and then add a small */
  /* skew angle */
  for (finger = INDEX; finger < FINGERS; finger++)
  {
    joint = PIP;
    digit[VX] = user->geom[finger][joint][VX] - user->geom[finger][joint-1][VX];
    digit[VY] = user->geom[finger][joint][VY] - user->geom[finger][joint-1][VY];
    oldtheta = atan2(digit[VY],digit[VX]);
    for (; joint < ABDUCT; joint++)
    {
      digit[VX] = user->geom[finger][joint][VX] -
                                            user->geom[finger][joint-1][VX];
      digit[VY] = user->geom[finger][joint][VY] -
	                                    user->geom[finger][joint-1][VY];
      digit[VZ] = user->geom[finger][joint][VZ] -
	                                    user->geom[finger][joint-1][VZ];
      length = vt_vec_length3(digit);
      digit[VX] = 0.0;
      digit[VY] = length;
      digit[VZ] = 0.0;
      vt_trans_matrix(digit,hand->digit_xform[finger][joint]);

      digit[VX] = user->geom[finger][joint+1][VX] -
	                                     user->geom[finger][joint][VX];
      digit[VY] = user->geom[finger][joint+1][VY] -
	                                     user->geom[finger][joint][VY];
      theta = atan2(digit[VY],digit[VX]);
      vt_mult_rot_matrix(theta-oldtheta,'z',Premult,
			 hand->digit_xform[finger][joint]);
      oldtheta = theta;
    }
  }
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*  VT_UPDATE_HAND_STATE --                                       */
/*                                                                */
/*  Update the hand's state data based on new data read or        */
/*  changes in the slider panel                                   */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void
CyberGloveBasic::vt_update_hand_state(VirtualHand hand)
{
 /* if hand is not on reflect changes due to sliders */
 if (hand->read_glove)
   vt_read_processed_glove_data();
 else
   vt_process_glove_data();	/* reflect changes due to sliders */
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*  VT_CALC_THUMB_ROLL --                                       */
/*                                                              */
/*  calculates the amount of thumb roll for a given thumb       */
/*  position.                                                   */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

float
CyberGloveBasic::vt_calc_thumb_roll(VirtualHand hand)
{
  float roll_vector[3],x_axis_desired[3],rot_axis[3];
  float *x_axis,*y_axis,*bone_vector;
  float roll_angle,cosangle;
  Matrix thumb_mcp_matrix;
  UserGeometry user = hand->private_data->user;

  vt_copy_matrix(hand->digit_xform[THUMB][MCP],thumb_mcp_matrix);
  vt_mult_rot_matrix(hand->joint_angle[THUMB][MCP],
		     'x',Premult,thumb_mcp_matrix);
  vt_mult_rot_matrix(hand->joint_angle[THUMB][ABDUCT] +
		     hand->unflexed_abduction,'z',Premult,thumb_mcp_matrix);

  /* the roll vector is the axis around which the thumb rolls as the thumb */
  /* abducts and pivots */
  vt_normalize3(user->thumb_roll_axis,roll_vector);

  x_axis = (float *) thumb_mcp_matrix[0];
  y_axis = (float *) thumb_mcp_matrix[1];
  bone_vector = y_axis;

  /* take cross product of vector along bone (current y-axis) and roll_vector */
  /* to find desired x-axis */
  vt_cross_prod3(roll_vector,bone_vector,x_axis_desired);
  vt_normalize3(x_axis_desired,x_axis_desired);

  /* take cross-prod of current MCP x-axis with desired x-axis */
  /* to find sense of required roll */
  vt_cross_prod3(x_axis,x_axis_desired,rot_axis);

  /* find angle between current MCP x-axis and desired x-axis */
  cosangle = vt_dot_prod3(x_axis,x_axis_desired);
  /* make sure absolute value of cosangle really is <= 1 */
  if (cosangle > 1.0)
    cosangle = 1.0;
  else if (cosangle < -1.0)
    cosangle = -1.0;

  roll_angle = acos(cosangle) * RAD2DEG;

  if (vt_dot_prod3(rot_axis,y_axis) < 0.0)
    roll_angle = -roll_angle;

  return (roll_angle);
}
