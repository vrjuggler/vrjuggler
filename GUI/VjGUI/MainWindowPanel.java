
package VjGUI;

import VjGUI.BorderedPanel;


public abstract class MainWindowPanel extends BorderedPanel {


    public MainWindowPanel () {
	super(20,10,1,5); 
	/* watch his every move - superconstructor
	 * orchestrate illusions - superconstructor
	 */
    }

    public abstract void activate();

    public abstract void deactivate();

    public abstract boolean update ();


}








