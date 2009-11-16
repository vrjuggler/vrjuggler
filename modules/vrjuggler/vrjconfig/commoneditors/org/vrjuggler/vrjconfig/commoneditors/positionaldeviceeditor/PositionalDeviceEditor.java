/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors.positionaldeviceeditor;

import java.awt.*;
import java.awt.image.*;
import java.awt.color.ColorSpace;
import java.awt.geom.AffineTransform;
import java.awt.event.*;
import net.java.games.jogl.*;
import net.java.games.jogl.util.GLUT;
import java.util.ArrayList;
import java.util.List;
import java.util.Iterator;
import javax.swing.*;
import javax.swing.JFileChooser;
import javax.swing.filechooser.*;
import java.io.File;
import gleem.*;
import gleem.linalg.*;
import javax.swing.border.*;
import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.editors.*;

public class PositionalDeviceEditor extends JPanel
   implements GLEventListener, CustomEditor
{
   /** Preffered size of this frame. */
   public static final Dimension PREFERRED_FRAME_SIZE =
      new Dimension (300, 400);

   private CameraParameters params = new CameraParameters();
   private MovableObject mTracker = null;

   private JPanel mTransformPanel = new JPanel();

   private Texture mCurrentFloorTexture = null;
   private int mCurrentFloorIndex = 0;
   private List mFloorTextures = new ArrayList();
   private JButton mChangeTex = new JButton("Change Floor");

   public ActionListener getHelpActionListener()
   {
      return null;
   }

   public void editorClosing()
   {
   }

   /**
    * We'd like to be 600x400, please.
    */
   public Dimension getPreferredSize()
   {
      return PREFERRED_FRAME_SIZE;
   }

   private void updatePosition(ActionEvent evt)
   {
      if (evt.getSource() == mTranslateX)
      {
         mPosFilterConfigElement.setProperty("pre_translate", 0, mTranslateX.getValue());
      }
      else if (evt.getSource() == mTranslateY)
      {
         mPosFilterConfigElement.setProperty("pre_translate", 1, mTranslateY.getValue());
      }
      else if (evt.getSource() == mTranslateZ)
      {
         mPosFilterConfigElement.setProperty("pre_translate", 2, mTranslateZ.getValue());
      }
      else if (evt.getSource() == mRotateX)
      {
         mPosFilterConfigElement.setProperty("pre_rotation", 0, mRotateX.getValue());
      }
      else if (evt.getSource() == mRotateY)
      {
         mPosFilterConfigElement.setProperty("pre_rotation", 1, mRotateY.getValue());
      }
      else if (evt.getSource() == mRotateZ)
      {
         mPosFilterConfigElement.setProperty("pre_rotation", 2, mRotateZ.getValue());
      }
   }

   public PositionalDeviceEditor()
   {
      // Register ourselves with the CustomEditorRegistry.
      CustomEditorRegistry.registerEditor("position_transform_filter",  PositionalDeviceEditor.class);

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }
   
   /** Simple state machine for computing distance dragged */
   private int lastX;
   private int lastY;
   private Vec3f mCameraPos = new Vec3f(-1.0f, 1.0f, 1.0f); 
   private float dollySpeed        = 2.0f;

   private MouseMotionAdapter mouseMotionListener = new MouseMotionAdapter()
   {
      public void mouseDragged(MouseEvent e)
      {
         int dx = e.getX() - lastX;
         int dy = e.getY() - lastY;
         
         
         lastX = e.getX();
         lastY = e.getY();
         
         // Translate functionality
         // Compute the local coordinate system's difference vector
         Vec3f diff = new Vec3f(-dollySpeed * dy / 100.0f,
                                      dollySpeed * dy / 100.0f,
                                      dollySpeed * dy / 100.0f);
         // Add on to center
         mCameraPos.add(diff);
      }

      public void mouseMoved(MouseEvent e)
      {
         //passiveMotionMethod(e);
      }
   };

   private MouseAdapter mouseListener = new MouseAdapter()
   {
      public void mousePressed(MouseEvent e)
      {
         lastX = e.getX();
         lastY = e.getY();
      }

      public void mouseReleased(MouseEvent e)
      {
         lastX = e.getX();
         lastY = e.getY();
      }
   };
   

   private ConfigElement mConfigElement = null;
   private ConfigElement mPosFilterConfigElement = null;
   private ConfigContext mConfigContext = null;

   /**
    * Specify the ConfigContext and ConfigElement to use with this
    * CustomEditor.
    */
   public void setConfig(ConfigContext ctx, ConfigElement elm)
   {
      mConfigContext = ctx;
      mConfigElement = elm;
      ConfigDefinition def = mConfigElement.getDefinition();
     
      boolean valid = false;

      for (Iterator itr = def.getParents().iterator() ; itr.hasNext() ; )
      {
         if (itr.next().equals("positional_device"))
         {
            valid = true;
         }
      }

      if (!valid)
      {
         System.out.println("ERROR: This editor can only be used with positional devices.");
      }
      
      mPosFilterConfigElement = (ConfigElement)mConfigElement.getProperty("position_filters", 0);
      if(null == mConfigContext)
      {
         System.out.println("ERROR: You must set the ConfigContext before you set the ConfigElement.");
      }

      mPosFilterConfigElement.addConfigElementListener(new ConfigElementAdapter()
            {
               public void propertyValueChanged(ConfigElementEvent evt)
               {
                  elementChanged();
               }
            });
   }

   public void elementChanged()
   {
      Float x_pos = (Float)mPosFilterConfigElement.getProperty("pre_translate", 0);
      Float y_pos = (Float)mPosFilterConfigElement.getProperty("pre_translate", 1);
      Float z_pos = (Float)mPosFilterConfigElement.getProperty("pre_translate", 2);
      
      mTranslateX.setValue(x_pos);
      mTranslateY.setValue(y_pos);
      mTranslateZ.setValue(z_pos);
      
      Float x_rot = (Float)mPosFilterConfigElement.getProperty("pre_rotation", 0);
      Float y_rot = (Float)mPosFilterConfigElement.getProperty("pre_rotation", 1);
      Float z_rot = (Float)mPosFilterConfigElement.getProperty("pre_rotation", 2);
      
      mRotateX.setValue(x_rot);
      mRotateY.setValue(y_rot);
      mRotateZ.setValue(z_rot);

      System.out.println("Edit in progress...");
      
      if (null != mTracker)
      {
         mTracker.setPosition(new Vec3f(x_pos.floatValue(), y_pos.floatValue(), z_pos.floatValue()));
      }
      
      if (null != mTracker)
      {
         Rotf x_rotation = new Rotf(new Vec3f(1.0f, 0.0f, 0.0f), toRad(x_rot.floatValue()));
         Rotf y_rotation = new Rotf(new Vec3f(0.0f, 1.0f, 0.0f), toRad(y_rot.floatValue()));
         Rotf z_rotation = new Rotf(new Vec3f(0.0f, 0.0f, 1.0f), toRad(z_rot.floatValue()));
         
         Rotf temp1 = x_rotation.times(y_rotation);
         Rotf temp2 = temp1.times(z_rotation);

         mTracker.setRotation(temp2);
      }
   }

   private float toRad(float in)
   {
      return((float)Math.toRadians(in));
   }

   public Container getPanel()
   {
//      this.setPreferredSize(new Dimension (450,375));
      return this;
   }
   
   public String getTitle()
   {
      return("Position Filter Editor");
   }
   
   private void jbInit() throws Exception
   {
      GLCanvas canvas = GLDrawableFactory.getFactory().createGLCanvas(new GLCapabilities());
      canvas.addGLEventListener(this);

      // Use debug pipeline
      //    canvas.setGL(new DebugGL(canvas.getGL()));
      System.err.println("CANVAS GL IS: " + canvas.getGL().getClass().getName());
      System.err.println("CANVAS GLU IS: " + canvas.getGLU().getClass().getName());

      double size[][] =
         {{-2.0, 0.25, 0.25, 0.25, -1.0},  // Columns
         {-2.0, 0.75, 0.25, -1.0}}; // Rows

      mTransformPanel.setLayout (new TableLayout(size));
        
      //mTransformPanel.setLayout(new BorderLayout());
      
      mTranslatePanel.setLayout(new GridLayout(3, 2));
      mTranslatePanel.setBorder(new TitledBorder(
               BorderFactory.createLineBorder(new Color(153, 153, 153),2),"Translation"));
      
      mRotatePanel.setLayout(new GridLayout(3, 2));
      mRotatePanel.setBorder(new TitledBorder(
               BorderFactory.createLineBorder(new Color(153, 153, 153),2),"Rotation"));
      
      mTranslateXLbl.setText("X:");
      mTranslateYLbl.setText("Y:");
      mTranslateZLbl.setText("Z:");

      mRotateXLbl.setText("X:");
      mRotateYLbl.setText("Y:");
      mRotateZLbl.setText("Z:");
      
      mTranslateX.addActionListener(new ActionListener()
             {
                public void actionPerformed(ActionEvent evt)
                {
                   updatePosition(evt);
                }
             });
      mTranslateY.addActionListener(new ActionListener()
             {
                public void actionPerformed(ActionEvent evt)
                {
                   updatePosition(evt);
                }
             });
      mTranslateZ.addActionListener(new ActionListener()
             {
                public void actionPerformed(ActionEvent evt)
                {
                   updatePosition(evt);
                }
             });

      mRotateX.addActionListener(new ActionListener()
             {
                public void actionPerformed(ActionEvent evt)
                {
                   updatePosition(evt);
                }
             });
      mRotateY.addActionListener(new ActionListener()
             {
                public void actionPerformed(ActionEvent evt)
                {
                   updatePosition(evt);
                }
             });
      mRotateZ.addActionListener(new ActionListener()
             {
                public void actionPerformed(ActionEvent evt)
                {
                   updatePosition(evt);
                }
             });
      
      mTranslatePanel.add(mTranslateXLbl, null);
      mTranslatePanel.add(mTranslateX, null);
      mTranslatePanel.add(mTranslateYLbl, null);
      mTranslatePanel.add(mTranslateY, null);
      mTranslatePanel.add(mTranslateZLbl, null);
      mTranslatePanel.add(mTranslateZ, null);
      
      mRotatePanel.add(mRotateXLbl, null);
      mRotatePanel.add(mRotateX, null);
      mRotatePanel.add(mRotateYLbl, null);
      mRotatePanel.add(mRotateY, null);
      mRotatePanel.add(mRotateZLbl, null);
      mRotatePanel.add(mRotateZ, null);
     
      //mTransformPanel.add(mChangeTex, BorderLayout.SOUTH);
      mTransformPanel.add(mChangeTex, "2, 2, 3, 2");
      
      //mTransformPanel.add(mTranslatePanel, BorderLayout.WEST);
      mTransformPanel.add(mTranslatePanel, "1, 1, 2, 1");
      mTransformPanel.add(mRotatePanel, "3, 1, 4, 1");
      //mTransformPanel.add(mRotatePanel, BorderLayout.EAST);

      mChangeTex.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  mCurrentFloorIndex++;
                  mCurrentFloorIndex = mCurrentFloorIndex % mFloorTextures.size();
                  mCurrentFloorTexture = (Texture)(mFloorTextures.get(mCurrentFloorIndex));
               }
            });


       setLayout(new BorderLayout());
       add(canvas, BorderLayout.CENTER);
       add(mTransformPanel, BorderLayout.SOUTH);

             final Animator animator = new Animator(canvas);
             /*
             this.addWindowListener(new WindowAdapter()
                   {
                      public void windowClosing(WindowEvent e)
                      {
                         animator.stop();
                         System.exit(0);
                   }
                   });
            */
             animator.start();

    }


   /**
    * main() just creates and shows a PositionalDeviceEditor Frame
    */
   public static void main(String[] args)
   {
      //Frame frame = new Frame("Cloud Simulator");
      /*
      PositionalDeviceEditor pfe = new PositionalDeviceEditor();

      pfe.setSize(750, 750);
      pfe.show();
      */
   }

   private static GLUT mGlut = new GLUT();
   
   protected JPanel mTranslatePanel = new JPanel();
   protected JFormattedTextField mTranslateZ = new JFormattedTextField("0.0");
   protected JLabel mTranslateZLbl = new JLabel();
   protected JFormattedTextField mTranslateX = new JFormattedTextField("0.0");
   protected JLabel mTranslateXLbl = new JLabel();
   protected JFormattedTextField mTranslateY = new JFormattedTextField("0.0");
   protected JLabel mTranslateYLbl = new JLabel();

   protected JPanel mRotatePanel = new JPanel();
   protected JFormattedTextField mRotateZ = new JFormattedTextField("0.0");
   protected JLabel mRotateZLbl = new JLabel();
   protected JFormattedTextField mRotateX = new JFormattedTextField("0.0");
   protected JLabel mRotateXLbl = new JLabel();
   protected JFormattedTextField mRotateY = new JFormattedTextField("0.0");
   protected JLabel mRotateYLbl = new JLabel();
   
   /**
    * Called by drawable to initiate drawing.
    */
   public void display(GLDrawable drawable)
   {
      GL gl = drawable.getGL();
      GLU glu = drawable.getGLU();

      if(null == mTracker && null != mConfigElement)
      {
         String token = mConfigElement.getDefinition().getToken();

         if(token.equals("intersense") ||
            token.equals("intersense_api"))
         {
            mTracker = new Intersense(gl, glu);
         }
         else if(token.equals("motion_star") ||
                 token.equals("flock") )
         {
            mTracker = new Ascension(gl, glu);
         }
         else
         {
            mTracker = new Ascension(gl, glu);
         }
         // Update the GUI to reflect new tracker configuration.
         elementChanged();
      }
      
      // Clear the GL context
      gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT);

      gl.glMatrixMode(gl.GL_PROJECTION);
      gl.glLoadIdentity();
      glu.gluPerspective(45.0f, ((float)mWSize)/((float)mHSize), 0.01f, 1000.0f);
      gl.glMatrixMode(gl.GL_MODELVIEW);
      gl.glLoadIdentity();
      glu.gluLookAt(mCameraPos.x(), mCameraPos.y(), mCameraPos.z(),
                0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f);

      // Draw Axis
      gl.glLineWidth(2.0f);
      gl.glDisable(gl.GL_LIGHTING);
      Vec3f x_axis = new Vec3f(1.0f, 0.0f, 0.0f);
      Vec3f y_axis = new Vec3f(0.0f, 1.0f, 0.0f);
      Vec3f z_axis = new Vec3f(0.0f, 0.0f, 1.0f);
      Vec3f origin = new Vec3f(0.0f, 0.0f, 0.0f);

      gl.glBegin(gl.GL_LINES);
         gl.glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
         gl.glVertex3f(origin.x(), origin.y(), origin.z());
         gl.glVertex3f(x_axis.x(), x_axis.y(), x_axis.z());

         gl.glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
         gl.glVertex3f(origin.x(), origin.y(), origin.z());
         gl.glVertex3f(y_axis.x(), y_axis.y(), y_axis.z());

         gl.glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
         gl.glVertex3f(origin.x(), origin.y(), origin.z());
         gl.glVertex3f(z_axis.x(), z_axis.y(), z_axis.z());
      gl.glEnd();
      gl.glEnable(gl.GL_LIGHTING);

      float half_floor_size = 20.0f;
      float half_tex = (half_floor_size/0.50f);
      
      mCurrentFloorTexture.bind(gl);
      gl.glTexEnvf(gl.GL_TEXTURE_ENV, gl.GL_TEXTURE_ENV_MODE, gl.GL_DECAL);
      gl.glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      gl.glBegin(gl.GL_QUADS);   
         gl.glTexCoord2d(-half_tex, -half_tex);
         gl.glVertex3f(-half_floor_size, 0.0f, -half_floor_size);
         
         gl.glTexCoord2d(half_tex, -half_tex);
         gl.glVertex3f(half_floor_size, 0.0f, -half_floor_size);
         
         gl.glTexCoord2d(half_tex, half_tex);
         gl.glVertex3f(half_floor_size, 0.0f, half_floor_size);
         
         gl.glTexCoord2d(-half_tex, half_tex);
         gl.glVertex3f(-half_floor_size, 0.0f, half_floor_size);
      gl.glEnd();
      mCurrentFloorTexture.unbind(gl);
      
      if (null != mTracker)
      {
         mTracker.render(gl, glu);
      }
   }

   /**
    * Called by drawable to indicate mode or device has changed.
    */
   public void displayChanged(GLDrawable drawable,
                              boolean modeChanged,
                              boolean deviceChanged)
   {
      System.out.println ("displayChanged()");
   }

   /**
    * Called after OpenGL is init'ed.
    */
   public void init(GLDrawable drawable)
   {
      System.out.println("init()");

      GL gl = drawable.getGL();
      GLU glu = drawable.getGLU();

      // Set the initial OpenGL state
      initGLState(gl);

      // Load textures
      Texture temp = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/images/carpet.png");
      mFloorTextures.add(temp);
      /*
      temp = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/images/wood.png");
      mFloorTextures.add(temp);
      temp = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/images/grassy_t.png");
      mFloorTextures.add(temp);
      temp = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/images/mixgrs_t.png");
      mFloorTextures.add(temp);
      temp = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/images/concrete.png");
      mFloorTextures.add(temp);
      temp = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/images/cncrt1_t.png");
      mFloorTextures.add(temp);
      */

      mCurrentFloorTexture = (Texture)(mFloorTextures.get(mCurrentFloorIndex));


      gl.glMatrixMode(GL.GL_PROJECTION);
      gl.glLoadIdentity();
      glu.gluPerspective(45, 1, 1, 100);
      gl.glMatrixMode(GL.GL_MODELVIEW);
      gl.glLoadIdentity();

      // Register the window with the ManipManager
      ManipManager manager = ManipManager.getManipManager();
      manager.registerWindow(drawable);

      drawable.addMouseMotionListener(mouseMotionListener);
      drawable.addMouseListener(mouseListener);   
   }

   /**
    * Initialize the OpenGL state for this application.
    */
   private void initGLState(GL gl)
   {
      float light0_ambient[] = { 0.1f,  0.1f,  0.1f,  1.0f};
      float light0_diffuse[] = { 0.7f,  0.7f,  0.7f,  1.0f};
      float light0_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
      float light0_position[] = {0.5f, 0.75f, 0.75f, 0.0f};

      float mat_ambient[] = { 0.7f, 0.7f,  0.7f,  1.0f};
      float mat_diffuse[] = { 1.0f,  0.5f,  0.8f,  1.0f};
      float mat_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
      float mat_shininess[] = { 50.0f};
      // GLfloat mat_emission[] = { 1.0,  1.0,  1.0,  1.0};
      float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f};

      gl.glLightfv(gl.GL_LIGHT0, gl.GL_AMBIENT,  light0_ambient);
      gl.glLightfv(gl.GL_LIGHT0, gl.GL_DIFFUSE,  light0_diffuse);
      gl.glLightfv(gl.GL_LIGHT0, gl.GL_SPECULAR, light0_specular);
      gl.glLightfv(gl.GL_LIGHT0, gl.GL_POSITION, light0_position);

      gl.glMaterialfv(gl.GL_FRONT, gl.GL_AMBIENT,   mat_ambient );
      gl.glMaterialfv(gl.GL_FRONT, gl.GL_DIFFUSE,   mat_diffuse );
      gl.glMaterialfv(gl.GL_FRONT, gl.GL_SPECULAR,  mat_specular );
      gl.glMaterialfv(gl.GL_FRONT, gl.GL_SHININESS, mat_shininess );
      gl.glMaterialfv(gl.GL_FRONT, gl.GL_EMISSION,  no_mat);

      gl.glEnable(gl.GL_DEPTH_TEST);
      gl.glEnable(gl.GL_NORMALIZE);
      gl.glEnable(gl.GL_LIGHTING);
      gl.glEnable(gl.GL_LIGHT0);
      gl.glEnable(gl.GL_COLOR_MATERIAL);
      gl.glShadeModel(gl.GL_SMOOTH);

      gl.glEnable(gl.GL_TEXTURE_2D);
//      gl.glEnable(gl.GL_BLEND);		    // Turn Blending On
//      gl.glDisable(gl.GL_DEPTH_TEST);         // Turn Depth Testing Off
      gl.glBlendFunc(gl.GL_ONE, gl.GL_ONE);
   }

   private int mWSize;
   private int mHSize;

   /**
    * Called to indicate the drawing surface has been moved and/or resized.
    */
   public void reshape(GLDrawable drawable, int x, int y, int w, int h)
   {
      mWSize = w;
      mHSize = h;
      // Unused function since the ExamineViewer takes care of this.
   }
   
   /** The handle to the configuration broker. */
   private ConfigBroker mBroker;
   
   /**
    * Gets a handle to the configuration broker.
    */
   protected ConfigBroker getBroker()
   {
      if (mBroker == null)
      {
         synchronized (this)
         {
            if (mBroker == null)
            {
               mBroker = new ConfigBrokerProxy();
            }
         }
      }
      return mBroker;
   }
}
