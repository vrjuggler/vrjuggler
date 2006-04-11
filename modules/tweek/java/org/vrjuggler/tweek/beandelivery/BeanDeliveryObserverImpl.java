/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.beandelivery;

import java.io.File;
import java.io.IOException;
import java.io.FileOutputStream;
import java.util.*;
import org.vrjuggler.tweek.beans.BeanAttributes;
import org.vrjuggler.tweek.beans.TweekBean;
import org.vrjuggler.tweek.beans.loader.*;
import tweek.*;


public class BeanDeliveryObserverImpl extends tweek.ObserverPOA
{
   public BeanDeliveryObserverImpl(BeanDeliverySubject subject)
   {
      mSubject = subject;
   }

   public void update()
   {
      String[] bean_names = mSubject.getAllBeanNames();
      List new_beans      = new ArrayList();

      for ( int i = 0; i < bean_names.length; ++i )
      {
         if ( mKnownBeans.get(bean_names[i]) == null )
         {
            BeanInfo bean_info = mSubject.getBean(bean_names[i]);

            try
            {
               // Create a unique filename for the Bean's JAR file.
               String base_name = bean_names[i].replace(' ', '_') +
                                  mRandomGen.nextLong();
               File temp_file = File.createTempFile(base_name, ".jar");
               temp_file.deleteOnExit();

               // Write the JAR data to the temporary file.
               FileOutputStream out_stream = new FileOutputStream(temp_file);
               out_stream.write(bean_info.jarFile);
               out_stream.close();

               // Create TweekBean objects for all the Beans found in the
               // temporary file.
               TweekBean bean = createBean(temp_file, bean_info);

               if ( null != bean )
               {
                  new_beans.add(bean);
                  mKnownBeans.put(bean.getName(), bean);
               }
            }
            catch (IOException ex)
            {
               System.err.println("I/O exception occurred when writing JAR " +
                                  "byte stream: " + ex.getMessage());
            }
         }
      }

      if ( new_beans.size() > 0 )
      {
         fireBeansAddedEvent(new_beans);
      }

      try
      {
         if ( mSubject.hasActiveBean() )
         {
            // Request this only once to reduce network traffic.
            String active_bean = mSubject.getActiveBeanInfo().name;

            // If the name of the active Bean does not match the name we
            // already have, then we have a new active Bean.  In that case,
            // we fire an event to inform our listeners of that fact.
            if ( ! mActiveBean.equals(active_bean) )
            {
               mActiveBean = active_bean;
               fireActiveBeanChangeEvent(mActiveBean);
            }
         }
         else
         {
            // If we had an active Bean already, we don't have one now.  We
            // must fire an event to inform our listeners of that fact.
            if ( ! mActiveBean.equals("") )
            {
               mActiveBean = "";
               fireActiveBeanChangeEvent(mActiveBean);
            }
         }
      }
      catch (Exception e)
      {
         System.err.println("Caught exception when checking active Bean state: " + e.getMessage());
         e.printStackTrace();
      }
   }

   /**
    * Detaches this observer from its subject.
    */
   public void detach()
   {
      mSubject.detach(this._this());
   }

   /**
    * Adds a new listener to the list of classes implementing the
    * BeanDeliverySubjectUpdateListener interface.
    *
    * @see BeanDeliverySubjectUpdateListener
    */
   public synchronized void addBeanDeliverySubjectUpdateListener(BeanDeliverySubjectUpdateListener l)
   {
      mListeners.add(l);
   }

   /**
    * Removes the listener from the list of classes implementing the
    * BeanDeliverySubjectUpdateListener interface.
    *
    * @see BeanDeliverySubjectUpdateListener
    */
   public synchronized void removeBeanDeliverySubjectUpdateListener(BeanDeliverySubjectUpdateListener l)
   {
      mListeners.remove(l);
   }

   /**
    * Informs any listeners that a new Bean has been added to our collection
    * of delivered Beans.
    */
   protected void fireBeansAddedEvent(List beanList)
   {
      Vector listeners = null;

      synchronized (this)
      {
         listeners = (Vector) mListeners.clone();
      }

      BeanDeliverySubjectUpdateListener l;
      BeanDeliverySubjectUpdateEvent e =
         new BeanDeliverySubjectUpdateEvent(this, beanList);

      for ( Iterator i = listeners.iterator(); i.hasNext(); )
      {
         l = (BeanDeliverySubjectUpdateListener) i.next();
         l.beansAdded(e);
      }
   }

   /**
    * Informs any listeners that our subject has changed its "active" Bean.
    */
   protected void fireActiveBeanChangeEvent(String beanName)
   {
      Vector listeners = null;

      synchronized (this)
      {
         listeners = (Vector) mListeners.clone();
      }

      BeanDeliverySubjectUpdateListener l;
      BeanDeliverySubjectUpdateEvent e =
         new BeanDeliverySubjectUpdateEvent(this, beanName);

      for ( Iterator i = listeners.iterator(); i.hasNext(); )
      {
         l = (BeanDeliverySubjectUpdateListener) i;
         l.activeBeanChanged(e);
      }
   }

   /**
    * Helper that creates a TweekBean object given a JAR file and the
    * information to extract from that JAR file.
    *
    * @return A new TweekBean object, as instantiated from the JAR file or
    *         null if no Bean information could be found.
    */
   private TweekBean createBean(File jarFile, BeanInfo beanInfo)
   {
      TweekBean new_bean = null;

      // XXX: Maybe this could throw exceptions instead of returning null.
      // Then again, having all of this error handling code hidden within
      // this method isn't necessarily a bad thing.
      try
      {
         BeanLoader loader = new BeanLoader();

         loader.loadBeanFromJar(jarFile, new ArrayList(), "");
         Object bean_object = loader.instantiate(BeanJarClassLoader.instance(),
                                                 beanInfo.introspectorClassName);

         if ( bean_object instanceof TweekBeanIntrospector )
         {
            TweekBeanIntrospector info_god =
               (TweekBeanIntrospector) bean_object;

            // Get the Bean contained in the JAR file.
            new_bean = info_god.getBean();
            new_bean.getAttributes().setJarURL("file:" +
                                               jarFile.getAbsolutePath());
         }
         else
         {
            System.err.println("Introspector '" +
                               beanInfo.introspectorClassName +
                               "' does not implement TweekBeanIntrospector");
         }
      }
      catch (IOException io_ex)
      {
         System.out.println("Failed to load JAR file " +
                            jarFile.getAbsolutePath() + " into Bean loader: " +
                            io_ex.getMessage());
      }
      catch (BeanInstantiationException inst_ex)
      {
         System.out.println("Failed to instantiate Bean '" + 
                            beanInfo.introspectorClassName + "' from " +
                            jarFile.getAbsolutePath() + ": " +
                            inst_ex.getMessage());
      }

      return new_bean;
   }

   private BeanDeliverySubject mSubject = null;

   private Random mRandomGen  = new Random();
   private Map    mKnownBeans = new HashMap();
   private String mActiveBean = "";
   private Vector mListeners  = new Vector();
}
