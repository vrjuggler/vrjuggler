package test;

import java.util.*;
import junit.framework.*;
import org.vrjuggler.jccl.config.*;

/**
 * Test the Category class
 */
 public class CategoryTest
   extends TestCase
{
   public CategoryTest(String name)
   {
      super(name);
   }

   public void testConstructorString()
   {
      Category cat;

      cat = new Category("/");
      assertEquals(cat.toString(), "/");

      cat = new Category("/foo");
      assertEquals(cat.toString(), "/foo/");

      cat = new Category("/foo/");
      assertEquals(cat.toString(), "/foo/");

      cat = new Category("/foo/bar");
      assertEquals(cat.toString(), "/foo/bar/");

      cat = new Category("/foo/bar/");
      assertEquals(cat.toString(), "/foo/bar/");

      cat = new Category("/foo/bar/sushi");
      assertEquals(cat.toString(), "/foo/bar/sushi/");
   }

   public void testGetParent()
   {
      Category cat;

      cat = new Category("/foo/bar/sushi");
      assertEquals(cat.getParent().toString(), "/foo/bar/");

      cat = cat.getParent();
      assertEquals(cat.getParent().toString(), "/foo/");

      cat = cat.getParent();
      assertEquals(cat.getParent().toString(), "/");

      cat = cat.getParent();
      assertNull(cat.getParent());
   }

   public void testGetName()
   {
      Category cat;

      cat = new Category("/");
      assertEquals(cat.getName(), "/");

      cat = new Category("/foo");
      assertEquals(cat.getName(), "foo");

      cat = new Category("/foo/bar");
      assertEquals(cat.getName(), "bar");

      cat = new Category("/foo/bar/sushi");
      assertEquals(cat.getName(), "sushi");
   }
}
