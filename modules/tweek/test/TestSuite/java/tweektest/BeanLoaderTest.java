package tweektest;

import junit.framework.*;
import org.vrjuggler.tweek.beans.*;


public class BeanLoaderTest extends TestCase
{
   public BeanLoaderTest (String name)
   {
      super(name);
   }

   public static Test suite ()
   {
      return new TestSuite(BeanLoaderTest.class);
   }

   public void setUp ()
   {
      BeanRegistry registry = BeanRegistry.instance();
      mRegListener.setBeanCount(0);
      registry.addBeanRegistrationListener(mRegListener);
   }

   public void tearDown ()
   {
      BeanRegistry registry = BeanRegistry.instance();
      registry.removeBeanRegistrationListener(mRegListener);
   }

   public void testBeanFinder ()
   {
      XMLBeanFinder finder = new XMLBeanFinder();

      try
      {
         java.util.List beans = finder.find(".");
         assertTrue(beans.size() > 0);
      }
      catch (org.vrjuggler.tweek.beans.BeanPathException e)
      {
         assertTrue(false);
      }
   }

   public void testBeanRegistration ()
   {
      BeanRegistry registry = BeanRegistry.instance();
      XMLBeanFinder finder  = new XMLBeanFinder();

      try
      {
         java.util.List beans = finder.find(".");

         for ( java.util.Iterator itr = beans.iterator(); itr.hasNext(); )
         {
            TweekBean bean = (TweekBean) itr.next();
            registry.registerBean(bean);
         }

         assertTrue(beans.size() == mRegListener.getBeanCount());
      }
      catch (org.vrjuggler.tweek.beans.BeanPathException e)
      {
         assertTrue(false);
      }
   }

   private class RegListener implements BeanRegistrationListener
   {
      public void beanRegistered (BeanRegistrationEvent regEvent)
      {
         assertTrue(regEvent.getBean() != null);
         beanCount++;
      }

      public void setBeanCount (int count)
      {
         beanCount = count;
      }

      public int getBeanCount ()
      {
         return beanCount;
      }

      int beanCount = 0;
   }

   RegListener mRegListener = new RegListener();
}