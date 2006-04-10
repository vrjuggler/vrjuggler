import socket
import struct
import select
import Command

def findDevices():
   boards = {}
   s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
   s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

   s.sendto(Command.Find[0], ("<broadcast>", 2424))

   ready_to_read, ready_to_write, in_error = \
      select.select([s], [], [], 0.1)

   while len(ready_to_read) > 0:
      (data,addr) = s.recvfrom(Command.Find[1])
      result = struct.unpack(Command.Find[2], data)
      response_id = ''.join(result[0:4])
      mac = '%02X:%02X:%02X:%02X:%02X:%02X' % (result[4], result[5], result[6], result[7], result[8], result[9])
      version = '%d.%d' % (result[10], result[11])
      print "ID: %s MAC: %s Version: %s" % (response_id, mac, version)

      boards[mac] = (addr, version)
      ready_to_read, ready_to_write, in_error = \
         select.select([s], [], [], 0.1)

   s.close()
   return boards

class Elexol(object):
   def __init__(self, addr):
      self.mAddress = addr
      self.mSocket = None

   def open(self):
      self.mSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 

   def close(self):
      pass

   def readData(self, cmd):
      self.mSocket.sendto(cmd[0], self.mAddress)
      (data, addr) = self.mSocket.recvfrom(cmd[1])
      result = struct.unpack(cmd[2], data)
      return result[-1]

   def writeData(self, cmd, val):
      if 1 == len(cmd):
         data = struct.pack('cB', cmd[0], val)
      else:
         data = struct.pack('ccB', cmd[0], cmd[1], val)

      self.mSocket.sendto(data, self.mAddress)

   def writeIPAddress(self, ipAddress):
      result = re.match(r'^([0-9]*).([0-9]*).([0-9]*).([0-9]*)$', ipAddress)
      if result is not None:
         firstByte = int(result.groups()[0])
         secondByte = int(result.groups()[1])
         thirdByte = int(result.groups()[2])
         fourthByte = int(result.groups()[3])
         packet1 = struct.pack('2c3B', '\'', 'W', 0x06, secondByte, firstByte)
         packet2 = struct.pack('2c3B', '\'', 'W', 0x07, fourthByte, thirdByte)
         self.mSocket.sendto(packet1, self.mAddress)
         self.mSocket.sendto(packet2, self.mAddress)

   def getHostData(self):
      self.mSocket.sendto(Command.SendHostData[0], self.mAddress)
      (data, addr) = self.mSocket.recvfrom(Command.SendHostData[1])
      result = struct.unpack(Command.SendHostData[2], data)
      serial = '%2.2x %2.2x %2.2x' % (result[1], result[2], result[3])
      ip = '%d.%d.%d.%d' % (result[4], result[5], result[6], result[7])
      mac = '%02X:%02X:%02X:%02X:%02X:%02X' % (result[8], result[9], result[10], result[11], result[12], result[13])
      port = result[14]
      return (serial, ip, mac, port)
