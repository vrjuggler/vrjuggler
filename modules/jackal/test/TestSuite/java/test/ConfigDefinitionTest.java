package test;

import java.util.*;
import junit.framework.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;

/**
 * Test the ConfigDefinition class.
 */
 public class ConfigDefinitionTest
   extends TestCase
{
   public ConfigDefinitionTest(String name)
   {
      super(name);
   }

   public void setUp()
   {
      // Create some simple property definitions we can use for the tests
      List prop_value_defs;
      PropertyDefinition prop_def;

      // The name property
      prop_value_defs = new ArrayList();
      prop_value_defs.add(new PropertyValueDefinition("Name", ""));
      mNamePropDef = new PropertyDefinition("Name",
                                            "name",
                                            String.class,
                                            "The first name of the person.",
                                            prop_value_defs,
                                            new TreeMap(),
                                            new ArrayList(),
                                            false);

      // The age property
      prop_value_defs = new ArrayList();
      prop_value_defs.add(new PropertyValueDefinition("Age", new Integer(0)));
      mAgePropDef = new PropertyDefinition("Age",
                                           "age",
                                           Integer.class,
                                           "The age of the person.",
                                           prop_value_defs,
                                           new TreeMap(),
                                           new ArrayList(),
                                           false);

      // The friends property
      prop_value_defs = new ArrayList();
      prop_value_defs.add(new PropertyValueDefinition("Friend", null));
      mFriendsPropDef = new PropertyDefinition("Friends",
                                               "friends",
                                               ConfigElement.class,
                                               "The people that are friends of this person.",
                                               prop_value_defs,
                                               new TreeMap(),
                                               new ArrayList(),
                                               true);
   }

   private ConfigDefinition makeConfigDef()
   {
      List categories = new ArrayList();
      categories.add("VRAC");

      List prop_defs = new ArrayList();
      prop_defs.add(mNamePropDef);
      prop_defs.add(mAgePropDef);
      prop_defs.add(mFriendsPropDef);

      return new ConfigDefinition("Person",
                                  "person",
                                  1,
                                  "A person on the VR Juggler team.",
                                  categories,
                                  prop_defs);
   }

   public void testConstructor()
   {
      List categories = new ArrayList();
      categories.add("VRAC");

      List prop_defs = new ArrayList();
      prop_defs.add(mNamePropDef);
      prop_defs.add(mAgePropDef);
      prop_defs.add(mFriendsPropDef);

      ConfigDefinition def = makeConfigDef();
      assertEquals(def.getName(), "Person");
      assertEquals(def.getToken(), "person");
      assertEquals(def.getVersion(), 1);
      assertEquals(def.getHelp(), "A person on the VR Juggler team.");
      assertEquals(def.getCategories(), categories);
      assertEquals(def.getPropertyDefinitions(), prop_defs);
   }

   public void testSetName()
   {
      class Listener extends ConfigDefinitionAdapter
      {
         public void nameChanged(ConfigDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      ConfigDefinition def = makeConfigDef();
      def.addConfigDefinitionListener(l);
      def.setName("Juggler Person");
      // Make sure the name was actually changed
      assertEquals(def.getName(), "Juggler Person");

      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testSetToken()
   {
      class Listener extends ConfigDefinitionAdapter
      {
         public void tokenChanged(ConfigDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      ConfigDefinition def = makeConfigDef();
      def.addConfigDefinitionListener(l);
      def.setToken("juggler_person");
      // Make sure the token was actually changed
      assertEquals(def.getToken(), "juggler_person");

      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testSetHelp()
   {
      class Listener extends ConfigDefinitionAdapter
      {
         public void helpChanged(ConfigDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      ConfigDefinition def = makeConfigDef();
      def.addConfigDefinitionListener(l);
      def.setHelp("Help me!");
      // Make sure the help was actually changed
      assertEquals(def.getHelp(), "Help me!");

      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testAddCategory()
   {
      class Listener extends ConfigDefinitionAdapter
      {
         public void categoryAdded(ConfigDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      ConfigDefinition def = makeConfigDef();
      def.addConfigDefinitionListener(l);
      def.addCategory("Personnel");
      // Make sure the category was actually added
      assertEquals(def.getCategories().get(1), "Personnel");

      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testRemoveCategory()
   {
      class Listener extends ConfigDefinitionAdapter
      {
         public void categoryRemoved(ConfigDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();

      ConfigDefinition def = makeConfigDef();
      def.addConfigDefinitionListener(l);
      def.removeCategory("VRAC");
      // Make sure the category was actually removed
      assertEquals(def.getCategories().size(), 0);

      // Make sure an event was posted
      assertTrue(l.fired);
   }

   public void testGetPropertyDefinition()
   {
      ConfigDefinition def = makeConfigDef();

      assertEquals(def.getPropertyDefinition("name"), mNamePropDef);
      assertEquals(def.getPropertyDefinition("age"), mAgePropDef);
      assertEquals(def.getPropertyDefinition("friends"), mFriendsPropDef);

      // Test failed get
      boolean get_failed = true;
      try
      {
         // This should throw an exception
         def.getPropertyDefinition("fail");
         get_failed = false;
      }
      catch (IllegalArgumentException iae)
      {}
      assertTrue(get_failed);
   }

   public void testAddPropertyDefinition()
   {
      class Listener extends ConfigDefinitionAdapter
      {
         public void propertyDefinitionAdded(ConfigDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();
      ConfigDefinition def = makeConfigDef();
      def.addConfigDefinitionListener(l);

      // Make the new property
      List prop_value_defs = new ArrayList();
      prop_value_defs.add(new PropertyValueDefinition("Last Name", ""));
      PropertyDefinition new_property_def;
      new_property_def = new PropertyDefinition("Last Name",
                                                "last_name",
                                                String.class,
                                                "The last name of this person.",
                                                prop_value_defs,
                                                new TreeMap(),
                                                new ArrayList(),
                                                false);

      // Add in the property
      def.addPropertyDefinition(new_property_def);
      assertEquals(def.getPropertyDefinition("last_name"), new_property_def);
      assertTrue(l.fired);
      l.fired = false;

      // Test add failure by trying to add the property a second time
      boolean add_failed = true;
      try
      {
         def.addPropertyDefinition(new_property_def);
         add_failed = false;
      }
      catch (IllegalArgumentException iae)
      {}
      assertTrue(add_failed);
      assertTrue(! l.fired);
   }

   public void testRemoveProperty()
   {
      class Listener extends ConfigDefinitionAdapter
      {
         public void propertyDefinitionRemoved(ConfigDefinitionEvent evt)
         {
            fired = true;
         }

         public boolean fired = false;
      }

      Listener l = new Listener();
      ConfigDefinition def = makeConfigDef();
      def.addConfigDefinitionListener(l);

      // Remove the age property
      def.removePropertyDefinition(mAgePropDef);
      assertTrue(! def.getPropertyDefinitions().contains(mAgePropDef));
      assertTrue(l.fired);
      l.fired = false;

      // Check for failure by trying to remove the age property again
      def.removePropertyDefinition(mAgePropDef);
      assertTrue(! def.getPropertyDefinitions().contains(mAgePropDef));
      assertTrue(! l.fired);
   }

   private PropertyDefinition mNamePropDef;
   private PropertyDefinition mAgePropDef;
   private PropertyDefinition mFriendsPropDef;
}
