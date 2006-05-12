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

package org.vrjuggler.jccl.editors;

import java.awt.Container;
import java.awt.event.ActionListener;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;


/**
 * Defines an interface for objects who are custom editors.  Custom editors
 * operate within a single configuration context.  They may be given a single
 * config element to edit, but they can access all elements within their
 * context.
 */
public interface CustomEditor
{
   /**
    * Sets the config context and ocnfig element that will be used by the
    * editor.  A null value for <code>elm</code> should be interpreted as an
    * indication that the editor should pull all the config elements it needs
    * from the context.  The context reference will never be null.  Custom
    * editors should be implemented such that this method is invoked once per
    * object lifetime.
    *
    * @param ctx The configuration context in which this editor will operate.
    * @param elm The config element that will be edited by thiis editor.
    *
    * @since 0.91.1
    */
   public void setConfig(ConfigContext ctx, ConfigElement elm);

   /**
    * Returns the GUI commponent that is this editor's user interface.
    */
   public Container getPanel();

   /**
    * Returns the title (or name) for this custom editor.
    */
   public String getTitle();

   /**
    * Returns the java.awt.event.ActionListener object that will be used to
    * handle requests for help by the user.  The editor may return null to
    * indicate that it does not respond to help requests.
    *
    * @since 0.92.2
    */
   public ActionListener getHelpActionListener();

   /**
    * Indicates to the editor that it is being closedd.  This offers the
    * custom editor the opportunity to clean up after itself.  The editor
    * implementation should expect that this method will be invoked no more
    * than once per object lifetime, though it may not be invoked at all.
    *
    * @since 0.92.10
    */
   public void editorClosing();
}
