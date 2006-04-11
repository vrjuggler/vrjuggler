#
# Program to start up (and make sure it's still running) the AutoBuild analyzer
#
import os;
import sys;
import time;
import getopt;

import ServerConfig     # Get configuration options
import RemoteNavFacade  # Base facade interface

# pyro stuff
import Pyro.naming;
import Pyro.core;
from Pyro.protocol import getHostname
from Pyro.errors import PyroError,NamingError

print " ---- OsgNavFacadeServer ---- "

use_test_facade = 0       # If true, just use a test version of the facade

# --- Cmd line params to use
nameserver_hostname = os.uname()[1];               # What host to use
nameserver_command = "ns";

def printUsage():
   print "Usage: %s --hostname=<default is local> --ns-command=<default 'ns'>" % (sys.argv[0],)

# Handles the command line parameters
def parseCmdLine():
   global nameserver_command, nameserver_hostname;
   try:
      opts, args = getopt.getopt(sys.argv[1:], "", ["help", "hostname=", "ns-command="])
   except getopt.GetoptError:
       # print help information and exit:
       printUsage()
       sys.exit(2)
   for o, a in opts:
       if o in ("-h", "--help"):
          printUsage()
          sys.exit()
       if o in ("--hostname",):
          nameserver_hostname = a;
          print "   Option: Set hostname to: ", nameserver_hostname
       if o in ("--ns-command",):
          nameserver_command = a;   
          print "   Option: Set nameserver command to: ", nameserver_command

def startNameserver():
   """ Start the name server up with some good options to use. """
   server_start_command = nameserver_command + " -n %s -m &" % nameserver_hostname;
   os.environ["PYRO_TRACELEVEL"] = "3";     # Set to log warnings
   os.environ["PYRO_CHECKSUM"] = "1";       # Turn on check summing
   print "Staring the name server:"
   print "  cmd: ", server_start_command;
   os.system(server_start_command);         # Start the server
   print "  Server should now be running. Sleeping for a little bit to wait.";
   time.sleep(5);
   print "  Done sleeping.  We should be good.";
   

def getOsgNavFacade():
   """ Return reference to a local object implementing MMAFacade. """
   facade = None
   if use_test_facade:
      print "   Creating TEST facade."
      facade = RemoteNavFacade.TestRemoteNavFacade()
   else:
      print "   Creating TWEEK facade from factory."
      import tweekfacadefactory
      facade = tweekfacadefactory.getTweekFacade()
      
   return facade

   
#
# Check that nameserver and server facade
#
def checkEverything():
   # First setup pyro and try to connect
   Pyro.core.initServer();
   
   daemon = Pyro.core.Daemon(host=nameserver_hostname);   # Create local daemon
   locator = Pyro.naming.NameServerLocator();             # Get locator to find the remote nameserver
   
   # Try to get to nameserver
   try:
      print "Attempting to find a namesever..."
      ns = locator.getNS(host=nameserver_hostname);   # Try to find a nameserver on the machine
   except PyroError, X:
      print "Can't find nameserver: \n Exception:", X;
      ns = None;
   else:
      print "   Found nameserver at: ", str(ns.URI)
   
   # If it is not started, then start it
   if not ns:
      startNameserver();
      try:
         ns = locator.getNS(host=nameserver_hostname);   # Try to find a nameserver on the machine
      except PyroError, X:
         print "Still can't find nameserverr: \n Exception:", X;
         print "I give up!!!";
         sys.exit();
   
   # Now I have a nameserver to make sure there is an object running
   assert ns;
   start_server = 0;
   
   # Setup the daemon
   daemon.useNameServer(ns);
   server_obj_uri = None;
   
   try:
      server_obj_uri = ns.resolve(ServerConfig.facade_obj_name);
   except NamingError, X:
      print "Nameserver does not have a facade object served up."
      start_server = 1;
   except PyroError, X:
      print "Unexpected pyro exception: ", X;
      print "Giving up.";
      sys.exit();
      
   # If I should be server
   if start_server:
      print "Creating server facade object";
      facade = getOsgNavFacade();

      facade_delegate = Pyro.core.SynchronizedObjBase();
      facade_delegate.delegateTo(facade);
      
      print "   Hooking up to daemon";
      daemon.connect(facade_delegate, ServerConfig.facade_obj_name);
      
      print "   Starting server loop";
      try:
         daemon.requestLoop();
      except KeyboardInterrupt:
         print "Recieved interupt keys."
      daemon.disconnect(facade_delegate);
      daemon.shutdown();
      print "Server is shutdown.\nbye.";
   
   else:
      print "Nothing more to do.  Daeomon server object is already running.";
   

#
# "Main" function
#
if __name__ == "__main__":
   parseCmdLine();                           # Process the command line parameters
   checkEverything();
   print "---------- Done -----------"

                
