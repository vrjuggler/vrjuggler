package test;

import java.io.*;
import java.util.*;
import junit.framework.*;
import org.jdom.*;
import org.jdom.input.SAXBuilder;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.parser.*;

/**
 * Test the ConfigurationReader class.
 */
 public class ConfigurationReaderTest
   extends TestCase
{
   public ConfigurationReaderTest(String name)
   {
      super(name);
   }

   public void setUp()
   {
      String base_dir = System.getProperty("DATA_DIR");
      mConfigFile = new File(base_dir, "test.jconf");
   }

   public void testParseConfiguration()
   {
      try
      {
         ConfigDefinitionRepository repos = new ConfigDefinitionRepository();

         ConfigDefinitionBuilder def_builder = new ConfigDefinitionBuilder();
         List defs = def_builder.parse(new File(System.getProperty("DATA_DIR"), "person.jdef"));
         for (Iterator itr = defs.iterator(); itr.hasNext(); )
         {
            repos.add((ConfigDefinition)itr.next());
         }

         ConfigurationReader reader = new ConfigurationReader(mConfigFile, repos);
         Configuration config = reader.readConfiguration();

         assertEquals(config.getName(), "VRAC People");
//         assertEquals(config.getIncludes().get(0), "blah.jconf");
         assertEquals(config.getDefinitionPath().get(0), "${DATA_DIR}");

         ConfigElement elt = (ConfigElement)config.getElements().get(0);
         assertEquals(elt.getName(), "Ben");
         assertEquals(elt.getDefinition().getVersion(), 1);
         assertEquals(elt.getProperty("name", 0), "Ben");
         assertEquals(elt.getProperty("age", 0), new Integer(22));
         assertEquals(elt.getPropertyValueCount("friends"), 0);
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

   private File mConfigFile;
}
