/*
 *  File:	    $Name$
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


package VjGUI;

import java.awt.*;
import javax.swing.*;
import javax.swing.tree.*;

public class ChunkDBTreeCellRenderer extends JLabel implements TreeCellRenderer {

    private Icon mOpenIcon;
    private Icon mClosedIcon;
    private Icon mLeafIcon;

    public ChunkDBTreeCellRenderer (Icon open, Icon close, Icon leaf) {
	mOpenIcon = open;
	mClosedIcon = close;
	mLeafIcon = leaf;

	if (open == null || close == null || leaf == null)
	    System.out.println ("one icon is null");

    }

//     private void setMyFont (Font f) {
// 	super.setFont (f);
//     }

//     public void setFont (Font f) {
// 	;
//     }

    public Component getTreeCellRendererComponent (JTree tree,
						   Object value,
						   boolean selected,
						   boolean expanded,
						   boolean leaf,
						   int row,
						   boolean hasFocus) {
	DefaultMutableTreeNode node = (DefaultMutableTreeNode)value;
	boolean disabled;
	try {
	    ChunkTreeNodeInfo info = (ChunkTreeNodeInfo)node.getUserObject();

	    setText (info.toString());
	    if (info.ch != null || info.childchunks > 0)
		disabled = false;
	    else
		disabled = true;
	}
	catch (ClassCastException e) {
	    setText ("foo");
	    disabled = false;
	}
	    

	if (leaf)
	    setIcon (mLeafIcon);
	else
	    if (expanded)
		setIcon (mOpenIcon);
	    else 
		setIcon (mClosedIcon);

	//UIDefaults def = UIManager.getDefaults();

	// broken - and using jdk1.2
// 	Font main_font = def.getFont ("Tree.font");
// 	System.out.println ("font size is " + main_font.getSize2D());
// 	Font smaller_font = main_font.deriveFont(main_font.getSize2D()-2f);
// 	//	Font smaller_font = new Font (main_font.getFontName(), main_font.getStyle(), main_font.getSize() - 4);
// 	if (leaf)
// 	    setFont (main_font);
// 	else
// 	    setFont (smaller_font);

	if (disabled) {
	    setForeground (Color.lightGray);
	    setBackground (UIManager.getColor ("Tree.textBackground"));
	}
	else if (selected) {
	    setForeground (UIManager.getColor ("Tree.selectionForeground"));
	    setBackground (UIManager.getColor ("Tree.selectionBackground"));
	}
	else {
	    setForeground (UIManager.getColor("Tree.textForeground"));
	    setBackground (UIManager.getColor("Tree.textBackground"));
	}

	return this;
    }


    public void paint (Graphics g) {
	int textstart, textwidth;
	textstart = getIcon().getIconWidth() + getIconTextGap();
	textwidth = getSize().width - textstart;
	g.setColor (getBackground() );
	g.fillRect (textstart, 0, textwidth, getSize().height);
	getIcon().paintIcon (this, g, 0, 0);
	g.setColor (getForeground());
	g.drawString (getText(), textstart, g.getFontMetrics().getAscent() );
    }

}




