#include <pfcave.h>
#include "simpleNavigator.h"

/************************************************************************/
/* Performer type data - this part is required for any class which
	is derived from a Performer class.  It creates a new pfType
	which identifies objects of this class.  All constructors for
	this class must then call setType(classType_).
*/

pfType *simpleNavigator::classType_ = NULL; 

void simpleNavigator::init(void)
{
 if (classType_ == NULL)
	{
        pfDCS::init();				/* Initialize the parent class */
        classType_ =  new pfType(pfDCS::getClassType(), "simpleNavigator");	/* Create the new pfType for this class, based on the parent class's type */
	}
}
/************************************************************************/



simpleNavigator::simpleNavigator(void)
{
 setType(classType_);  /* set the Performer type of this instance */
 transSpeed_ = 1.0f;
 rotSpeed_ = 1.0f;
}


void simpleNavigator::setTransSpeed(float s)
{
 transSpeed_ = s;
}


void simpleNavigator::setRotSpeed(float s)
{
 rotSpeed_ = s;
}


/*  app() - APP traversal function.  This overloads the standard pfNode
	app() method, which will be called each frame during the APP
	traversal of the scene graph (*only if* needsApp() (below) returns
	TRUE).  Here it performs the same actions as the navigate()
	callback in navobj.c++.
	app() is called automatically by Performer; it is not called directly
	by a program.
*/
int simpleNavigator::app(pfTraverser *trav)
{
 float jx=CAVE_JOYSTICK_X,jy=CAVE_JOYSTICK_Y;
 if (fabs(jx) > 0.125)
	CAVENavRot(-jx * rotSpeed_,'z');
 if (fabs(jy) > 0.125)
	{
	pfVec3 dir;
	CAVEGetVector(CAVE_WAND_FRONT,dir.vec);
	dir *= jy * transSpeed_;
	CAVENavTranslate(dir[0],dir[1],dir[2]);
	}
 if (CAVEBUTTON1 && CAVEBUTTON3)
	CAVENavLoadIdentity();
 pfCAVEDCSNavTransform(this);
 return pfDCS::app(trav);			/* Finish by calling the parent class's app() */
}

/* needsApp() - tell Performer to call app().  Most Performer node classes
	do not need to do anything in the APP traversal, and so app() is
	not called unless this function is overloaded to return TRUE.
*/
int simpleNavigator::needsApp(void)
{
 return TRUE;
}
