

package VjComponents.PerfMonitor;

import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.event.*;

public class PanelTreeFolder extends JPanel implements ActionListener {

    JPanel mNorth;
    //Component mWest;
    JComponent mCenter;
    JButton mExpandButton;
    boolean mExpanded;

    public PanelTreeFolder (JComponent top_component) {
        super();

        setLayout (new BorderLayout());
        add (Box.createHorizontalStrut(40), BorderLayout.WEST);

        mNorth = new JPanel();
	mNorth.setLayout (new BoxLayout(mNorth, BoxLayout.X_AXIS));
        add (mNorth, BorderLayout.NORTH);

        mExpandButton = new JButton ("Expand");
        mNorth.add (mExpandButton);
        mNorth.add (top_component);

        mCenter = new JPanel();
        mCenter.setLayout (new BoxLayout (mCenter, BoxLayout.Y_AXIS));
        add (mCenter, BorderLayout.CENTER);

	mExpanded = true;
	mExpandButton.addActionListener (this);
    }

    public int getSubpanelCount() {
        return mCenter.getComponentCount();
    }

    public JComponent getSubpanel(int i) {
        return (JComponent)mCenter.getComponent(i);
    }

    public void addSubpanel(JComponent p) {
	mCenter.add(p);
    }

    public void actionPerformed (ActionEvent e) {
	if (mExpanded) {
	    mExpanded = false;
	    remove (mCenter);
	}
	else {
	    mExpanded = true;
	    add (mCenter, BorderLayout.CENTER);
	}
	mCenter.revalidate();
	repaint();
    }
}



        
