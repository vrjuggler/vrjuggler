package networktest;

import javax.swing.JSlider;
import tweek.*;


public class SliderObserverImpl extends ObserverPOA
{
   public SliderObserverImpl ()
   {
   }

   public void setSliderSubject (SliderSubject _subject)
   {
      slider_subject = _subject;
      update();
   }

   public void setSlider (JSlider _slider)
   {
      slider = _slider;
   }

   public void update ()
   {
      if ( slider != null )
      {
         slider.setValue(slider_subject.getValue());
      }
   }

   private SliderSubject slider_subject = null;
   private JSlider       slider         = null;
}
