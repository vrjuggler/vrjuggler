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

package org.vrjuggler.perfmon;

import javax.swing.DefaultBoundedRangeModel;
import javax.swing.JSlider;

import tweek.ObserverPOA;
import vrj.PerformanceMonitorSubjectPackage.SampleTimeMap;
import vrj.*;


/**
 * Implementation of the Observer side of the Tweek Subject/Observer pattern.
 * It must extend tweek.ObserverPOA so that instances of this class can be
 * registered as CORBA servants.  In addition, CORBA references to the
 * servants must be capable of being attached to remote subjects.
 */
public class PerformanceMonitorObserverImpl extends ObserverPOA
{
   public PerformanceMonitorObserverImpl(PerformanceMonitorSubject subject)
   {
      mPerformanceMonitorSubject = subject;
   }

   /**
    * Implements the required method in tweek.ObserverPOA.  The remote subject
    * will invoke this method whenever it is notified of a change.
    */
   public void update ()
   {
      System.out.println("Received update notification from our subject!");
   }

   /**
    * Detaches this observer from our subject.  This is needed when shutting
    * down a CORBA connection.
    */
   public void detach ()
   {
      mPerformanceMonitorSubject.detach(this._this());
   }

   public String[] getAllSections()
   {
      try
      {
         return mPerformanceMonitorSubject.getAllSections();
      }
      catch ( org.omg.CORBA.COMM_FAILURE comm_ex )
      {
         System.out.println (comm_ex);
         comm_ex.printStackTrace();
      }
      return null;
   }

   // TODO: Implement lookup
   public float getValue(float param)
   {
      try
      {
         return mPerformanceMonitorSubject.getValue(param);
      }
      catch ( org.omg.CORBA.COMM_FAILURE comm_ex )
      {
         System.out.println (comm_ex);
         comm_ex.printStackTrace();
      }
      return 0.0f;
   }

   public SampleTimeMap[] getValueMap()
   {
      try
      {
         if ( mPerformanceMonitorSubject == null )
         {
            System.out.println("PerfMonSubject nullified");
         }

         SampleTimeMap[] n = mPerformanceMonitorSubject.getValueMap();

         return n;
      }
      catch ( org.omg.CORBA.COMM_FAILURE comm_ex )
      {
         System.out.println(comm_ex);
         comm_ex.printStackTrace();
      }
      SampleTimeMap[] j = null;
      return j;
   }
   private PerformanceMonitorSubject mPerformanceMonitorSubject = null;
}
