/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

package org.vrjuggler.vrjconfig.commoneditors;

import java.util.*;
import javax.swing.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigEvent;
import org.vrjuggler.jccl.config.event.ConfigListener;

public class ConfigPtrListModel
   extends DefaultListModel
   implements ConfigListener
{
   ConfigBroker mBroker = null;
   ConfigContext mContext = null;
   List mElementTypes = new ArrayList();

   public ConfigPtrListModel(ConfigContext context)
   {
      mBroker = new ConfigBrokerProxy();
      mContext = context;

      update();
   }

   public void addElementType(String element_type)
   {
      mElementTypes.add(element_type);
      update();
   }

   public void removeElementType(String element_type)
   {
      mElementTypes.remove(element_type);
      update();
   }

   public void removeAllElementTypes()
   {
      mElementTypes.clear();
      update();
   }

   public void configElementAdded(ConfigEvent evt)
   {
      update();
      fireIntervalAdded(evt.getSource(), 0, getSize());
   }

   public void configElementRemoved(ConfigEvent evt)
   {
      update();
      fireIntervalRemoved(evt.getSource(), 0, getSize());
   }

   public void update()
   {
      this.removeAllElements();
      List matches = new ArrayList();
      List elts = mBroker.getElements(mContext);

      for(Iterator i = mElementTypes.iterator() ; i.hasNext() ;)
      {
         String temp_type = (String)i.next();
         matches.addAll(ConfigUtilities.getElementsWithDefinition(elts, temp_type));
      }

      for(int i = 0; i < matches.size() ; i++)
      {
         this.addElement(((ConfigElement)matches.get(i)).getName());
      }
   }
}
