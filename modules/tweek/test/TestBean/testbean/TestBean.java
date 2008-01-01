/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

package testbean;

import java.awt.*;
import java.net.URL;
import javax.swing.*;
import org.vrjuggler.tweek.event.*;


/**
 * @version 1.0
 */
public class TestBean extends JPanel implements java.io.Serializable,
                                                UserLevelChangeListener
{
   public TestBean ()
   {
      try
      {
         jbInit();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }
   }

   public void userLevelChange (UserLevelChangeEvent e)
   {
      int new_level = e.getNewLevel();

      try
      {
         if ( new_level < 5 )
         {
            webPane.setPage(new URL("http://www.yahoo.com/"));
         }
         else if ( new_level == 5 )
         {
            webPane.setPage(new URL("http://antwrp.gsfc.nasa.gov/apod/"));
         }
         else if ( new_level < 9 )
         {
            webPane.setPage(new URL("http://www.jwz.org/"));
         }
         else
         {
            webPane.setPage(new URL("http://www.vrjuggler.org/"));
         }
      }
      catch (Exception url_ex)
      {
         url_ex.printStackTrace();
      }

      webPane.repaint();
      this.repaint();
   }

   private void jbInit() throws Exception
   {
      this.setLayout(m_layout);
      webPane.setEditable(false);
      webPane.setContentType("text/html");
      webPane.setPage(new URL("http://www.yahoo.com/"));
      this.add(scrollPane,  BorderLayout.CENTER);
      scrollPane.getViewport().add(webPane, null);
   }

   private BorderLayout m_layout = new BorderLayout();
   private JScrollPane scrollPane = new JScrollPane();
   private JEditorPane webPane = new JEditorPane();
}
