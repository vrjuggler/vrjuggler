/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



/* VarValuePanel is just a little panel that we'll stick several of 
 * in a PropertyPanel.
 *
 *     -----------------------------------------------------
 *     |                                                   |
 *     | Type(optional) valuegadget  removebutton(optional)|
 *     |                                                   |
 *     -----------------------------------------------------
 *
 * the value gadget is going to be either a TextArea or a multisetSelectedItem
 * with the acceptable values in it.  removebutton will be there if the
 * associated property has var numbers of values.
 */

package VjGUI.configchunk;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import VjConfig.*;
import VjGUI.*;
import VjGUI.configchunk.*;
import VjGUI.util.*;

abstract public class VarValuePanel extends JPanel {

    public interface VarValuePanelParent {
	public void removePanel (VarValuePanel p);
    }

    abstract public void setValue (VarValue v);
    abstract public VarValue getValue ();



}

