package networktest;

import javax.swing.JTextArea;
import tweek.*;


public class WhiteboardObserverImpl extends ObserverPOA
{
   public WhiteboardObserverImpl ()
   {
   }

   public void setWhiteboardSubject (WhiteboardSubject _subject)
   {
      whiteboard_subject = _subject;
      update();
   }

   public void setWhiteboard (JTextArea _whiteboard)
   {
      whiteboard = _whiteboard;
   }

   public void update ()
   {
      if ( whiteboard != null )
      {
         whiteboard.setText(whiteboard_subject.getAllText());
      }
   }

   private WhiteboardSubject whiteboard_subject = null;
   private JTextArea         whiteboard         = null;
}
