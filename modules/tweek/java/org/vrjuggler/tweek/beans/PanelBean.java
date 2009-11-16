/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

package org.vrjuggler.tweek.beans;

import java.util.List;
import javax.swing.Icon;
import javax.swing.JComponent;
import org.vrjuggler.tweek.beans.loader.*;


/**
 * Simple container for putting GUI component JavaBeans into a Viewer Bean.
 * The contained Beans can be displayed when the corresponding item in the
 * viewer is selected.  Within the viewer, the description can be shown to
 * give users a recognizable name for the encapsulated Bean.
 *
 * @see ViewerBean
 */
public class PanelBean extends TweekBean
{
   /**
    * @param attrs           The common BeanAttributes for this bean.
    * @param icon_url        The URL that can be used to get this Bean's icon
    *                        (if it has one).
    * @param tool_tip        A tooltip for this Bean suitable for being
    *                        displayed in a Bean viewer of some sort.
    * @param paths           The paths relative to the root of a tree structure
    *                        in which this bean should belong
    *
    * @see BeanAttributes
    */
   public PanelBean( BeanAttributes attrs, String icon_url, String tool_tip,
                     List paths )
   {
      super( attrs );

      m_icon_url = icon_url;
      toolTip    = tool_tip;
      this.paths = paths;
   }

   public JComponent getComponent ()
   {
      return (JComponent)getBean();
   }

   public void setIcon (Icon _icon)
   {
      icon = _icon;
   }

   public Icon getIcon ()
   {
      if ( icon == null && m_icon_url != null )
      {
         try
         {
            java.net.URL url = new java.net.URL(m_icon_url);
            icon = new javax.swing.ImageIcon(url);
         }
         catch (java.net.MalformedURLException e)
         {
            /* Oh well. */ ;
         }
         catch (NullPointerException e)
         {
            System.out.println("WARNING: Could not find icon " + m_icon_url);
         }
      }

      return icon;
   }

   public void setToolTip (String tip)
   {
      toolTip = tip;
   }

   public String getToolTip ()
   {
      return toolTip;
   }

   public List getPaths ()
   {
      return paths;
   }

   /**
    * Instantiates the encapsulated JavaBean.  This process involves first
    * loading the Bean JAR file and then getting an instance of the contained
    * Bean.
    */
   public void instantiate () throws BeanInstantiationException
   {
      doInstantiation();
   }

   protected String     toolTip   = null;

   protected String  m_icon_url = null;
   protected Icon    icon       = null;
   protected List    paths      = null;
}
