/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.perfmonitor;

import java.awt.event.*;
import java.io.*;
import java.util.*;

import org.jdom.*;
import org.jdom.input.SAXBuilder;

import org.vrjuggler.jccl.config.ConfigChunk;
import org.vrjuggler.jccl.net.ProtocolInputStream;
import org.vrjuggler.jccl.vjcontrol.*;

/** Core Module for storing VR Juggler performance data.
 *
 *  @version $Revision$
 */
public class PerformanceModule extends DefaultCoreModule
{


   public List collectors;  // v of PerfDataCollector
   public int max_samples;
   public File file;
   public List performance_module_listeners;


   public PerformanceModule ()
   {
      component_name = "Performance Module";
      collectors = new ArrayList();
      max_samples = 500;
      file = new File(System.getProperty("user.dir",""), "perfdata");
      performance_module_listeners = new ArrayList();
   }



   public void setConfiguration (ConfigChunk ch) throws VjComponentException {
      component_name = ch.getName();
      component_chunk = ch;
   }


   public void initialize () throws VjComponentException {
      int i;
      List perfdatanames = new ArrayList();
      String lastfname;

      // check command-line arguments stored in Core
      String[] args = Core.getCommandLine();
      for(i = 0; i < args.length; i++)
      {
         if(args[i].startsWith ("-p"))
         {
            if(args[i].length() == 2)
               perfdatanames.add (args[++i]);
            else
               perfdatanames.add (args[i].substring(2));
         }
      }

      for(i = 0; i < perfdatanames.size(); i++)
      {
         lastfname = (String)perfdatanames.get(i);
         loadNewPerfDataFile (lastfname);
      }
   }


   public File getFile ()
   {
      return file;
   }


   public synchronized void write (DataOutputStream out) throws IOException {
      for(int i = 0; i < collectors.size(); i++)
      {
         PerfDataCollector col = (PerfDataCollector)collectors.get(i);
         col.write (out);
      }
   }


   public void removeAllData ()
   {
      synchronized (collectors)
      {
         collectors.clear();
      }
      notifyPerformanceModuleListenersRemoveAll();
   }


   public void setMaxSamples (int n)
   {
      max_samples = n;
      for(int i = 0; i < collectors.size(); i++)
         ((PerfDataCollector)collectors.get(i)).setMaxSamples(n);
   }



   public int getSize()
   {
      return collectors.size();
   }


   public PerfDataCollector getCollector (int i) throws ArrayIndexOutOfBoundsException {
      synchronized (collectors)
      {
         return(PerfDataCollector)collectors.get(i);
      }
   }



   public PerfDataCollector getCollector (String _name)
   {
      synchronized (collectors)
      {
         int i;
         PerfDataCollector p;
         for(i = 0; i < collectors.size(); i++)
         {
            p = (PerfDataCollector) collectors.get (i);
            if(p.getName().equalsIgnoreCase (_name))
               return p;
         }
         return null;
      }
   }


   public PerfDataCollector addCollector (PerfDataCollector pdc)
   {
      System.out.println ("Added collector: " + pdc.getName());
      synchronized (collectors)
      {
         collectors.add (pdc);
      }
      notifyPerformanceModuleListenersAdd (pdc);
      return pdc;
   }


   /** Read an xml-formatted stream of performance data. */
   public void readXMLStream (InputStream instream)
   {
      // new xml format for labeled buffers.
      // the xml dom may include the jccl protocol element...
      // if so just descend down into its tree.
      System.out.println ("read xml stream");

      // build the XML stream into a DOM tree
      try
      {
         SAXBuilder builder = new SAXBuilder();
         Document doc = builder.build(instream);
         interpretXMLCommands(doc.getRootElement());
      }
      catch (JDOMException ex)
      {
         ex.printStackTrace();
      }
   }


   /** Utility for readXMLStream. */
   protected void interpretXMLCommands (Element doc)
   {
      String name = doc.getName();

      //System.out.println ("interpretXMLCommands");

      if ( name.equalsIgnoreCase("jcclstream") )
      {
         Iterator i = doc.getChildren().iterator();

         while ( i.hasNext() )
         {
            interpretXMLCommands((Element) i.next());
         }
      }
      else if ( name.equalsIgnoreCase("protocol") )
      {
         // should check to verify it's jccl_protocol...
         Iterator i = doc.getChildren().iterator();

         while ( i.hasNext() )
         {
            interpretXMLCommands((Element) i.next());
         }
      }
      else if ( name.equals("labeledbuffer") )
      {
         Attribute attr = doc.getAttribute("name");
         if ( attr != null )
         {
            String buffername = attr.getValue();
            PerfDataCollector pdc = getCollector (buffername);
            if (pdc == null)
            {
               pdc = new LabeledPerfDataCollector (buffername, max_samples);
               addCollector (pdc);
            }

            if (pdc instanceof LabeledPerfDataCollector)
            {
               ((LabeledPerfDataCollector)pdc).interpretXMLData (doc);
            }
            else
            {
               System.out.println ("perfdatacollector type mismatch");
            }
         }
         else
         {
            System.out.println ("buffername wasn't specified...");
         }
      }
   }

   public String dumpAverages (int preskip, int postskip, boolean doanomaly, float anomalycutoff)
   {
      PerfDataCollector p;
      int i;
      String s = "";

      synchronized (collectors)
      {
         for(i = 0; i < collectors.size(); i++)
         {
            p = (PerfDataCollector) collectors.get (i);
            s += p.dumpAverages (preskip, postskip, doanomaly, anomalycutoff);
            s += "--------------------------------------\n";
         }
      }
      return s;
   }



   /************************ Perf File Methods ***************************/

   public String loadNewPerfDataFile (String name)
   {
      name = Core.file.mangleFileName (name);
      File f = new File(name);
      return loadNewPerfDataFile (f);
   }

   public String loadNewPerfDataFile (File f)
   {

      if(f == null)
         return null;

      Core.consoleInfoMessage (component_name,
                               "Loading Timing Data file: " + f);

      try
      {

         FileInputStream temp_in = new FileInputStream (f);
         int ch = temp_in.read();
         if(ch != '<')
         {
            Core.consoleErrorMessage (component_name, "File isn't XML.");
            return null;
         }
         else
         {
            // new format
            removeAllData();
            //readXMLStream (new FileInputStream(f));
            readXMLPerformanceFile (new FileInputStream (f));
         }

         file = f;

         return f.toString();
      }
      catch(FileNotFoundException e)
      {
         Core.consoleErrorMessage (component_name, "File not found: " + e);
         return null;
      }
      catch(IOException e)
      {
         Core.consoleErrorMessage (component_name, "Loading error: " + e);
         return null;
      }
   }


   protected String readLine (InputStream instream) throws IOException {
      StringBuffer s = new StringBuffer(64);
      char ch;
      int i;
      for(;;)
      {
         i = instream.read();
         if(i == -1)
            throw new EOFException();
         ch = (char)i;
         if(ch == '\n')
            break;
         s.append (ch);
      }
      return new String(s);
   }

   protected void readXMLPerformanceFile (InputStream instream)
   {
      ConfigChunk c;
      String id = null;
      String s;
      int j, k;

      try
      {
         while(true)
         {
            try
            {
               // need to parse the <protocol handler="foo"> line
               id = null;
               do
               {
                  s = readLine (instream);
                  System.out.println ("read stream begin: '" + s + "'");
                  j = s.indexOf ("<protocol handler=\"");
                  k = s.lastIndexOf ('"');
                  if((j != -1) && (k >= j + 19))
                     id = s.substring (j+19,k);

               } while(id == null);
               System.out.println ("protocol id name is '" + id + "'");
            }
            catch(IOException e)
            {
               if(e instanceof EOFException)
                  // Throw the EOFException here to terminate the read thread.
                  throw (EOFException)e;
               else
               {
                  Core.consoleInfoMessage (component_name, e.toString());
                  return;
               }
            }

            ProtocolInputStream in = new ProtocolInputStream (instream, "</protocol>");
            readXMLStream (in);

         }
      }
      catch(EOFException e)
      {
         // expected this, do nothing.
         ;
      }
   }



   public String savePerfDataFile (File f)
   {
      if(f == null)
         return "";

      try
      {
         DataOutputStream out = new DataOutputStream(new FileOutputStream(f));
         out.writeBytes ("VR Juggler FileConnect output gui_save\n");
         write(out);
         Core.consoleInfoMessage (component_name, "Saved PerfData file: " + f);
         file = f;
         return f.getName();
      }
      catch(IOException e)
      {
         Core.consoleErrorMessage (component_name, "IOerror saving file " + f);
         return "";
      }
   }

   //------------------ PerformanceModuleEvent Stuff ------------------------

   public void addPerformanceModuleListener (PerformanceModuleListener l)
   {
      synchronized (performance_module_listeners)
      {
         performance_module_listeners.add (l);
      }
   }

   public void removePerformanceModuleListener (PerformanceModuleListener l)
   {
      synchronized (performance_module_listeners)
      {
         performance_module_listeners.remove (l);
      }
   }


   private void notifyPerformanceModuleListenersRemoveAll ()
   {
      PerformanceModuleEvent e = new PerformanceModuleEvent (this, PerformanceModuleEvent.REMOVE_ALL, null);
      PerformanceModuleListener l;
      int i, n;
      synchronized (performance_module_listeners)
      {
         n = performance_module_listeners.size();
         for(i = 0; i < n; i++)
         {
            l = (PerformanceModuleListener)performance_module_listeners.get(i);
            l.perfDataCollectorsCleared (e);
         }
      }
   }

   private void notifyPerformanceModuleListenersAdd (PerfDataCollector p)
   {
      PerformanceModuleEvent e = new PerformanceModuleEvent (this, PerformanceModuleEvent.ADD_COLLECTOR, p);
      PerformanceModuleListener l;
      int i, n;
      synchronized (performance_module_listeners)
      {
         n = performance_module_listeners.size();
         for(i = 0; i < n; i++)
         {
            l = (PerformanceModuleListener)performance_module_listeners.get(i);
            l.perfDataCollectorAdded (e);
         }
      }
   }

   private void notifyPerformanceModuleListenersRemove (PerfDataCollector p)
   {
      PerformanceModuleEvent e = new PerformanceModuleEvent (this, PerformanceModuleEvent.REMOVE_COLLECTOR, p);
      PerformanceModuleListener l;
      int i, n;
      synchronized (performance_module_listeners)
      {
         n = performance_module_listeners.size();
         for(i = 0; i < n; i++)
         {
            l = (PerformanceModuleListener)performance_module_listeners.get(i);
            l.perfDataCollectorRemoved (e);
         }
      }
   }


}
