package test;

import java.io.*;
import java.util.*;
import junit.framework.*;
import org.jdom.*;
import org.jdom.input.SAXBuilder;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.io.*;

/**
 * Test the ConfigDefinitionReader class.
 */
 public class ConfigDefinitionReaderTest
   extends TestCase
{
   public ConfigDefinitionReaderTest(String name)
   {
      super(name);
   }

   public void setUp()
   {
      String base_dir = System.getProperty("DATA_DIR");
      mPersonDefFile = new File(base_dir, "person.jdef");
   }

   public void testParseSingleVersion()
   {
      try
      {
         ConfigDefinitionReader reader = new ConfigDefinitionReader(mPersonDefFile);
         List defs = reader.readDefinition();

         assertTrue(defs.size() == 1);
         PropertyDefinition prop_def;

         // Person element
         ConfigDefinition def = (ConfigDefinition)defs.get(0);
         assertEquals(def.getName(), "Person");
         assertEquals(def.getToken(), "person");
         assertEquals(def.getVersion(), 1);
         assertEquals(def.getHelp(), "This is a person.");
         assertEquals(def.getCategories().get(0), new Category("/Personnel"));
         // Name property
         prop_def = (PropertyDefinition)def.getPropertyDefinition("name");
         assertEquals(prop_def.getName(), "name");
         assertEquals(prop_def.getToken(), "name");
         assertEquals(prop_def.getType(), String.class);
         assertTrue(!prop_def.isVariable());
         assertEquals(prop_def.getHelp(), "This person's first name.");
         assertEquals(prop_def.getPropertyValueDefinitions().get(0),
                      new PropertyValueDefinition("Name", ""));
         // Age property
         prop_def = (PropertyDefinition)def.getPropertyDefinition("age");
         assertEquals(prop_def.getName(), "age");
         assertEquals(prop_def.getToken(), "age");
         assertEquals(prop_def.getType(), Integer.class);
         assertTrue(!prop_def.isVariable());
         assertEquals(prop_def.getHelp(), "This person's age.");
         assertEquals(prop_def.getPropertyValueDefinitions().get(0),
                      new PropertyValueDefinition("Age", new Integer(0)));
         // Friends property
         prop_def = (PropertyDefinition)def.getPropertyDefinition("friends");
         assertEquals(prop_def.getName(), "friends");
         assertEquals(prop_def.getToken(), "friends");
         assertEquals(prop_def.getType(), ConfigElement.class);
         assertTrue(prop_def.isVariable());
         assertEquals(prop_def.getHelp(), "All of this person's friends.");
         assertEquals(prop_def.getPropertyValueDefinitions().get(0),
                      new PropertyValueDefinition("Friend"));
         assertEquals(prop_def.getAllowedTypes().get(0), "person");
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
         fail(ioe.getMessage());
      }
      catch (ParseException pe)
      {
         pe.printStackTrace();
         fail(pe.getMessage());
      }
   }

   private File mPersonDefFile;
}
