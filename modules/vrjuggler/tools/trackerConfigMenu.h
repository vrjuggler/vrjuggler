/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _TRK_CONFIG_MENU_H_
#define _TRK_CONFIG_MENU_H_

#include <sgGluiWin.h>
#include <trackerConfigApp.h>

//--------------- PANEL CLASS --------------------------------------
//
//: A class the creates a panel with pos and rotation information in glui
//
//------------------------------------------------------------------------
class posRotPanel
{
public:
   posRotPanel(GLUI* theGlui)
   {
      mGLUI = theGlui;
   }

   //: Called to add the panel to the glui window
   //! NOTE: Only call once!!!
   //! ARGS: name - Name of the rollout
   //! ARGS: open - Open initialy?
   //! ARGS: spinnerCallbackId - Id passed to the callback for the spinners
   //! ARGS: tbCallbackId - Id passed to the callback for the trackball thingie
   //! POST: The rollout is build and added to the glui window
   GLUI_Rollout* add_panel(char* name, bool open, int spinnerCallbackId, int tbCallbackId)
   {
      mRollout = mGLUI->add_rollout(name,open);
      constructPanel(spinnerCallbackId,tbCallbackId);
   }

   //: Called to add the panel to another panel
   //! NOTE: Only call once!!!
   //! ARGS: panel - The panel (or rollout) to add the new rollout to
   //! ARGS: (Same as add_panel())
   //! POST: The rollout is built and added to the glui window
   GLUI_Rollout* add_panel_to_panel(GLUI_Panel* panel, char* name, bool open, int spinnerCallbackId, int tbCallbackId)
   {
      mRollout = mGLUI->add_rollout_to_panel(panel, name, open);
      constructPanel(spinnerCallbackId, tbCallbackId);
   }

   // Called when one of the spinners causes an event
   //! POST: mMatrix is in sync with the new values
   void processSpinnerEvent()
   {
      // Make a matrix T*R
      vrj::Matrix rot_mat;
      rot_mat.makeXYZEuler(mXRotSpinner->get_float_val(), mYRotSpinner->get_float_val(), mZRotSpinner->get_float_val());

      mMatrix.makeTrans(mXPosSpinner->get_float_val(), mYPosSpinner->get_float_val(), mZPosSpinner->get_float_val());
      mMatrix.postMult(rot_mat);         // xformMat = T*R

      // Update the trackball with not translated version
      mTbRotation->set_float_array_val(rot_mat.getFloatPtr());
   }

   // Called when the trackball generates an event
   //! POST: mMatrix is in sync with the new values
   //+       The spinners are updated with the equivalent values
   void processTbEvent()
   {
      // Set mMatrix to new value
      float m[16];
      vrj::Matrix rot_mat;
      mTbRotation->get_float_array_val(m);
      rot_mat.set(m);

      // Extract the individual components
      float xr,yr,zr,xt,yt,zt;
      rot_mat.getXYZEuler(xr,yr,zr);

      // Update the spinners
      mXRotSpinner->set_float_val(xr); mYRotSpinner->set_float_val(yr); mZRotSpinner->set_float_val(zr);

      // Rebuild the full matrix
      rot_mat.makeXYZEuler(xr,yr,zr);
      mMatrix.makeTrans(mXPosSpinner->get_float_val(), mYPosSpinner->get_float_val(), mZPosSpinner->get_float_val());
      mMatrix.postMult(rot_mat);         // xformMat = T*R
   }

   vrj::Matrix getMatrix()
   { return mMatrix; }

protected:
   // Build the panel inside the rollout
   void constructPanel(int spinnerCallbackId, int tbCallbackId)
   {
      const int SpinnerWidth(20);
      const float MinAngle(-180.0f);
      const float MaxAngle(180.0f);

      // Build Pos panel
      mPosPanel = mGLUI->add_panel_to_panel(mRollout,"Pos");
      mXPosSpinner = mGLUI->add_spinner_to_panel(mPosPanel,"X",GLUI_SPINNER_FLOAT,NULL,spinnerCallbackId,sgGluiWin::gluiCallback);
      mXPosSpinner->set_alignment(GLUI_ALIGN_LEFT); mXPosSpinner->set_w(SpinnerWidth);
      mGLUI->add_column_to_panel(mPosPanel,false);
      mYPosSpinner = mGLUI->add_spinner_to_panel(mPosPanel,"Y",GLUI_SPINNER_FLOAT,NULL,spinnerCallbackId,sgGluiWin::gluiCallback);
      mYPosSpinner->set_alignment(GLUI_ALIGN_LEFT); mYPosSpinner->set_w(SpinnerWidth);
      mGLUI->add_column_to_panel(mPosPanel,false);
      mZPosSpinner = mGLUI->add_spinner_to_panel(mPosPanel,"Z",GLUI_SPINNER_FLOAT,NULL,spinnerCallbackId,sgGluiWin::gluiCallback);
      mZPosSpinner->set_alignment(GLUI_ALIGN_LEFT); mZPosSpinner->set_w(SpinnerWidth);

      // Build Rotation panel
      mRotPanel = mGLUI->add_panel_to_panel(mRollout,"Rot");
      mXRotSpinner = mGLUI->add_spinner_to_panel(mRotPanel,"X",GLUI_SPINNER_FLOAT,NULL,spinnerCallbackId,sgGluiWin::gluiCallback);
      mXRotSpinner->set_alignment(GLUI_ALIGN_LEFT); mXRotSpinner->set_w(SpinnerWidth);
      mXRotSpinner->set_float_limits(MinAngle,MaxAngle);
      mGLUI->add_column_to_panel(mRotPanel,false);
      mYRotSpinner = mGLUI->add_spinner_to_panel(mRotPanel,"Y",GLUI_SPINNER_FLOAT,NULL,spinnerCallbackId,sgGluiWin::gluiCallback);
      mYRotSpinner->set_alignment(GLUI_ALIGN_LEFT); mYRotSpinner->set_w(SpinnerWidth);
      mYRotSpinner->set_float_limits(MinAngle,MaxAngle);
      mGLUI->add_column_to_panel(mRotPanel,false);
      mZRotSpinner = mGLUI->add_spinner_to_panel(mRotPanel,"Z",GLUI_SPINNER_FLOAT,NULL,spinnerCallbackId,sgGluiWin::gluiCallback);
      mZRotSpinner->set_alignment(GLUI_ALIGN_LEFT); mZRotSpinner->set_w(SpinnerWidth);
      mZRotSpinner->set_float_limits(MinAngle,MaxAngle);

      // Build the cool panel
      mTbPanel = mGLUI->add_panel_to_panel(mRollout,"");
      mTbRotation = mGLUI->add_rotation_to_panel(mTbPanel,"Rot",NULL,tbCallbackId,sgGluiWin::gluiCallback);
      mTbRotation->set_spin(0.97f);
   }

public:
   GLUI* mGLUI;

   GLUI_Rollout* mRollout;
   GLUI_Panel   *mPosPanel, *mRotPanel;
   GLUI_Spinner *mXPosSpinner,*mYPosSpinner,*mZPosSpinner;
   GLUI_Spinner *mXRotSpinner,*mYRotSpinner,*mZRotSpinner;

   GLUI_Panel     *mTbPanel;
   GLUI_Rotation  *mTbRotation, *mTbRotation2;

   vrj::Matrix    mMatrix;       // Matrix of the T*R
};


//-----------------------------------------------------------------------------------
//: This is a base class for general glui windows.
//
// NOTE: For now this is a singleton.  Only one is allowed to be created at a time.
//
//------------------------------------------------------------------------------------
class trackerConfigMenu : public sgGluiWin
{
protected:
	trackerConfigMenu() : mApp(NULL)
	{
		createWindow("Configuration Information",0,900,300);
	}

	enum { modelRotCB, tmtSpinnerCB, tmtTbCB, trk1SpinnerCB, trk1TbCB, trk2SpinnerCB, trk2TbCB, wireframeCB};

public:
		// The interface should be created in this function.
		// All glui interface items should be created in this class
	virtual void constructUI()
	{
		gluiWin()->add_statictext("Positional Information");
		gluiWin()->add_separator();

      // Model rotation
      mModelRotation = gluiWin()->add_rotation("Model Rotation",NULL,modelRotCB,sgGluiWin::gluiCallback);
      mModelRotation->set_spin(0.97);

      // Transmitter
      mTmtPosRot = new posRotPanel(gluiWin());
      mTmtPosRot->add_panel("Transmitter",false,tmtSpinnerCB,tmtTbCB);

      // Tracker 1
      mTrk1PosRot = new posRotPanel(gluiWin());
      mTrk1PosRot->add_panel("Tracker",false,trk1SpinnerCB,trk1TbCB);

      // Tracker 2
      mTrk2PosRot = new posRotPanel(gluiWin());
      mTrk2PosRot->add_panel("Tracker Relative Proxy",false,trk2SpinnerCB,trk2TbCB);

      gluiWin()->add_separator();
      mWireframeCB =
			gluiWin()->add_checkbox( "Wireframe", NULL, wireframeCB, sgGluiWin::gluiCallback);
		gluiWin()->add_separator();		
	}
	
   void updateModelMatrix()
   {
      float m[16];
      vrj::Matrix rot_mat;
      mModelRotation->get_float_array_val(m);
      rot_mat.set(m);

      mApp->setModelMatrix(rot_mat);
   }

	//: This function is called by the callback to interact with widgets
	virtual void interact(int wId)
	{
		//vprASSERT(mApp != NULL);
		cout << "Glui:interact: wId: " << wId << endl;

		switch(wId)
		{
		case wireframeCB:
			if(mWireframeCB->get_int_val() == 0)
				mApp->setWireframeMode(false);
			else
				mApp->setWireframeMode(true);
			break;
		
      // Model rotation
      case modelRotCB:
         updateModelMatrix();
         break;

      // Transmitter
      case tmtSpinnerCB:
			mTmtPosRot->processSpinnerEvent();
         mApp->setTransmitterPos(mTmtPosRot->getMatrix());
         // Post redisplay to make trackball draw again.  XXX: glui bug work around
         /*
	 int old_win = glutGetWindow();
         glutSetWindow(gluiWin()->get_glut_window_id());
         glutPostRedisplay();
         glutSetWindow(old_win);
	 */
			break;
      case tmtTbCB:
         mTmtPosRot->processTbEvent();
         mApp->setTransmitterPos(mTmtPosRot->getMatrix());
         break;

      // Tracker1
      case trk1SpinnerCB:
			mTrk1PosRot->processSpinnerEvent();
         mApp->setTrackerPos(mTrk1PosRot->getMatrix());
			break;
      case trk1TbCB:
         mTrk1PosRot->processTbEvent();
         mApp->setTrackerPos(mTrk1PosRot->getMatrix());
         break;

      // Tracker2
      case trk2SpinnerCB:
			mTrk2PosRot->processSpinnerEvent();
         mApp->setProxyPos(mTrk2PosRot->getMatrix());
			break;
      case trk2TbCB:
         mTrk2PosRot->processTbEvent();
         mApp->setProxyPos(mTrk2PosRot->getMatrix());
         break;


		default:
			break;
		}
	}

	void setApp(trackerConfigApp* app)
	{ mApp = app; }

protected:
	trackerConfigApp*	mApp;

   GLUI_Rotation* mModelRotation;

   // Tmt <--- Transmitter
   // Trk <--- Tracker
   posRotPanel* mTmtPosRot;
   posRotPanel* mTrk1PosRot;
   posRotPanel* mTrk2PosRot;

   GLUI_Checkbox* mWireframeCB;

		// ---------- SINGLETON -------- //
public:
		// NOTE: When deriving subclass, override this function to create a
		//        New get instance.
		// NOTE2: This one will be that one that muiCallback uses, but that is fine
		//        since it will never be called with a NULL _instance
	static trackerConfigMenu* getInstance()
	{
		if(_instance == NULL)
			_instance = new trackerConfigMenu;
		return dynamic_cast<trackerConfigMenu*>(_instance);
	}
};


#endif
