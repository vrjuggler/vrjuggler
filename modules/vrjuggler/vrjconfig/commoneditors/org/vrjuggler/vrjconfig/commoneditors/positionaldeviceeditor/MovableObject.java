package org.vrjuggler.vrjconfig.commoneditors.positionaldeviceeditor;

import javax.swing.event.EventListenerList;
import java.awt.event.MouseEvent;
import java.util.List;
import gleem.linalg.*;
import net.java.games.jogl.*;

public abstract class MovableObject
{
   /** Simple state machine for computing distance dragged */
   private int lastX;
   private int lastY;

   /** Motion parameters */
   private Vec3f center       = new Vec3f(0, 0,  0); // Position of focal point in world coordinates
   private Rotf  orientation  = new Rotf();

   /** Current transformation matrix */
   protected Mat4f   mTransform;

   public MovableObject()
   {
      super();
      mTransform = new Mat4f();
      mTransform.makeIdent();
   }

   public void setPosition(Vec3f pos)
   {
      center = pos;
      recalc();
   }

   public Vec3f getPosition()
   {
      return center;
   }
   
   public void recalc()
   {
      // Compute modelview matrix based on camera parameters, position and
      // orientation
      Mat4f tmpMat = new Mat4f();
      tmpMat.makeIdent();

      Mat4f trans_matrix = new Mat4f();
      trans_matrix.makeIdent();
      trans_matrix.setTranslation(center);
      tmpMat = tmpMat.mul(trans_matrix);

      Mat4f rot_matrix = new Mat4f();
      rot_matrix.makeIdent();
      orientation.toMatrix(rot_matrix);
      tmpMat = tmpMat.mul(rot_matrix);
      
      // Make sure to call setTransform so that ManipPartTriBased regenerates
      // its vertices.
      setTransform(tmpMat);
   }

   public void setTransform(Mat4f xform)
   {
      mTransform.set(xform);
   }

   /**
    * Render this Manipulator now using the given OpenGL
    * routines and assuming an OpenGL context is current.
    */
   public abstract void render(GL gl, GLU glu);
}
