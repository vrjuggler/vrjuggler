package beandeliverytest;

import org.vrjuggler.tweek.beandelivery.TweekBeanIntrospector;
import org.vrjuggler.tweek.beans.BeanAttributes;
import org.vrjuggler.tweek.beans.PanelBean;
import org.vrjuggler.tweek.beans.TweekBean;


public class TestIntrospector implements TweekBeanIntrospector
{
   public TweekBean getBean()
   {
      BeanAttributes attrs = new BeanAttributes("Delivery Test Bean", "",
                                                "beandeliverytest.TestBean",
                                                null, null);
      java.util.List paths = new java.util.ArrayList();
      paths.add("/Pushed Beans");

      return new PanelBean(attrs, "", "Delivered Bean", paths);
   }
}
