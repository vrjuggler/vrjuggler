public class FloatObserverImpl implements tweek.ObserverPOA
{
   public FloatObserverImpl(example.FloatSubject subj)
   {
      this.mSubject = subj;
   }

   public void update()
   {
      float value = mSubject.getValue();
   }

   private example.FloatSubject mSubject;
}
