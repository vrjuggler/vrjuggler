
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

    public String toString () {
        StringBuffer s = new StringBuffer();
        switch (status) {
        case SUCCESS:
            s.append ("Completed succesfully.\n");
            break;
        case WARNINGS:
            s.append ("Completed with ");
            s.append (num_warnings);
            s.append ((num_errors == 1)?" warning.\n":" warnings.\n");
            break;
        case ERRORS:
            s.append ("Completed with ");
            s.append (num_errors);
            s.append ((num_errors == 1)?" error, ":" errors, ");
            s.append (num_warnings);
            s.append ((num_errors == 1)?" warning.\n":" warnings.\n");
            break;
        case FAILURE:
            s.append ("Failed.\n");
            break;
        }
        for (int i = 0; i < messages.size(); i++) {
            Object o = messages.get(i);
            s.append (o.toString());
        }

        return new String(s);
    }

}

