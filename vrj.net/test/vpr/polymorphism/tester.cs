using System;
using System.Runtime.InteropServices;

public class MyGuid : vpr.GUID
{
   public override vpr.ReturnStatus writeObject(vpr.ObjectWriter w)
   {
      Console.WriteLine("[C#] MyGuid.writeObject(): Success!");
      return base.writeObject(w);
   }
}

public class Tester
{
   [DllImport("handle_obj")]
   private static extern void handleSerializableObject(
      [MarshalAs(UnmanagedType.CustomMarshaler,
                 MarshalTypeRef = typeof(vpr.SerializableObjectMarshaler))] vpr.SerializableObject obj);

   public static void Main()
   {
      Console.WriteLine("[C#] Starting");
      vpr.GUID g = new vpr.GUID();
      Console.WriteLine("[C#] vpr.GUID created");
      Console.WriteLine("[C#] g before vpr.GUID.generate(): " + g);
      g.generate();
      Console.WriteLine("[C#] g after vpr.GUID.generate(): " + g);

      const string guid_str = "a1ede580-8186-4453-86b8-108ecd421da6";

      // Test string marshaling.
      vpr.GUID g2 = new vpr.GUID(guid_str);
      Console.WriteLine("[C#] g2: " + g2);
      handleSerializableObject(g2);
      Console.WriteLine("[C#] After handleSerialiazableObject() call");

      vpr.GUID g3 = new vpr.GUID(guid_str);
      if ( ! g2.Equals(g3) )
      {
         throw new Exception("[C#] vpr.GUID equality test failed");
      }
   }
}
