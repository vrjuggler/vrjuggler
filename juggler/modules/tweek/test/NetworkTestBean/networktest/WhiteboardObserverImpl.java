package networktest;

import javax.swing.JTextArea;
import javax.swing.text.*;
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
         org.omg.CORBA.IntHolder offset = null, length = null;
         org.omg.CORBA.StringHolder text = null;

         whiteboard_subject.getLastChange(offset, length, text);

         Document doc = whiteboard.getDocument();
//         doc.insertString(offset.value, text.value, null);
//         whiteboard.setText(whiteboard_subject.getAllText());
         whiteboard.repaint();
      }
   }

   private WhiteboardSubject whiteboard_subject = null;
   private JTextArea         whiteboard         = null;
}
