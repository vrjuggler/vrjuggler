package org.vrjuggler.tweek.net.corba;

import java.util.Properties;
import org.omg.CORBA.*;
import org.omg.CosNaming.*;


public class CorbaService
{
   public CorbaService (String init_host, int init_port)
   {
      initHost = init_host;
      initPort = init_port;
   }

   public void init (String[] args) throws SystemException
   {
      Properties props = new Properties();
      props.put("org.omg.CORBA.ORBInitialHost", initHost);
      props.put("org.omg.CORBA.ORBInitialPort", String.valueOf(initPort));

      m_orb = ORB.init(args, props);

      org.omg.CORBA.Object init_ref = null;

      try
      {
         init_ref      = m_orb.resolve_initial_references("NameService");
         namingContext = NamingContextHelper.narrow(init_ref);
      }
      catch (UserException user_ex)
      {
         user_ex.printStackTrace();
      }
   }

   public String getInitHost ()
   {
      return initHost;
   }

   public int getInitPort ()
   {
      return initPort;
   }

   public org.omg.CORBA.Object getReference (String id, String kind)
      throws SystemException, UserException
   {
      NameComponent[] name_comp = new NameComponent[1];

      name_comp[0] = new NameComponent(id, kind);

      org.omg.CORBA.Object ref = null;

      ref = namingContext.resolve(name_comp);

      return ref;
   }

   private String initHost = null;
   private int    initPort = 0;

   private ORB           m_orb         = null;
   private NamingContext namingContext = null;
}
