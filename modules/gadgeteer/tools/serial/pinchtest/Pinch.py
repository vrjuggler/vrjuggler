##
##
import serial
import Command
import time
import sys

class Pinch(object):
   def __init__(self):
      self.port_num = 0
    
      # Default settings
      self.baudrate = 9600;
      # Timestamps are on by default. (Pinch Glove Manual p. 8)
      self.timestamps = True
      
      self.bytesize = serial.EIGHTBITS
      self.parity = serial.PARITY_NONE
      self.stopbits = serial.STOPBITS_ONE
      self.timeout = 1
    
      self.port = None
    
   def turnOnTimeStamps(self):
      print """--- Trying to turn on timestamps ---"""
      self.sendCommand("TurnTimeStampsOn")
      result = self.getResult()
      if(result == "1"):
         self.timestamps = True
         print "Timestamps successfully turned on."
      else:
         self.timestamps = False
         print "ERROR: Failed to turn on time stamps."
         print "       Recieved '%s' when expecting '1'" % result
      
   def turnOffTimeStamps(self):
      print """--- Trying to turn off timestamps ---"""
      self.sendCommand("TurnTimeStampsOff")
      result = self.getResult()
      if(result == "0"):
         self.timestamps = False
         print "Timestamps successfully turned off."
      else:
         self.timestamps = True
         print "ERROR: Failed to turn off time stamps."
         print "       Recieved '%s' when expecting '0'" % result

   def getData(self):
      rstr = self.port.read(1)
      # Waiting for START_BYTE
      while(len(rstr) <= 0  \
            or ((ord(rstr) != int(Command.START_BYTE_TS)) \
            and (ord(rstr) != int(Command.START_BYTE_NTS)))):
         rstr = self.port.read(1)
      # Getting all chars until END_BYTE
      result = ''
      rstr = self.port.read(1)
      while(ord(rstr) != int(Command.END_BYTE)):
         result += rstr
         rstr = self.port.read(1)
      #for ch in result:
      #   print ord(ch)
      
      self.parseWithTimestamps(result)
      return result


      #if(self.timestamps):
      #   self.getDataWithTimeStamps()
      #else
      #   self.getDataWithoutTimeStamps()

   def parseWithTimestamps(self, input):
      data_pairs = [ (input[i],input[i+1]) for i in range(0,len(input)-2,2)]
      time_stamp_pair = (input[len(input)-2],input[len(input)-1])
      num_ticks = (ord(time_stamp_pair[1])<<7) | ord(time_stamp_pair[0])
      print "Number of ticks since last packet: ", num_ticks
      print "                         seconds: ", (num_ticks*633.3)/1000000
      print "NOTE: This value will saturate at 16382 ticks."
      print "      (13.6511206 seconds) (p. 10 PinchGlove Manual)"
      
      print "\n======================================"
      print "Number of touches:", len(data_pairs)
      self.parseDigitalValues(data_pairs)
      for pair in data_pairs:
         #print ord(pair[0])
         #print ord(pair[1])
         self.printHand(pair)

   def parseDigitalValues(self, data):
      left = [0,0,0,0,0]
      right = [0,0,0,0,0]
      for pair in data:
         left[4] |= (ord(pair[0]) & 0x1)
         left[3] |= (ord(pair[0]) & 0x2)>>1
         left[2] |= (ord(pair[0]) & 0x4)>>2
         left[1] |= (ord(pair[0]) & 0x8)>>3
         left[0] |= (ord(pair[0]) & 0x10)>>4
         
         right[0] |= (ord(pair[1]) & 0x1)
         right[1] |= (ord(pair[1]) & 0x2)>>1
         right[2] |= (ord(pair[1]) & 0x4)>>2
         right[3] |= (ord(pair[1]) & 0x8)>>3
         right[4] |= (ord(pair[1]) & 0x10)>>4
      print left, "    ", right         
         
   def parseGestureValues(self, data):
      left = [0,0,0,0,0]
      right = [0,0,0,0,0]
      touch_num = 1; 
      for pair in data:
         if(ord(pair[0]) & 0x1):
            left[4] += touch_num
         if(ord(pair[0]) & 0x2):
            left[3] += touch_num
         if(ord(pair[0]) & 0x4):
            left[2] += touch_num
         if(ord(pair[0]) & 0x8):
            left[1] += touch_num
         if(ord(pair[0]) & 0x10):
            left[0] += touch_num
            
         if(ord(pair[1]) & 0x1):
            right[0] += touch_num
         if(ord(pair[1]) & 0x2):
            right[1] += touch_num
         if(ord(pair[1]) & 0x4):
            right[2] += touch_num
         if(ord(pair[1]) & 0x8):
            right[3] += touch_num
         if(ord(pair[1]) & 0x10):
            right[4] += touch_num

         touch_num *= 2
      print left, "    ", right         
     
   def printHand(self, data):
      left = ["","",""]
      right = ["","",""]
      
      i = 16
      while(i >=1):
         if(ord(data[0]) & i):
            left[0] += '   '
            left[1] += '   '
            left[2] += ' _ '
         else:
            left[0] += ' - '
            left[1] += '| |'
            left[2] += '| |'
         if(ord(data[1]) & i):
            right[0] = '   ' + right[0]
            right[1] = '   ' + right[1]
            right[2] = ' _ ' + right[2]
         else:
            right[0] = ' - ' + right[0]
            right[1] = '| |' + right[1]
            right[2] = '| |' + right[2]
         i /= 2
      print "Left Hand:      ", "    ", "Right Hane:     "
      for i in range(0, 3):
         print left[i], "    ", right[i]
  
   def printRightHandData(self, data):
      upper_hand = ""
      i = 1
      while(i <= 16):
         print "i=", i
         if(ord(data) & i):
            upper_hand += ' '
         else:
            upper_hand += '|'
         i *= 2
      print upper_hand

   
   def open(self):
      print "Opening Port #", self.port_num
      self.port = serial.Serial(port=self.port_num,
                                baudrate=self.baudrate,
                                bytesize=self.bytesize,
                                parity=self.parity,
                                stopbits=self.stopbits,
                                timeout=self.timeout)
     
   def sendCommand(self, cmd):
      """ Prints the version of the glove given in argument hand.
          Valid arguments are LeftVersion & RightVersion """
      self.port.flushInput()
      self.port.flushOutput()

      self.port.write(chr(Command.ExamineParam[cmd][0]))
      # If there was already a byte in the PinchGlove's input buffer
      # the hardware will get the last byte and return a '?' because
      # it is confused. If we can read this, we need to fix it. In a
      # way we are clearing the PinchGloves input buffer.
      if(self.port.read(1)):
         print """PinchGlove input buffer already had an old byte. Resending the \
first byte of the command"""
         self.port.write(chr(Command.ExamineParam[cmd][0]))
      # Wait between sending bytes.
      time.sleep(1)
      self.port.write(chr(Command.ExamineParam[cmd][1]))
      
   def getResult(self):
      rstr = self.port.read(1)
      assert(len(rstr) > 0 and "ERROR: Unable to connect to PinchGloves")
      # Waiting for START_BYTE
      while(ord(rstr) != int(Command.START_BYTE)):
         rstr = self.port.read(1)
      # Getting all chars until END_BYTE
      result = ''
      rstr = self.port.read(1)
      while(ord(rstr) != int(Command.END_BYTE)):
         result += rstr
         rstr = self.port.read(1)
      return result

   def print_version(self):
      print """--- Getting version information ---"""
      self.sendCommand("LeftVersion")
      print self.getResult()
      self.sendCommand("RightVersion")
      print self.getResult()
   
   def print_processor_code(self):
      print """--- Getting processor code ---"""
      self.sendCommand("ProcessorCode")
      print self.getResult()
   
   def print_tick_length(self):
      print """--- Getting tick length ---"""
      self.sendCommand("TickLength")
      print self.getResult()

   def get_tick_length(self):
      print """--- Getting tick length ---"""
      self.sendCommand("TickLength")
      return float(self.getResult().split()[0])

   def test_timestamps(self):
      self.turnOnTimeStamps()
      self.turnOffTimeStamps()

   def test_data_format_version(self):
      print """--- Trying set the data format version. ---"""
      self.sendCommand("DataFormatVersion")
      result = self.getResult()
      if(result == "1"):
         print "Data format version set successfully."
      else:
         print "ERROR: Failed to set data format version."
         print "       Recieved '%s' when expecting '1'" % result
