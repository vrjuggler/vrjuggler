package networktest;

import javax.swing.DefaultBoundedRangeModel;
import javax.swing.JSlider;
import tweek.*;


public class SliderObserverImpl extends ObserverPOA
{
   public SliderObserverImpl (JSlider _slider, SliderSubject _subject)
   {
      slider         = _slider;
      slider_subject = _subject;
   }

   public void update ()
   {
      System.out.println("Updated from our subject!");

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

   public void detach ()
   {
      slider_subject.detach(this._this());
   }

   private SliderSubject slider_subject = null;
   private JSlider       slider         = null;
}
