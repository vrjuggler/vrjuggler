package testbean;

import java.awt.*;
import java.net.URL;
import javax.swing.*;
import org.vrjuggler.tweek.beans.*;


/**
 * Title:        Plexus Network Monitor and Simulation Controller
 * Description:
 * Copyright:    Copyright (c) 2001
 * Company:      VRAC
 * @author
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
