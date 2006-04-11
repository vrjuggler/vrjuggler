#
#
import serial
import Command
import time
import sys

class Flock(object):
   def __init__(self):
      self.port_num = 0
      self.baudrate = 38400;
    
      # Standard bird settings: Page 20 FOB manual
      self.bytesize = serial.EIGHTBITS
      self.parity = serial.PARITY_NONE
      self.stopbits = serial.STOPBITS_ONE
      self.timeout = 1
    
      self.port = None
    
    
   def open(self):
      self.port = serial.Serial(port=self.port_num,
                                baudrate=self.baudrate,
                                bytesize=self.bytesize,
                                parity=self.parity,
                                stopbits=self.stopbits,
                                timeout=self.timeout)
     
      # Open the port and get started
      #port.open()
      
      print "Resetting flock",
      sys.stdout.flush()
      self.port.setRTS(1)       # Reset the flock
#      while(self.port.getCTS()):
#         print "x",
      for i in range(5):       # Wait 1 second
         time.sleep(0.2)
         sys.stdout.write(".")
         sys.stdout.flush()
      
      self.port.setRTS(0)       # Set back to working
#      while(not self.port.getCTS()):
#         print ".",
      for i in range(5):       # Wait 1 second
         time.sleep(0.2)
         sys.stdout.write(".")
         sys.stdout.flush()
         
      print "complete."
    
   def setCmdAddr(self, addr):
      """ Set destination addr for next command """
      self.port.write(chr(Command.ToFbbNormal))
      self.port.write(chr(addr))
      
   def getSoftwareRevision(self):
      cmd_ret = self.examineAttribute("SoftwareRevision")
      assert(len(cmd_ret) == 2)
      sw_ver = tuple(map(ord,cmd_ret))
      return sw_ver
   
   def getCurrentError(self):
      cmd_ret = self.examineAttribute("BirdErrorCode")
      assert(len(cmd_ret) == 1)
      error_code = ord(cmd_ret)
      return error_code

   def getAddressingMode(self):
      cmd_ret = self.examineAttribute("AddressingMode")
      assert(len(cmd_ret) == 1)
      return ord(cmd_ret)
   
   def getBirdAddress(self):
      cmd_ret = self.examineAttribute("FbbAddress")
      assert(len(cmd_ret) == 1)
      return ord(cmd_ret)   
   
   def getBirdStatus(self):
      cmd_ret = self.examineAttribute("BirdStatus")
      assert(len(cmd_ret) == 2)
      MasterBit = (1<<15)
      InitBit = (1<<14)
      ErrorBit = (1<<13)
      RunningBit = (1<<12)
      
      rval = ord(cmd_ret[0]) + (ord(cmd_ret[1])<<8)
      is_master = bool(MasterBit & rval)
      is_init = bool(InitBit & rval)
      is_error = bool(ErrorBit & rval)
      is_running = bool(RunningBit & rval)
      return (is_master, is_init, is_error, is_running)
   
   def autoConfig(self, numUnits):
      self.port.flushInput()
      self.port.flushOutput()
      time.sleep(1)
      self.port.write(chr(Command.ChangeValue))
      self.port.write(chr(Command.ExamineParam["FbbAutoConfig"][0]))
      self.port.write(chr(numUnits))
      time.sleep(1)
      
   
   def printFlockSystemStatus(self):
      cmd_ret = self.examineAttribute("FlockSystemStatus")
      assert(len(cmd_ret) == 14)
      AccessBit = (1<<7)
      RunningBit = (1<<6)
      SensorBit = (1<<5)
      IsErtBit = (1<<4)
      TransmitterBit = (1<<0)
      
      for i in range(14):
         sval = ord(cmd_ret[i])
         print "%s: dev:%s run:%s sensor:%s ert:%s trans:%s"%(i,
                 bool(AccessBit&sval), bool(RunningBit&sval), bool(SensorBit&sval),
                 bool(IsErtBit&sval), bool(TransmitterBit&sval))
      
      
   def examineAttribute(self, attrib):
      """ Send examine command for attrib and returns the result.
           attrib must be an entry in Command.ExamineParam """
      assert(Command.ExamineParam.has_key(attrib))
      cmd_info = Command.ExamineParam[attrib]

      self.port.flushInput()
      self.port.write(chr(Command.ExamineValue))
      self.port.write(chr(cmd_info[0]))
      result = self.port.read(cmd_info[1])
      if self.port.inWaiting():
         print "Throwing away bytes: ", self.port.inWaiting()
         self.port.flushInput()
         
      return result
