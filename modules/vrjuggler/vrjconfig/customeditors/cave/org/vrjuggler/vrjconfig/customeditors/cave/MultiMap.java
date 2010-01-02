/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.cave;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

public class MultiMap
{
   private Map mInternalMap = new HashMap();
   
   public void addValue(Object key, Object value)
   {
      Object obj = mInternalMap.get(key);
      
      if (null == obj)
      {
         List new_list = new ArrayList();
         new_list.add(value);
         mInternalMap.put(key, new_list);
      }
      else
      {
         ((List)obj).add(value);
      }
   }

   public void addValues(Object key, Collection values)
   {
      Object obj = mInternalMap.get(key);
      
      if (null == obj)
      {
         List new_list = new ArrayList();
         new_list.addAll(values);
         mInternalMap.put(key, new_list);
      }
      else
      {
         ((List)obj).addAll(values);
      }
   }

   public Collection getKeys()
   {
      return mInternalMap.keySet();
   }
   
   public Collection getValues(Object key)
   {
      return((List)mInternalMap.get(key));
   }
   
   public Collection removeKey(Object key)
   {
      return((List)mInternalMap.remove(key));
   }
   
   public void removeValue(Object key, Object value)
   {
      ((List)mInternalMap.get(key)).remove(value);
   }
   public void removeValues(Object key, Collection values)
   {
      ((List)mInternalMap.get(key)).removeAll(values);
   }

   public String toString()
   {
	StringBuffer buf = new StringBuffer();
	buf.append("{");

	Iterator i = mInternalMap.entrySet().iterator();
        boolean hasNext = i.hasNext();
        while (hasNext)
        {
	    Entry e = (Entry) (i.next());
            Object key = e.getKey();
            Object value = e.getValue();
            buf.append((key == this ?  "(this Map)" : key) + "=" + 
                       (value == this ? "(this Map)": value));

            hasNext = i.hasNext();
            if (hasNext)
                buf.append(", ");
        }

	buf.append("}");
	return buf.toString();
   }
}
