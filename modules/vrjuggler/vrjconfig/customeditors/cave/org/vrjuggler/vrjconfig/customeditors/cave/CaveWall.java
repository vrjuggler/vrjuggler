/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

package org.vrjuggler.vrjconfig.customeditors.cave;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.*;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.border.TitledBorder;
import javax.swing.event.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;
//import org.vrjuggler.vrjconfig.commoneditors.TransmitterTransformPanel;
import info.clearthought.layout.TableLayout;

public class CaveWall implements EditorConstants
{
   private CaveModel mCaveModel = null;
   private ConfigElement mLeftView = null;
   private ConfigElement mRightView = null;
   private ConfigElement mStereoView = null;
   private float[] ll_corner = new float[3];
   private float[] lr_corner = new float[3];
   private float[] ur_corner = new float[3];
   private float[] ul_corner = new float[3];

   private Point3D[] mCorners = new Point3D[4];
   
   double mWallWidth, mWallHeight;
   int mPlane = LEFT_PLANE;

   private String mName = null;
   
   private int mStereoMode = -1;
  
   public CaveWall()
   {
      mCorners[LOWER_LEFT] = new Point3D(-5.0f, -5.0f, 0.0f);
      mCorners[LOWER_RIGHT] = new Point3D(5.0f, -5.0f, 0.0f);
      mCorners[UPPER_RIGHT] = new Point3D(5.0f, 5.0f, 0.0f);
      mCorners[UPPER_LEFT] = new Point3D(-5.0f, 5.0f, 0.0f);
      
      ll_corner[0] = -5.0f;
      ll_corner[1] = -5.0f;
      ll_corner[2] =  0.0f;
      
      lr_corner[0] =  5.0f;
      lr_corner[1] = -5.0f;
      lr_corner[2] =  0.0f;
      
      ur_corner[0] =  5.0f;
      ur_corner[1] =  5.0f;
      ur_corner[2] =  0.0f;
      
      ul_corner[0] = -5.0f;
      ul_corner[1] =  5.0f;
      ul_corner[2] =  0.0f;
   }

   public CaveWall(String name)
   {
      this();
      mName = name;
   }
   
   public CaveWall(float[] ll, float[] lr, float[] ur, float[] ul)
   {
      mName = new String("");
      for (int i = 0 ; i < 3 ; i++)
      {
         ll_corner[i] = ll[i];
         lr_corner[i] = lr[i];
         ur_corner[i] = ur[i];
         ul_corner[i] = ul[i];
      }
      
      mCorners[LOWER_LEFT] = new Point3D((double)ll[0], (double)ll[1], (double)ll[2]);
      mCorners[LOWER_RIGHT] = new Point3D((double)lr[0], (double)lr[1], (double)lr[2]);
      mCorners[UPPER_RIGHT] = new Point3D((double)ur[0], (double)ur[1], (double)ur[2]);
      mCorners[UPPER_LEFT] = new Point3D((double)ul[0], (double)ul[1], (double)ul[2]);
   }
   
   public CaveWall(CaveModel cm, Corners c, List views)
   {
      mCaveModel = cm;
         
      ll_corner[0] = c.getLL()[0];
      ll_corner[1] = c.getLL()[1];
      ll_corner[2] = c.getLL()[2];
      lr_corner[0] = c.getLR()[0];
      lr_corner[1] = c.getLR()[1];
      lr_corner[2] = c.getLR()[2];
      ul_corner[0] = c.getUL()[0];
      ul_corner[1] = c.getUL()[1];
      ul_corner[2] = c.getUL()[2];
      ur_corner[0] = c.getUR()[0];
      ur_corner[1] = c.getUR()[1];
      ur_corner[2] = c.getUR()[2];
      
      mCorners[LOWER_LEFT] = new Point3D((double)c.getLL()[0], (double)c.getLL()[1], (double)c.getLL()[2]);
      mCorners[LOWER_RIGHT] = new Point3D((double)c.getLR()[0], (double)c.getLR()[1], (double)c.getLR()[2]);
      mCorners[UPPER_RIGHT] = new Point3D((double)c.getUR()[0], (double)c.getUR()[1], (double)c.getUR()[2]);
      mCorners[UPPER_LEFT] = new Point3D((double)c.getUL()[0], (double)c.getUL()[1], (double)c.getUL()[2]);
         
      int num_views = views.size();
      if(num_views > 2 || num_views < 1)
      {
         throw new IllegalArgumentException("Invalid number of views: " + Integer.toString(num_views));
      }
      
      // When we create walls from scratch we will name it's viewports
      // [Wall Name] - [Right/Left/Stereo/] depending on the view selected.
      // So we can strip the first part to get the wall name from the first
      // view.
      ConfigElement name_pair = (ConfigElement)views.get(0);
      String full_name = name_pair.getName();
      mName = full_name;
         
      // - If we have only one
      //   - If stereo active and view stereo
      //     - ACTIVE_STEREO
      //   - Else if stereo active or view stereo
      //     - ERROR, choose correct view
      //   - Else MONO
      // - If we have two views
      //   - If views are the same
      //     - ERROR, prompt to correct
      //   - Else
      //     - PASSIVE_STEREO
      mRightView = null;
      mLeftView = null;
      mStereoView = null;
      

      if(num_views == 1)
      {
         ConfigElement view_elm = (ConfigElement)views.get(0);
         ConfigElement screen_elm = (ConfigElement)mCaveModel.getViewToScreenMap().get(view_elm);
         
         int view = ((Number)view_elm.getProperty("view", 0)).intValue();
         boolean stereo;

         // When we do not yet have a screen assume that we are creating
         // a new wall which has a default mono view.
         if (null == screen_elm)
         {
            stereo = false;
         }
         else
         {
            stereo = ((Boolean) screen_elm.getProperty("stereo", 0)).booleanValue();
         }

         if( stereo && (3 == view))
         {
            mStereoView = view_elm;
            System.out.println("Setting stereo mode: ACTIVE_STEREO");
            mStereoMode = ACTIVE_STEREO;
         }
         else if( stereo || (3 == view))
         {
            System.out.println("ERROR: Active stereo walls should have stereo enabled, and their view set to Stereo.");
         }
         else if(1 == view)
         {
            mLeftView = view_elm;
            System.out.println("Setting stereo mode: MONO");
            mStereoMode = MONO;            
         }
         else
         {
            mRightView = view_elm;
            System.out.println("Setting stereo mode: MONO");
            mStereoMode = MONO;
         }
      }
      else if(num_views == 2)
      {
         ConfigElement view_elm0 = (ConfigElement)views.get(0);
         ConfigElement view_elm1 = (ConfigElement)views.get(1);
         ConfigElement screen_elm0 = (ConfigElement)mCaveModel.getViewToScreenMap().get(view_elm0);
         ConfigElement screen_elm1 = (ConfigElement)mCaveModel.getViewToScreenMap().get(view_elm1);
         int view0 = ((Number)view_elm0.getProperty("view", 0)).intValue();
         int view1 = ((Number)view_elm1.getProperty("view", 0)).intValue();
         boolean stereo0 = ((Boolean) screen_elm0.getProperty("stereo", 0)).booleanValue();
         boolean stereo1 = ((Boolean) screen_elm1.getProperty("stereo", 0)).booleanValue();

         if (stereo0 || stereo1)
         {
            System.out.println("ERROR: Two surface viewports with same corners should not be on stereo screen.");
         }
         else if(3 == view0 || 3 == view1)
         {
            System.out.println("ERROR: Two surface viewports with same corners should not have stereo views.");
         }
         else if(view0 == view1)
         {
            System.out.println("ERROR: Two surface viewports with same corners should not have the same view.");
         }
         else
         {
            mLeftView = (1 == view0 ? view_elm0 : view_elm0);
            mRightView = (2 == view0 ? view_elm0 : view_elm1);
            System.out.println("Setting stereo mode: PASSIVE_STEREO");
            mStereoMode = PASSIVE_STEREO;
         }
      }
   }
    
   /** Reference to the ConfigBroker used in this object. */
   private ConfigBroker mBroker = null; 

   /**
    * Gets a handle to the configuration broker.
    */
   private ConfigBroker getBroker()
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
   
   public void setStereoMode(int stereo, ConfigContext context)
   {
      // PASSIVE -> ACTIVE
      //   - Remove right view
      //   - Set stereo true on screen
      //   - Set left view to "Stereo"
      // PASSIVE -> MONO
      //   - Remove right view
      //   - Set left view to "Left Eye"
      // MONO -> ACTIVE
      //   - Set stereo true on screen
      //   - Set left view to "Stereo"
      // MONO -> PASSIVE
      //   - Add right view
      //   - Set right view to "Right Eye"
      // ACTIVE -> PASSIVE
      //   - Add right view
      //   - Set left view to "Left Eye"
      //   - Set right view to "Right Eye"
      //   - Set stereo false on screen
      // ACTIVE -> MONO
      //   - Set left view to "Left Eye"
      //   - Set stereo false on screen

      if ( stereo < 0 || stereo > MONO )
      {
         throw new IllegalArgumentException("Not a valid stereo mode: " + stereo);
      }
      
      if (stereo == mStereoMode)
      {
         return;
      }
      
      PropertyDefinition prop_def = mLeftView.getDefinition().getPropertyDefinition(VIEW_PROPERTY);
      Map enums = prop_def.getEnums();
      
      if (PASSIVE_STEREO == mStereoMode)
      {
         if (stereo == ACTIVE_STEREO)
         {
            ConfigElement screen;
            screen = (ConfigElement)mCaveModel.getViewToScreenMap().get(mRightView);
            mCaveModel.getViewToScreenMap().remove(mRightView);

            if (null != screen)
            {
               screen.removeProperty(SURFACE_VIEWPORTS_PROPERTY, mRightView, context);
            }
            else
            {
               System.out.println("No screen set for right view, so we can not remove the view.");
            }
            mRightView = null;
            
            screen = (ConfigElement)mCaveModel.getViewToScreenMap().get(mLeftView);

            if (null != screen)
            {
               screen.setProperty(STEREO_PROPERTY, 0, new Boolean(true), context);
            }
            else
            {
               System.out.println("No screen set for left view, so we can not enable stereo.");
            }
            mLeftView.setProperty(VIEW_PROPERTY, 0, enums.get("Stereo"));
         }
         else if (stereo == MONO)
         {
            ConfigElement screen = (ConfigElement)mCaveModel.getViewToScreenMap().get(mRightView);
            mCaveModel.getViewToScreenMap().remove(mRightView);

            if (null != screen)
            {
               screen.removeProperty(SURFACE_VIEWPORTS_PROPERTY, mRightView, context);
            }
            else
            {
               System.out.println("No screen set for right view, so we can not remove the view.");
            }

            mLeftView.setProperty(VIEW_PROPERTY, 0, enums.get("Left Eye"));
         }
      }
      if (MONO == mStereoMode)
      {
         if (stereo == ACTIVE_STEREO)
         {
            ConfigElement screen = (ConfigElement)mCaveModel.getViewToScreenMap().get(mLeftView);
            if (null != screen)
            {
               screen.setProperty(STEREO_PROPERTY, 0, new Boolean(true), context);
            }
            else
            {
               System.out.println("No screen set for left view, so we can not enable stereo.");
            }
            mLeftView.setProperty(VIEW_PROPERTY, 0, enums.get("Stereo"));
         }
         else if (stereo == PASSIVE_STEREO)
         {
            // Get the ConfigDefinition of the proxy we want to create.
            ConfigDefinition surface_def = getBroker().getRepository().get(SURFACE_VIEWPORT_TYPE);
            
            // Create a temporary list of ConfigDefinitions to pass to factory.
            java.util.List def_list = new ArrayList();
            def_list.add(surface_def);
           
            ConfigElement screen = (ConfigElement)mCaveModel.getViewToScreenMap().get(mLeftView);
            
            // Initialize a ConfigElementFactory with the needed 
            // ConfigDefinition. And create a new ConfigElement.
            ConfigElementFactory temp_factory = new ConfigElementFactory(def_list);
            ConfigElement element = temp_factory.create("Right", surface_def);
            mRightView = element;
            
            mCaveModel.getViewToScreenMap().put(mRightView, screen);

            if (null != screen)
            {
               screen.addProperty(SURFACE_VIEWPORTS_PROPERTY, mRightView, context);
            }
            else
            {
               System.out.println("No screen set for left view, so we can not just add a right.");
            }
            

            mLeftView.setProperty(VIEW_PROPERTY, 0, enums.get("Left Eye"));
            mRightView.setProperty(VIEW_PROPERTY, 0, enums.get("Right Eye"));
         }
      }
      if (ACTIVE_STEREO == mStereoMode)
      {
         if (stereo == PASSIVE_STEREO)
         {
            // Get the ConfigDefinition of the proxy we want to create.
            ConfigDefinition surface_def = getBroker().getRepository().get(SURFACE_VIEWPORT_TYPE);
            
            // Create a temporary list of ConfigDefinitions to pass to factory.
            java.util.List def_list = new ArrayList();
            def_list.add(surface_def);
           
            ConfigElement screen = (ConfigElement)mCaveModel.getViewToScreenMap().get(mLeftView);
            
            // Initialize a ConfigElementFactory with the needed 
            // ConfigDefinition. And create a new ConfigElement.
            ConfigElementFactory temp_factory = new ConfigElementFactory(def_list);
            ConfigElement element = temp_factory.create("Right", surface_def);
            mRightView = element;
            
            mCaveModel.getViewToScreenMap().put(mRightView, screen);
            screen.addProperty(SURFACE_VIEWPORTS_PROPERTY, mRightView, context);
            
            mLeftView.setProperty(VIEW_PROPERTY, 0, enums.get("Left Eye"));
            mRightView.setProperty(VIEW_PROPERTY, 0, enums.get("Right Eye"));
            
            screen.setProperty(STEREO_PROPERTY, 0, new Boolean(false), context);
         }
         else if (stereo == MONO)
         {
            mLeftView.setProperty(VIEW_PROPERTY, 0, enums.get("Left Eye"));
            
            ConfigElement screen = (ConfigElement)mCaveModel.getViewToScreenMap().get(mLeftView);
            screen.setProperty(STEREO_PROPERTY, 0, new Boolean(false), context);
         }
      }
      
      mStereoMode = stereo;

      // Use the setName function to set the names of the surface_viewports.
      setName(mName);
   }

   public int getStereoMode()
   {
      return mStereoMode;
   }
   
   public Point3D[] getCorners()
   {
      return mCorners;
   }

   public void setCorners(Point3D[] corners, ConfigContext context)
   {
      for (int i = 0 ; i < 4 ; i++)
      {
         mCorners[i] = corners[i];
      }
      
      mLeftView.setProperty(LOWER_LEFT_CORNER_PROPERTY, 0,
                            new Float(corners[0].x), context);
      mLeftView.setProperty(LOWER_LEFT_CORNER_PROPERTY, 1,
                            new Float(corners[0].y), context);
      mLeftView.setProperty(LOWER_LEFT_CORNER_PROPERTY, 2,
                            new Float(corners[0].z), context);
      mLeftView.setProperty(LOWER_RIGHT_CORNER_PROPERTY, 0,
                            new Float(corners[1].x), context);
      mLeftView.setProperty(LOWER_RIGHT_CORNER_PROPERTY, 1,
                            new Float(corners[1].y), context);
      mLeftView.setProperty(LOWER_RIGHT_CORNER_PROPERTY, 2,
                            new Float(corners[1].z), context);
      mLeftView.setProperty(UPPER_RIGHT_CORNER_PROPERTY, 0,
                            new Float(corners[2].x), context);
      mLeftView.setProperty(UPPER_RIGHT_CORNER_PROPERTY, 1,
                            new Float(corners[2].y), context);
      mLeftView.setProperty(UPPER_RIGHT_CORNER_PROPERTY, 2,
                            new Float(corners[2].z), context);
      mLeftView.setProperty(UPPER_LEFT_CORNER_PROPERTY, 0,
                            new Float(corners[3].x), context);
      mLeftView.setProperty(UPPER_LEFT_CORNER_PROPERTY, 1,
                            new Float(corners[3].y), context);
      mLeftView.setProperty(UPPER_LEFT_CORNER_PROPERTY, 2,
                            new Float(corners[3].z), context);

      if (null != mRightView)
      {
         mRightView.setProperty(LOWER_LEFT_CORNER_PROPERTY, 0,
                               new Float(corners[0].x), context);
         mRightView.setProperty(LOWER_LEFT_CORNER_PROPERTY, 1,
                               new Float(corners[0].y), context);
         mRightView.setProperty(LOWER_LEFT_CORNER_PROPERTY, 2,
                               new Float(corners[0].z), context);
         mRightView.setProperty(LOWER_RIGHT_CORNER_PROPERTY, 0,
                               new Float(corners[1].x), context);
         mRightView.setProperty(LOWER_RIGHT_CORNER_PROPERTY, 1,
                               new Float(corners[1].y), context);
         mRightView.setProperty(LOWER_RIGHT_CORNER_PROPERTY, 2,
                               new Float(corners[1].z), context);
         mRightView.setProperty(UPPER_RIGHT_CORNER_PROPERTY, 0,
                               new Float(corners[2].x), context);
         mRightView.setProperty(UPPER_RIGHT_CORNER_PROPERTY, 1,
                               new Float(corners[2].y), context);
         mRightView.setProperty(UPPER_RIGHT_CORNER_PROPERTY, 2,
                               new Float(corners[2].z), context);
         mRightView.setProperty(UPPER_LEFT_CORNER_PROPERTY, 0,
                               new Float(corners[3].x), context);
         mRightView.setProperty(UPPER_LEFT_CORNER_PROPERTY, 1,
                               new Float(corners[3].y), context);
         mRightView.setProperty(UPPER_LEFT_CORNER_PROPERTY, 2,
                               new Float(corners[3].z), context);
      }
   }

   public void setTracked(Boolean tracked, ConfigContext context)
   {
      mLeftView.setProperty(TRACKED_PROPERTY, 0, tracked,
                            context);
      
      if (null != mRightView)
      {
         mRightView.setProperty(TRACKED_PROPERTY, 0, tracked,
                                context);
      }
   }
   
   public void setTrackerProxy(Object proxy, ConfigContext context)
   {
      mLeftView.setProperty(TRACKER_PROXY_PROPERTY, 0, proxy,
                            context);
      
      if (null != mRightView)
      {
         mRightView.setProperty(TRACKER_PROXY_PROPERTY, 0, proxy,
                                context);
      }
   }
   
   public void setUser(Object proxy, ConfigContext context)
   {
      mLeftView.setProperty(USER_PROPERTY, 0, proxy,
                            context);
      
      if (null != mRightView)
      {
         mRightView.setProperty(USER_PROPERTY, 0, proxy,
                                context);
      }
   }

   public CaveWall(ConfigElement elm, ConfigElement screen_elm)
   {
      //mScreenElement = screen_elm;
      mName = elm.getName();

      ll_corner[0] = ((Number) elm.getProperty("lower_left_corner", 0)).floatValue();
      ll_corner[1] = ((Number) elm.getProperty("lower_left_corner", 1)).floatValue();
      ll_corner[2] = ((Number) elm.getProperty("lower_left_corner", 2)).floatValue();
      lr_corner[0] = ((Number) elm.getProperty("lower_right_corner", 0)).floatValue();
      lr_corner[1] = ((Number) elm.getProperty("lower_right_corner", 1)).floatValue();
      lr_corner[2] = ((Number) elm.getProperty("lower_right_corner", 2)).floatValue();
      ul_corner[0] = ((Number) elm.getProperty("upper_left_corner", 0)).floatValue();
      ul_corner[1] = ((Number) elm.getProperty("upper_left_corner", 1)).floatValue();
      ul_corner[2] = ((Number) elm.getProperty("upper_left_corner", 2)).floatValue();
      ur_corner[0] = ((Number) elm.getProperty("upper_right_corner", 0)).floatValue();
      ur_corner[1] = ((Number) elm.getProperty("upper_right_corner", 1)).floatValue();
      ur_corner[2] = ((Number) elm.getProperty("upper_right_corner", 2)).floatValue();
      
      mCorners[LOWER_LEFT] = new Point3D(
             (double)((Number) elm.getProperty("lower_left_corner", 0)).floatValue(),
             (double)((Number) elm.getProperty("lower_left_corner", 1)).floatValue(),
             (double)((Number) elm.getProperty("lower_left_corner", 2)).floatValue());
      mCorners[LOWER_RIGHT] = new Point3D(
             (double)((Number) elm.getProperty("lower_right_corner", 0)).floatValue(),
             (double)((Number) elm.getProperty("lower_right_corner", 1)).floatValue(),
             (double)((Number) elm.getProperty("lower_right_corner", 2)).floatValue());
      mCorners[UPPER_RIGHT] = new Point3D(
             (double)((Number) elm.getProperty("upper_right_corner", 0)).floatValue(),
             (double)((Number) elm.getProperty("upper_right_corner", 1)).floatValue(),
             (double)((Number) elm.getProperty("upper_right_corner", 2)).floatValue());
      mCorners[UPPER_LEFT] = new Point3D(
             (double)((Number) elm.getProperty("upper_left_corner", 0)).floatValue(),
             (double)((Number) elm.getProperty("upper_left_corner", 1)).floatValue(),
             (double)((Number) elm.getProperty("upper_left_corner", 2)).floatValue());
   }

   public String getName()
   {
      return mName;
   }

   public void setName(String name)
   {
      mName = name;
      
      if ( ACTIVE_STEREO == mStereoMode)
      {
         mLeftView.setName(mName + " (Stereo)");
      }
      if ( MONO == mStereoMode)
      {
         mLeftView.setName(mName + " (Mono)");
      }
      if ( PASSIVE_STEREO == mStereoMode)
      {
         mLeftView.setName(mName + " (Left Eye)");
         mRightView.setName(mName + " (Right Eye)");
      }
   }
   
   public void updateWidthHeight()
   {
      if ( mCorners[LOWER_LEFT].x == mCorners[LOWER_RIGHT].x &&
           mCorners[LOWER_LEFT].x == mCorners[UPPER_LEFT].x )
      {
         if ( mCorners[LOWER_LEFT].z > mCorners[LOWER_RIGHT].z )
         {
            mPlane = LEFT_PLANE;
            mWallWidth  = mCorners[LOWER_LEFT].z - mCorners[LOWER_RIGHT].z;
            mWallHeight = mCorners[UPPER_LEFT].y - mCorners[LOWER_LEFT].y;
         }
         else
         {
            mPlane = RIGHT_PLANE;
            mWallWidth  = mCorners[LOWER_RIGHT].z - mCorners[LOWER_LEFT].z;
            mWallHeight = mCorners[UPPER_LEFT].y - mCorners[LOWER_LEFT].y;
         }
      }
      else if ( mCorners[LOWER_LEFT].z == mCorners[LOWER_RIGHT].z &&
                mCorners[LOWER_LEFT].z == mCorners[UPPER_LEFT].z )
      {
         if ( mCorners[LOWER_RIGHT].x > mCorners[LOWER_LEFT].x )
         {
            mPlane = FRONT_PLANE;
            mWallWidth  = mCorners[LOWER_RIGHT].x - mCorners[LOWER_LEFT].x;
            mWallHeight = mCorners[UPPER_LEFT].y - mCorners[LOWER_LEFT].y;
         }
         else
         {
            mPlane = BACK_PLANE;
            mWallWidth  = mCorners[LOWER_LEFT].x - mCorners[LOWER_RIGHT].x;
            mWallHeight = mCorners[UPPER_LEFT].y - mCorners[LOWER_LEFT].y;
         }
      }
      else if ( mCorners[LOWER_LEFT].y == mCorners[UPPER_LEFT].y &&
                mCorners[LOWER_LEFT].y == mCorners[LOWER_RIGHT].y )
      {
         if ( mCorners[LOWER_LEFT].z > mCorners[UPPER_LEFT].z )
         {
            mPlane = BOTTOM_PLANE;
            mWallWidth  = mCorners[LOWER_RIGHT].x - mCorners[LOWER_LEFT].x;
            mWallHeight = mCorners[LOWER_LEFT].z - mCorners[UPPER_LEFT].z;
         }
         else
         {
            mPlane = TOP_PLANE;
            mWallWidth  = mCorners[LOWER_RIGHT].x - mCorners[LOWER_LEFT].x;
            mWallHeight = mCorners[UPPER_LEFT].z - mCorners[LOWER_LEFT].z;
         }
      }
      else
      {
         mPlane = CUSTOM_PLANE;

         double x_diff = mCorners[LOWER_RIGHT].x - mCorners[LOWER_LEFT].x;
         double y_diff = mCorners[LOWER_RIGHT].y - mCorners[LOWER_LEFT].y;
         double z_diff = mCorners[LOWER_RIGHT].z - mCorners[LOWER_LEFT].z;
         double len_sq = x_diff * x_diff + y_diff * y_diff + z_diff * z_diff;
         mWallWidth = Math.sqrt(len_sq);

         x_diff = mCorners[UPPER_LEFT].x - mCorners[LOWER_LEFT].x;
         y_diff = mCorners[UPPER_LEFT].y - mCorners[LOWER_LEFT].y;
         z_diff = mCorners[UPPER_LEFT].z - mCorners[LOWER_LEFT].z;
         double width_sq = x_diff * x_diff + y_diff * y_diff + z_diff * z_diff;
         mWallHeight = Math.sqrt(width_sq);

         // XXX: At this point, there needs to be code that figures out the
         // rotational angles for the custom-defined plane.  Those angles
         // then need to go into the text fields.
      }
   }
   
   public double getWidth()
   {
      return mWallWidth;
   }
   
   public double getHeight()
   {
      return mWallHeight;
   }

   public int getPlane()
   {
      return mPlane;
   }

   public ConfigElement getLeftView()
   {
      return mLeftView;
   }
   
   public ConfigElement getRightView()
   {
      return mRightView;
   }
   
   public ConfigElement getLeft()
   {
      return mLeftView;
   }

   public ConfigElement getRight()
   {
      return mRightView;
   }
   
   public int hashCode()
   {
      return 0;
      //XXX: Worried about floating point errors because HashMaps use both
      //obj1.hashCode() == obj2hashCode() & obj1.equals(obj2). Maybe Set
      //would be better.
      //return (ll_corner[0] + lr_corner[0] + ul_corner[0] + ur_corner[0]);
   }
   
   public boolean equals(Object obj)
   {
      if (obj instanceof CaveWall)
      {
         CaveWall cw = (CaveWall)obj;
         if (mCorners[0].equals(cw.mCorners[0]) &&
             mCorners[1].equals(cw.mCorners[1]) &&
             mCorners[2].equals(cw.mCorners[2]) &&
             mCorners[3].equals(cw.mCorners[3]))
         {
            return true;
         }
      }
      return false;
   }

   public String toString()
   {
      String str = new String("Cave Wall:");
      str = "Cave Wall: " + ((ConfigElement)mLeftView).getName()
         + " " + ((ConfigElement)mRightView).getName();

      return str;
   }
}
