package org.vrjuggler.vrjconfig.commoneditors;

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

   private JPanel mPosPanel = new JPanel();

   private Texture mCurrentFloorTexture = null;
   private int mCurrentFloorIndex = 0;
   private List mFloorTextures = new ArrayList();
   private JButton mChangeTex = new JButton("Change Floor");
   
   /**
    * We'd like to be 600x400, please.
    */
   public Dimension getPreferredSize()
   {
      return PREFERRED_FRAME_SIZE;
   }

   private void updatePosition(ActionEvent evt)
   {
      if (evt.getSource() == mX)
      {
         mConfigElement.setProperty("pre_translate", 0, mX.getValue());
      }
      else if (evt.getSource() == mY)
      {
         mConfigElement.setProperty("pre_translate", 1, mY.getValue());
      }
      else if (evt.getSource() == mZ)
      {
         mConfigElement.setProperty("pre_translate", 2, mZ.getValue());
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
   private ConfigContext mConfigContext = null;
   
   public void setConfigElement(ConfigElement elm)
   {
      mConfigElement = elm;
      if(null == mConfigContext)
      {
         System.out.println("ERROR: You must set the ConfigContext before you set the ConfigElement.");
      }

      mConfigElement.addConfigElementListener(new ConfigElementAdapter()
            {
               public void propertyValueChanged(ConfigElementEvent evt)
               {
                  elementChanged();
               }
            });

      
      elementChanged();
   }

   public void elementChanged()
   {
      Float x_pos = (Float)mConfigElement.getProperty("pre_translate", 0);
      Float y_pos = (Float)mConfigElement.getProperty("pre_translate", 1);
      Float z_pos = (Float)mConfigElement.getProperty("pre_translate", 2);
      
      mX.setValue(x_pos);
      mY.setValue(y_pos);
      mZ.setValue(z_pos);

      System.out.println("Edit in progress...");
      
      if (null != mTracker)
      {
         mTracker.setPosition(new Vec3f(x_pos.floatValue(), y_pos.floatValue(), z_pos.floatValue()));
      }
   }

   /**
    * Specify the ConfigContext to use with this CustomEditor.
    */
   public void setContext(ConfigContext ctx)
   {
      mConfigContext = ctx;
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
      titledBorder1 = new TitledBorder(BorderFactory.createLineBorder(new Color(153, 153, 153),2),"Translation");
      canvas.addGLEventListener(this);

      // Use debug pipeline
      //    canvas.setGL(new DebugGL(canvas.getGL()));
      System.err.println("CANVAS GL IS: " + canvas.getGL().getClass().getName());
      System.err.println("CANVAS GLU IS: " + canvas.getGLU().getClass().getName());

      mPosPanel.setLayout(new GridLayout(3, 2));
      mTranslatePanel.setBorder(titledBorder1);
      mTranslatePanel.setLayout(gridLayout1);
      
      mXLbl.setText("X:");
      mYLabel.setText("Y:");
      mZLabel.setText("Z:");
      
      mX.addActionListener(new ActionListener()
             {
                public void actionPerformed(ActionEvent evt)
                {
                   updatePosition(evt);
                }
             });
      mY.addActionListener(new ActionListener()
             {
                public void actionPerformed(ActionEvent evt)
                {
                   updatePosition(evt);
                }
             });
      mZ.addActionListener(new ActionListener()
             {
                public void actionPerformed(ActionEvent evt)
                {
                   updatePosition(evt);
                }
             });
      
      mPosPanel.add(mTranslatePanel, null);
      mTranslatePanel.add(mXLbl, null);
      mTranslatePanel.add(mX, null);
      mTranslatePanel.add(mYLabel, null);
      mTranslatePanel.add(mY, null);
      mTranslatePanel.add(mZLabel, null);
      mTranslatePanel.add(mZ, null);
      mTranslatePanel.add(mChangeTex, 0);

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
       add(mPosPanel, BorderLayout.SOUTH);

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
   protected TitledBorder titledBorder1;
   protected JFormattedTextField mZ = new JFormattedTextField("0.0");
   protected JLabel mZLabel = new JLabel();
   protected JFormattedTextField mX = new JFormattedTextField("0.0");
   protected JLabel mXLbl = new JLabel();
   protected JFormattedTextField mY = new JFormattedTextField("0.0");
   protected JLabel mYLabel = new JLabel();
   protected GridLayout gridLayout1 = new GridLayout();

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
      Texture temp = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/images/carpet.png");
      mFloorTextures.add(temp);
      temp = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/images/wood.png");
      mFloorTextures.add(temp);
      temp = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/images/grassy_t.png");
      mFloorTextures.add(temp);
      temp = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/images/mixgrs_t.png");
      mFloorTextures.add(temp);
      temp = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/images/concrete.png");
      mFloorTextures.add(temp);
      temp = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/images/cncrt1_t.png");
      mFloorTextures.add(temp);

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
