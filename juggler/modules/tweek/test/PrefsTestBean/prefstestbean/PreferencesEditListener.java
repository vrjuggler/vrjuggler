package prefstestbean;

import java.util.EventListener;


/**
 */
public interface PreferencesEditListener extends EventListener
{
   public void preferencesEdited (PreferencesEditEvent e);
}