package org.vrjuggler.perfmon;

import javax.swing.*;

import org.jfree.data.time.*;


public class Updater implements Runnable
{
  TimeSeries mTimeSeries;
  double mLastValue = 100.0;
  SpinnerModel mSpinnerModel = null;



  public Updater ( TimeSeries series, SpinnerModel model )
  {
    mTimeSeries = series;
    mSpinnerModel = model;
    try {
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
    catch (Exception e) {
        System.out.println("ERROR : " + e) ;
        e.printStackTrace(System.out);
    }

  }

  public void run ()
  {
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

      double factor = 0.90 + 0.2 * Math.random();
      this.mLastValue = this.mLastValue * factor;
      Millisecond now = new Millisecond();
      //System.out.println( "Now = " + now.toString() + " Value: " + mLastValue);
      mTimeSeries.add( new Millisecond(), this.mLastValue );

      Thread.yield();
    }
  }
}
