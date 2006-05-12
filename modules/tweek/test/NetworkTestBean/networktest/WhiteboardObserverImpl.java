/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

   public void detach ()
   {
      whiteboard_subject.detach(this._this());
   }

   private WhiteboardSubject whiteboard_subject = null;
   private JTextArea         whiteboard         = null;
}
