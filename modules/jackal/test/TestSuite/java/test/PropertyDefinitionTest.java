package test;

import java.util.*;
import junit.framework.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;

/**
 * Test the PropertyDefinition class.
 */
 public class PropertyDefinitionTest
   extends TestCase
{
   public PropertyDefinitionTest(String name)
   {
      super(name);
   }

   private PropertyDefinition makePropertyDef()
   {
      List prop_value_defs = new ArrayList();
      prop_value_defs.add(new PropertyValueDefinition("Age", new Integer(0)));
      return new PropertyDefinition("age",
                                    Integer.class,
                                    "The age of a person.",
                                    prop_value_defs,
                                    false,
                                    new TreeMap(),
                                    new ArrayList(),
                                    false);
   }

   public void testConstructor()
   {
      List prop_value_defs = new ArrayList();
      prop_value_defs.add(new PropertyValueDefinition("Age", new Integer(0)));

      PropertyDefinition def = makePropertyDef();
      assertEquals(def.getName(), "age");
      assertEquals(def.getToken(), "age");
      assertEquals(def.getType(), Integer.class);
      assertEquals(def.getHelp(), "The age of a person.");
      assertEquals(def.getPropertyValueDefinitions(), prop_value_defs);
      assertTrue(! def.isVariable());
   }

   public void testSetToken()
   {
      class Listener extends PropertyDefinitionAdapter
      {
         public void tokenChanged(PropertyDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      PropertyDefinition def = makePropertyDef();
      def.addPropertyDefinitionListener(l);
      def.setToken("your_age");
      // Make sure the token was actually changed
      assertEquals(def.getToken(), "your_age");
      
      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testSetType()
   {
      class Listener extends PropertyDefinitionAdapter
      {
         public void typeChanged(PropertyDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      PropertyDefinition def = makePropertyDef();
      def.addPropertyDefinitionListener(l);
      def.setType(String.class);
      // Make sure the type was actually changed
      assertEquals(def.getType(), String.class);
      
      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testSetHelp()
   {
      class Listener extends PropertyDefinitionAdapter
      {
         public void helpChanged(PropertyDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      PropertyDefinition def = makePropertyDef();
      def.addPropertyDefinitionListener(l);
      def.setHelp("Help me!");
      // Make sure the help was actually changed
      assertEquals(def.getHelp(), "Help me!");
      
      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testGetPropertyValueDefinition()
   {
      PropertyDefinition def = makePropertyDef();
      assertEquals(def.getPropertyValueDefinition(0),
                   new PropertyValueDefinition("Age", new Integer(0)));
   }

   public void testAddPropertyValueDefinition()
   {
      class Listener extends PropertyDefinitionAdapter
      {
         public void propertyValueDefinitionAdded(PropertyDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      PropertyDefinition def = makePropertyDef();
      def.addPropertyDefinitionListener(l);
      def.addPropertyValueDefinition(new PropertyValueDefinition("Other", new Integer(14)));
      // Make sure the value def was actually added
      assertEquals(def.getPropertyValueDefinition(1),
                   new PropertyValueDefinition("Other", new Integer(14)));
      
      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testRemovePropertyValueDefinition()
   {
      class Listener extends PropertyDefinitionAdapter
      {
         public void propertyValueDefinitionRemoved(PropertyDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      PropertyDefinition def = makePropertyDef();
      def.addPropertyDefinitionListener(l);
      def.removePropertyValueDefinition(0);
      // Make sure the property value definition was actually removed
      assertEquals(def.getPropertyValueDefinitionCount(), 0);
      
      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testAddEnum()
   {
      class Listener extends PropertyDefinitionAdapter
      {
         public void enumAdded(PropertyDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      PropertyDefinition def = makePropertyDef();
      def.addPropertyDefinitionListener(l);
      def.addEnum("Legal", new Integer(18));
      // Make sure the enum was actually added
      assertEquals(def.getEnums().get("Legal"), new Integer(18));

      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testRemoveEnum()
   {
      class Listener extends PropertyDefinitionAdapter
      {
         public void enumRemoved(PropertyDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      PropertyDefinition def = makePropertyDef();
      def.addPropertyDefinitionListener(l);
      def.addEnum("Legal", new Integer(18));
      def.removeEnum("Legal");
      // Make sure the enum was actually removed
      assertTrue(! def.getEnums().containsKey("Legal"));

      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testAddAllowedType()
   {
      class Listener extends PropertyDefinitionAdapter
      {
         public void allowedTypeAdded(PropertyDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      PropertyDefinition def = makePropertyDef();
      def.addPropertyDefinitionListener(l);
      def.addAllowedType("stupid");
      // Make sure the enum was actually added
      assertTrue(def.getAllowedTypes().contains("stupid"));

      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testRemoveAllowedType()
   {
      class Listener extends PropertyDefinitionAdapter
      {
         public void allowedTypeRemoved(PropertyDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      PropertyDefinition def = makePropertyDef();
      def.addPropertyDefinitionListener(l);
      def.addAllowedType("stupid");
      def.removeAllowedType("stupid");
      // Make sure the enum was actually added
      assertTrue(! def.getAllowedTypes().contains("stupid"));

      // Make sure an event was posted
      assertTrue(l.fired);
   }
   public void testSetVariable()
   {
      class Listener extends PropertyDefinitionAdapter
      {
         public void variableChanged(PropertyDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();
      PropertyDefinition def = makePropertyDef();
      def.addPropertyDefinitionListener(l);

      def.setVariable(true);
      assertTrue(def.isVariable());
      assertTrue(l.fired);
   }
}
