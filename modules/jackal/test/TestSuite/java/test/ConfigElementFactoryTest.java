package test;

import java.util.*;
import junit.framework.*;
import org.vrjuggler.jccl.config.*;

/**
 * Test the ConfigElementFactory class.
 */
 public class ConfigElementFactoryTest
   extends TestCase
{
   public ConfigElementFactoryTest(String name)
   {
      super(name);
   }

   public void setUp()
   {
      mDefs = new ArrayList();
      mDefs.add(makePersonDefinition());
      mDefs.add(makeWallDefinition());
      mDefs.add(makeCornerDefinition());
   }

   private ConfigDefinition makePersonDefinition()
   {
      List prop_value_defs;
      List prop_defs = new ArrayList();

      prop_value_defs = new ArrayList();
      prop_value_defs.add(new PropertyValueDefinition("Name", ""));
      prop_defs.add(new PropertyDefinition("name",
                                           String.class,
                                           "Your name",
                                           prop_value_defs,
                                           false,
                                           new TreeMap(),
                                           new ArrayList(),
                                           false));

      prop_value_defs = new ArrayList();
      prop_value_defs.add(new PropertyValueDefinition("Age", new Integer(0)));
      prop_defs.add(new PropertyDefinition("age",
                                           Integer.class,
                                           "Your age",
                                           prop_value_defs,
                                           false,
                                           new TreeMap(),
                                           new ArrayList(),
                                           false));

      prop_value_defs = new ArrayList();
      prop_value_defs.add(new PropertyValueDefinition("Friends"));
      prop_defs.add(new PropertyDefinition("friends",
                                           ConfigElement.class,
                                           "Your friends",
                                           prop_value_defs,
                                           false,
                                           new TreeMap(),
                                           new ArrayList(),
                                           true));

      List categories = new ArrayList();
      categories.add("VRAC");

      List parents = new ArrayList();
      return new ConfigDefinition("Person",
                                  "person",
                                  "",
                                  1,
                                  false,
                                  parents,
                                  "A person",
                                  categories,
                                  prop_defs,
                                  null);
   }

   private ConfigDefinition makeWallDefinition()
   {
      List prop_value_defs;
      List prop_defs = new ArrayList();
      List allowed_types;

      prop_value_defs = new ArrayList();
      prop_value_defs.add(new PropertyValueDefinition("Lower Left Corner"));
      prop_value_defs.add(new PropertyValueDefinition("Lower Right Corner"));
      prop_value_defs.add(new PropertyValueDefinition("Upper Right Corner"));
      prop_value_defs.add(new PropertyValueDefinition("Upper Left Corner"));
      allowed_types = new ArrayList();
      allowed_types.add("corner");
      prop_defs.add(new PropertyDefinition("corners",
                                           ConfigElement.class,
                                           "The corners that make up the wall.",
                                           prop_value_defs,
                                           false,
                                           new TreeMap(),
                                           allowed_types,
                                           false));

      List categories = new ArrayList();
      categories.add("VR");

      List parents = new ArrayList();
      return new ConfigDefinition("Wall",
                                  "wall",
                                  "",
                                  1,
                                  false,
                                  parents,
                                  "A wall in 3D space.",
                                  categories,
                                  prop_defs,
                                  null);
   }

   private ConfigDefinition makeCornerDefinition()
   {
      List prop_value_defs;
      List prop_defs = new ArrayList();

      prop_value_defs = new ArrayList();
      prop_value_defs.add(new PropertyValueDefinition("x", new Float(0)));
      prop_value_defs.add(new PropertyValueDefinition("y", new Float(0)));
      prop_value_defs.add(new PropertyValueDefinition("z", new Float(0)));
      prop_defs.add(new PropertyDefinition("position",
                                           Float.class,
                                           "The position of the corner in 3D space.",
                                           prop_value_defs,
                                           false,
                                           new TreeMap(),
                                           new ArrayList(),
                                           false));

      List categories = new ArrayList();
      categories.add("VR");

      List parents = new ArrayList();
      return new ConfigDefinition("Corner",
                                  "corner",
                                  "",
                                  1,
                                  false,
                                  parents,
                                  "A corner of a wall in 3D space.",
                                  categories,
                                  prop_defs,
                                  null);
   }

   public void testCreateNonRecursive()
   {
      ConfigElementFactory factory = new ConfigElementFactory(mDefs);
      ConfigElement e = factory.create("Aron", "person");

      assertEquals(e.getName(), "Aron");
      assertEquals(e.getDefinition(), mDefs.get(0));
      assertEquals(e.getProperty("name", 0), "");
      assertEquals(e.getProperty("age", 0), new Integer(0));
      assertEquals(e.getPropertyValueCount("friends"), 0);
   }

   public void testCreateRecursive()
   {
      ConfigElementFactory factory = new ConfigElementFactory(mDefs);
      ConfigElement e = factory.create("My Wall", "wall");

      // Wall
      assertEquals(e.getName(), "My Wall");
      assertEquals(e.getDefinition(), mDefs.get(1));
      assertEquals(e.getPropertyValueCount("corners"), 4);

      ConfigElement corner;

      // Lower left corner
      corner = (ConfigElement)e.getProperty("corners", 0);
      assertEquals(corner.getName(), "corners 0");
      assertEquals(corner.getDefinition(), mDefs.get(2));
      assertEquals(corner.getProperty("position", 0), new Float(0.0f));
      assertEquals(corner.getProperty("position", 1), new Float(0.0f));
      assertEquals(corner.getProperty("position", 2), new Float(0.0f));

      // Lower right corner
      corner = (ConfigElement)e.getProperty("corners", 1);
      assertEquals(corner.getName(), "corners 1");
      assertEquals(corner.getDefinition(), mDefs.get(2));
      assertEquals(corner.getProperty("position", 0), new Float(0.0f));
      assertEquals(corner.getProperty("position", 1), new Float(0.0f));
      assertEquals(corner.getProperty("position", 2), new Float(0.0f));

      // Upper right corner
      corner = (ConfigElement)e.getProperty("corners", 2);
      assertEquals(corner.getName(), "corners 2");
      assertEquals(corner.getDefinition(), mDefs.get(2));
      assertEquals(corner.getProperty("position", 0), new Float(0.0f));
      assertEquals(corner.getProperty("position", 1), new Float(0.0f));
      assertEquals(corner.getProperty("position", 2), new Float(0.0f));

      // Upper left corner
      corner = (ConfigElement)e.getProperty("corners", 3);
      assertEquals(corner.getName(), "corners 3");
      assertEquals(corner.getDefinition(), mDefs.get(2));
      assertEquals(corner.getProperty("position", 0), new Float(0.0f));
      assertEquals(corner.getProperty("position", 1), new Float(0.0f));
      assertEquals(corner.getProperty("position", 2), new Float(0.0f));
   }

   List mDefs;
}
