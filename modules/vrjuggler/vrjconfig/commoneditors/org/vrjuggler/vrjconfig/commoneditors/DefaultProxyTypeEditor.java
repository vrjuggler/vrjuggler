package org.vrjuggler.vrjconfig.commoneditors;

import javax.swing.JPanel;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import javax.swing.JLabel;
import java.awt.BorderLayout;
import javax.swing.*;

public class DefaultProxyTypeEditor
   extends JPanel
   implements ProxyTypeEditor
{
   public DefaultProxyTypeEditor()
   {
      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
   }

   private void jbInit()
      throws Exception
   {
      mInfoLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mInfoLabel.setText("No editable properties");
      this.setLayout(mMainLayout);
      this.add(mInfoLabel, java.awt.BorderLayout.NORTH);
   }

   private JLabel mInfoLabel = new JLabel();
   private BorderLayout mMainLayout = new BorderLayout();
}
