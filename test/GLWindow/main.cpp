/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#include <GL/gl.h>
#include <GL/glu.h>
#include "../../C2glxWindow.h"
#include <unistd.h>

#include <sys/types.h>
#include <sys/prctl.h>

void  initScene(void *);
void cjDrawScene(void*);
void cjFrameFunc(void*);

float rotation=0.0;
float distance(float a[3], float b[3]);
float conepos[3] = {0.0, 0.0, 0.0};
float *sphereyptr;
float min_dist = 0.5;

float *cnpos, *clpos, *sppos;
float cnrotate = 0.0;
GLUquadricObj *cyl, *sp, *cone;

  C2glxWindow w,w2,w3;

void childp (void* i) {
  for(;;) {
    cout << "sphere height: " << sppos[1] << endl;
  w.makeCurrent();
  cjDrawScene(NULL);
  w.swapBuffers();
  }
}


main() {

  cnpos = new float[3];
  clpos = new float[3];
  sppos = new float[3];
  cnpos[0] = -4.0;
  cnpos[1] = 0.0;
  cnpos[2] = 0.0;
  clpos[0] = 0.0;
  clpos[1] = 0.5;
  clpos[2] = 3.0;
  sppos[0] = 0.0;
  sppos[1] = 3.8;
  sppos[2] = -1.0;





  w.config();
  w.open();
  w2.config();
  w2.open();
  w3.config();
  w3.open();

  initScene(NULL);

sproc(childp, PR_SADDR);

  for(;;) {
    cjFrameFunc(NULL);
    //    w.makeCurrent();
    //cjDrawScene(NULL);
    //w.swapBuffers();
    w2.makeCurrent();
    cjDrawScene(NULL);
    w2.swapBuffers();
    w3.makeCurrent();
    cjDrawScene(NULL);
    w3.swapBuffers();
  }
}

void initScene(void *unused) {   

  GLfloat light0_ambient[] = { .1,  .1,  .1,  1.0 };
    GLfloat light0_diffuse[] = { .8,  0.8,  0.8,  1.0 };
    GLfloat light0_specular[] = { 0.4,  0.4,  0.5,  1.0 };
    GLfloat light0_position[] = {10.0, 10.0, 00.0, 1.0 };  
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

                
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH); 
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);    


    cone = gluNewQuadric();
  cyl = gluNewQuadric();
  sp = gluNewQuadric();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (90,1,0.5,50);
  glMatrixMode(GL_MODELVIEW);
  }

void cjFrameFunc(void *unused) {

 sppos[1] = (sppos[1] > 3.0)?-3.0:(sppos[1] + .01);
 cnrotate += 1.0;
 if (cnrotate > 360.0) cnrotate = 0;
}

void cjDrawScene(void *unused) {
  //  sginap(50);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT  );
  /* draw objects */
  // cone
  glRotatef (cnrotate, 0.0, 1.0, 0.0);
  glTranslatef (cnpos[0],cnpos[1],cnpos[2]);
    glColor3f (1.0, 0.3, 0.3);
  gluCylinder ( cone, 1.0, 0.0, 1.0, 20, 5);
  glTranslatef (-cnpos[0], -cnpos[1], -cnpos[2]);
  glRotatef (-cnrotate, 0.0, 1.0, 0.0);

  // cylinder
  glTranslatef (clpos[0],clpos[1],clpos[2]);
  glColor3f (0.3, 1.0, 0.3);
  gluCylinder (cyl, 0.3, 0.3, 0.8, 8, 8);
  glTranslatef (-clpos[0], -clpos[1], -clpos[2]);

  // sphere
  glTranslatef (sppos[0],sppos[1],sppos[2]);
  glColor3f (0.3, 0.3, 1.0);
  gluSphere (sp, 0.5,8,8);
  glTranslatef (-sppos[0], -sppos[1], -sppos[2]);


}


