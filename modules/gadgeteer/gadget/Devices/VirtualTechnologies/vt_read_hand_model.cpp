/*  FILE: vt_read_hand_model.c                                      */
/*                                                                  */
/*      routines for reading in and building boundary               */
/*      representation of hand model                                */
/*                                                                  */
/*  ==============================================================  */
/*  --  Copyright (C) 1992, 1993 Virtual Technologies           --  */
/*  --                                                          --  */
/*  --    Author: Larry Edwards                                 --  */
/*  ==============================================================  */

#include <gadget/gadgetConfig.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>

#include <gadget/Devices/VirtualTechnologies/CyberGlove.h>
#include <gadget/Devices/VirtualTechnologies/vt_read_user_cal-types.h>
#include <gadget/Devices/VirtualTechnologies/vt_virtual_hand-types.h>
#include <gadget/Devices/VirtualTechnologies/vt_globals.h>
#include <gadget/Devices/VirtualTechnologies/vt_math3d.h>
#include <gadget/Devices/VirtualTechnologies/vt_read_object-types.h>


static void
transform_object(objptr thisobj, matrix4x4 xform)
{
  int i,numverts;
  vertptr vertices;

  numverts = thisobj->numverts;
  vertices = thisobj->vertices;

  for (i=0; i < numverts; i++)
    vt_transform3(vertices[i].pos,xform,vertices[i].pos);
}

void
read_model_geom(char infilename[], vec3d geom[5][4], vec3d thumb_roll_vecs[2])
{
  char dummy[82], input_line[82];
  FILE *inputfp;
  int finger,joint,i;
  pos3d axis_seg[2];
  matrix4x4 rotmatrix;

  inputfp = fopen(infilename,"r");

  /* The nice gentlemen at Viewpoint have their hand pointing in */
  /* the -y direction */
  vt_rot_matrix(M_PI,'z',rotmatrix);

  for (i=0; i<2; i++)
  {
    while (sscanf(fgets(input_line, 81, inputfp), "%[#]", dummy));
    if (!feof(inputfp))
      sscanf(input_line,"%f %f %f",&axis_seg[0][VX],&axis_seg[0][VY],
         &axis_seg[0][VZ]);
    while (sscanf(fgets(input_line, 81, inputfp), "%[#]", dummy));
    if (!feof(inputfp))
      sscanf(input_line,"%f %f %f",&axis_seg[1][VX],&axis_seg[1][VY],
         &axis_seg[1][VZ]);
    /* inside edge minus outside edge of thumb */
    vt_vec_diff3(axis_seg[0],axis_seg[1],thumb_roll_vecs[i]);
    vt_transform3(thumb_roll_vecs[i],rotmatrix,thumb_roll_vecs[i]);
  }

  for (finger=THUMB; finger < FINGERS; finger++)
  {
    for (joint=MCP; joint < JOINTS; joint++)
    {
      /* skip comments */
      while (sscanf(fgets(input_line, 81, inputfp), "%[#]", dummy));
      if (feof(inputfp))
    break;
      sscanf(input_line,"%f %f %f",&geom[finger][joint][VX],
         &geom[finger][joint][VY],&geom[finger][joint][VZ]);
      vt_transform3(geom[finger][joint],rotmatrix,geom[finger][joint]);
    }
    if (feof(inputfp))
      break;
  }
  fclose(inputfp);
}

void
angle_fudge_factors(int finger, int joint, vec3d angles)
{
  angles[VX] = angles[VY] = angles[VZ] = 0.0;

  angles[VX] = 0.1;
  if (finger == THUMB)
    switch (joint)
    {
    case MCP: angles[VX] = -0.02; break;
    case PIP: angles[VX] = -0.1; angles[VZ] = 0.0; break;
    case DIP: break;
    }
}

void
scale_fudge_factors(int finger, int joint, vec3d scalevec)
{
  scalevec[VY] = 1.0;
  scalevec[VX] = scalevec[VZ] = 1.2; /* Make the finger diameter larger. */

  if (finger == THUMB)
    scalevec[VZ] = 1.0;
}

void
translation_fudge_factors(int finger, int joint, vec3d transvec)
{
  transvec[VX] = transvec[VY] = transvec[VZ] = 0.0;

  if (finger == THUMB && joint == DIP) {
      transvec[VX] = 0.0;
      transvec[VY] = -0.1;
      transvec[VZ] = 0.0;
    }
}


static void
adjust_hand_model_geometry(vec3d geom[5][4], vec3d thumb_roll_vecs[2],
               VirtualHand hand)
{
  UserGeometry user = hand->private_data->user;
  vec3d angles,scalevec,segvec,jointpos,handmodelpos,tempvec;
  matrix4x4 rotmatrix,transmatrix,scalematrix,totmatrix;
  float length_model_seg,length_hand_seg;
  int finger,joint;

  for (finger=THUMB; finger < FINGERS; finger++)
  {
    for (joint=MCP; joint < ABDUCT; joint++)
    {
      translation_fudge_factors(finger,joint,jointpos);
      angle_fudge_factors(finger,joint,angles);
      scale_fudge_factors(finger,joint,scalevec);

      /* first we move the joint to the origin */
      vt_vec_add3(jointpos,geom[finger][joint],jointpos);
      vt_vec_neg3(jointpos,jointpos);
      vt_trans_matrix(jointpos,transmatrix);

      /* then we rotate it */
      vt_vec_diff3(geom[finger][joint],geom[finger][joint+1],segvec);

      /* we align by rotating around z first then x */
      angles[VZ] += M_PI/2.0 - atan2(segvec[VY],segvec[VX]);
      vt_rot_matrix(angles[VZ],'z',rotmatrix);
      vt_transform3(segvec,rotmatrix,segvec);
      angles[VX] += -atan2(segvec[VZ],segvec[VY]);
      vt_mult_rot_matrix(angles[VX],'x',Postmult,rotmatrix);
      if ((finger == THUMB) && (joint != MCP)) /* we also roll */
      {
    vt_copy_vec3(thumb_roll_vecs[joint-1],tempvec);
    vt_transform3(thumb_roll_vecs[joint-1],rotmatrix,
              thumb_roll_vecs[joint-1]);
    /* after being transformed the roll vec should be perpendicular      */
    /* to the y-axis we assume roll vec is pointing to the inside of the */
    /* thumb. NOTE: below we do not negate the value returned by atan2   */
    /* this is because for the x-z plane the angle returned by atan2     */
    /* is the negative of the actual angle                               */
    angles[VY] += atan2(thumb_roll_vecs[joint-1][VZ],
                thumb_roll_vecs[joint-1][VX]);
    vt_mult_rot_matrix(angles[VY],'y',Premult,rotmatrix);
      }

      /* finally we scale it */
      length_model_seg = vt_vec_length3(segvec);
      vt_vec_diff3(user->geom[finger][joint],user->geom[finger][joint+1],segvec);
      length_hand_seg = vt_vec_length3(segvec);
      scalevec[VY] *= length_hand_seg/length_model_seg;
      scalevec[VX] *= 1.8;
      scalevec[VZ] *= 1.8;
      if (user->right_hand != TRUE)
    scalevec[VX] = -scalevec[VX];
      vt_scale_matrix(scalevec,scalematrix);

      vt_mult_matrix(transmatrix,rotmatrix,totmatrix);
      vt_mult_matrix(totmatrix,scalematrix,totmatrix);
      transform_object(&(hand->surface->digit[finger][joint]),totmatrix);
    }
  }

  /* lets do the hand now... */
  vt_set_vec3(0.06,-4.21,0.0,handmodelpos);
  vt_vec_neg3(handmodelpos,handmodelpos);
  vt_trans_matrix(handmodelpos,transmatrix);
  /* determine scaling of hand model based on the location of the user's first */
  /* knuckle of the index finger relative to the origin */
  vt_set_vec3(0.06,-4.21,0.0,handmodelpos);
  vt_vec_diff3(handmodelpos,geom[INDEX][MCP],jointpos);
  if ((jointpos[VX] != 0.0) && (user->geom[INDEX][MCP][VX] != 0.0))
    scalevec[VX] = fabs(user->geom[INDEX][MCP][VX]/jointpos[VX]);
  else
    scalevec[VX] = 1.0;     /* can't get there by scaling! */
  if ((jointpos[VY] != 0.0) && (user->geom[INDEX][MCP][VY] != 0.0))
    scalevec[VY] = fabs(user->geom[INDEX][MCP][VY]/jointpos[VY]);
  else
    scalevec[VY] = 1.0;     /* can't get there by scaling! */
  scalevec[VX] *= 1.1;
  scalevec[VZ] = 1.9;

  if (user->right_hand != TRUE)
    scalevec[VX] = -scalevec[VX];

  vt_scale_matrix(scalevec,scalematrix);
  vt_mult_matrix(transmatrix,scalematrix,totmatrix);
  transform_object(&(hand->surface->palm),totmatrix);

  /* lets do the forearm now... translation and scaling of forearm are same */
  /* as hand, so we just reuse totmatrix */
  transform_object(&(hand->surface->forearm),totmatrix);
}

static void
skip_comments(FILE *inputfp)
{
  char ch,comment_line[256];

  while (((ch = getc(inputfp)) == '!') && (ch != EOF))
    fgets(comment_line,256,inputfp);
  ungetc(ch,inputfp);
}

void
CyberGloveBasic::vt_read_hand_model(char infilename[], VirtualHand hand, char *glovedir)
{
  int finger,joint,numobjects;
  vec3d thumb_roll_vecs[2];
  vec3d geom[5][4];
  FILE *inputfp;
  matrix4x4 totmatrix,rotmatrix;
  static vec3d scalevec = {0.4475,0.4475,0.4475};
  static vec3d transvec = {-0.06,4.2,0.1};
  Boolean right_handed;
  char geomfile[100];

/*  cout << "\nReading "<< infilename << " ..."; */

  if (hand->right_hand != NULL)
    right_handed = *(hand->right_hand);
  else
    right_handed = TRUE;

  inputfp = fopen(infilename,"r");
  /* skip any comments at the beginning of the file */
  skip_comments(inputfp);
  fscanf(inputfp,"\tTotal Number of Objects In World:%d\n",&numobjects);

  if (numobjects < (1+FINGERS*ABDUCT))
    printf("WARNING in vt_read_hand_model: file contains too few objects");

  /* the palm and first phalanx of the thumb were created at a different     */
  /* scale and orientation than the rest of the model, so we use a different */
  /* transform for them                                                      */
  vt_rot_matrix(M_PI,'z',totmatrix);
  vt_mult_scale_matrix(scalevec,Postmult,totmatrix);
  vt_mult_trans_matrix(transvec,Postmult,totmatrix);
  vt_mult_rot_matrix(M_PI,'z',Postmult,totmatrix);

  /* Viewpoint has their hand pointing in the -y direction !@%$&*^ */
  vt_rot_matrix(M_PI,'z',rotmatrix);

  vt_read_object(inputfp,&(hand->surface->forearm));
  transform_object(&(hand->surface->forearm),rotmatrix);

  vt_read_object(inputfp,&(hand->surface->palm));
  transform_object(&(hand->surface->palm),totmatrix);

  for (finger=THUMB; finger < FINGERS; finger++)
    for (joint=MCP; joint < ABDUCT; joint++)
    {
      vt_read_object(inputfp,&(hand->surface->digit[finger][joint]));
      if ((finger == THUMB) && (joint == MCP))
    transform_object(&(hand->surface->digit[finger][joint]),totmatrix);
      else
    transform_object(&(hand->surface->digit[finger][joint]),rotmatrix);

      vt_calculate_face_normals(&(hand->surface->digit[finger][joint]),
                right_handed);
      vt_calculate_dihedral_angles(&(hand->surface->digit[finger][joint]));
      vt_calculate_vertex_normals(&(hand->surface->digit[finger][joint]));
    }
  /* HACK ALERT hardwired filename - LJE */
  sprintf(geomfile, "%s/%s", glovedir, "hand_model.geom");
  read_model_geom(geomfile,geom,thumb_roll_vecs);
  adjust_hand_model_geometry(geom,thumb_roll_vecs,hand);

  vt_calculate_face_normals(&(hand->surface->forearm),right_handed);
  vt_calculate_dihedral_angles(&(hand->surface->forearm));
  vt_calculate_vertex_normals(&(hand->surface->forearm));

  vt_calculate_face_normals(&(hand->surface->palm),right_handed);
  vt_calculate_dihedral_angles(&(hand->surface->palm));
  vt_calculate_vertex_normals(&(hand->surface->palm));

  for (finger=THUMB; finger < FINGERS; finger++)
    for (joint=MCP; joint < ABDUCT; joint++)
    {
      vt_calculate_face_normals(&(hand->surface->digit[finger][joint]),
                right_handed);
      vt_calculate_dihedral_angles(&(hand->surface->digit[finger][joint]));
      vt_calculate_vertex_normals(&(hand->surface->digit[finger][joint]));
    }

  fclose(inputfp);
}

void
CyberGloveBasic::vt_read_lowres_hand_model(char infilename[], VirtualHand hand)
{
  int finger,joint,numobjects;
  FILE *inputfp;
  matrix4x4 totmatrix;
  static vec3d digitvec,scalevec;
  Boolean right_handed;

  if (hand->right_hand != NULL)
    right_handed = *(hand->right_hand);
  else
    right_handed = TRUE;

  inputfp = fopen(infilename,"r");
  /* skip any comments at the beginning of the file */
  skip_comments(inputfp);
  fscanf(inputfp,"\tTotal Number of Objects In World:%d\n",&numobjects);

  if (numobjects < 3)
    printf("WARNING in vt_read_hand_model: file contains too few objects");

  vt_read_object(inputfp,&(hand->surface->forearm));
  vt_set_vec3(3.5,1.0,1.5,scalevec);
  if (!right_handed)
    scalevec[VX] = -scalevec[VX];

  vt_scale_matrix(scalevec,totmatrix);
  transform_object(&(hand->surface->forearm),totmatrix);
  vt_calculate_face_normals(&(hand->surface->forearm),right_handed);
  vt_calculate_dihedral_angles(&(hand->surface->forearm));
  vt_calculate_vertex_normals(&(hand->surface->forearm));

  vt_read_object(inputfp,&(hand->surface->palm));
  vt_set_vec3(1.0,1.0,1.0,scalevec);
  if (!right_handed)
    scalevec[VX] = -scalevec[VX];

  vt_scale_matrix(scalevec,totmatrix);
  transform_object(&(hand->surface->palm),totmatrix);
  vt_calculate_face_normals(&(hand->surface->palm),right_handed);
  vt_calculate_dihedral_angles(&(hand->surface->palm));
  vt_calculate_vertex_normals(&(hand->surface->palm));

  for (finger=THUMB; finger < FINGERS; finger++)
    for (joint=MCP; joint < ABDUCT; joint++)
    {
      vt_read_object(inputfp,&(hand->surface->digit[finger][joint]));
      vt_vec_sub3(hand->geom[finger][joint],hand->geom[finger][joint+1],
          digitvec);
      vt_set_vec3(1.0,vt_vec_length3(digitvec)/5.0,1.0,scalevec);

      if (!right_handed)
    scalevec[VX] = -scalevec[VX];

      vt_scale_matrix(scalevec,totmatrix);
      transform_object(&(hand->surface->digit[finger][joint]),totmatrix);
      vt_calculate_face_normals(&(hand->surface->digit[finger][joint]),
                right_handed);
      vt_calculate_dihedral_angles(&(hand->surface->digit[finger][joint]));
      vt_calculate_vertex_normals(&(hand->surface->digit[finger][joint]));
    }

  fclose(inputfp);
}
