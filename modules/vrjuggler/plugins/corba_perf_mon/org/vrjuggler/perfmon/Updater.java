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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.perfmon;

import javax.swing.*;
import java.lang.reflect.*;
import org.jfree.data.time.*;
import vrj.PerformanceMonitorSubjectPackage.SampleTimeMap;
import vrj.*;

public class Updater implements Runnable
{
   TimeSeriesCollection mTimeSeriesCollection;
   double mLastValue = 100.0;
   SpinnerModel mSpinnerModel = null;
   private PerformanceMonitorObserverImpl mObserver;


   public Updater ( TimeSeriesCollection series, SpinnerModel model,
                    PerformanceMonitorObserverImpl observer )
   {
      //Will get multiple time series based on the list that comes back
      //from c++ side
      mTimeSeriesCollection = series;
      mSpinnerModel = model;
      mObserver = observer;
   }

   public void run ()
   {
      if ( mObserver == null )
      {
         System.out.println("mObserver in Performance Monitoring is null!");
      }
      SampleTimeMap[] value_map = mObserver.getValueMap();
      int length = Array.getLength(value_map);
      System.out.println("Received a samples list of length " + length);
      float initialValues[] = new float[length];
      float lastValues[] = new float[length];
      float thisSample = 0.0f;

      for ( int i = 1; i < length; ++i )
      {
         System.out.println("Adding a new series for " + value_map[i].mName);
         TimeSeries s = new TimeSeries(value_map[i].mName, Millisecond.class);
         mTimeSeriesCollection.addSeries(s);
         initialValues[i] = value_map[i].mSampleTime;
         lastValues[i] = 1234.56f;
      }
      while ( true )
      {
         try
         {
            Thread.sleep( ( ( Integer ) mSpinnerModel.getValue() ).longValue() );
         }
         catch ( InterruptedException ex )
         {
            ex.printStackTrace();
         }

         value_map = mObserver.getValueMap();
         length = Array.getLength(value_map);

         // subtract one off length to account for taking out root node
         for ( int i = 0; i < length-1; ++i )
         {
            thisSample = value_map[i].mSampleTime;
            if(thisSample != initialValues[i] && thisSample != lastValues[i])
            {
              mTimeSeriesCollection.getSeries( i ).add( new Millisecond(), thisSample );
            }
            lastValues[i] = thisSample;
         }

         Thread.yield();
      }
   }
}
