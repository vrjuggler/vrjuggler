package org.vrjuggler.vrjconfig.ui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.util.EventListener;
import javax.swing.event.EventListenerList;

public class SImage extends JComponent
{
   public SImage()
   {
      //setImageFile("watermark_logo.png");
   }

   public static void main(String[] args)
   {
      JFrame test = new JFrame();
      final SImage intcomp = new SImage();
      Component top = Box.createVerticalStrut(15);
      Component bottom = Box.createVerticalStrut(15);
      Component left = Box.createHorizontalStrut(15);
      //Component right = Box.createHorizontalStrut(15);
	  JButton right = new JButton("Add");
      //JLabel intcomp = new JLabel("Test");
      //intcomp.setBorder(BorderFactory.createRaisedBevelBorder());
      test.getContentPane().setLayout(new BorderLayout());
      test.getContentPane().add(top, BorderLayout.NORTH);
      test.getContentPane().add(bottom, BorderLayout.SOUTH);
      test.getContentPane().add(left, BorderLayout.WEST);
      test.getContentPane().add(right, BorderLayout.EAST);
      test.getContentPane().add(intcomp, BorderLayout.CENTER);
      test.pack();
      test.setVisible(true);
   }
   public Dimension getPreferredSize()
   {
      Dimension size = new Dimension(100,500);
      return size;
   }
   public Dimension getMaximumSize()
   {
     Dimension size = new Dimension(100,5000);
      return size;
   }
   public Dimension getMinimumSize()
   {
     Dimension size = new Dimension(100,100);
      return size;
   }

   protected void drawInnerBorder(Graphics g, int x, int y, int w, int h)
   {
      Color oldColor = g.getColor();

      g.translate(x, y);

      Color highlightOuterColor = getBackground().brighter().brighter();
      Color highlightInnerColor = getBackground().brighter();
      Color shadowOuterColor = getBackground().darker().darker();
      Color shadowInnerColor = getBackground().darker();

      g.setColor(shadowInnerColor);
      g.drawLine(0, 0, 0, h-1);
      g.drawLine(1, 0, w-1, 0);

      g.setColor(shadowOuterColor);
      g.drawLine(1, 1, 1, h-2);
      g.drawLine(2, 1, w-2, 1);

      g.setColor(highlightOuterColor);
      g.drawLine(0, h-1, w-1, h-1);
      g.drawLine(w-1, 1, w-1, h-2);

      g.setColor(highlightInnerColor);
      g.drawLine(2, h-2, w-2, h-2);
      g.drawLine(w-2, 2, w-2, h-3);

      g.translate(-x, -y);
      g.setColor(oldColor);
   }
   protected void drawBar(Graphics g, int x, int y, int w, int h)
   {
   	Color oldColor = g.getColor();

   	g.translate(x, y);

   	Color highlightOuterColor = getBackground().brighter().brighter();
   	Color highlightInnerColor = getBackground().brighter();
   	Color shadowOuterColor = getBackground().darker().darker();
   	Color shadowInnerColor = getBackground().darker();

   	g.setColor(shadowInnerColor);
   	g.drawLine(1, 0, w-1, 0);

   	g.setColor(shadowOuterColor);
   	g.drawLine(2, 1, w-2, 1);

   	g.setColor(highlightOuterColor);
   	g.drawLine(0, -1, w-1, -1);

   	g.setColor(highlightInnerColor);
   	g.drawLine(2, -2, w-2, -2);

   	g.translate(-x, -y);
   	g.setColor(oldColor);
   }
     private Image image;
  public void setImageFile(String imageFile)
  {
    String fieldImageFile = imageFile;
    java.net.URL url = getClass().getResource(imageFile);
    if (url != null) {
      image = Toolkit.getDefaultToolkit().getImage(url);
      if (image != null) {
        MediaTracker tracker = new MediaTracker(this);
        tracker.addImage(image, 0);
        try {
          tracker.waitForAll();
        }
        catch (InterruptedException e) {
          System.out.println(e);
          e.printStackTrace();
        }
      }
    }
  }
  public void setImageFile(java.net.URL url)
  {
    if (url != null) {
      image = Toolkit.getDefaultToolkit().getImage(url);
      if (image != null) {
        MediaTracker tracker = new MediaTracker(this);
        tracker.addImage(image, 0);
        try {
          tracker.waitForAll();
        }
        catch (InterruptedException e) {
          System.out.println(e);
          e.printStackTrace();
        }
      }
    }
  }

   protected void paintComponent(Graphics g)
   {
      if (!(g instanceof Graphics2D))
      {
         return;
      }

      Insets insets = getInsets();
      
      Dimension d;
      if(getParent() != null)
      {
         d = getParent().getSize();
         setSize(d);
      }
      else
      {
         d = getSize();
      }
      
      if (image != null)
      {
         int current_width = d.width - insets.left - insets.right;
         int current_height = d.height - insets.top - insets.bottom;

         g.drawImage(image, insets.left, insets.right, current_width, current_height, this);
      }
   }
}
