#
#
import serial
import Command

class Flock(object):
   def __init__(self):
      self.port_num = 0
      self.baudrate = 38400;
    
      # Standard bird settings: Page 20 FOB manual
      self.bytesize = serial.EIGHTBITS
      self.parity = serial.PARITY_NONE
      self.stopbits = serial.STOPBITS_ONE
      self.timeout = 3
    
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
      self.port.setRTS(0)            # Don't reset the bird
    
   def getSoftwareRevision(self):
      cmd_ret = self.examineAttribute("SoftwareRevision")
      assert(len(cmd_ret) ==2)
      sw_ver = tuple(map(ord,cmd_ret))
      return sw_ver;
      
      
   def examineAttribute(self, attrib):
      """ Send examine command for attrib and returns the result.
           attrib must be an entry in Command.ExamineParam """
      assert(Command.ExamineParam.has_key(attrib))
      cmd_info = Command.ExamineParam[attrib]
    
      self.port.write(chr(Command.ExamineValue))
      self.port.write(chr(cmd_info[0]))
      result = self.port.read(cmd_info[1])
      return result
