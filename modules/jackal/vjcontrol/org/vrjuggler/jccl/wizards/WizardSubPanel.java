

package VjWizards;

import java.io.*;
import javax.swing.JComponent;

import VjConfig.ConfigChunkDB;

public interface WizardSubPanel {

    public JComponent getUIComponent();

    public boolean initUIComponent();

    public File getHelpFile();

    public boolean checkState();

    public ConfigChunkDB getResultDB();

    /** Must not alter db! */
    public boolean setInitialDB (ConfigChunkDB db);

    // called on all subpanels when the wizard is reset (finished)
    public void reset ();
    
}
