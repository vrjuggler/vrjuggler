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
package org.vrjuggler.jccl.editors;

import java.awt.*;
import java.beans.*;
import java.util.*;

import org.vrjuggler.tweek.TweekCore;
import org.vrjuggler.tweek.text.MessageDocument;
import org.vrjuggler.jccl.config.*;


/**
 * This all-static class is a registry for specialized PropertyEditors for
 * PropertyDefinitions.
 */
public class CustomEditorRegistry
{
   /**
    * Attempts to find a PropertyEditor that can edit property values described
    * by the PropertyDefinition identified by the given token.
    *
    * @param token      the token of the PropertyDefinition for which to get an
    *                   editor
    *
    * @return  an editor object for the given PropertyDefinition token; null if
    *          no suitable editor can be found
    */
   public static java.util.List findEditors(String token)
   {
      initialize();
     
      java.util.List editorList = null;
      
      if (mEditors.containsKey(token))
      {
         MessageDocument doc = TweekCore.instance().getMessageDocument();
         doc.printStatusnl("Found: " + token);

         try
         {
            java.util.List classList = (java.util.List)mEditors.get(token);
               
            editorList = new ArrayList();
            
            for(Iterator itr = classList.iterator() ; itr.hasNext() ; )
            {
               
               Class editorClass = (Class)itr.next();
               CustomEditor editor = null;
               
               editor = (CustomEditor)editorClass.newInstance();
               
               doc.printStatusnl("Editor: " + editor.getTitle());
               
               editorList.add(editor);   
            }
            
         }
         catch (Exception e)
         {
            e.printStackTrace();
            // Silently ignore errors
         }
      }
      return editorList;
   }

   /**
    * Register an editor class to be used to edit property values described by
    * the PropertyDefinition identified by the given token.
    *
    * @param token         the token of the PropertyDefinition describing the
    *                      property that will be edited
    * @param editorClass   the Class object of the editor class. If this is
    *                      null, the existing definition will be removed.
    */
   public static void registerEditor(String token, Class editorClass)
   {
      initialize();
      
      // Register the new editor class with the given token
      if (editorClass != null)
      {
         MessageDocument doc = TweekCore.instance().getMessageDocument();
         doc.printStatusnl("CustomEditorRegistry - Registering[" + token +
                           " -> " + editorClass);

         if(mEditors.containsKey(token))
         {
            java.util.List editorList = (java.util.List)mEditors.get(token);
            if(!editorList.contains(editorClass))
            {
               editorList.add(editorClass);
            }
         }
         else
         {
            java.util.List editorList = new ArrayList();
            editorList.add(editorClass);
            mEditors.put(token, editorList);
         }
      }
      // We have been asked to remove the editor under the given token
      else
      {
         mEditors.remove(token);
      }
   }

   /**
    * Initializes the internal editor map. We don't do initialization statically
    * so we don't eat up memory if this registry is never used.
    */
   public static void initialize()
   {
      if (mEditors == null)
      {
         mEditors = new HashMap();
      }
   }

   /**
    * The map of PropertyDefinition tokens to PropertyEditors.
    */
   private static Map mEditors = null;
}
