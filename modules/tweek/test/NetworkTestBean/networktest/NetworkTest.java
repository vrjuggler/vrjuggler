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

package networktest;

import java.awt.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import org.omg.CORBA.BAD_PARAM;
import org.vrjuggler.tweek.event.*;
import org.vrjuggler.tweek.net.*;
import org.vrjuggler.tweek.net.corba.*;
import tweek.*;


/**
 * This is an example of a JavaBean that Tweek can load dynamically.  It holds
 * a JSlider and a (broken) JTextArea that act as Observers in the Tweek CORBA
 * Subject/Observer pattern implementation.
 *
 * @version 1.0
 */
public class NetworkTest
   extends JPanel
   implements CommunicationListener
{
   public NetworkTest()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      mFrameListener = new FrameListener(this);
      EventListenerRegistry.instance().registerListener(mFrameListener,
                                                        TweekFrameListener.class);
   }

   /**
    * Implements the Tweek CommunicationListener interface needed for being
    * informed of connections and disconnections with remote CORBA servers.
    */
   public void connectionOpened(CommunicationEvent e)
   {
      // The first thing to do is get the CORBA service object from the
      // event.  We need this so we know to whom we are are connecting.  Once
      // we have the CORBA service, we get its Subject Manager since that's
      // what contains the actual subjects we need.
      CorbaService corba_service = e.getCorbaService();
      SubjectManager mgr         = corba_service.getSubjectManager();

      Subject subject = mgr.getSubject("SliderSubject");
      SliderSubject slider_subject = null;

      // Try to narrow the Subjet object to a SliderSubject object.  If this
      // fails, it throws a CORBA BAD_PARAM exception.  In that case, we open
      // a dialog box saying that the narrowing failed.
      try
      {
         slider_subject = SliderSubjectHelper.narrow(subject);
      }
      catch (BAD_PARAM narrow_ex)
      {
         JOptionPane.showMessageDialog(null,
                                       "Failed to narrow subject to SliderSubject",
                                       "SliderSubject Narrow Error",
                                       JOptionPane.ERROR_MESSAGE);
      }

      subject = mgr.getSubject("WhiteboardSubject");
      WhiteboardSubject whiteboard_subject = null;

      // Try to do the same narrowing as above, but this time for Subject to
      // WhitboardSubjectHelper.
      try
      {
         whiteboard_subject = WhiteboardSubjectHelper.narrow(subject);
      }
      catch (BAD_PARAM narrow_ex)
      {
         JOptionPane.showMessageDialog(null,
                                       "Failed to narrow subject to WhiteboardSubject",
                                       "WhiteboardSubject Narrow Error",
                                       JOptionPane.ERROR_MESSAGE);
      }

      // Ensure that slider_subject is a valid object just to be safe.
      if ( slider_subject != null )
      {
         // First, we need a Java object that implements the Observer.  That
         // object must be registered with the Java CORBA service.
         mSliderObserver = new SliderObserverImpl(mDataSlider,
                                                  slider_subject);
         corba_service.registerObject(mSliderObserver, "SliderObserver");

         // Now that the observer is registered, we can attach it to the
         // subject.  The subject needs to know who its observers are so
         // that it can notify them of updates.
         slider_subject.attach(mSliderObserver._this());

         // Now we set the slider in our GUI to be whatever value the
         // remote subject is holding for us.
         mDataSlider.setValue(slider_subject.getValue());
         mDataSlider.addChangeListener(new SliderChangeListener(slider_subject));
         mDataSlider.setEnabled(true);
      }

      if ( whiteboard_subject != null )
      {
         mWhiteboardObserver = new WhiteboardObserverImpl(mWhiteboard,
                                                            whiteboard_subject);
         corba_service.registerObject(mWhiteboardObserver, "WhiteboardObserver");
         whiteboard_subject.attach(mWhiteboardObserver._this());
         mWhiteboard.setText(whiteboard_subject.getAllText());
         mWhiteboard.getDocument().addDocumentListener(new DocumentChangeListener(whiteboard_subject));
         mWhiteboard.setEnabled(true);
      }
   }

   public void connectionClosed(CommunicationEvent e)
   {
      disconnect();
   }

   public boolean frameClosing()
   {
      System.out.println("frameClosing()");
      disconnect();
      return true;
   }

   private void disconnect()
   {
      if ( mSliderObserver != null )
      {
         mSliderObserver.detach();
         mSliderObserver = null;
      }

      if ( mWhiteboardObserver != null )
      {
         mWhiteboardObserver.detach();
         mWhiteboardObserver = null;
      }

      mDataSlider.setEnabled(false);
      mWhiteboard.setEnabled(false);
   }

   private void jbInit() throws Exception
   {
      this.setLayout(mBeanLayout);
      mWhiteboardPanel.setLayout(mWhiteboardLayout);
      mDataSlider.setMajorTickSpacing(20);
      mDataSlider.setMinorTickSpacing(5);
      mDataSlider.setPaintTicks(true);
      mDataSlider.setEnabled(false);
      mWhiteboardLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mWhiteboardLabel.setText("Whiteboard");
      mWhiteboard.setBorder(BorderFactory.createLoweredBevelBorder());
      mWhiteboard.setMinimumSize(new Dimension(200, 25));
      mWhiteboard.setPreferredSize(new Dimension(300, 200));
      mWhiteboard.setLineWrap(true);
      mWhiteboard.setWrapStyleWord(true);
      mWhiteboardPanel.setEnabled(false);
      mSliderPanel.add(mDataSlider, null);
      mWhiteboardScroller.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
      mWhiteboardScroller.getViewport().add(mWhiteboard, null);
      mWhiteboardPanel.add(mWhiteboardScroller, BorderLayout.CENTER);
      mWhiteboardPanel.add(mWhiteboardLabel, BorderLayout.NORTH);

      this.add(mSliderPanel, BorderLayout.NORTH);
      this.add(mWhiteboardPanel, BorderLayout.CENTER);
   }

   private class SliderChangeListener implements ChangeListener
   {
      public SliderChangeListener (SliderSubject subject)
      {
         m_slider_subject = subject;
      }

      public void stateChanged (javax.swing.event.ChangeEvent e)
      {
         JSlider source = (JSlider) e.getSource();

         if ( ! source.getValueIsAdjusting() )
         {
            System.out.println("Informing subject of new value ...");
            m_slider_subject.setValue(source.getValue());
            System.out.println("Done informing subject");
         }
      }

      private SliderSubject m_slider_subject = null;
   }

   private class DocumentChangeListener implements DocumentListener
   {
      public DocumentChangeListener (WhiteboardSubject subject)
      {
         mWhiteboard_subject = subject;
      }

      public void insertUpdate (DocumentEvent e)
      {
         Document doc = e.getDocument();

         try
         {
            int offset = e.getOffset();
            int length = e.getLength();
            String new_text = doc.getText(offset, length);
            System.out.println("New text: " + new_text);
            mWhiteboard_subject.insertText(offset, length, new_text);
         }
         catch (BadLocationException loc_ex)
         {
            /* This probably won't happen. */ ;
         }
      }

      public void removeUpdate (DocumentEvent e)
      {
      }

      public void changedUpdate (DocumentEvent e)
      {
      }

      private WhiteboardSubject mWhiteboard_subject = null;
   }

   private TweekFrameListener mFrameListener;
   private BorderLayout mBeanLayout = new BorderLayout();

   private JPanel mSliderPanel = new JPanel();
   private JPanel mWhiteboardPanel = new JPanel();
   private JSlider mDataSlider = new JSlider();
   private JScrollPane mWhiteboardScroller = new JScrollPane();
   private BorderLayout mWhiteboardLayout = new BorderLayout();
   private JTextArea mWhiteboard = new JTextArea();
   private JLabel mWhiteboardLabel = new JLabel();

   private SliderObserverImpl mSliderObserver = null;
   private WhiteboardObserverImpl mWhiteboardObserver = null;
}

class FrameListener extends TweekFrameAdapter
{
   public FrameListener(NetworkTest bean)
   {
      this.bean = bean;
   }

   public boolean frameClosing(TweekFrameEvent e)
   {
      return bean.frameClosing();
   }

   private NetworkTest bean = null;
}
