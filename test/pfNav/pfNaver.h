#ifndef _PF_NAVER_H_
#define _PF_NAVER_H_

#include <Performer/pf/pfDCS.h>

#include <Kernel/vjKernel.h>
#include <Math/vjCoord.h>
#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>
#include <Kernel/vjDebug.h>


class pfNaver : public pfDCS
{
public:
   pfNaver();

   float& transVelocity(void) { return mTransVelocity;}
   float& rotVelocity(void) { return mRotVelocity;}

private:
   float mTransVelocity;
   float mRotVelocity;

   vjPosInterface       mWand;
   vjDigitalInterface   mButton0;
   vjDigitalInterface   mButton1;

   vjKernel* mKern;        // The Kernel


public:  // APP traversal
   virtual int app(pfTraverser*);
   virtual int needsApp(void) {return TRUE;}

   // Required for Performer class
public:
   static void init(void);
   static pfType* getClassType(void){ return mClassType;};
private:
   static pfType* mClassType;
};



pfNaver::pfNaver()
{
   setType(mClassType);  // Set the type
   mRotVelocity = 1.0f;
   mTransVelocity = 0.1f;
   mKern = vjKernel::instance();    // Store the kernel
   mWand.init("VJWand");
   mButton0.init("VJButton0");
   mButton1.init("VJButton1");
}



// app() - APP traversal function.  This overloads the standard pfNode
//	app() method, which will be called each frame during the APP
//	traversal of the scene graph (*only if* needsApp() (below) returns
//	TRUE).
//	app() is called automatically by Performer; it is not called directly
//	by a program.
int pfNaver::app(pfTraverser *trav)
{
   int button0_state = mButton0->GetData();
   int button1_state = mButton1->GetData();

   vjDEBUG(vjDEBUG_ALL, 2) << "b0: " << button0_state << "\t" << vjDEBUG_FLUSH;
   vjDEBUG(vjDEBUG_ALL, 2) << "b1: " << button1_state << endl << vjDEBUG_FLUSH;

   vjMatrix* wandMatrix;
   wandMatrix = mWand->GetData();

   vjCoord wand_coord(*wandMatrix);
   vjDEBUG(vjDEBUG_ALL, 2) << "Wand pos:" << wand_coord.pos << endl
                           << vjDEBUG_FLUSH;
   vjDEBUG(vjDEBUG_ALL, 2) << "      or:" << wand_coord.orient << endl
                           << vjDEBUG_FLUSH;

   if(1 == button0_state)   // Translation
   {
      vjVec3   forward(0.0, 0.0, -1.0f);     // -Z is forward in C2
      forward.xformFull(*wandMatrix, forward);

      pfMatrix xform;
      //xform.makeTrans(forward[0], -forward[2], forward[1]);
         // Since we are moving the model, we have to move it opposite
         // To give the apperance of moving the user
      xform.makeTrans(-forward[0], forward[2], -forward[1]);
      //xform.makeTrans(0.0f, 0.2f, 0.0f);

      pfMatrix curMat;
      getMat(curMat);
      curMat.postMult(xform);
      setMat(curMat);
   }


   if(1 == button1_state)     // Rotation
   {
      cout << "Button 1 Pressed." << endl;

      // Should really use quaternions,  For now we will use this
      float x_rot, y_rot, z_rot;
      wandMatrix->getXYZEuler(x_rot, y_rot, z_rot);

      vjMatrix new_or;
      new_or.makeXYZEuler(x_rot*0.25, y_rot*0.25, z_rot*0.25);    // Scale the rotation
      new_or.invert(new_or);                                      // Invert the rotation

      pfMatrix  new_pf_mat;
      new_pf_mat = vjGetPfMatrix(new_or);       // Get the performer equivalent matrix

      pfMatrix curMat;
      getMat(curMat);
      curMat.postMult(new_pf_mat);
      setMat(curMat);
   }

   return pfDCS::app(trav);        /* Finish by calling the parent class's app() */
}


//---------------------------------------------------------------------//
// Performer type data - this part is required for any class which
//	is derived from a Performer class.  It creates a new pfType
//	which identifies objects of this class.  All constructors for
//	this class must then call setType(classType_).
pfType *pfNaver::mClassType = NULL;

void pfNaver::init(void)
{
 if (mClassType == NULL)
	{
        pfDCS::init();				// Initialize my parent
        mClassType =  new pfType(pfDCS::getClassType(), "pfNaver");	// Create the new type
	}
}
//----------------------------------------------------------------------//

#endif
