package networktest;

import javax.swing.JTextArea;
import tweek.*;


public class WhiteboardObserverImpl extends ObserverPOA
{
   public WhiteboardObserverImpl (JTextArea _whiteboard,
                                  WhiteboardSubject _subject)
   {
      whiteboard         = _whiteboard;
      whiteboard_subject = _subject;
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
