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


public class CaveWall
{
   private CaveModel mCaveModel = null;
   private ConfigElement mLeftView = null;
   private ConfigElement mRightView = null;
   private ConfigElement mStereoView = null;
   private float[] ll_corner = new float[3];
   private float[] lr_corner = new float[3];
   private float[] ur_corner = new float[3];
   private float[] ul_corner = new float[3];

   private String mName = null;
   
   public static final int ACTIVE_STEREO = 0;
   public static final int PASSIVE_STEREO = 1;
   public static final int MONO = 2;

   private int mStereoMode = -1;
  
   public CaveWall()
   {
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
   }
   
   public CaveWall(CaveModel cm, Corners c, List views)
   {
      mCaveModel = cm;

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
         boolean stereo = ((Boolean) screen_elm.getProperty("stereo", 0)).booleanValue();

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

   public void setStereoMode(int stereo)
   {
      if ( stereo < 0 || stereo > MONO )
      {
         throw new IllegalArgumentException("Not a valid stereo mode: " + stereo);
      }
      
      mStereoMode = stereo;
   }

   public int getStereoMode()
   {
      return mStereoMode;
   }

   public float[] getLL()
   {
      return ll_corner;
   }
   public float[] getLR()
   {
      return lr_corner;
   }
   public float[] getUR()
   {
      return ur_corner;
   }
   public float[] getUL()
   {
      return ul_corner;
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
   }

   public String getName()
   {
      return mName;
   }

   /*
   public boolean guessMode()
   {
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
      
      int num_views = mViewList.size();
      if(num_views > 2)
      {
         return false;
      }
      else if(num_views < 1)
      {
         return false;
      }
      else if(num_views == 1)
      {
         ConfigElement elm = (ConfigElement)mViewList.get(0);
         int view = ((Number)elm.getProperty("view", 0)).intValue();
         boolean stereo = ((Boolean) mScreenElement.getProperty("stereo", 0)).booleanValue();

         if( stereo && (3 == view))
         {
            mStereoView = elm;
            mStereoMode = ACTIVE_STEREO;
         }
         else if( stereo || (3 == view))
         {
            System.out.println("ERROR: Active stereo walls should have stereo enabled, and their view set to Stereo.");
            return false;
         }
         else if(1 == view)
         {
            mLeftView = elm;
            mStereoMode = MONO;            
         }
         else
         {
            mRightView = elm;
            mStereoMode = MONO;
         }
      }
      else if(num_views == 2)
      {
         ConfigElement elm0 = (ConfigElement)mViewList.get(0);
         ConfigElement elm1 = (ConfigElement)mViewList.get(1);
         int view0 = ((Number)elm0.getProperty("view", 0)).intValue();
         int view1 = ((Number)elm1.getProperty("view", 0)).intValue();
         boolean stereo = ((Boolean) mScreenElement.getProperty("stereo", 0)).booleanValue();

         if (stereo)
         {
            System.out.println("ERROR: Two surface viewports with same corners should not be on stereo screen.");
            return false;
         }
         else if(3 == view0 || 3 == view1)
         {
            System.out.println("ERROR: Two surface viewports with same corners should not have stereo views.");
            return false;
         }
         else if(view0 == view1)
         {
            System.out.println("ERROR: Two surface viewports with same corners should not have the same view.");
            return false;
         }
         else
         {
            mLeftView = (1 == view0 ? elm0 : elm1);
            mRightView = (2 == view0 ? elm0 : elm1);
            mStereoMode = PASSIVE_STEREO;
         }
      }

      return false;
   }
*/
      /*
      mRightView = null;
      mLeftView = null;

      if(mViewList.size() > 2)
      {
         return false;
      }
      
      for(Iterator itr = mViewList.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         int view = ((Number) elm.getProperty("view", 2)).intValue();

      }

      
      switch( view )
      {
         case 1:
            if (null != mLeftView)
            {
               System.out.println("ERROR: Multiple left eye views for the same wall.");
            }
            else
            {
               mLeftView = elm;
            }
            break;
         case 2:
            if (null != mRightView)
            {
               System.out.println("ERROR: Multiple left eye views for the same wall.");
            }
            else
            {
               mRightView = elm;
            }

            break;
         case 3:
            if (null != mStereoView)
            {
               System.out.println("ERROR: Multiple left eye views for the same wall.");
            }
            else
            {
               mStereoView = elm;
            }
            break;
      }
      mViewList.add(elm);
      if ( mLeftView != null && mRightView != null)
      {
         setStereoMode(PASSIVE_STEREO);
      }
   }
   */

   public ConfigElement getLeftView()
   {
      return mLeftView;
   }
   
   public ConfigElement getRightView()
   {
      return mRightView;
   }
   
   public ConfigElement getStereoView()
   {
      return mStereoView;
   }

   public ConfigElement getLeft()
   {
      return mLeftView;
   }

   public ConfigElement getRight()
   {
      return mRightView;
   }
   
   public ConfigElement getStereo()
   {
      return mStereoView;
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
         if (ll_corner[0] == cw.ll_corner[0] &&
             ll_corner[1] == cw.ll_corner[1] &&
             ll_corner[2] == cw.ll_corner[2] &&
             lr_corner[0] == cw.lr_corner[0] &&
             lr_corner[1] == cw.lr_corner[1] &&
             lr_corner[2] == cw.lr_corner[2] &&
             ur_corner[0] == cw.ur_corner[0] &&
             ur_corner[1] == cw.ur_corner[1] &&
             ur_corner[2] == cw.ur_corner[2] &&
             ul_corner[0] == cw.ul_corner[0] &&
             ul_corner[1] == cw.ul_corner[1] &&
             ul_corner[2] == cw.ul_corner[2])
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
         + " " + ((ConfigElement)mRightView).getName() + " " + ((ConfigElement)mStereoView).getName();

      return str;
   }
}
