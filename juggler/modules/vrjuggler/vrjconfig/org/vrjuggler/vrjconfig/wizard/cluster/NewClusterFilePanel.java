package org.vrjuggler.vrjconfig.wizard.cluster;

import java.util.Iterator;
import java.io.IOException;
import javax.swing.*;
import java.awt.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.wizard.*;
import java.awt.event.*;

public class NewClusterFilePanel extends JPanel
{
  BorderLayout baseLayout = new BorderLayout();
  JPanel centerPnl = new JPanel();


  private DefaultListModel lstModelNodes = new DefaultListModel();
  private String mFileSourceName = null;

  ////////
  private ConfigContext mContext = null;
  private FileDataSource mFileSource = null;
  private ConfigBroker mBroker = null;
  private JPanel directionsPanel = new JPanel();
  private JFileChooser mFileChooser = new JFileChooser();
  private Box mMenuBox2;
  private Component spacer;
  private JButton browse = new JButton();
  private BorderLayout centerPanelBorderLayout = new BorderLayout();
  private JTextField textFilename = new JTextField();
  private JPanel jPanel1 = new JPanel();
//  private JLabel jLabel1 = new JLabel();
  private BorderLayout borderLayout1 = new BorderLayout();
  private JPanel jPanel2 = new JPanel();
  private JPanel jPanel3 = new JPanel();
  private JLabel lblTitle = new JLabel();
  private JLabel lblDirections = new JLabel();
  private JLabel jLabel2 = new JLabel();
  private Component component1;
  ///////

  public NewClusterFilePanel()
  {
     try
     {
       jbInit();
     }
     catch(Exception e)
     {
        e.printStackTrace();
     }
     try
     {
       jLabel2.setIcon(new ImageIcon(this.getClass().getClassLoader().getResource("org/vrjuggler/vrjconfig/wizard/cluster/images/ClusterIcon.png")));
     }
     catch(NullPointerException exp)
     {
       jLabel2.setText("");
     }
  }

  private void jbInit() throws Exception
  {
    component1 = Box.createVerticalStrut(150);
    baseLayout.setHgap(5);
    baseLayout.setVgap(5);
    this.setLayout(baseLayout);
    directionsPanel.setBorder(BorderFactory.createEtchedBorder());
    lblTitle.setFont(new java.awt.Font("Serif", 1, 20));
    lblTitle.setHorizontalAlignment(SwingConstants.LEFT);
    lblTitle.setText("New Cluster Config File");
    lblDirections.setText("Select the file that you want to create.");
    jLabel2.setIcon(new ImageIcon("/home/users/aronb/Source/ClusterWizard/images/ClusterIcon.png"));
    this.add(centerPnl, BorderLayout.SOUTH);
    mMenuBox2 = Box.createHorizontalBox();
    spacer = Box.createHorizontalStrut(8);
    directionsPanel.setLayout(borderLayout1);
    browse.setText("Browse...");
    browse.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        //addNode_actionPerformed(e);
        browse();
      }
    });
    centerPnl.setLayout(centerPanelBorderLayout);
    this.add(directionsPanel, BorderLayout.NORTH);
    directionsPanel.add(jPanel2,  BorderLayout.CENTER);
    jPanel2.add(lblTitle, null);
    jPanel2.add(lblDirections, null);
    directionsPanel.add(jPanel3,  BorderLayout.EAST);
    jPanel3.add(jLabel2, null);
    this.add(jPanel1,  BorderLayout.CENTER);
    centerPnl.add(mMenuBox2, BorderLayout.CENTER);
    mMenuBox2.add(browse, null);
    mMenuBox2.add(spacer, null);
    mMenuBox2.add(textFilename, null);
    centerPnl.add(component1,  BorderLayout.SOUTH);
  }

  private void browse()
  {
    int result = mFileChooser.showSaveDialog(this);
    if(result == JFileChooser.APPROVE_OPTION)
    {
      String filename = mFileChooser.getSelectedFile().getAbsolutePath();
      textFilename.setText(filename);
    }
  }

  public void setFileSourceName(String file_name)
  {
    mFileSourceName = file_name;
  }
  public String getFileSourceName()
  {
    return(mFileSourceName);
  }


  public void init()
  {
    this.textFilename.setText("");
  }
  public boolean loadFile()
  {
    java.io.File temp_file = new java.io.File(textFilename.getText());

    if(temp_file.exists())
    {
      int result = JOptionPane.showConfirmDialog(this,"Do you want to load this existing file?.",
          "ClusterWizard",JOptionPane.YES_NO_OPTION);
      if(result == JOptionPane.NO_OPTION)
      {
        return false;
      }
      else if(!temp_file.canWrite())
      {
        JOptionPane.showMessageDialog(this,"You can not write to this.","ClusterWizard",JOptionPane.ERROR_MESSAGE);
        return false;
      }
      // We have gotten a positive responce so try to create it below
    }
    try{
      // Get handle to broker
      mBroker = new ConfigBrokerProxy();

      // Create New FileSource
      if(!mBroker.containsDataSource(mFileSourceName))
      {
        mFileSource = new FileDataSource(textFilename.getText(),FileDataSource.ELEMENTS);
        mBroker.add(mFileSourceName, mFileSource);
      }
    }
    catch(java.io.IOException exp)
    {
      exp.printStackTrace();
      return(false);
    }
    return(true);
  }
}
