package org.vrjuggler.perfmon;

import javax.swing.*;
import java.lang.reflect.*;
import org.jfree.data.time.*;
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
    try 
    {
      /*
       *
      // create and initialize the ORB
      ORB mOrb = ORB.init();

      // get the root naming context
      org.omg.CORBA.Object mObjRef =
        mOrb.resolve_initial_references("Performance Monitor");

      // Use NamingContextExt instead of NamingContext. This is
      // part of the Interoperable Naming Service.
      NamingContextExt mRef =
        NamingContextExtHelper.narrow(mObjRef);
       */
    }
    catch (Exception e) 
    {
       System.out.println("ERROR : " + e) ;
       e.printStackTrace(System.out);
    }

  }

  public void run ()
  {
    SampleTimeMap value_map = mObserver.getValueMap();
    int length = Array.getLength(value_map.mNames);
    System.out.println("[DBG] Got a samples list of length " + length);
    for (int i = 0; i < length; ++i)
    {
      System.out.println("[DBG] Adding a new series for " + value_map.mNames[i]);
      TimeSeries s = new TimeSeries(value_map.mNames[i]);
      mTimeSeriesCollection.addSeries(s);
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
      for (int i = 0; i < length; ++i)
      {
         mTimeSeriesCollection.getSeries(i).add( new Millisecond(), value_map.mSampleTimes[i] );
         System.out.println("[DBG] Setting value for series " + i);
      }
      
      Thread.yield();
    }
  }
}
