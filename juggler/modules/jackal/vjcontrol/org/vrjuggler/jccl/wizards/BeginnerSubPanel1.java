
package VjWizards;

import java.io.*;
import javax.swing.*;
import VjConfig.*;

public class BeginnerSubPanel1 extends JPanel implements WizardSubPanel {

    public BeginnerSubPanel1 () {
        super();
    }

    //----------------------- WizardSubPanel Stuff --------------------------

    public JComponent getUIComponent () {
        return this;
    }

    public boolean initUIComponent () {
        return true;
    }

    public File getHelpFile () {
        return null;
    }

    public boolean checkState () {
        return true;
    }

    public boolean setInitialDB (ConfigChunkDB db) {
        return true;
    }

    public ConfigChunkDB getResultDB () {
        return new ConfigChunkDB();
    }

    public void reset() {
        ;
    }
}
