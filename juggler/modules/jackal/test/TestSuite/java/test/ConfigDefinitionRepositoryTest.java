package test;

import java.util.*;
import junit.framework.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;

/**
 * Test the ConfigDefinitionRepository class.
 */
 public class ConfigDefinitionRepositoryTest
   extends TestCase
{
   public ConfigDefinitionRepositoryTest(String name)
   {
      super(name);
   }

   public void setUp()
   {
   }

   private ConfigDefinition makeConfigDef(String name, int version)
   {
      return new ConfigDefinition(name,
                                  name.toLowerCase(),
                                  version,
                                  "Some Help",
                                  new ArrayList(),
                                  new ArrayList());
   }

   public void testConstructor()
   {
      ConfigDefinitionRepository repos = new ConfigDefinitionRepository();
      assertNull(repos.get("bad"));
      assertNull(repos.get("bad", 1));
   }

   public void testAdd()
   {
      ConfigDefinitionRepository repos = new ConfigDefinitionRepository();

      // Add in a single definition
      repos.add(makeConfigDef("one", 1));
      assertEquals(repos.get("one"), makeConfigDef("one", 1));
      assertEquals(repos.get("one", 1), makeConfigDef("one", 1));

      // Add in a new version of that definition
      repos.add(makeConfigDef("one", 2));
      assertEquals(repos.get("one"), makeConfigDef("one", 2));
      assertEquals(repos.get("one", 1), makeConfigDef("one", 1));
      assertEquals(repos.get("one", 2), makeConfigDef("one", 2));

      // Add in a different definition
      repos.add(makeConfigDef("two", 1));
      assertEquals(repos.get("one"), makeConfigDef("one", 2));
      assertEquals(repos.get("one", 1), makeConfigDef("one", 1));
      assertEquals(repos.get("one", 2), makeConfigDef("one", 2));
      assertEquals(repos.get("two"), makeConfigDef("two", 1));
      assertEquals(repos.get("two", 1), makeConfigDef("two", 1));
   }
}
