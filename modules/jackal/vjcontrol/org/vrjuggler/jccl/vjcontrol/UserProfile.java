
package VjControl;

import VjConfig.*;

public class UserProfile {

    int user_level;
    boolean[] user_types;

    // User levels
    public static final int BEGINNER = 0;
    public static final int EXPERT = 10;

    // User types
    public static final int POWER_USER = 0;
    public static final int ALWAYS_SHOW = 0;
    public static final int CONFIG_EDITOR = 1;
    public static final int VRJUGGLER_CONTROL = 2;
    public static final int PERFORMANCE_VIEW = 3;
    public static final int CONFIG_EXTEND = 4;
    public static final int NUM_TYPES = 5;

    public UserProfile () {
        user_level = BEGINNER;
        user_types = new boolean[NUM_TYPES];
    }

    public boolean setFromConfigChunk (ConfigChunk ch) {
        // looks for user_level and user_type tokens...
        // user_level just has an int value; user_type is expected to be
        // a set of ints.
        // chunks that use these should use enumerations to give them
        // reasonable names.
        Property p;
        boolean retval = true;
        p = ch.getPropertyFromToken ("user_level");
        if (p != null)
            user_level = p.getValue(0).getInt();
        else {
            user_level = BEGINNER;
            retval = false;
        }
        clearUserTypes();
        p = ch.getPropertyFromToken ("user_type");
        if (p != null) {
            int i, n = p.getNum();
            for (i = 0; i < n; i++) {
                user_types[i] = p.getValue(i).getBoolean();
            }
        }
        else {
            user_types[ALWAYS_SHOW] = true;
            retval = false;
        }
        //System.out.println ("chunk named: " + ch.getName() + "; retval is " + retval);
        return retval;
    }

//      static public int userLevelStringToInt (String s) {
//          if (s.equalsIgnoreCase ("beginner"))
//              return BEGINNER;
//          else if (s.equalsIgnoreCase ("expert"))
//              return EXPERT;
//          else
//              return BEGINNER;
//      }

//      static public int userTypeStringToInt (String s) {
//          if (s.equalsIgnoreCase ("config_editor"))
//              return CONFIG_EDITOR;
//          else if (s.equalsIgnoreCase ("vrjuggler_control"))
//              return VRJUGGLER_CONTROL;
//          else if (s.equalsIgnoreCase ("config_extend"))
//              return CONFIG_EXTEND;
//          else if (s.equalsIgnoreCase ("power_user"))
//              return POWER_USER;
//          else
//              return INVALID_USER_TYPE;
//      }

//      static public String userLevelIntToString (int level) {
//          switch (level) {
//          case BEGINNER:
//              return "beginner";
//          case EXPERT:
//              return "expert";
//          default:
//              return "beginner";
//          }
//      }

//      static public String userTypeIntToString (int user_type) {
//          switch (user_type) {
//          case CONFIG_EDITOR:
//              return "config_editor";
//          case VRJUGGLER_CONTROL:
//              return "vrjuggler_control";
//          case CONFIG_EXTEND:
//              return "config_extend";
//          case POWER_USER:
//              return "power_user";
//          default:
//              return "invalid_user_type";
//          }
//      }

    public void setUserLevel (int _user_level) {
        user_level = _user_level;
    }

    public void addUserType (int _user_type) {
        user_types[_user_type] = true;
    }

    public void removeUserType (int _user_type) {
        user_types[_user_type] = false;
    }

    public void clearUserTypes () {
        for (int i = 1; i < NUM_TYPES; i++) {
            user_types[i] = false;
        }
    }

    public boolean accepts (int _user_level) {
        return (_user_level <= user_level);
    }

    public boolean accepts (UserProfile _profile) {
        if (_profile.user_level <= user_level) {
            if (user_types[POWER_USER] || _profile.user_types[ALWAYS_SHOW])
                return true;
            for (int i = 0; i < NUM_TYPES; i++)
                if (user_types[i] && _profile.user_types[i])
                    return true;
            return false;
        }
        else
            return false;
    }

    public boolean accepts (ConfigChunk ch) {
        UserProfile p = new UserProfile();
        if (!p.setFromConfigChunk (ch))
            return true; // safety measure; accept it if there's no user
                         // profile data.
        return accepts (p);
    }
}
