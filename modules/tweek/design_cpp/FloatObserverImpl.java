public class FloatObserverImpl implements tweek.ObserverPOA
{
   public FloatObserverImpl(example.FloatObserverSubject subj)
   {
      this.mSubject = subj;
   }

   public void update()
   {
      float value = mSubject.getValue();
   }

   private example.FloatObserverSubject mSubject;
}
