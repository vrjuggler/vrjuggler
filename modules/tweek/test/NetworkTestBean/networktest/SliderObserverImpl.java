package networktest;

import javax.swing.DefaultBoundedRangeModel;
import javax.swing.JSlider;
import tweek.*;


/**
 * Implementation of the Observer side of the Tweek Subject/Observer pattern.
 * It must extend tweek.ObserverPOA so that instances of this class can be
 * registered as CORBA servants.  In addition, CORBA references to the
 * servants must be capable of being attached to remote subjects.
 */
public class SliderObserverImpl extends ObserverPOA
{
   public SliderObserverImpl (JSlider _slider, SliderSubject _subject)
   {
      slider         = _slider;
      slider_subject = _subject;
   }

   /**
    * Implements the required method in tweek.ObserverPOA.  The remote subject
    * will invoke this method whenever it is notified of a change.
    */
   public void update ()
   {
      System.out.println("Updated from our subject!");

      // If we have a valid slider object, we need to update its value to
      // whatever our subject has.
      if ( slider != null )
      {
         DefaultBoundedRangeModel model = (DefaultBoundedRangeModel) slider.getModel();
         System.out.println("Got slider model, setting value");
         model.setValue(slider_subject.getValue());
         System.out.println("Set value, repainting slider");
         slider.repaint();
      }

      System.out.println("Update processed");
   }

   /**
    * Detaches this observer from our subject.  This is needed when shutting
    * down a CORBA connection.
    */
   public void detach ()
   {
      slider_subject.detach(this._this());
   }

   private SliderSubject slider_subject = null;
   private JSlider       slider         = null;
}
