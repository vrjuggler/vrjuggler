#!/bin/env python
#
#
# Test program for flock of birds.
#
# Relies upon pySerial: pyserial.sf.net
#
import serial
import sys
import getopt

def scan():
    """scan for available ports. return a list of tuples (num, name)"""
    available = []
    for i in range(256):
        try:
            s = serial.Serial(i)
            available.append( (i, s.portstr))
            s.close()   #explicit close 'cause of delayed GC in java
        except serial.serialutil.SerialException:
            pass
    return available

def usage():
    print """Program Usage:
   --test=output - Just print the data read from the bird for fob test output
          echo - Take line of input, send to bird and then read it back
          host_data_read - Perform the host data read bird test.
    """

def openBirdConnection():
    """ Returns an open bird connection """
    fob = serial.Serial()
    fob.port = 0
    fob.baudrate = 38400;
    
    # Standard bird settings: Page 20 FOB manual
    fob.bytesize = serial.EIGHTBITS
    fob.parity = serial.PARITY_NONE
    fob.stopbits = serial.STOPBITS_ONE
    fob.timeout = 3
    
    # Open the port and get started
    fob.open()
    fob.setRTS(0)            # Don't reset the bird
                    
    print "Opened port: %s"% (fob,)
    return fob

def test_output():
    print "---- Testing output -----"
    print "Reading 10 OK samples"
    
    fob = openBirdConnection()
    
    for i in xrange(10):
        rstr = fob.read(4)
        print "%s: %s"%(i,rstr)
    
    fob.close()
    
def test_echo():
    print """---- Testing echo -----
Read and echo data. 'q' to quit."""
    
    fob = openBirdConnection()
    
    raw_str = raw_input("> ")
    while(raw_str != 'q'):
        fob.write(raw_str)
        rstr = fob.read(len(raw_str))
        print rstr
        raw_str = raw_input("> ")
        
    fob.close()
    
    
def test_host_data_read():
    """ Each time data goes to the birt it will respond with 0-255 decimal.
        This is as a response in sequence.
        """
    fob = openBirdConnection()
    vals = []
    
    for i in xrange(255):
        fob.write('a')
        val = ord(fob.read(1))
        print val
        vals.append(val)
        
    good_vals = range(256)
    if(good_vals == vals):
        print "Test passed."
    else:
        print "Test failed."
        
    fob.close()
    

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h", ["test=", "help"])
    except getopt.GetoptError:
        # print help information and exit:
        usage()
        sys.exit(2)
    if not len(opts):
        usage()
        sys.exit()
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        if o in ("--test",):
            if "output" == a:
                test_output()
            elif "echo" == a:
                test_echo()
            elif "host_data_read" == a:
                test_host_data_read()

if __name__=='__main__':
    #print "Found ports:"
    #for n,s in scan():
    #    print "(%d) %s" % (n,s)
    
    main()
    
   
    
                        
