/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

/*
 * Created on Jul 23, 2003
 *
 * To change the template for this generated file go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
package org.vrjuggler.vrjconfig.customeditors.surfacedisplayeditor;
import java.io.IOException;

import javax.media.j3d.*;
import javax.vecmath.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;

import com.sun.j3d.utils.geometry.*;


class ViewPort extends Object implements ConfigElementListener
{
   private Point3f[] mCorners =
   {new Point3f(1, 1, 0),
   new Point3f(-1, 1, 0),
   new Point3f(-1, -1, 0),
   new Point3f(1, -1, 0)};

   private Color3f mColor;
   private ConfigElement mConfigElement = null;
   public static Color3f mSphereSelectedColor = new Color3f(1.0f, 0.0f, 0.0f);
   public static Color3f mSphereUnSelectedColor = new Color3f(0.4f, 0.4f, 0.4f);
   private Shape3D mBorder;
   
   private BranchGroup[] mSphereBranchGroups = 
   {new BranchGroup(),
   new BranchGroup(),
   new BranchGroup(),
   new BranchGroup()};
   
   private Sphere[] mSpheres = 
   {new Sphere(0.2f),
   new Sphere(0.2f),
   new Sphere(0.2f),
   new Sphere(0.2f)};
   
   private TransformGroup[] mSphereTransformGroups = 
   {new TransformGroup(), 
   new TransformGroup(),
   new TransformGroup(),
   new TransformGroup()};

   private TriangleFanArray mGeomotry = null;
   private TransformGroup mViewPortTransformGroup = new TransformGroup();
   private ConfigElement[] mCornerConfigElements = { null, null, null, null };

   
   public ConfigElement getConfigElement()
   {
      return mConfigElement;
   }

   public Sphere[] getCornerSpheres()
   {
      return mSpheres;
   }

   public void showCornerSpheres(boolean show)
   {
      for (int i = 0 ; i < 4 ; i++)
      {
         if (show)
         {
            // Add the sphere to the Viewport TransformGroup
            mSphereTransformGroups[i].addChild(mSphereBranchGroups[i]);				
         }
         else
         {
            if(mSphereBranchGroups[i].isLive())
            {
               mSphereTransformGroups[i].removeChild(mSphereBranchGroups[i]);
            }
         }
      }		
   }

   public ViewPort(ConfigElement elt, Color3f color)
   {
      mColor = color;
      // Get the corners from the config element
      if (!elt.getDefinition().getToken().equals("surface_viewport"))
      {
         //throw new IOException("ConfigElement s not a surface_viewport!");
         System.out.println("ConfigElement s not a surface_viewport!");
      }

      mConfigElement = elt;

      for(int i = 0 ; i < 4 ; i ++)
      {
         ConfigElement embedded_elt = (ConfigElement)elt.getProperty("corners", i);
         String name = embedded_elt.getName(); 

         System.out.println("Viewport Constructor - Corner name: " + name);

         if (name.equals("lower left"))
         {
            mCornerConfigElements[0] = embedded_elt;
            mCornerConfigElements[0].addConfigElementListener(this);
         }
         else if (name.equals("lower right"))
         {
            mCornerConfigElements[1] = embedded_elt;
            mCornerConfigElements[1].addConfigElementListener(this);
         }
         else if(name.equals("upper right"))
         {
            mCornerConfigElements[2] = embedded_elt;
            mCornerConfigElements[2].addConfigElementListener(this);
         }
         else if(name.equals("upper left"))
         {
            mCornerConfigElements[3] = embedded_elt;
            mCornerConfigElements[3].addConfigElementListener(this);
         }
      }

      updateCornerLocations();
      createBorder();
      createSurface();

      // Add a Sphere on each corner						
      createSpheres();

      // Update the location of all spheres
      updateSpheres();			
   }
   
   private void createBorder()
   {
      Appearance surface_apperance2 = new Appearance();
      surface_apperance2.setCapability(Appearance.ALLOW_COLORING_ATTRIBUTES_READ);
      surface_apperance2.setCapability(Appearance.ALLOW_COLORING_ATTRIBUTES_WRITE);						

      // Create a new ColoringAttributes object for each new Sphere
      ColoringAttributes coloring_attrib22 = new ColoringAttributes(mSphereUnSelectedColor, ColoringAttributes.NICEST);
      coloring_attrib22.setCapability(ColoringAttributes.ALLOW_COLOR_WRITE);

      TransparencyAttributes ta2 = new TransparencyAttributes();
      ta2.setTransparencyMode(TransparencyAttributes.BLENDED);
      ta2.setTransparency(0.45f);

      PolygonAttributes polygon_attributes2 = new PolygonAttributes();
      polygon_attributes2.setCullFace(PolygonAttributes.CULL_NONE);
      surface_apperance2.setPolygonAttributes(polygon_attributes2);

      surface_apperance2.setTransparencyAttributes(ta2);

      // Add new ColoringAttribute object the the Sphere's Appearance object
      surface_apperance2.setColoringAttributes(coloring_attrib22);

      int[] stripVertex = {5};

      LineStripArray border_line_array =
      new LineStripArray(5, LineStripArray.COORDINATES | LineStripArray.COLOR_3, stripVertex);

      border_line_array.setCoordinates(0, mCorners);
      border_line_array.setCoordinate(4, mCorners[0]);

      // Set the color of the ViewPort
      for (int i = 0; i < 4; i++)
      {
         border_line_array.setColor(i, mColor);
      }
      mBorder = new Shape3D(border_line_array, surface_apperance2);
      mBorder.setPickable(false);
      mBorder.setCapability(Shape3D.ALLOW_APPEARANCE_READ);
      mBorder.setCapability(Shape3D.ALLOW_APPEARANCE_WRITE);
      mBorder.setCapability(Shape3D.ALLOW_PICKABLE_READ);
      mBorder.setUserData(new String("Border!!!"));
      mViewPortTransformGroup.addChild(mBorder);
   }
   
   private void createSurface()
   {
      int[] num = { 4 };
      mGeomotry = new TriangleFanArray(4, TriangleFanArray.COORDINATES | TriangleFanArray.COLOR_3, num);
      mGeomotry.setCapability(TriangleFanArray.ALLOW_COORDINATE_WRITE);
      mGeomotry.setCoordinates(0, mCorners);

      // Set the color of the ViewPort
      /*for (int i = 0; i < 4; i++) 
      {
         mGeomotry.setColor(i, mColor);
      }
      */

      float ll_scale = 0.10f;
      float ul_scale = 0.55f;
      float ur_scale = 1.0f;
      
      float red = mColor.get().getRed()/255;
      float green = mColor.get().getGreen()/255;
      float blue = mColor.get().getBlue()/255;

      Color3f ll_clr = new Color3f(ll_scale*red, ll_scale*green, ll_scale*blue);
      Color3f ul_clr = new Color3f(ul_scale*red, ul_scale*green, ul_scale*blue);
      Color3f lr_clr = new Color3f(ul_scale*red, ul_scale*green, ul_scale*blue);
      Color3f ur_clr = new Color3f(ur_scale*red, ur_scale*green, ur_scale*blue);

      mGeomotry.setColor(0, ll_clr);
      mGeomotry.setColor(1, ul_clr);
      mGeomotry.setColor(2, lr_clr);
      mGeomotry.setColor(3, ur_clr);
      
      // Create a new appearance object for each new Sphere

      Appearance surface_apperance = new Appearance();
      surface_apperance.setCapability(Appearance.ALLOW_COLORING_ATTRIBUTES_READ);
      surface_apperance.setCapability(Appearance.ALLOW_COLORING_ATTRIBUTES_WRITE);						

      // Create a new ColoringAttributes object for each new Sphere
      ColoringAttributes coloring_attrib2 = new ColoringAttributes(mSphereUnSelectedColor, ColoringAttributes.NICEST);
      coloring_attrib2.setCapability(ColoringAttributes.ALLOW_COLOR_WRITE);

      TransparencyAttributes ta = new TransparencyAttributes();
      ta.setTransparencyMode(TransparencyAttributes.BLENDED);
      ta.setTransparency(0.4f);

      PolygonAttributes polygon_attributes = new PolygonAttributes();
      polygon_attributes.setCullFace(PolygonAttributes.CULL_NONE);
      surface_apperance.setPolygonAttributes(polygon_attributes);

      Appearance viewport_app = new Appearance();
      surface_apperance.setTransparencyAttributes(ta);

      // Add new ColoringAttribute object the the Surface Appearance object
      surface_apperance.setColoringAttributes(coloring_attrib2);

      Shape3D surface = new Shape3D(mGeomotry);

      // Assign the new Apperance to the sphere
      surface.setAppearance(surface_apperance);

      surface.setPickable(true);
      surface.setCapability(Shape3D.ALLOW_APPEARANCE_READ);
      surface.setCapability(Shape3D.ALLOW_APPEARANCE_WRITE);
      surface.setCapability(Shape3D.ALLOW_PICKABLE_READ);

      // Assign UserData to be used later
      //surface.setUserData(new String("Surface..."));
      surface.setUserData(this);

      mViewPortTransformGroup.setCapability(TransformGroup.ALLOW_CHILDREN_READ);
      mViewPortTransformGroup.setCapability(TransformGroup.ALLOW_CHILDREN_WRITE);
      mViewPortTransformGroup.setCapability(TransformGroup.ALLOW_CHILDREN_EXTEND);
      mViewPortTransformGroup.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
      mViewPortTransformGroup.setCapability(TransformGroup.ENABLE_PICK_REPORTING);

      mViewPortTransformGroup.addChild(surface);
   }
   
   private void createSpheres()
   {
      // Add 4 Spheres for the corners
      for (int i = 0 ; i < 4 ; i++)
      {
         // Create a new appearance object for each new Sphere
         Appearance temp_app = new Appearance();
         temp_app.setCapability(Appearance.ALLOW_COLORING_ATTRIBUTES_READ);
         temp_app.setCapability(Appearance.ALLOW_COLORING_ATTRIBUTES_WRITE);						

         // Create a new ColoringAttributes object for each new Sphere
         ColoringAttributes coloring_attrib = new ColoringAttributes(mSphereUnSelectedColor, ColoringAttributes.NICEST);
         coloring_attrib.setCapability(ColoringAttributes.ALLOW_COLOR_WRITE);

         // Set Transparancy properties for each sphere 
         TransparencyAttributes corner_ta = new TransparencyAttributes();
         corner_ta.setTransparencyMode(TransparencyAttributes.BLENDED);
         corner_ta.setTransparency(0.2f);			

         // Add Coloring and Transparency attributes to the Sphere's Appearance object
         temp_app.setColoringAttributes(coloring_attrib);
         temp_app.setTransparencyAttributes(corner_ta);

         // Assign the Apperance to the sphere
         mSpheres[i].setAppearance(temp_app);

         // Allow picking for each sphere			
         mSpheres[i].getShape().setPickable(true);
         mSpheres[i].getShape().setCapability(Shape3D.ALLOW_APPEARANCE_READ);
         mSpheres[i].getShape().setCapability(Shape3D.ALLOW_APPEARANCE_WRITE);
         mSpheres[i].getShape().setCapability(Shape3D.ALLOW_PICKABLE_READ);

         // Assign UserData to be used later
         mSpheres[i].getShape().setUserData(mSpheres[i]);

         // Add the sphere to its BranchGroup
         // NOTE:	We must use BranchGroups here to allow usto add/remove the spheres
         // 				at runtime. Java3D only allows us to add/remove BranchGroups
         mSphereBranchGroups[i].addChild(mSpheres[i]);

         //mSphereBranchGroups[i].setCapability(BranchGroup.ALLOW_CHILDREN_READ);
         //mSphereBranchGroups[i].setCapability(BranchGroup.ALLOW_CHILDREN_WRITE);
         //mSphereBranchGroups[i].setCapability(BranchGroup.ALLOW_CHILDREN_EXTEND);
         //mSphereBranchGroups[i].clearCapability(BranchGroup.ENABLE_PICK_REPORTING);
         //mSphereBranchGroups[i].setCapability(BranchGroup.ALLOW_PICKABLE_READ);
         //mSphereBranchGroups[i].setCapability(BranchGroup.ALLOW_PICKABLE_WRITE);
         mSphereBranchGroups[i].setCapability(BranchGroup.ALLOW_DETACH);

         mSphereTransformGroups[i].setCapability(BranchGroup.ALLOW_CHILDREN_READ);
         mSphereTransformGroups[i].setCapability(BranchGroup.ALLOW_CHILDREN_WRITE);
         mSphereTransformGroups[i].setCapability(BranchGroup.ALLOW_CHILDREN_EXTEND);
         mSphereTransformGroups[i].setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         mSphereTransformGroups[i].setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         //mSphereTransformGroups[i].clearCapability(BranchGroup.ENABLE_PICK_REPORTING);
         //mSphereTransformGroups[i].setCapability(BranchGroup.ALLOW_PICKABLE_READ);
         //mSphereTransformGroups[i].setCapability(BranchGroup.ALLOW_PICKABLE_WRITE);
         //mSphereTransformGroups[i].setCapability(BranchGroup.ALLOW_DETACH);


         // Create a TransformGroup for each sphere to allow translation, and add it to the ViewPort. 
         mSphereTransformGroups[i].setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         mViewPortTransformGroup.addChild(mSphereTransformGroups[i]);
      }
   }

   public TransformGroup getTransformGroup()
   {
      return mViewPortTransformGroup;
   }

   /**
   * Set the corner value of this Viewport.
   * @param index - The index of the corner to change
   * @param corner - The point for the corner.
   */
   /*public void setCorner(int index, Point3f corner) {
   mCorners[index] = corner;

   }*/

   public void updateCornerLocations()
   {
      for (int i = 0 ; i < 4 ; i++)
      {
         float x = Float.parseFloat(mCornerConfigElements[i].getProperty("x", 0).toString());
         float y = Float.parseFloat(mCornerConfigElements[i].getProperty("y", 0).toString());
         float z = Float.parseFloat(mCornerConfigElements[i].getProperty("z", 0).toString());

         mCorners[i] = new Point3f(x, y, z);  
      }
      updateSpheres();
   }

   private void updateSpheres()
   {
      for (int i = 0 ; i < 4 ; i++)
      {
         // Get the transform for the sphere
         Transform3D temp_trans = new Transform3D();
         mSphereTransformGroups[i].getTransform(temp_trans);

         // Set the translation to the correct location
         temp_trans.setTranslation(new Vector3f(mCorners[i]));
         mSphereTransformGroups[i].setTransform(temp_trans);
      }
   }



   /* (non-Javadoc)
   * @see org.vrjuggler.jccl.config.ConfigElementListener#nameChanged(org.vrjuggler.jccl.config.ConfigElementEvent)
   */
   public void nameChanged(ConfigElementEvent arg0)
   {
      // TODO Auto-generated method stub
      System.out.println("Name Changed...");
   }

   /* (non-Javadoc)
   * @see org.vrjuggler.jccl.config.ConfigElementListener#propertyValueChanged(org.vrjuggler.jccl.config.ConfigElementEvent)
   */
   public void propertyValueChanged(ConfigElementEvent arg0)
   {
      // TODO Auto-generated method stub
      System.out.println("Value Changed...");
      updateCornerLocations();
      updateSpheres();
      mGeomotry.setCoordinates(0, mCorners);
   }

   public void propertyValueOrderChanged(ConfigElementEvent arg0)
   {
      // TODO Auto-generated method stub
      System.out.println("Value Order Changed...");
   }

   /* (non-Javadoc)
   * @see org.vrjuggler.jccl.config.ConfigElementListener#propertyValueAdded(org.vrjuggler.jccl.config.ConfigElementEvent)
   */
   public void propertyValueAdded(ConfigElementEvent arg0) 
   {
      // TODO Auto-generated method stub
      System.out.println("Value Added...");
   }

   /* (non-Javadoc)
   * @see org.vrjuggler.jccl.config.ConfigElementListener#propertyValueRemoved(org.vrjuggler.jccl.config.ConfigElementEvent)
   */
   public void propertyValueRemoved(ConfigElementEvent arg0) 
   {
      // TODO Auto-generated method stub
      System.out.println("Value Removed...");
   }
}
