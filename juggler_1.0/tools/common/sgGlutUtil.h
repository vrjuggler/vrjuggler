#ifndef _SG_GLUT_UTIL_H_
#define _SG_GLUT_UTIL_H_
//
// Utility functions for using glut
//
#include <iostream.h>
#include <stdio.h>
#include <GL/glut.h>


class sgGlutUtil
{
public:
		//   general purpose text routine.  draws a string according to the
		//   format in a stroke font at x, y after scaling it by the scale
		//   specified.  x, y and scale are all in window-space [i.e., pixels]
		//   with origin at the lower-left.
	static void text(GLuint x, GLuint y, GLfloat scale, char* text)
	{
		GLfloat font_scale = 119.05 + 33.33;

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glTranslatef(x, y, 0.0);

		glScalef(scale/font_scale, scale/font_scale, scale/font_scale);

		for(char* p = text; *p; p++)
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);

		glPopAttrib();

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
};


#endif
