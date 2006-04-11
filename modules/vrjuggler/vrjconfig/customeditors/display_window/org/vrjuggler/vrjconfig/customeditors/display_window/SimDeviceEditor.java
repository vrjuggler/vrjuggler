/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.display_window;

import javax.swing.JComponent;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;

import org.vrjuggler.vrjconfig.commoneditors.KeyboardEditorPanel;


/**
 * The interface that all simulated device type editors must implement.
 * This is utilized by SimKeyboardEditorPanel.
 */
public interface SimDeviceEditor
{
   /**
    * Provides the simulated device editor with a KeyboardEditorPanel
    * instance that can be used for visualizing the keyboard/mouse bindings
    * that provide the input to the simulated device whose config element is
    * being edited.  The gven reference may be null, and editor implementations
    * must be prepared for that case.
    */
   public void setKeyboardEditorPanel(KeyboardEditorPanel panel);

   /**
    * Sets up the configuration that the editor implementation will display
    * and modify.
    */
   public void setConfig(ConfigContext ctx, ConfigElement elt);

   /**
    * Returns the GUI component for this simulated device type editor
    * implementation.
    */
   public JComponent getEditor();

   public void editorClosing();
}
