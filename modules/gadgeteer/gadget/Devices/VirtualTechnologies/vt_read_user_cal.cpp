/*  FILE: vt_read_user_cal.c  --                                */
/*                                                              */
/*  Routines to read and write hand calibration/geometry data   */
/*                                                              */
/*  ==========================================================  */
/*  --  Copyright (C) 1990,91,92,93 Virtual Technologies    --  */
/*  --                                                      --  */
/*  --    Author: William Chapin                            --  */
/*  --    Modified by Larry Edwards                         --  */
/*  ==========================================================  */

#include <vjConfig.h>
#include <stdio.h>

#include <X11/Intrinsic.h>

#include <Input/vjGlove/vjCyberGlove.h>
#include <Input/vjGlove/vt_read_user_cal-types.h>
#include <Input/vjGlove/vt_virtual_hand-types.h>
#include <Input/vjGlove/vt_globals.h>
#include <Input/vjGlove/vt_error.h>

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                               */
/*  Private Global Variables                                     */
/*                                                               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

/* comments and version number for writing out the calibration/geometry file */

static char *geo_header[] =
{
  "! ==",
  "3D HAND GEOMETRY follows:",
  "! ==",
  "! NOTE:",
  "!     The geometry of the system axes is to change in",
  "!   in future releases to account for the roll of the arm",
  "!   to equate to roll of about the X-axis.  The Y-axis",
  "!   will then be through the wrist from right to left.",
  "!   The Z-axis will always be up.",
  "! ==",
  "! This geometry is for a:",
  ""
  };

static char *sensor_header[] =
{
  "!   The handedness can be changed by flipping the geometry",
  "!   across its axis of symmetry.",
  "!   * Currently this is { x = -x }, but it will be { y = -y }",
  "!     with the X-axis as the axis of symmetry.",
  "! ==",
  "! backbone of hand -- flat structural geometry (centimeters)",
  "! ==",
  "!       wrist_offset  =  offset of wrist's center of rotation",
  "!    x      y      z        relative to tracker receiver.",
  ""
  };

static char *hand_header[] =
{
  "! ==",
  "! # of fingers, # of vertices/finger, thumb-aspect and hand-roll",
  "!       thumb-roll = roll angle of thumb interphalangeal bend axis",
  "!       hand-roll  = roll angle of wrist pitch axis",
  "!    in degrees relative to plane of metacarpals.",
  ""
  };

static char *finger_header[] =
{
  "! ==",
  "! Each finger is with <x, y, z> joint/tip position relative to",
  "!   the plane of the metacarpals (unrolled) with origin=wrist",
  "!   the initial integer is currently used as a VGA color reference",
  "!   the fourth real is the finger diameter at the joint.",
  ""
  };

static char *carp_header[] =
{
  "! ==",
  "! There will be several parameters describing the",
  "!   carpals.  Presently only one point is used.",
  "! pisiform carpal bone = the bone that the hand rests on",
  ""
  };

static char *forearm_header[] =
{
  "! ==",
  "! radial-carpus, ulna-carpus at the wrist joint",
  "! radial-cranon, olecranon, and ulna-coronoid points at elbow joint",
  ""
  };

static char *thumb_roll_axis_header[] =
{
  "! Thumb roll axis. This is the vector the thumb rolls about as it abducts",
  "! and rotates about the MCP joint",
  "! Fairly good results are had just using the vector from the thumb MCP",
  "! joint to the pinky MCP joint",
  ""
};

static char *thumb_mcp_axis_header[] =
{
  "! MCP rotation axis. Nominally the vector from the thumb"
    " MCP joint to the index",
    "! finger MCP joint",
    ""
};


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                               */
/*  Low level support routines                                   */
/*                                                               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


void
skip_comments(FILE *inputfp, char input_line[])
{
  while (fgets(input_line, 81, inputfp))
  {
    if (input_line[0] != '!')
      break;
  }

}

static void
print_list(FILE *outputfp, char **list)
{
  char **ptr;
  for (ptr = list ; **ptr != '\0' ; ptr++)
    fprintf(outputfp, "%s\n", *ptr);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                               */
/*  Routines to read and write the hand geometry                 */
/*                                                               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  VT_GEOM_IN --                                                */
/*                                                               */
/*    Read in the user's geometry from the file                  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::vt_geom_in(FILE *inputfp, UserGeometry user)
{
  int i, finger, joint, finger_color;
  char input_line[82];
  static char geom_header[] = "3D HAND GEOMETRY";
  vec3d tempvec[5];
  Boolean found_geom = FALSE;

  rewind(inputfp);		/* make sure we're at beginning of file */

  while (fgets(input_line, 81, inputfp))
    if ((found_geom =
	 (strncmp(input_line,geom_header,sizeof(geom_header)-1) == 0)))
      break;
  if (!found_geom)
    return (vt_set_error("vt_read_calibration",CAL_ERROR1));

  skip_comments(inputfp,input_line);
  if (input_line[0] != ' ')
    user->right_hand = (input_line[0] == 'R');

  skip_comments(inputfp,input_line);
  /* read wrist offset */
  sscanf(input_line, "%f %f %f", &user->wrist_offset[VX],
	 &user->wrist_offset[VY],&user->wrist_offset[VZ]);

  skip_comments(inputfp,input_line);
  sscanf(input_line, "%d %d %f %f", &user->fingers, &user->joints,
	 &user->thumb_roll, &user->hand_roll);
  user->thumb_roll *= DEG2RAD;	/*  conversion to radians  */
  user->hand_roll *= DEG2RAD;	/*  conversion to radians  */

  for (finger = THUMB; finger < user->fingers; finger++)
  {
    skip_comments(inputfp,input_line);
    sscanf(input_line, "%d", &finger_color);
    for (joint = MCP; joint < user->joints; joint++)
    {
      fgets(input_line, 81, inputfp);
      sscanf(input_line, "%f %f %f %f",
	     &user->geom[finger][joint][VX], &user->geom[finger][joint][VY],
	     &user->geom[finger][joint][VZ],
	     &user->knuckle_radius[finger][joint]);
    }
  }

  skip_comments(inputfp,input_line);
  sscanf(input_line, "%f %f %f",
	 &user->pisiform[VX], &user->pisiform[VY], &user->pisiform[VZ]);

  /*  The following kludge just goes to show you that the geometry */
  /*  file format is due to be upgraded.                           */
  for (i = 0 ; i < 5 ; i++)
  {
    skip_comments(inputfp,input_line);
    sscanf(input_line, "%f %f %f",
	   &tempvec[i][VX], &tempvec[i][VY], &tempvec[i][VZ]);
  }
  vt_copy_vec3(tempvec[0],user->radi_ulna[0]);
  user->radi_ulna[0][VZ] -= 1.0;
  vt_copy_vec3(tempvec[0],user->radi_ulna[1]);
  user->radi_ulna[1][VZ] += 1.0;

  vt_copy_vec3(tempvec[1],user->radi_ulna[2]);
  vt_copy_vec3(tempvec[1],user->radi_ulna[3]);
  user->radi_ulna[3][VZ] -= 2.0;
  vt_copy_vec3(tempvec[2],user->radi_ulna[4]);
  vt_copy_vec3(tempvec[3],user->radi_ulna[5]);
  vt_copy_vec3(tempvec[4],user->radi_ulna[6]);
  vt_copy_vec3(tempvec[3],user->radi_ulna[7]);
  user->radi_ulna[7][VY] += 5.0;
  user->radi_ulna[7][VZ] += 5.0;

  /* read thumb roll and mcp vectors */
  skip_comments(inputfp,input_line);
  sscanf(input_line, "%f %f %f",&user->thumb_roll_axis[VX],
	 &user->thumb_roll_axis[VY], &user->thumb_roll_axis[VZ]);
  skip_comments(inputfp,input_line);
  sscanf(input_line, "%f %f %f", &user->thumb_mcp_axis[VX],
	 &user->thumb_mcp_axis[VY], &user->thumb_mcp_axis[VZ]);

  return (Ok);
}

void
CyberGloveBasic::vt_geom_out( FILE *outputfp, UserGeometry user)
{
  int i, finger, joint;

  vec3d tempvec[5];
  char *finger_name[] = {"Thumb","Index","Middle","Ring","Pinky","WCarp"};

  print_list(outputfp, geo_header);
  fprintf(outputfp, "%s HAND.\n",(user->right_hand ? "RIGHT" : "LEFT" ));

  print_list(outputfp, sensor_header);
  fprintf(outputfp, "%6.2f %6.2f %6.2f\n",user->wrist_offset[VX],
	  user->wrist_offset[VY], user->wrist_offset[VZ]);

  print_list(outputfp, hand_header);
  fprintf(outputfp, "%6d %6d %6.2f %6.2f\n",
	  user->fingers, user->joints,
	  user->thumb_roll*RAD2DEG, user->hand_roll*RAD2DEG);

  print_list(outputfp, finger_header);
  for (finger = THUMB; finger < user->fingers ; finger++)
  {
    fprintf(outputfp, "! %s\n", finger_name[finger]);
    fprintf(outputfp, "%d\n",1); /* we don't use the finger color field now */
    for (joint = MCP; joint < user->joints ; joint++)
    {
      fprintf(outputfp, "%6.2f %6.2f %6.2f %6.2f\n",
	      user->geom[finger][joint][VX],
	      user->geom[finger][joint][VY],
	      user->geom[finger][joint][VZ],
	      user->knuckle_radius[finger][joint]);
    }
  }

  print_list(outputfp, carp_header);
  fprintf(outputfp, "%6.2f %6.2f %6.2f\n",user->pisiform[VX],
	  user->pisiform[VY], user->pisiform[VZ]);

  print_list(outputfp, forearm_header);
  vt_copy_vec3(user->radi_ulna[0],tempvec[0]);
  tempvec[0][VZ] += 1.0;
  vt_copy_vec3(user->radi_ulna[2],tempvec[1]);
  vt_copy_vec3(user->radi_ulna[4],tempvec[2]);
  vt_copy_vec3(user->radi_ulna[5],tempvec[3]);
  vt_copy_vec3(user->radi_ulna[6],tempvec[4]);

  for ( i = 0 ; i < 5 ; i++)
  {
    fprintf(outputfp, "%6.2f %6.2f %6.2f\n",
	    tempvec[i][VX], tempvec[i][VY], tempvec[i][VZ]);
  }

  print_list(outputfp, thumb_roll_axis_header);
  fprintf(outputfp, "%6.2f %6.2f %6.2f\n",user->thumb_roll_axis[VX],
	  user->thumb_roll_axis[VY], user->thumb_roll_axis[VZ]);

  print_list(outputfp, thumb_mcp_axis_header);
  fprintf(outputfp, "%6.2f %6.2f %6.2f\n",user->thumb_mcp_axis[VX],
	  user->thumb_mcp_axis[VY], user->thumb_mcp_axis[VZ]);

  fprintf(outputfp, "! E_O_GEO\n!\n");
  fflush(outputfp);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*                                                                */
/*  Routines to read and write calibration data for glove sensors */
/*                                                                */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/


int
CyberGloveBasic::vt_read_glove_calibration(FILE *inputfp, CbGlove glove)
{
  int finger,joint,temp;
  char input_line[128];
  static char glove_cal_header[] = "GLOVE CALIBRATION";
  int offset;
  float version,gain;
  Boolean found_cal;

  rewind(inputfp);	/* make sure we're at beginning of file */

  fgets(input_line,81,inputfp);
  if (sscanf(input_line,"VHv%f.%*d",&version) < 1)
    printf("\n\n          Old format VH file ... please update.\n\n");

  /* look for the beginning of the hand calibration data */
  while (fgets(input_line,81,inputfp))
    if ((found_cal = (strncmp(input_line,glove_cal_header,
			     sizeof(glove_cal_header)-1) == 0)))
      break;
  if (!found_cal)
    return (vt_set_error("vt_read_calibration",CAL_ERROR1));
	
  /* read the calibration data */
  while (fgets(input_line,81,inputfp))
  {
    if (input_line[0] != '!')	/* skip comments */
    {
      while (sscanf(input_line,"FINGER %d:",&finger))
      {
	for (joint = 0; joint < JOINTS; joint++)
	{
	  if (fgets(input_line,81,inputfp) == NULL)
	  {
	    return (vt_set_error("vt_read_calibration",CAL_ERROR1));
	  }

	  sscanf(input_line,"%d",&temp);
	  if (temp == joint)
	  {
	    sscanf(input_line,"%*d%d%f", &offset,&gain);
            offset &= 0xFF;
            glove->private_data->mapping[finger][joint].offset = offset;
            glove->private_data->mapping[finger][joint].gain = gain;
	  }
	  else
	  {
	    return (vt_set_error("vt_read_calibration",CAL_ERROR1));
	  }
	}
      }
    }
  }

  return(Ok);
}

void
CyberGloveBasic::vt_write_glove_calibration(FILE *outputfp, CbGlove glove,
			   char *version_string)
{
  int finger,joint;

  fprintf(outputfp,"VHv%s\n",version_string);
  fprintf(outputfp,
	  "! Virtex CbGlove (TM) -> Virtual Hand calibration file\n");
  fprintf(outputfp,"! == Virtual Hand Software %s ==\n!\n",version_string);
  fprintf(outputfp,"GLOVE CALIBRATION follows:\n");
  fprintf(outputfp,"! joint, offset, gain, gesture\n");

  for (finger=0; finger < FINGERS+1; finger++)
  {
    fprintf(outputfp,"FINGER %1d:\n",finger);
    for (joint = 0; joint < JOINTS; joint++)
      fprintf(outputfp,"%3d %6d %10.5f %5d\n", joint,
              glove->private_data->mapping[finger][joint].offset,
              glove->private_data->mapping[finger][joint].gain,
	      0);
  }

  fprintf(outputfp,"! E_O_CAL\n!\n");
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                               */
/*  Routines to read and write both geometry and calibration     */
/*  data.                                                        */
/*                                                               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


int
CyberGloveBasic::vt_hand_read(VirtualHand hand, char *filename)
{
  int success;
  FILE *inputfp;
  char errmessg[80] = "vt_hand_read\n     ";

  if ((inputfp = fopen(filename, "r")) == NULL)
  {
    strcat(errmessg,filename);
    return (vt_print_unix_error(errmessg,"fopen"));
  }

  success = vt_read_glove_calibration(inputfp,hand->private_data->glove);
  success = success && vt_geom_in(inputfp,hand->private_data->user);
  fclose(inputfp);

  hand->right_hand = &(hand->private_data->user->right_hand);

  /* build matrices for new geometry */
  vt_build_hand_matrices(hand);

  return (success);
}

int
CyberGloveBasic::vt_hand_write(VirtualHand hand, char *filename, char *version_string)
{
  FILE *outputfp;
  char errmessg[80] = "vt_hand_write\n     ";

  if ((outputfp = fopen(filename, "w")) == NULL)
  {
    strcat(errmessg,filename);
    return (vt_print_unix_error(errmessg,"fopen"));
  }

  vt_write_glove_calibration(outputfp, hand->private_data->glove,
			     version_string);

  vt_geom_out(outputfp,hand->private_data->user);
  fclose(outputfp);
  return (Ok);
}
