/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/



package VjConfig;

import java.io.*;

import VjConfig.*;
import VjControl.Core;

/** Master IO handler for VR Juggler-style configuration files.
 *  There are currently two different formats for these configuration files:
 *  the "standard" style introduced with the initial release of VR Juggler,
 *  and the "xml" style which will become the default in VR Juggler 1.2.
 *  ConfigIO provides an integrated API for using both of these file formats.
 *  ConfigChunkDB and ChunkDescDB files can be read and written using 
 *  either format; the format can be stated explicitly or a default can be
 *  used.  ConfigIO is also capable of analyzing a file and determining 
 *  which IO handler to use to load it.
 *  <p>
 *  All of the reader/writer methods in ConfigIO take a handler_type argument,
 *  which is either DEFAULT, XML, or STANDARD (see declarations below).  The
 *  file reader methods also take a GUESS method (instead of DEFAULT) which
 *  tells them to use heuristics to determine the file format.
 *  <p>
 *  Any application that wishes to read or write a ConfigChunkDB or ChunkDescDB
 *  should do so through the ConfigIO methods.  Using the read() and fileRep()
 *  methods of those classes is strongly discouraged.
 *
 *  @see ConfigChunkDB
 *  @see ChunkDescDB
 *  @see ConfigIOHandler
 *  @author Christopher Just
 *  @version $Revision$
 */
public class ConfigIO {

    public final static int DEFAULT = 0;
    public final static int XML = 1;
    public final static int STANDARD = 2;
    public final static int GUESS = 0;

    private ConfigIOHandler xml_handler;
    private ConfigIOHandler standard_handler;
    private ConfigIOHandler default_handler;

    private static ConfigIO inst = null;


    /** Constructor.
     *  ConfigIO is a singleton object with static accessor methods. An
     *  instance is created to store its member variables, but it can note
     *  be instantiated by any external code.
     */
    private ConfigIO () {
        xml_handler = new XMLConfigIOHandler();
        standard_handler = new StandardConfigIOHandler();
        default_handler = xml_handler;
    }



    /** Gets the singleton instance, creating it if necessary.
     *  If initialization of the JAXP subsystem fails, this method will
     *  terminate the application with an error message.
     */
    private static ConfigIO instance() {
        if (inst == null) {
            try {
                inst = new ConfigIO();
            }
            catch (NoClassDefFoundError e) {
                System.out.println 
                    ("ERROR: JAXP (The \"Java API for XML Processing\")\n" +
                     "Was not found.  If you are using JDK 1.3 or\n" +
                     "earlier, you need to install the JAXP packages\n" +
                     "or make sure the JAXP .jar files are included\n" +
                     "in your CLASSPATH.\n" +
                     "See http://java.sun.com/xml/index.html for\n" + 
                     "more information.\n");
                System.exit (1);
            }
        }
        return inst;
    }



    /** Get a reference to a ConfigIOHandler.
     *  This is public so that e.g. the XMLConfigCommunicator can grab
     *  a XMLConfigIOHandler from us for parsing network communications.
     */
    public static ConfigIOHandler getHandler (int handler_type) {
        switch (handler_type) {
        case XML:
            return instance().xml_handler;
        case STANDARD:
            return instance().standard_handler;
        default:
            return instance().default_handler;
        }
    }



    /** Release a ConfigIOHandler that had been obtained with getHandler().
     *  The current implementations of the IO handlers are fully reentrant,
     *  and thus it's safe to use the same one in multiple places & never
     *  worry about releasing it.  Do we still want to keep this method
     *  around, or just remove it?
     */
    public static void releaseHandler (ConfigIOHandler handler) {
        ;
    }



    // ConfigChunkDB methods


    /** Reads a ConfigChunkDB from file into db.
     *  If handler_type is DEFAULT or GUESS, we use a heuristic to decide 
     *  which handler should try to parse the file.
     *  @returns true - success
     *  @returns false - on any parsing error.
     */
    public static void readConfigChunkDB (File file, ConfigChunkDB db, int handler_type) throws IOException /*FileNotFoundException*/ {
        ConfigIOHandler handler = null;
        if (handler_type == DEFAULT || handler_type == GUESS) {
//             try {
                FileInputStream f = new FileInputStream (file);
                int ch = f.read();
                if (ch == '<')
                    handler = getHandler (XML);
                else
                    handler = getHandler (STANDARD);
//             }
//             catch (IOException e) {
//                 return false;
//             }
        }
        else
            handler = getHandler (handler_type);
        if (handler == null)
            throw new IOException ("Can't get protocol handler.");

        if (!handler.readConfigChunkDB (file, db))
            throw new IOException ("Protocol-specific parser failed.");
    }



    /** Reads a ConfigChunkDB from input into db.
     *  @return true if succesful, false on error.
     */
    public static void readConfigChunkDB (InputStream input, ConfigChunkDB db, int handler_type) throws IOException {
        ConfigIOHandler handler = null;
        if (handler_type == GUESS) {
            if (!input.markSupported())
                input = new BufferedInputStream (input, 20);
//             try {
                input.mark(10);
                int ch = input.read();
                if (ch == '<')
                    handler = getHandler (XML);
                else
                    handler = getHandler (STANDARD);
                input.reset();
//             }
//             catch (IOException e) {
//                 return false;
//             }
        }
        else
            handler = getHandler (handler_type);
        if (handler == null)
            throw new IOException ("Can't get protocol handler.");

        if (!handler.readConfigChunkDB (input, db))
            throw new IOException ("Protocol-specific parser failed.");
    }



    public static boolean writeConfigChunkDB (File file, ConfigChunkDB db, int handler_type) {
        ConfigIOHandler handler = getHandler (handler_type);
        if (handler == null)
            return false;

        return handler.writeConfigChunkDB (file, db);
    }



    public static boolean writeConfigChunkDB (DataOutputStream output, ConfigChunkDB db, int handler_type) {
        ConfigIOHandler handler = getHandler (handler_type);
        if (handler == null)
            return false;

        return handler.writeConfigChunkDB (output, db);
    }



    // ChunkDescDB methods


    /** Reads a ChunkDescDB from file into db.
     *  If handler_type is DEFAULT or GUESS, we use a heuristic to decide 
     *  which handler should try to parse the file.
     *  @returns true - success
     *  @returns false - on any parsing error.
     */
    public static void readChunkDescDB (File file, ChunkDescDB db, int handler_type) throws IOException {
        ConfigIOHandler handler = null;
        if (handler_type == DEFAULT || handler_type == GUESS) {
            try {
                FileInputStream f = new FileInputStream (file);
                int ch = f.read();
                if (ch == '<')
                    handler = getHandler (XML);
                else
                    handler = getHandler (STANDARD);
            }
            catch (FileNotFoundException e) {
                throw e;
            }
            catch (IOException e) {
                throw new IOException ("Couldn't determine file format.");
            }
        }
        else
            handler = getHandler (handler_type);
        if (handler == null)
            throw new IOException ("Can't get protocol handler.");

        if (!handler.readChunkDescDB (file, db))
            throw new IOException ("Protocol-specific parser failed.");
    }



    /** Reads a ChunkDescDB from input into db.
     *  @return true if succesful, false on error.
     */
    public static void readChunkDescDB (InputStream input, ChunkDescDB db, int handler_type) throws IOException {
        ConfigIOHandler handler = null;
        if (handler_type == GUESS) {
            if (!input.markSupported())
                input = new BufferedInputStream (input, 20);
            try {
                input.mark(10);
                int ch = input.read();
                if (ch == '<')
                    handler = getHandler (XML);
                else
                    handler = getHandler (STANDARD);
                input.reset();
            }
            catch (FileNotFoundException e) {
                throw e;
            }
            catch (IOException e) {
                throw new IOException ("Couldn't determine file format.");
            }
        }
        else
            handler = getHandler (handler_type);
        if (handler == null)
            throw new IOException ("Can't get protocol handler.");

        if (!handler.readChunkDescDB (input, db))
            throw new IOException ("Protocol-specific parser failed.");
    }



    public static boolean writeChunkDescDB (File file, ChunkDescDB db, int handler_type) {
        ConfigIOHandler handler = getHandler (handler_type);
        if (handler == null) {
            Core.consoleErrorMessage ("ConfigIO", "Couldn't get ConfigIOHandler for writeChunkDescDB.");
            return false;
        }
        else {
            return handler.writeChunkDescDB (file, db);
        }
    }



    public static boolean writeChunkDescDB (DataOutputStream output, ChunkDescDB db, int handler_type) {
        ConfigIOHandler handler = getHandler (handler_type);
        if (handler == null) {
            Core.consoleErrorMessage ("ConfigIO", "Couldn't get ConfigIOHandler for writeChunkDescDB.");
            return false;
        }
        else {
            return handler.writeChunkDescDB (output, db);
        }
    }

}

