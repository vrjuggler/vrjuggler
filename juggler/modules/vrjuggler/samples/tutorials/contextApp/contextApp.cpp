#include <vrj/vjConfig.h>

#include <math.h>
#include <GL/gl.h>
//#include <GL/glu.h>

#include <vrj/Math/Vec3.h>
#include <vrj/Math/Coord.h>

#include <contextApp.h>

using namespace vrj;


// Draw an axis in OpenGL
void drawAxis();

//----------------------------------------------
//  Draw the scene.
//
// - Draws a box and a coordinate axis set on the box
// - Offset and rotate the box by a matrix that we create
//----------------------------------------------
void contextApp::draw()
{
   // --- Setup for drawing --- //
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);

   // -- Get Wand matrix --- //
   Matrix wand_matrix;
   wand_matrix = *(mWand->getData());

   // --- Create a color for the wand --- //
   float wand_color[3];
   wand_color[0] = wand_color[1] = wand_color[2] = 0.7f;

   glPushMatrix();

      // --- Draw the box --- //
      glPushMatrix();
         glMultMatrixf(wand_matrix.getFloatPtr());    // Push the wand offset matrix on the stack
         glColor3fv(wand_color);
         glCallList(*mCubeDlId);
      glPopMatrix();

      // Draw a coordinate axis "on" the box
      glLineWidth(5.0f);
      glDisable(GL_LIGHTING);
      glPushMatrix();
         glMultMatrixf(wand_matrix.getFloatPtr());    // Goto wand position
         drawAxis();
      glPopMatrix();
      glEnable(GL_LIGHTING);
   glPopMatrix();
}

void contextApp::contextInit()
{
   // --- Allocate context specific data --- //
   (*mCubeDlId) = glGenLists(1);

   glNewList((*mCubeDlId), GL_COMPILE);
      glScalef(0.50f, 0.50f, 0.50f);
      drawCube();
   glEndList();

   // --- Init GL State --- //
   initGLState();    // Initialize the state like we expect it
}


void contextApp::initGLState()
{
   GLfloat light0_ambient[] = { 0.1f,  0.1f,  0.1f,  1.0f};
   GLfloat light0_diffuse[] = { 0.8f,  0.8f,  0.8f,  1.0f};
   GLfloat light0_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
   GLfloat light0_position[] = {0.0f, 0.75f, 0.75f, 0.0f};

   GLfloat mat_ambient[] = { 0.7, 0.7,  0.7,  1.0};
   GLfloat mat_diffuse[] = { 1.0,  0.5,  0.8,  1.0};
   GLfloat mat_specular[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat mat_shininess[] = { 50.0};
   GLfloat mat_emission[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat no_mat[] = { 0.0,  0.0,  0.0,  1.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

   glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
   glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
   glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
   glMaterialfv( GL_FRONT,  GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);
}

//: Utility function for drawing a cube
void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
             GLdouble z0, GLdouble z1, GLenum type)
{
   static GLdouble n[6][3] = {
      {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
   };
   static GLint faces[6][4] = {
      { 0, 1, 2, 3}, { 3, 2, 6, 7}, { 7, 6, 5, 4},
      { 4, 5, 1, 0}, { 5, 6, 2, 1}, { 7, 4, 0, 3}
   };
   GLdouble v[8][3], tmp;
   GLint i;

   if (x0 > x1)
   {
      tmp = x0; x0 = x1; x1 = tmp;
   }
   if (y0 > y1)
   {
      tmp = y0; y0 = y1; y1 = tmp;
   }
   if (z0 > z1)
   {
      tmp = z0; z0 = z1; z1 = tmp;
   }
   v[0][0] = v[1][0] = v[2][0] = v[3][0] = x0;
   v[4][0] = v[5][0] = v[6][0] = v[7][0] = x1;
   v[0][1] = v[1][1] = v[4][1] = v[5][1] = y0;
   v[2][1] = v[3][1] = v[6][1] = v[7][1] = y1;
   v[0][2] = v[3][2] = v[4][2] = v[7][2] = z0;
   v[1][2] = v[2][2] = v[5][2] = v[6][2] = z1;

   for (i = 0; i < 6; i++)
   {
      glBegin(type);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][0]][0]);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][1]][0]);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][2]][0]);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][3]][0]);
      glEnd();
   }
}


void drawAxis()
{
   Vec3 x_axis(7.0f,0.0f,0.0f);
   Vec3 y_axis(0.0f, 7.0f, 0.0f);
   Vec3 z_axis(0.0f, 0.0f, 7.0f);
   Vec3 origin(0.0f, 0.0f, 0.0f);

   glBegin(GL_LINES);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex3fv(origin.vec);
      glVertex3fv(x_axis.vec);

      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex3fv(origin.vec);
      glVertex3fv(y_axis.vec);

      glColor3f(0.0f, 0.0f, 1.0f);
      glVertex3fv(origin.vec);
      glVertex3fv(z_axis.vec);
   glEnd();
}
