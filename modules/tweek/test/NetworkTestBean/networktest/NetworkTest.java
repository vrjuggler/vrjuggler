package networktest;

import java.awt.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import org.vrjuggler.tweek.event.*;
import org.vrjuggler.tweek.net.*;
import org.vrjuggler.tweek.net.corba.*;
import tweek.*;


/**
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

   public void connectionStateChanged (CommunicationEvent e)
   {
      CorbaService corba_service = e.getCorbaService();
      SubjectManager mgr         = corba_service.getSubjectManager();

      Subject subject = mgr.getSubject("SliderSubject");
      SliderSubject slider_subject = SliderSubjectHelper.narrow(subject);
      subject = mgr.getSubject("WhiteboardSubject");
      WhiteboardSubject whiteboard_subject = WhiteboardSubjectHelper.narrow(subject);

      if ( CommunicationEvent.CONNECT == e.getType() )
      {
         m_slider_observer = new SliderObserverImpl(m_data_slider, slider_subject);
         corba_service.registerObject(m_slider_observer, "SliderObserver");
         slider_subject.attach(m_slider_observer._this());
         m_data_slider.setValue(slider_subject.getValue());
         m_data_slider.addChangeListener(new SliderChangeListener(slider_subject));

         m_whiteboard_observer = new WhiteboardObserverImpl(m_whiteboard,
                                                            whiteboard_subject);
         corba_service.registerObject(m_whiteboard_observer, "WhiteboardObserver");
         whiteboard_subject.attach(m_whiteboard_observer._this());
         m_whiteboard.setText(whiteboard_subject.getAllText());
         m_whiteboard.getDocument().addDocumentListener(new DocumentChangeListener(whiteboard_subject));
      }
      else if ( CommunicationEvent.DISCONNECT == e.getType() )
      {
         slider_subject.detach(m_slider_observer._this());
         whiteboard_subject.detach(m_whiteboard_observer._this());
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
