#ifndef _WIRE_SPHER_MODE_H_
#define _WIRE_SPHER_MODE_H_

#include <TestMode.h>

#include "WallTest.h"
#include "WallTestHelpers.h"

#include <GL/gl.h>
#include <GL/glu.h>

namespace
{
void fghCircleTable(double **sint,double **cost,const int n)
{
    int i;

    /* Table size, the sign of n flips the circle direction */

    const int size = abs(n);

    /* Determine the angle between samples */

    const double angle = 2*M_PI/(double)( ( n == 0 ) ? 1 : n );

    /* Allocate memory for n samples, plus duplicate of first entry at the end */

    *sint = (double *) calloc(sizeof(double), size+1);
    *cost = (double *) calloc(sizeof(double), size+1);

    if (!(*sint) || !(*cost))
    {
        free(*sint);
        free(*cost);
    }

    /* Compute cos and sin around the circle */

    (*sint)[0] = 0.0;
    (*cost)[0] = 1.0;

    for (i=1; i<size; i++)
    {
        (*sint)[i] = sin(angle*i);
        (*cost)[i] = cos(angle*i);
    }

    /* Last sample is duplicate of the first */

    (*sint)[size] = (*sint)[0];
    (*cost)[size] = (*cost)[0];
}

void drawWireSphere(GLdouble radius, GLint slices, GLint stacks)
{
    int i,j;

    /* Adjust z and radius as stacks and slices are drawn. */

    double r;
    double x,y,z;

    /* Pre-computed circle */

    double *sint1,*cost1;
    double *sint2,*cost2;

    fghCircleTable(&sint1,&cost1,-slices  );
    fghCircleTable(&sint2,&cost2, stacks*2);

    /* Draw a line loop for each stack */

    for (i=1; i<stacks; i++)
    {
        z = cost2[i];
        r = sint2[i];

        glBegin(GL_LINE_LOOP);

            for(j=0; j<=slices; j++)
            {
                x = cost1[j];
                y = sint1[j];

                glNormal3d(x,y,z);
                
		float red = ((x*r) + 1.0) / 2.0;
		float green = ((y*r) + 1.0) / 2.0;
		float blue = ((z*r) + 1.0) / 2.0;
	
                glColor3f(red, green, blue);
                glVertex3d(x*r*radius,y*r*radius,z*radius);
            }

        glEnd();
    }

    /* Draw a line loop for each slice */

    for (i=0; i<slices; i++)
    {
        glBegin(GL_LINE_STRIP);

            for(j=0; j<=stacks; j++)
            {
                x = cost1[i]*sint2[j];
                y = sint1[i]*sint2[j];
                z = cost2[j];

                glNormal3d(x,y,z);

		float red = ((x) + 1.0) / 2.0;
		float green = ((y) + 1.0) / 2.0;
		float blue = ((z) + 1.0) / 2.0;

                glColor3f(red, green, blue);
                glVertex3d(x*radius,y*radius,z*radius);
            }

        glEnd();
    }

    /* Release sin and cos tables */

    free(sint1);
    free(cost1);
    free(sint2);
    free(cost2);
}
}

class WireSphereMode : public TestMode
{
public:
   virtual std::string getName()
   { return std::string("WireSphereMode"); }

   virtual void update(WallTest *wallTest)
   {
   }
   
   virtual void draw(WallTest *wallTest)
   {
   
      glPushMatrix();
      glDisable(GL_LIGHTING);
      glLineWidth(2.0f);
      glRotatef(90.0f-25.0f, 1.0f, 0.0f, 0.0f);
      drawWireSphere (75.0, 50, 50);
      glEnable(GL_LIGHTING);
      glPopMatrix();   
   }


};



#endif

