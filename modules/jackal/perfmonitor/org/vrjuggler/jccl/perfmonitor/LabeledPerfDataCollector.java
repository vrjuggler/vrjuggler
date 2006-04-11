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

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.vjcontrol.Core;

/** Class for storing performance data from a single source.
 *  A source is, for example, a jccl::PerfDataBuffer.
 *
 *  PerfDataCollector is a source for ActionEvents, with the following
 *  ActionCommands:
 *      "Update" - when data is appended to the Collector.
 *      "Clear" - when the Collector clears out all its data.
 */
public class LabeledPerfDataCollector implements PerfDataCollector
{

   protected class DataElem
   {
      public double stamp;
      public double delta;
      public IndexInfo index_info;

      public DataElem (IndexInfo _index_info, double _stamp, double _delta)
      {
         index_info = _index_info;
         stamp = _stamp;
         delta = _delta;
      }
   }


   /** Stores a single frame's worth of DataElems. */
   protected class DataLine
   {
      List samples;  // list of DataElems
      boolean cutoff;     // true if there was lost data at end of line.
      double total;

      public DataLine ()
      {
         samples = new ArrayList();
         cutoff = false;
         total = 0.0;
      }

      public void addSample (IndexInfo index_info, double stamp, double delta)
      {
         samples.add (new DataElem (index_info, stamp, delta));
         total += delta;
      }

      public void setCutoff (boolean _cutoff)
      {
         cutoff = _cutoff;
      }

      public Iterator iterator ()
      {
         return samples.iterator();
      }
   }


   /** Stores information about a particular label. */
   protected class IndexInfo
   {
      // summary of info for a given index string
      int num_samples;
      double total_times;
      public String category;
      public String index;
      protected double max_time;
      List label_components;

      public IndexInfo (String _category, String _index)
      {
         category = _category;
         index = _index;
         num_samples = 0;
         total_times = 0.0;
         max_time = 0.0;
         label_components = new ArrayList();

         // want to break the index string into subparts based on
         // a separator character...
         char separator = '/';
         char escape = '\\';
         int n = index.length();
         int startpos = 0;
         int currentpos = 0;
         boolean escaped = false;
         StringBuffer substring = new StringBuffer();
         while (currentpos < n)
         {
            char ch = index.charAt(currentpos);
            if (escaped)
            {
               escaped = false;
               currentpos++;
            }
            else
            {
               if (ch == escape)
               {
                  escaped = true;
                  substring.append (index.substring(startpos,currentpos));
                  currentpos++;
                  startpos = currentpos;
               }
               else if (ch == separator)
               {
                  substring.append (index.substring(startpos, currentpos));
                  currentpos++;
                  startpos = currentpos;
                  label_components.add (new String (substring));
                  substring = new StringBuffer();
               }
               else
               {
                  currentpos++;
               }
            }
         }
         // and add in the last part of the string
         substring.append (index.substring (startpos, currentpos));
         label_components.add (new String (substring));

         System.out.println ("Created IndexInfo for: " + label_components);
      }


      public void addSample (double sample_time)
      {
         num_samples++;
         total_times = total_times + sample_time;
         if (sample_time > max_time)
            max_time = sample_time;
         if (getAverage() < 0.0)
         {
            System.out.println ("index " + index + " went negative adding sample " + sample_time);
            System.exit(1);
         }
      }


      public void removeSample (double sample_time)
      {
         num_samples--;
         total_times = total_times - sample_time;
         if (getAverage() < 0.0)
         {
            System.out.println ("index " + index + " went negative removing sample " + sample_time);
            System.exit(1);
         }
      }


      public double getAverage ()
      {
         return total_times/num_samples;
      }


      public double getMaximum ()
      {
         return max_time;
      }
   }


   // stores all performance data coming from a particular buffer
   public String name;
   public List datalines; // list of DataLines
   public int num;        // # of unique indices - useful for graphing

   HashMap index_info;          // for each unique index string, store:
   // total # of values, total of values
   // so that we can create averages, etc.
   List ordered_index_info;

   public double totalsum;
   public int totalsamps;
   DataLine current_dl = null;
   int maxdatalines;
   protected List action_listeners;
   double last_stamp; // last recorded sample, for finding deltas.
   boolean last_stamp_set;


   public LabeledPerfDataCollector(String _name, int maxsamples)
   {
      datalines = new LinkedList();
      action_listeners = new ArrayList();
      index_info = new HashMap();
      ordered_index_info = new ArrayList();
      name = _name;
      totalsum = 0.0;
      totalsamps = 0;
      last_stamp = 0.0;
      current_dl = new DataLine ();
      maxdatalines = maxsamples;
      System.out.println ("creating PerfDataCollector " + _name + ".");
      last_stamp_set = false;

   }


   public Iterator indexIterator()
   {
      return ordered_index_info.iterator();
   }


   public void write (DataOutputStream out) throws IOException
   {
//  	out.writeBytes ("PerfData1 \"" + name + "\" " + num + "\n");
//  	ListIterator li = datalines.listIterator(0);
//  	while (li.hasNext()) {
//  	    DataLine dl = (DataLine)li.next();
//  	    int i = 0;
//  	    while ((i < num) && Double.isNaN(dl.vals[i]))
//  		i++;
//  	    while ((i < num) && !Double.isNaN(dl.vals[i])) {
//  		if (dl.vals[i] > 0.0f)
//  		    out.writeBytes (" " + i + "  " + dl.vals[i] + "\n");
//  		i++;
//  	    }
//  	    if (i != num) {
//  		out.writeBytes ("-1 " + dl.numlost + "\n");
//  		out.writeBytes ("PerfData1 \"" + name + "\" " + num + "\n");
//  	    }
//  	}
//  	out.writeBytes ("-1 0\n");
   }


   /** Set the maximum number of samples to store.
    *  When new samples are added, self will remove the oldest samples
    *  to make room.
    *  @param n Maximum number of samples to allow.  If n = -1, no
    *           purging will be done and the collector will add new
    *           samples so long as memory is available.
    */
   public void setMaxSamples (int n)
   {
      maxdatalines = n;
      System.out.println ("collector: max lines set to " + maxdatalines);
   }


   public int getNumIndices()
   {
      return index_info.size();
   }


   public String getName()
   {
      return name;
   }


   private void addDataLine (DataLine new_dl)
   {
      Iterator i;
      DataLine dl;

      if (maxdatalines >= 0)
      {
         while (datalines.size() > maxdatalines)
         {
            dl = (DataLine)datalines.remove(0);
            i = dl.iterator();
            while (i.hasNext())
            {
               DataElem de = (DataElem)i.next();
               de.index_info.removeSample (de.delta);
            }
         }
      }

      datalines.add(new_dl);
   }



   /** Utility method for interpretXMLData. */
   protected void addDataElem (String index, String category, double stamp)
   {
      //System.out.println ("addDataElem called");
      if (index.endsWith ("startframe"))
      {
         // start a new data line...
         addDataLine (current_dl);
         current_dl = new DataLine ();
      }
      IndexInfo ii = (IndexInfo)index_info.get(index);
      if (ii == null)
      {
         //System.out.println ("adding indexinfo for " + index);
         ii = new IndexInfo(category, index);
         index_info.put (index, ii);
         ordered_index_info.add (ii);
      }
      if (!last_stamp_set)
      {
         last_stamp = stamp;
         last_stamp_set = true;
      }
      ii.addSample (stamp - last_stamp);
      current_dl.addSample (ii, stamp, stamp - last_stamp);

      last_stamp = stamp;
   }


   /** doc should be the element node for this labeledbuffer... we just
    *  extract all the subelements from it...
    */
   public void interpretXMLData (Element doc)
   {
      String name = doc.getName();

      if (name.equalsIgnoreCase ("labeledbuffer"))
      {
         // should check to verify it's got the right name
         Iterator i = doc.getChildren().iterator();

         while ( i.hasNext() )
         {
            interpretXMLData((Element) i.next());
         }
      }
      else if (name.equalsIgnoreCase ("stamp"))
      {
         String label = "";
         String category = "";
         double time = 0.0;
         Attribute attr;

         attr = doc.getAttribute("label");
         if ( attr != null )
         {
            label = attr.getValue();
         }

         attr = doc.getAttribute("category");
         if ( attr != null )
         {
            category = attr.getValue();
         }

         attr = doc.getAttribute("time");
         if ( attr != null )
         {
            try
            {
               time = attr.getFloatValue();
            }
            catch (DataConversionException e)
            {
               System.err.println("WARNING: Failed to convert '" +
                                  attr.getValue() + "' to float");
            }
         }

         if (!label.equals(""))
         {
            addDataElem(label, category, time);
         }
      }
      else
      {
         // shouldn't be anything else, eh?
      }

      notifyActionListenersUpdate();
   }



   /** Recalculate the 'max' value stored for each label.
    *  (The max value can become outdated when old DataLines are
    *  purged from the collector to reduce memory usage).
    */
   public void refreshMaxValues ()
   {
      IndexInfo ii;
      DataLine dl;
      DataElem de;
      Iterator ii_it, dl_it, de_it;

      // zero out old values.
      ii_it = indexIterator();
      while (ii_it.hasNext())
      {
         ii = (IndexInfo)ii_it.next();
         ii.max_time = 0.0;
      }

      // calc new value.
      dl_it = datalines.iterator();
      while (dl_it.hasNext())
      {
         dl = (DataLine)dl_it.next();
         de_it = dl.iterator();
         while (de_it.hasNext())
         {
            de = (DataElem)de_it.next();
            if (de.index_info.max_time < de.delta)
            {
               de.index_info.max_time = de.delta;
            }
         }
      }
   }


//     public double getMaxValue ()
//     {
//  //  	double maxval = 0.0;
//  //  	for (int i = 0; i < num; i++)
//  //  	    maxval += maxvals[i];
//  //  	System.out.println ("alt generation method show " + maxval);
//  //  	return maxlinetotal;
//        return 0.0;
//     }


   public String toString ()
   {
      return name;
   }


   public String prebuff (double d, int b)
   {
      String s = Double.toString(d);
      while (s.length() < b)
      {
         s = " " + s;
      }
      return s;
   }


   public String dumpData()
   {
      String s = name + "\n";

//  	DataLine d;
//  	ListIterator li;
//  	int i,j;
//  	for (j = 0; j < num; j++)
//  	    s = s + prebuff (j, (j==0)?9:18);
//  	s += "               Total\n";
//  	//for (i = 0; i < datalines.size(); i++) {
//  	li = datalines.listIterator(0);
//  	while (li.hasNext()) {
//  	    d = (DataLine)li.next();
//  	    //d = (DataLine)datalines.elementAt(i);
//  	    for (j = 0; j < num; j++) {
//  		s = s + prebuff(d.diffs[j],(j==0)?9:18);
//  	    }
//  	    if (d.numlost > 0)
//  		s = s + prebuff(d.linetotal,18) + "\t(lost " + d.numlost + ")\n";
//  	    else
//  		s += "\n";
//  	}

      return s;
   }


   private String padFloat (double f)
   {
      // reformats f to a string w/ 7 spaces before decimal, and
      // 3 after...
      String s = Double.toString(f);
      int i = s.lastIndexOf('.');
      if ((i >= 0) && (i +3 < s.length()))
      {
         s = s.substring (0, i + 3);
      }
      while (s.length() < 11)
      {
         s = ' ' + s;
      }
      return s;
   }


   public void generateAverages (int preskip, int postskip)
   {

   }


   public String dumpAverages (int preskip, int postskip, boolean doanomaly, double cutoff)
   {
      return "";
//  	int i;
//    	Property labelsprop = null;
//    	String label;

//          // kludgey - make sure we have latest perfdata labels chunk
//          List infochunks = Core.vjcontrol_chunkdb.getOfDescToken ("PerfData");
//          ConfigChunk ch;
//          for (int j = 0; j < infochunks.size(); j++) {
//              ch = (ConfigChunk)infochunks.get(j);
//              if (name.startsWith (ch.getName())) {
//                  infochunk = ch;
//                  break;
//              }
//          }

//          if (infochunk != null)
//              labelsprop = infochunk.getProperty ("labels");

//  	String s = name + ":  averages Report per Cycle\n";
//  	double avg;
//  	for (i = 0; i < num; i++) {
//  	    avg = (sums[i]/numsamps[i]);
//  	    if (avg == 0.0)
//  		continue;
//  	    label = "";
//  	    if (labelsprop != null) {
//  		if ( i < labelsprop.getNum())
//  		    label = labelsprop.getValue(i).getString();
//  	    }
//  	    s += "  part " + i + ":  " + padFloat(avg) + " us\t"
//  		+ label + "\n";
//  	}

//  	s += "  Total: " + (totalsum/totalsamps) + " us\n";

      //return s;
   }



   //--------------------- ActionEvent Stuff ------------------------

   public void addActionListener (ActionListener l)
   {
      synchronized (action_listeners)
      {
         action_listeners.add (l);
      }
   }

   public void removeActionListener (ActionListener l)
   {
      synchronized (action_listeners)
      {
         action_listeners.remove (l);
      }
   }


   private void notifyActionListenersUpdate ()
   {
      ActionEvent e = new ActionEvent (this, ActionEvent.ACTION_PERFORMED,
                                       "Update");
      notifyActionListeners (e);
   }


   private void notifyActionListenersClear ()
   {
      ActionEvent e = new ActionEvent (this, ActionEvent.ACTION_PERFORMED,
                                       "Clear");
      notifyActionListeners (e);
   }


   private void notifyActionListeners (ActionEvent e)
   {
      ActionListener l;
      int i, n;
      synchronized (action_listeners)
      {
         n = action_listeners.size();
         for (i = 0; i < n; i++)
         {
            l = (ActionListener)action_listeners.get(i);
            l.actionPerformed (e);
         }
      }
   }




};
