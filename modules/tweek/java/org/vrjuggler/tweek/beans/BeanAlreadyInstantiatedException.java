package org.vrjuggler.tweek.beans;


/**
 * @since 1.0
 */
public class BeanAlreadyInstantiatedException extends Exception
{
   public BeanAlreadyInstantiatedException (String msg)
   {
      super(msg);
   }
}