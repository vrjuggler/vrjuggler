package networktest;

import java.awt.*;
import javax.swing.*;
import org.vrjuggler.tweek.net.*;
import org.vrjuggler.tweek.net.corba.*;
import tweek.*;


/**
 * @version 1.0
 */
public class NetworkTest extends JPanel implements CommunicationListener
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

   public void communication (CommunicationEvent e)
   {
      CorbaService corba_service = e.getCorbaService();
      SubjectManager mgr         = corba_service.getSubjectManager();

      Subject subject = mgr.getSubject("SliderSubject");
      SliderSubject slider_subject = SliderSubjectHelper.narrow(subject);
      m_slider_observer = new SliderObserverImpl(m_data_slider, slider_subject);
      corba_service.registerObject(m_slider_observer, "SliderObserver");
      slider_subject.attach(m_slider_observer._this());
      m_data_slider.setValue(slider_subject.getValue());
      m_data_slider.addChangeListener(new SliderChangeListener(slider_subject));

      subject = mgr.getSubject("WhiteboardSubject");
      WhiteboardSubject whiteboard_subject = WhiteboardSubjectHelper.narrow(subject);
      m_whiteboard_observer = new WhiteboardObserverImpl(m_whiteboard,
                                                         whiteboard_subject);
      corba_service.registerObject(m_whiteboard_observer, "WhiteboardObserver");
      whiteboard_subject.attach(m_whiteboard_observer._this());
   }

   private void jbInit() throws Exception
   {
      this.setLayout(m_bean_layout);
      m_whiteboard.setBorder(BorderFactory.createLoweredBevelBorder());
      m_whiteboard.setPreferredSize(new Dimension(300, 200));
      m_whiteboard_panel.setLayout(m_whitboard_layout);
      m_data_slider.setMajorTickSpacing(20);
      m_data_slider.setMinorTickSpacing(5);
      m_data_slider.setPaintTicks(true);
      m_whiteboard_label.setHorizontalAlignment(SwingConstants.CENTER);
      m_whiteboard_label.setText("Whiteboard");
      this.add(m_slider_panel, BorderLayout.NORTH);
      m_slider_panel.add(m_data_slider, null);
      this.add(m_whiteboard_panel, BorderLayout.CENTER);
      m_whiteboard_panel.add(m_whiteboard, BorderLayout.CENTER);
      m_whiteboard_panel.add(m_whiteboard_label, BorderLayout.NORTH);
   }

   private class SliderChangeListener
      implements javax.swing.event.ChangeListener
   {
      public SliderChangeListener (SliderSubject subject)
      {
         slider_subject = subject;
      }

      public void stateChanged (javax.swing.event.ChangeEvent e)
      {
         JSlider source = (JSlider) e.getSource();

         if ( ! source.getValueIsAdjusting() )
         {
            System.out.println("Informing subject of new value ...");
            slider_subject.setValue(source.getValue());
            System.out.println("Done informing subject");
         }
      }

      private SliderSubject slider_subject = null;
   }

   private JPanel m_slider_panel = new JPanel();
   private JPanel m_whiteboard_panel = new JPanel();
   private JSlider m_data_slider = new JSlider();
   private JTextArea m_whiteboard = new JTextArea();
   private BorderLayout m_whitboard_layout = new BorderLayout();
   private BorderLayout m_bean_layout = new BorderLayout();
   private JLabel m_whiteboard_label = new JLabel();

   private SliderObserverImpl m_slider_observer = null;
   private WhiteboardObserverImpl m_whiteboard_observer = null;
}
