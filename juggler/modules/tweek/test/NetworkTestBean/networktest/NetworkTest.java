 ackage networktest;

import java.awt.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import orb.omg.CORBA.BAD_PARAM;
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
   implements CommunicationListener, TweekFrameListener
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
   }

   /**
    * Implements the Tweek CommunicationListener interface needed for being
    * informed of connections and disconnections with remote CORBA servers.
    */
   public void connectionStateChanged (CommunicationEvent e)
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

      // Handle a CORBA connection event from Tweek.
      if ( CommunicationEvent.CONNECT == e.getType() )
      {
         // Ensure that slider_subject is a valid object just to be safe.
         if ( slider_subject != null )
         {
            // First, we need a Java object that implements the Observer.  That
            // object must be registered with the Java CORBA service.
            m_slider_observer = new SliderObserverImpl(m_data_slider,
                                                       slider_subject);
            corba_service.registerObject(m_slider_observer, "SliderObserver");

            // Now that the observer is registered, we can attach it to the
            // subject.  The subject needs to know who its observers are so
            // that it can notify them of updates.
            slider_subject.attach(m_slider_observer._this());

            // Now we set the slider in our GUI to be whatever value the
            // remote subject is holding for us.
            m_data_slider.setValue(slider_subject.getValue());
            m_data_slider.addChangeListener(new SliderChangeListener(slider_subject));
         }

         if ( whiteboard_subject != null )
         {
            m_whiteboard_observer = new WhiteboardObserverImpl(m_whiteboard,
                                                               whiteboard_subject);
            corba_service.registerObject(m_whiteboard_observer, "WhiteboardObserver");
            whiteboard_subject.attach(m_whiteboard_observer._this());
            m_whiteboard.setText(whiteboard_subject.getAllText());
            m_whiteboard.getDocument().addDocumentListener(new DocumentChangeListener(whiteboard_subject));
         }
      }
      // Handle a CORBA disconnect event from Tweek.
      else if ( CommunicationEvent.DISCONNECT == e.getType() )
      {
         if ( slider_subject != null )
         {
            slider_subject.detach(m_slider_observer._this());
         }

         if ( whiteboard_subject != null )
         {
            whiteboard_subject.detach(m_whiteboard_observer._this());
         }
      }
   }

   public void frameStateChanged (TweekFrameEvent e)
   {
      if ( e.getType() == TweekFrameEvent.FRAME_CLOSE )
      {
         if ( m_slider_observer != null )
         {
            m_slider_observer.detach();
            m_slider_observer = null;
         }

         if ( m_whiteboard_observer != null )
         {
            m_whiteboard_observer.detach();
            m_whiteboard_observer = null;
         }
      }
   }

   private void jbInit() throws Exception
   {
      this.setLayout(m_bean_layout);
      m_whiteboard_panel.setLayout(m_whiteboard_layout);
      m_data_slider.setMajorTickSpacing(20);
      m_data_slider.setMinorTickSpacing(5);
      m_data_slider.setPaintTicks(true);
      m_whiteboard_label.setHorizontalAlignment(SwingConstants.CENTER);
      m_whiteboard_label.setText("Whiteboard");
      m_whiteboard.setBorder(BorderFactory.createLoweredBevelBorder());
      m_whiteboard.setMinimumSize(new Dimension(200, 25));
      m_whiteboard.setPreferredSize(new Dimension(300, 200));
      m_whiteboard.setLineWrap(true);
      m_whiteboard.setWrapStyleWord(true);
      m_slider_panel.add(m_data_slider, null);
      m_whiteboard_scroller.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
      m_whiteboard_scroller.getViewport().add(m_whiteboard, null);
      m_whiteboard_panel.add(m_whiteboard_scroller, BorderLayout.CENTER);
      m_whiteboard_panel.add(m_whiteboard_label, BorderLayout.NORTH);

      this.add(m_slider_panel, BorderLayout.NORTH);
      this.add(m_whiteboard_panel, BorderLayout.CENTER);
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
         m_whiteboard_subject = subject;
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
            m_whiteboard_subject.insertText(offset, length, new_text);
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

      private WhiteboardSubject m_whiteboard_subject = null;
   }

   private BorderLayout m_bean_layout = new BorderLayout();

   private JPanel m_slider_panel = new JPanel();
   private JPanel m_whiteboard_panel = new JPanel();
   private JSlider m_data_slider = new JSlider();
   private JScrollPane m_whiteboard_scroller = new JScrollPane();
   private BorderLayout m_whiteboard_layout = new BorderLayout();
   private JTextArea m_whiteboard = new JTextArea();
   private JLabel m_whiteboard_label = new JLabel();

   private SliderObserverImpl m_slider_observer = null;
   private WhiteboardObserverImpl m_whiteboard_observer = null;
}
