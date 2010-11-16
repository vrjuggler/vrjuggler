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

package org.vrjuggler.vrjconfig.customeditors.surfacedisplayeditor;

import com.sun.j3d.utils.behaviors.mouse.MouseRotate;
import com.sun.j3d.utils.behaviors.mouse.MouseTranslate;
import com.sun.j3d.utils.behaviors.mouse.MouseZoom;
import com.sun.j3d.utils.geometry.Sphere;
import com.sun.j3d.utils.picking.behaviors.PickRotateBehavior;

import java.awt.*;
import java.awt.event.ActionListener;

import java.io.File;
import java.io.IOException;

import java.util.ArrayList;
import java.util.Iterator;

import javax.swing.*;
import javax.vecmath.*;
import javax.media.j3d.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;

public class SurfaceDisplayEditor extends JSplitPane implements CustomEditor 
{
   private Canvas3D mCanvas = null;
   private ConfigElementPropertySheet mSheet = new ConfigElementPropertySheet();
   private JScrollPane mScrollPane = new JScrollPane();
   private JScrollPane m3DScrollPane = new JScrollPane();
   private java.util.List mViewPorts = new ArrayList();
   private ConfigBroker mBroker = null;
   private ViewPort mActiveViewPort = null;
   private ConfigContext mCtx = null;
   private ConfigElement mConfigElement;
   private String fontName = "TestFont";
   private String textString = "BillBoard";
   private float sl = textString.length();	
   private UniverseBuilder mUniverse = null;
   private ConfigContext mContext = new ConfigContext();

   public SurfaceDisplayEditor()
   {
      CustomEditorRegistry.registerEditor("display_window",
         SurfaceDisplayEditor.class);
      CustomEditorRegistry.registerEditor("surface_viewport",
         SurfaceDisplayEditor.class);

      try 
      {
         // Init() must be called before jbInit to ensure that the Canvas3D
         // is created correctly before trying to add it.
         init();
         jbInit();
      }
      catch(Exception e) 
      {
         e.printStackTrace();
      }
   }

   
   private void init() throws IOException	
   {		
      mBroker = new ConfigBrokerProxy();
      mCtx = new ConfigContext();

      GraphicsConfigTemplate3D tmpl = new GraphicsConfigTemplate3D(); 
      GraphicsEnvironment env =
         GraphicsEnvironment.getLocalGraphicsEnvironment(); 
      GraphicsDevice device = env.getDefaultScreenDevice(); 
      GraphicsConfiguration config = device.getBestConfiguration( tmpl ); 
      //GraphicsConfiguration config = device.getDefaultConfiguration(); 

      mCanvas = new Canvas3D( config );
   }
        
   private void jbInit() throws Exception 
   {
      //setLayout(new BorderLayout());



      //m3DScrollPane.setLayout(new ScrollPaneLayout());
      //m3DScrollPane.setViewportView(mCanvas);

      //JPanel temp = new JPanel();
      //temp.setLayout(new BorderLayout());
      //temp.add(mCanvas, BorderLayout.CENTER);

      //setTopComponent(/*m3DScrollPane*/temp);

      mScrollPane.getViewport().add(mSheet, null);

      setOneTouchExpandable(true);
      setDividerLocation(200);
      setOrientation(JSplitPane.VERTICAL_SPLIT);
      setResizeWeight(1.0);
      add(mCanvas, JSplitPane.TOP);
      add(mScrollPane, JSplitPane.BOTTOM);
      
   }
	
   public void destroy() 
   {
      //mUniverse.cleanup();
   }	
       
   public BranchGroup createSceneGraph()
   {
      BranchGroup branchGroup = new BranchGroup();
      TransformGroup transformGroup = new TransformGroup();
      //myNav.setCapability(TransformGroup.ALLOW_CHILDREN_EXTEND);
      //myNav.setCapability(TransformGroup.ALLOW_CHILDREN_WRITE);
      //myNav.setCapability(TransformGroup.ALLOW_CHILDREN_READ);
      transformGroup.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
      transformGroup.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);

      for (Iterator vp = mViewPorts.iterator() ; vp.hasNext() ; )
      {
         transformGroup.addChild(((ViewPort)vp.next()).getTransformGroup());	
      }

      branchGroup.addChild(transformGroup);
      setupMouse(branchGroup, transformGroup);

      // Add Axis lines
      LineArray axis = new LineArray(6, LineArray.COORDINATES | LineArray.COLOR_3);

      Point3d[] axis_points = 
         {new Point3d(0,0,0), new Point3d(1,0,0),
         new Point3d(0,0,0), new Point3d(0,1,0),
         new Point3d(0,0,0), new Point3d(0,0,1)};

      Color3f[] axis_colors = 
         {new Color3f(0,0,0), new Color3f(1,0,0),
         new Color3f(0,0,0), new Color3f(0,1,0),
         new Color3f(0,0,0), new Color3f(0,0,1)};

      axis.setCoordinates(0,axis_points);
      axis.setColors(0,axis_colors);
      Shape3D axis_shape = new Shape3D(axis);
      transformGroup.addChild(axis_shape);

      //Add Billboards
      transformGroup.addChild(createBillboardText(new Vector3f(1,0,0),"X"));
      transformGroup.addChild(createBillboardText(new Vector3f(0,1,0),"Y"));
      transformGroup.addChild(createBillboardText(new Vector3f(0,0,1),"Z"));

      PickBehavior pickBehavior = new PickBehavior(branchGroup, mCanvas, this);	


      BoundingSphere bounds = new BoundingSphere(new Point3d(0.0,0.0,0.0), 100.0);
      //pickBehavior.setSchedulingBounds(bounds);
      //branchGroup.addChild(pickBehavior);
      return branchGroup;
   }
	
   private void setupMouse(BranchGroup mySceneGraph, TransformGroup myNav)
   {
      MouseRotate myMouseRotate = new MouseRotate();
      myMouseRotate.setTransformGroup(myNav);
      myMouseRotate.setSchedulingBounds(new BoundingSphere());
      mySceneGraph.addChild(myMouseRotate);

      MouseTranslate myMouseTranslate = new MouseTranslate();
      myMouseTranslate.setTransformGroup(myNav);
      myMouseTranslate.setSchedulingBounds(new BoundingSphere());
      mySceneGraph.addChild(myMouseTranslate);

      MouseZoom myMouseZoom = new MouseZoom();
      myMouseZoom.setTransformGroup(myNav);
      myMouseZoom.setSchedulingBounds(new BoundingSphere());
      mySceneGraph.addChild(myMouseZoom);

      if(mySceneGraph == null)
      {
         System.out.println("mySceneGraph is null.");
      }
      if(mCanvas == null)
      {
         System.out.println("mCanvas is null.");
      }
      //BoundingSphere bounds = new BoundingSphere(new Point3d(0.0,0.0,0.0), 100.0);
      //PickRotateBehavior test = new PickRotateBehavior(mySceneGraph, mCanvas, bounds); 
      //mySceneGraph.addChild(test);
      
   }
	
   public void setActiveViewPort(ViewPort viewport)
   {
      mActiveViewPort = viewport;  	
      mSheet.setElement(mActiveViewPort.getConfigElement());

      for (Iterator s = mViewPorts.iterator() ; s.hasNext() ; )
      {
         ((ViewPort)s.next()).showCornerSpheres(false);
      }
      mActiveViewPort.showCornerSpheres(true);  	
   }

   public void setActiveSphere(Sphere selected_sphere)
   {	  	
      for (Iterator s = mViewPorts.iterator() ; s.hasNext() ; )
      {
         Sphere[] sps = ((ViewPort)s.next()).getCornerSpheres();
         for (int i = 0 ; i < 4 ; i++)
         {
            sps[i].getShape().getAppearance().getColoringAttributes().setColor(ViewPort.mSphereUnSelectedColor);
         }
      }
      selected_sphere.getShape().getAppearance().getColoringAttributes().setColor(ViewPort.mSphereSelectedColor);  	
   }
         
   public Container getPanel()
   {
      return this;
   }

   public String getTitle()
   {
      return("3D Surface Editor");
   }
   
   public ActionListener getHelpActionListener()
   {
      return null;
   }

   public void editorClosing()
   {
   }

   public void setConfig(ConfigContext context, ConfigElement elm)
   {
      mContext = context;
      java.util.List elms = mBroker.getElements(mContext);
      
      for (Iterator e = elms.iterator(); e.hasNext();) 
      {
         addConfigElement((ConfigElement)e.next());
         
      }

      mConfigElement = elm;
      mSheet.setElement(elm);

      if (elm.getDefinition().getToken().equals("display_window")) 
      {
         addDisplayWindow(elm);
      }
      else if (elm.getDefinition().getToken().equals("surface_viewport")) 
      {
         addSurfaceViewport(elm);
      }
      
      mUniverse = new UniverseBuilder(mCanvas);
      BranchGroup scene = createSceneGraph();
      mUniverse.addBranchGraph(scene);
   }
 
   public void addConfigElement(ConfigElement elm)
   {
      //mConfigElement = elm;
      mSheet.setElement(elm);

      if (elm.getDefinition().getToken().equals("display_window")) 
      {
         addDisplayWindow(elm);
      }
      else if (elm.getDefinition().getToken().equals("surface_viewport")) 
      {
         addSurfaceViewport(elm);
      }
   }

   static int surface_viewport_number = 0;

   private void addSurfaceViewport(ConfigElement elm)
   {
      if (elm.getDefinition().getToken().equals("surface_viewport")) 
      {	
         /*
         int temp_red = (int)(mRand.nextFloat()*4);
         float real_red = (float) (temp_red * 0.25);
         int temp_green = (int)(mRand.nextFloat()*4);
         float real_green = (float) (temp_green * 0.25);
         int temp_blue = (int)(mRand.nextFloat()*4);
         float real_blue = (float) (temp_blue * 0.25);

         Color3f color = new Color3f(real_red, real_green, real_blue);
         */
         
         // Create color values that are unique
         // Basically count in binary (skipping 0), and use the first 3 digits.  That will give six colors
         int red_on = (surface_viewport_number  & 0x1);
         int green_on = ((surface_viewport_number >> 1) & 0x1);
         int blue_on = ((surface_viewport_number >> 2) & 0x1);

         float red = 0.0f, green = 0.0f, blue = 0.0f;
         if (red_on > 0)
         {
            red = 1.0f;
            green = 0.15f;
            blue = 0.15f;
         }
         if (green_on > 0)
         {
            red = 0.15f;
            green = 1.0f;
            blue = 0.15f;
         }
         if (blue_on > 0)
         {
            red = 0.15f;
            green = 0.15f;
            blue = 1.0f;
         }

         if ((red_on==0) && (blue_on==0) && (green_on==0))      // Case of 0's (black is bad)
            red = blue = green = 1.0f;
         
         Color3f color = new Color3f(red, green, blue);
         ViewPort temp_viewport = new ViewPort(elm, color);
         mViewPorts.add(temp_viewport);
         elm.addConfigElementListener(temp_viewport);
         
         surface_viewport_number++;
      }
   }
           
   private void addDisplayWindow(ConfigElement elm)
   {
      if (elm.getDefinition().getToken().equals("display_window")) 
      {
         java.util.List embedded_elts = elm.getPropertyValues("surface_viewports");
 
         for (Iterator e = embedded_elts.iterator(); e.hasNext();) 
         {
            ConfigElement disp_elt = (ConfigElement) e.next();
            addSurfaceViewport(disp_elt);
         }
      } 
   }

   private void addConfigContext()
   {
      // Get all elements from the two files
      //java.util.List all_elts = mBroker.getElements(mCtx);

      //for (Iterator i = all_elts.iterator(); i.hasNext();) 
      //{
      //ConfigElement mConfigElement = (ConfigElement) i.next();
      //addDisplayWindow();
      //   } 
   }

   private BranchGroup createBillboardText(Vector3f location, String text)
   {
      //             objRoot
      //             /
      //       objTrans
      //        /    \
      // billBoad    bb_translate_node
      //    |            \ 
      //    +-->  billBoardTransformGroup
      //                   \
      //                textShape
      // Create the root of the branch graph

      BranchGroup objRoot = new BranchGroup();


      TransformGroup bb_translate_node = new TransformGroup();
      Transform3D text_transform = new Transform3D();
      text_transform.setTranslation(location);
      text_transform.setScale(0.25);
      bb_translate_node.setTransform(text_transform);

      // Create the transform group node that the billboard node 
      // will change when needed.
      TransformGroup objTrans = new TransformGroup();
      objTrans.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
      objTrans.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
      
      // Create the transform group node that the billboard node 
      // will change when needed.
      TransformGroup billBoardTransformGroup = new TransformGroup();
      billBoardTransformGroup.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
      billBoardTransformGroup.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);

      
      // Create the 3D text. 
      Font3D f3d = new Font3D(new Font(fontName, Font.PLAIN, 1), new FontExtrusion());
      Text3D txt = new Text3D(f3d, /*textString*/text, new Point3f( /*-sl/2.0f*/0.0f, 0.0f, 0.0f));
      Shape3D textShape = new Shape3D();
      textShape.setGeometry(txt);

      // Using billboard behavior on text3d
      BoundingSphere bounds = new BoundingSphere(new Point3d(0.0,0.0,0.0), 100.0);


      // Create a BillBoard that will change all children of billBoardTransformGroup
      Billboard billBoad = new Billboard( billBoardTransformGroup );

      billBoad.setSchedulingBounds( bounds );
      //billBoad.setAlignmentAxis( 0.0f, 1.0f, 0.0f);
      billBoad.setRotationPoint(location.x, location.y, location.z);
      billBoad.setAlignmentMode(Billboard.ROTATE_ABOUT_POINT);


      objRoot.addChild(objTrans);
      
      objTrans.addChild(billBoad);
      objTrans.addChild(bb_translate_node);
      
      bb_translate_node.addChild( billBoardTransformGroup );
      
      billBoardTransformGroup.addChild( textShape );

      return objRoot;
   }
}

class UniverseBuilder extends Object
{
   private Locale locale;

   public UniverseBuilder(Canvas3D c)
   {
      Transform3D t = new Transform3D();
      Transform3D t2 = new Transform3D();
      t2.setEuler(
      new Vector3d(
              -35.0 * (Math.PI / 180.0),
              45.0 * (Math.PI / 180.0),
              0.0));
      t.set(4, new Vector3d(4.0, 3.0, 4.0));
      t.mul(t, t2);

      Transform3D start = new Transform3D();
      start.setTranslation(new Vector3f(0.0f, 6.0f, 20.0f));

      VirtualUniverse universe = new VirtualUniverse();
      locale = new Locale(universe);

      PhysicalBody body = new PhysicalBody();
      PhysicalEnvironment environment = new PhysicalEnvironment();
      BranchGroup viewPlatformBranchGroup = new BranchGroup();
      //TransformGroup viewPlatformTransformGroup = new TransformGroup(t);
      TransformGroup viewPlatformTransformGroup = new TransformGroup(start);
      ViewPlatform viewPlatform = new ViewPlatform();

      View view = new View();
      view.addCanvas3D(c);
      view.setPhysicalBody(body);
      view.setPhysicalEnvironment(environment);
      view.attachViewPlatform(viewPlatform);

      viewPlatformTransformGroup.addChild(viewPlatform);
      viewPlatformBranchGroup.addChild(viewPlatformTransformGroup);
      locale.addBranchGraph(viewPlatformBranchGroup);
   }

   void addBranchGraph(BranchGroup bg) 
   {
      locale.addBranchGraph(bg);
   }
}

class AxisLines extends Object
{
   private static Point3f[] axis_verts =
   {new Point3f(0, 0, 0),
   new Point3f(1, 0, 0),
   new Point3f(0, 0, 0),
   new Point3f(0, 1, 0),
   new Point3f(0, 0, 0),
   new Point3f(0, 0, 1)};
   
   private static Color3f[] axis_colors =
   {new Color3f(1.0f, 0.0f, 0.0f),
   new Color3f(1.0f, 0.0f, 0.0f),
   new Color3f(0.0f, 1.0f, 0.0f),
   new Color3f(0.0f, 1.0f, 0.0f),
   new Color3f(0.0f, 0.0f, 1.0f),
   new Color3f(0.0f, 0.0f, 1.0f)};

   private Shape3D shape;

   public AxisLines()
   {
      LineArray lines =
         new LineArray(6, LineArray.COORDINATES | LineArray.COLOR_3);

      lines.setCoordinates(0, axis_verts);
      lines.setColors(0, axis_colors);

      shape = new Shape3D(lines, new Appearance());
   }

   public Shape3D getShape()
   {
      return shape;
   }
}
