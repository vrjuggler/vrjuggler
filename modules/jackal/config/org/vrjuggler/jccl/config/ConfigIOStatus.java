
package VjConfig;

import java.util.List;
import java.util.ArrayList;

public class ConfigIOStatus {

    List messages;  // either Strings or Throwables.
    int status;

    int num_warnings;
    int num_errors;
    
    public static final int SUCCESS = 1;
    public static final int WARNINGS = 2;
    public static final int ERRORS = 3;   // loaded, but there were errors
    public static final int FAILURE = 4;  // total failure; nothing loaded

    public ConfigIOStatus () {
        messages = new ArrayList();
        clear();
    }

    public void clear () {
        messages.clear();
        status = SUCCESS;
        num_warnings = 0;
        num_errors = 0;
    }

    private void setStatus (int new_status) {
        // wish i had an assert

        // things can only go downhill from here
        if (new_status > status)
            status = new_status;
    }

    public int getStatus () {
        return status;
    }
    
//     public void addMessage (String msg) {
//         messages.add (msg);
//     }

    public void addWarning (String msg) {
        setStatus (WARNINGS);
        messages.add (msg);
        num_warnings++;
    }

    public void addError (String msg) {
        setStatus (ERRORS);
        messages.add (msg);
        num_errors++;
    }

    public void addFailure (String msg) {
        setStatus (FAILURE);
        messages.add (msg);
    }

    public void addWarning (Throwable msg) {
        setStatus (WARNINGS);
        messages.add (msg);
        num_warnings++;
    }

    public void addError (Throwable msg) {
        setStatus (ERRORS);
        messages.add (msg);
        num_errors++;
    }

    public void addFailure (Throwable msg) {
        setStatus (FAILURE);
        messages.add (msg);
    }

    public int size() {
        return messages.size();
    }

    public Object get (int i) {
        return messages.get(i);
    }

    public String getSummary () {
        switch (status) {
        case SUCCESS:
            return "Loaded succesfully.";
        case WARNINGS:
            StringBuffer s = new StringBuffer();
            s.append ("Loaded with ");
            s.append (num_warnings);
            s.append ((num_warnings == 1)?" warning.\n":" warnings.");
            return new String(s);
        case ERRORS:
            StringBuffer s2 = new StringBuffer();
            s2.append ("Loaded with ");
            s2.append (num_errors);
            s2.append ((num_errors == 1)?" error, ":" errors, ");
            s2.append (num_warnings);
            s2.append ((num_warnings == 1)?" warning.\n":" warnings.");
            return new String(s2);
        case FAILURE:
        default:
            return "Load failed.";
        }
    }

    public String toString () {
        StringBuffer s = new StringBuffer();
        s.append (getSummary());
        s.append ("\n");

        for (int i = 0; i < messages.size(); i++) {
            Object o = messages.get(i);
            s.append (o.toString());
        }

        return new String(s);
    }

}

