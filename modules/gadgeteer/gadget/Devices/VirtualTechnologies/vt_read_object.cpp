/*  FILE: vt_read_object.c                                          */
/*                                                                  */
/*      routines for reading in and building 3D object boundary     */
/*      representation                                              */
/*                                                                  */
/*  ==============================================================  */
/*  --  Copyright (C) 1992, 1993 Virtual Technologies           --  */
/*  --  Copyright (C) 1987,88,89,90,91,92 Larry Edwards         --  */
/*  --                                                          --  */
/*  --    Author: Larry Edwards                                 --  */
/*  ==============================================================  */

#include <vjConfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>

#include <X11/Intrinsic.h>

#include <Input/vjGlove/vjCyberGlove.h>
#include <Input/vjGlove/vt_globals.h>
#include <Input/vjGlove/vt_math3d.h>
#include <Input/vjGlove/vt_error.h>
#include <Input/vjGlove/vt_read_object-types.h>

int
vt_prev_index(int current_index, int numelems)
{
  return ((--current_index + numelems) % numelems);
}

int
vt_next_index(int current_index, int numelems)
{
  return (++current_index % numelems);
}

int
vt_get_index(void *baseofarray, void *thiselem, int sizeof_elem)
{
  caddr_t base,elem;

  base = (caddr_t) baseofarray;
  elem = (caddr_t) thiselem;

  return ((elem-base)/sizeof_elem);
}

loopsegptr
CyberGloveBasic::vt_loopseg_twin(loopsegptr theloopseg)
{
  if (theloopseg->edge == NULL)
    return(theloopseg);

  if (theloopseg->edge->loopseg1 == theloopseg)
    return(theloopseg->edge->loopseg2);

  return(theloopseg->edge->loopseg1);
}

static void
read_matrix(FILE *infile, matrix4x4 thematrix)
{
  int i,j;
  ;
  for (i = 0; i < 4; ++i)
  {
    fscanf(infile," [ ");
    for (j = 0; j <4; ++j)
      fscanf(infile," %f ",&(thematrix[i][j]));
    fscanf(infile," ]\n");
  }
}

static void
read_vertices(FILE *infile, objptr thisobj)
{
  int i,vertex_index,numverts;
  vec3d newvert;

  fscanf (infile," Number of Vertices:%d\n",&numverts);
  fscanf (infile," VFE List:\n");

  /* we allocate one more than numverts in case indices are 1 based */
  /* instead of 0 based */
  if ((thisobj->vertices = (vertptr) calloc(numverts+1,sizeof(vertstruct))) ==
      NULL)
    vt_fatal_unix_error("read_vertices","vertstruct calloc");

  thisobj->numverts = numverts;

  for (i = 0; i < numverts; ++i)
  {
    fscanf (infile,"Vertex %d:%f %f %f\n",&vertex_index,
	    &(newvert[VX]),&(newvert[VY]),&(newvert[VZ]));
    if (vertex_index < numverts)
    {
      thisobj->vertices[vertex_index].pos[VX] = newvert[VX];
      thisobj->vertices[vertex_index].pos[VY] = newvert[VY];
      thisobj->vertices[vertex_index].pos[VZ] = newvert[VZ];
      thisobj->vertices[vertex_index].pos[VW] = 1.0;
    }
    else
      printf("WARNING in read_vertices: index is >= numverts\n");
  }
}

static void
read_face_loop(FILE *infile, objptr thisobj, faceptr thisface)
{
  int dumfacenum,dumnumbounds,dumboundnum,index,numloopsegs,vertex_index;
  int numverts;
  ;
  /* we ignore the face index and boundary count */
  fscanf(infile,"Face:%d (%d bounds)\n",&dumfacenum,&dumnumbounds);
  /* le's are loopsegs */
  fscanf (infile,"\tBound %d (%d le's):",&dumboundnum,&numloopsegs);

  if ((thisface->loopsegs =
       (loopsegptr) calloc(numloopsegs,sizeof(loopsegstruct))) == NULL)
    vt_fatal_unix_error("read_face_loop","loopsegstruct calloc");

  thisface->numloopsegs = numloopsegs;

  numverts = thisobj->numverts;

  for (index = 0; index < numloopsegs; ++index)
  {
    fscanf (infile,"%d ",&vertex_index);
    if (vertex_index < numverts)
    {
      thisface->loopsegs[index].parentface = thisface;
      thisface->loopsegs[index].vert = &(thisobj->vertices[vertex_index]);
      thisface->loopsegs[index].vert->loopseg0 = &(thisface->loopsegs[index]);
      thisface->loopsegs[index].edge = NULL; /* determined in make_edges */
    }
    else
      printf("WARNING in read_face_loop: index is >= numverts\n");
  }
}

static void
read_faces(FILE *infile, objptr thisobj)
{
  int index,numfaces;

  fscanf (infile," Number of Faces:%d\n",&numfaces);
  fscanf(infile," FVE List:\n");

  if ((thisobj->faces = (faceptr) calloc(numfaces,sizeof(facestruct))) == NULL)
    vt_fatal_unix_error("read_faces","facestruct calloc");

  thisobj->numfaces = numfaces;

  /* we ignore the face index in the file and just make it base 0 */
  for (index = 0; index < numfaces; ++index)
  {
    thisobj->faces[index].parentobj = thisobj;
    read_face_loop(infile,thisobj,&(thisobj->faces[index]));
  }
}

static loopsegptr
find_loopseg(objptr thisobj, faceptr thisface, vertptr v1, vertptr v2)
{
  loopsegptr next_loopseg,loopsegs;
  int i,numloopsegs;
  ;
  loopsegs = thisface->loopsegs;
  numloopsegs = thisface->numloopsegs;

  for (i=0; i < numloopsegs; i++)	/* go through loop forwards */
  {
    next_loopseg = &(loopsegs[vt_next_index(i,numloopsegs)]);
    if (((loopsegs[i].vert == v1) && (next_loopseg->vert == v2)) ||
	((loopsegs[i].vert == v2) && (next_loopseg->vert == v1)))
      return (&loopsegs[i]);
  }

  return (NULL);		/*  the edge was not in this face */
}

static loopsegptr
find_other_loopseg(objptr thisobj, faceptr currentface, int loopsegindex)
{
  loopsegptr loopsegs,other_loopseg,next_loopseg;
  int i,numloopsegs;
  ;
  loopsegs = currentface->loopsegs;
  numloopsegs = currentface->numloopsegs;
  next_loopseg = &(loopsegs[vt_next_index(loopsegindex,numloopsegs)]);
  for (i=0; i<thisobj->numfaces; i++)
  {
    if (&(thisobj->faces[i]) != currentface)
    {
      other_loopseg = find_loopseg(thisobj,&(thisobj->faces[i]),
				   loopsegs[loopsegindex].vert,
				   next_loopseg->vert);
      if ((other_loopseg != NULL) && (other_loopseg->edge == NULL))
	return (other_loopseg);
    }
  }

  /* this should never happen!!! */
  /* vt_print_error("find_other_loopseg: other loopseg not found!\n");*/
  return (NULL);
}

static void
make_edges(objptr thisobj)
{
  faceptr faces;
  loopsegptr other_loopseg,loopsegs;
  edgeptr edges;
  int i,j,k,numedges,numloopsegs, edge_index = 0;

  faces = thisobj->faces;

  numedges = thisobj->numfaces + thisobj->numverts - 2;
  if ((thisobj->edges = (edgeptr) calloc(numedges,sizeof(edgestruct))) == NULL)
    vt_fatal_unix_error("make_edges","edgestruct calloc");

  edges = thisobj->edges;

  for (i=0; i < thisobj->numfaces; i++)
  {
    numloopsegs = faces[i].numloopsegs;
    loopsegs = faces[i].loopsegs;
    for (j=0; j < numloopsegs; j++)
    {
      if (loopsegs[j].edge == NULL)
      {
	/* the following should never happen but we may have a topologically */
	/* bad object so we check anyway */
	if (edge_index >= numedges)
	{			
/* Put back someday when we have valid objects
	  printf("\nWARNING in make_edges: F+V-E != 2, continuing anyway");
*/
	  /* make new bigger edges array */
	  edges = thisobj->edges = (edgeptr) realloc(edges,(numedges+64)*
						     sizeof(edgestruct));
	  /* point loopseg ptrs at the new edges array elements */
	  for (k=0; k<numedges; k++)
	  {
	    if (edges[k].loopseg1 != NULL) /* check just in case... */
	      edges[k].loopseg1->edge = &(edges[k]);
	    if (edges[k].loopseg2 != NULL)
	      edges[k].loopseg2->edge = &(edges[k]);
	  }
	  numedges += 64;
	}

	other_loopseg = find_other_loopseg(thisobj,&faces[i],j);
	if (other_loopseg == NULL)
	  other_loopseg = &(loopsegs[j]);


	/* a loopseg always points to the edge having endpoints of */
	/* loopseg->vert and loopseg[next_index(i,numloopsegs)].vert */
	loopsegs[j].edge = &(edges[edge_index]);
	other_loopseg->edge = &(edges[edge_index]);
	edges[edge_index].loopseg1 = &(loopsegs[j]);
	edges[edge_index].loopseg2 = other_loopseg;
	edges[edge_index].vertex1 = loopsegs[j].vert;
	edges[edge_index].vertex2 = other_loopseg->vert;

	edge_index++;
      }
    }
  }
  thisobj->numedges = edge_index;
}

void
CyberGloveBasic::vt_calculate_face_normals(objptr thisobj, Boolean clockwise)
{
  /* NOTE: we assume a consistent face orientation here! */
  int i,j,nextj,numloopsegs;
  float *norm;
  faceptr faces;
  loopsegptr loopsegs;

  faces = thisobj->faces;

  for (i=0; i< thisobj->numfaces; i++)
  {
    loopsegs = faces[i].loopsegs;
    numloopsegs = faces[i].numloopsegs;
    norm = faces[i].norm;
    norm[VX] = norm[VY] = norm[VZ] = 0.0;
    if (clockwise)
    {
      for (j=0; j < numloopsegs; j++)
      {
	nextj = vt_next_index(j,numloopsegs);

	norm[VX] += (loopsegs[j].vert->pos[VY] - loopsegs[nextj].vert->pos[VY]) *
	  (loopsegs[j].vert->pos[VZ] + loopsegs[nextj].vert->pos[VZ]);

	norm[VY] += (loopsegs[j].vert->pos[VZ] - loopsegs[nextj].vert->pos[VZ]) *
	  (loopsegs[j].vert->pos[VX] + loopsegs[nextj].vert->pos[VX]);

	norm[VZ] += (loopsegs[j].vert->pos[VX] - loopsegs[nextj].vert->pos[VX]) *
	  (loopsegs[j].vert->pos[VY] + loopsegs[nextj].vert->pos[VY]);
      }
    }
    else
    {
      for (j=0; j < numloopsegs; j++)
      {
	nextj = vt_next_index(j,numloopsegs);

	norm[VX] -= (loopsegs[j].vert->pos[VY] - loopsegs[nextj].vert->pos[VY]) *
	  (loopsegs[j].vert->pos[VZ] + loopsegs[nextj].vert->pos[VZ]);

	norm[VY] -= (loopsegs[j].vert->pos[VZ] - loopsegs[nextj].vert->pos[VZ]) *
	  (loopsegs[j].vert->pos[VX] + loopsegs[nextj].vert->pos[VX]);

	norm[VZ] -= (loopsegs[j].vert->pos[VX] - loopsegs[nextj].vert->pos[VX]) *
	  (loopsegs[j].vert->pos[VY] + loopsegs[nextj].vert->pos[VY]);
      }
    }
    vt_normalize3(norm,norm);
  }

}

void
CyberGloveBasic::vt_calculate_dihedral_angles(objptr thisobj)
{
  int i;
  edgeptr edges;
  float *norm1,*norm2;

  edges = thisobj->edges;
  for (i=0; i< thisobj->numedges; i++)
  {
    norm1 = edges[i].loopseg1->parentface->norm;
    norm2 = edges[i].loopseg2->parentface->norm;
    /* for the moment we don't care wether angle is acute or obtuse */
    edges[i].dihedral_angle = RAD2DEG * acos(vt_dot_prod3(norm1,norm2));
  }
}

void
CyberGloveBasic::vt_calculate_vertex_normals(objptr thisobj)
{
  /* NOTE: we assume a consistent clockwise face orientation here! */
  int i,numfaces,loopsegindex;
  float *vertnorm;
  vertptr vertices;
  loopsegptr loopseg0,thisloopseg,nextloopseg;
  faceptr parentface;

  vertices = thisobj->vertices;

  for (i=0; i< thisobj->numverts; i++)
  {
    numfaces = 0;
    vertnorm = vertices[i].norm;
    vertnorm[VX] = vertnorm[VY] = vertnorm[VZ] = 0.0;
    thisloopseg = loopseg0 = vertices[i].loopseg0;
    if (thisloopseg != NULL)
      do
      {
	vertnorm[VX] += thisloopseg->parentface->norm[VX];
	vertnorm[VY] += thisloopseg->parentface->norm[VY];
	vertnorm[VZ] += thisloopseg->parentface->norm[VZ];
	numfaces++;
	
	nextloopseg = vt_loopseg_twin(thisloopseg);
	if (thisloopseg == nextloopseg)
	  break;
	
	thisloopseg = nextloopseg;
	parentface = thisloopseg->parentface;
	loopsegindex = vt_get_index(parentface->loopsegs,thisloopseg,
				    sizeof(loopsegstruct));
	thisloopseg = &(parentface->loopsegs[vt_next_index(loopsegindex,
				   parentface->numloopsegs)]);
      } while (thisloopseg != loopseg0);

    vertnorm[VX] = vertnorm[VX] / (float) numfaces;
    vertnorm[VY] = vertnorm[VY] / (float) numfaces;
    vertnorm[VZ] = vertnorm[VZ] / (float) numfaces;
    vt_normalize3(vertnorm,vertnorm);
  }
}

void
CyberGloveBasic::vt_read_object(FILE *infile, objptr thisobj)
{
  int ch,dumobjnum,dumnumshells,dumshellnum;
  char objname[256],comment_line[256];

  /* skip any comments before each object */
  while (((ch = getc(infile)) == '!') && (ch != EOF))
    fgets(comment_line,256,infile);
  ungetc(ch,infile);

  /* the following 2 lines are only in the file format for readability */
  fscanf(infile,"\n\n\tObject %d:\n",&dumobjnum);
  fscanf(infile,"Object Name:%s\n",objname);

  fscanf (infile," Object Transform:\n");
  read_matrix(infile,thisobj->xform);
  fscanf (infile," Object Inverse Transform:\n");
  read_matrix(infile,thisobj->invxform);

  /* the following 2 lines of information are not used although */
  /* they may be in the future */
  fscanf (infile,"\nNumber of Shells:%d\n",&dumnumshells);
  fscanf(infile, "Shell %d:\n", &dumshellnum);

  /* read in all the vertices */
  read_vertices(infile,thisobj);
  /* read in all the faces */
  read_faces(infile,thisobj);
  /* construct the edges from the loopedges */
  make_edges(thisobj);
}
