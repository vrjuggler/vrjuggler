package org.vrjuggler.vrjconfig.customeditors.surfacedisplayeditor;

import java.awt.*;
import java.awt.event.*;
import java.util.Enumeration;

import javax.media.j3d.*;
import javax.vecmath.*;

import com.sun.j3d.utils.geometry.Sphere;


class PickBehavior extends Behavior 
{
   private WakeupCriterion mouseCriterion;
   private BranchGroup branchGroup;	
   private Canvas3D canvas3D;
   private SurfaceDisplayEditor mController = null;

   public PickBehavior(BranchGroup branchGroup, Canvas3D canvas3D, SurfaceDisplayEditor controller)
   {
      branchGroup = branchGroup;		
      canvas3D = canvas3D;
      mController = controller;
      //this.shape3D1 = shape3D1;
      //this.shape3D2 = shape3D2;
   }

   public void initialize() 
   {
      mouseCriterion = new WakeupOnAWTEvent(MouseEvent.MOUSE_CLICKED);    

      System.out.println("Initialize");
      wakeupOn (mouseCriterion);
   }

   public void processStimulus (Enumeration criteria) 
   {
      AWTEvent[] event = ((WakeupOnAWTEvent)criteria.nextElement()).getAWTEvent();
      for (int i=0; i<event.length; i++) 
      { 
         int x = ((MouseEvent)event[i]).getX();
         int y = ((MouseEvent)event[i]).getY();

         PickRay pickRay = generatePickRay(x,y);					
         SceneGraphPath sceneGraphPath = branchGroup.pickClosest(pickRay);
         if (null == sceneGraphPath)
         {
            //System.out.println("Nothing picked...");
         }
         else
         {
            Shape3D actualShape3D = (Shape3D)sceneGraphPath.getObject();							

            if (actualShape3D.getUserData() instanceof ViewPort)
            {
               System.out.println("Show corners");
               mController.setActiveViewPort((ViewPort)actualShape3D.getUserData());
            }
            else if (actualShape3D.getUserData() instanceof Sphere)
            {
               System.out.println("Select Cornet");
               mController.setActiveSphere((Sphere)actualShape3D.getUserData());
            }
         }
         //System.out.println(sceneGraphPath);
      }
      wakeupOn(mouseCriterion);
   }

   public PickRay generatePickRay(int xpos, int ypos)    
   {
      Transform3D motion=new Transform3D();
      Point3d eyePosn = new Point3d();
      Point3d mousePosn = new Point3d();
      Vector3d mouseVec=new Vector3d();
      canvas3D.getCenterEyeInImagePlate(eyePosn);
      canvas3D.getPixelLocationInImagePlate(xpos,ypos,mousePosn);
      canvas3D.getImagePlateToVworld(motion);

      motion.transform(eyePosn);
      motion.transform(mousePosn);
      mouseVec.sub(mousePosn, eyePosn);
      mouseVec.normalize();

      return new PickRay(eyePosn, mouseVec);
   }
}
