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

	UIDefaults def = UIManager.getDefaults();
	if (disabled) {
	    setForeground (Color.lightGray);
	    setBackground (def.getColor ("Tree.textBackground"));
	}
	else if (selected) {
	    //setForeground (SystemColor.textHighlightText);
	    //setBackground (SystemColor.textHighlight);
	    setForeground (def.getColor ("Tree.selectionForeground"));
	    setBackground (def.getColor ("Tree.selectionBackground"));
	}
	else {
	    //setForeground (SystemColor.textText);
	    //setBackground (SystemColor.window);
	    setForeground (def.getColor("Tree.textForeground"));
	    setBackground (def.getColor("Tree.textBackground"));
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




