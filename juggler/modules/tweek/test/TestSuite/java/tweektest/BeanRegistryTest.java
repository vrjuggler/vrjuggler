package tweektest;

import junit.framework.*;
import org.vrjuggler.tweek.beans.*;


/**
 * @version 1.0
 */
public class BeanRegistryTest extends TestCase
{
   public BeanRegistryTest (String name)
   {
      super(name);
   }

   public static Test suite ()
   {
      return new TestSuite(BeanRegistryTest.class);
   }

   public void testBeanRegistration ()
   {
      BeanRegistry registry = BeanRegistry.instance();

      BeanAttributes attrs = new BeanAttributes("TestBean");
      GenericBean test_bean = new GenericBean(attrs);
      registry.registerBean(test_bean);

      TweekBean bean = registry.getBean("TestBean");
      assertTrue(test_bean == bean);
   }
}