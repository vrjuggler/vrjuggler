# Copyright (C) Infiscape Corporation 2006

#import math
import sys
#, os, os.path, time, traceback
#pj = os.path.join

from PyQt4 import QtGui, QtCore, QtOpenGL
import Ether24Base
import ElexolPort
import Elexol
import Command

class ElexolEther24GUI(QtGui.QMainWindow, Ether24Base.Ui_Ether24Base):
   def __init__(self, parent = None):
      QtGui.QMainWindow.__init__(self, parent)
      self.setupUi(self)
      self.mDevice = None


   def setupUi(self, widget):
      Ether24Base.Ui_Ether24Base.setupUi(self, widget)
      
      self.connect(self.mFindDevices, QtCore.SIGNAL("clicked()"), self.onFindDevices)
      self.connect(self.mDeviceCB, QtCore.SIGNAL("currentIndexChanged(int)"), self.onDeviceSelected)
      
      self.connect(self.mStatusRefresh, QtCore.SIGNAL("clicked()"), self.onStatusRefresh)
      self.connect(self.mDirectionRefresh, QtCore.SIGNAL("clicked()"), self.onDirectionRefresh)
      self.connect(self.mPullUpRefresh, QtCore.SIGNAL("clicked()"), self.onPullUpRefresh)
      self.connect(self.mThresholdRefresh, QtCore.SIGNAL("clicked()"), self.onThresholdRefresh)
      self.connect(self.mSchmittRefresh, QtCore.SIGNAL("clicked()"), self.onSchmittRefresh)


      # Add Status Ports
      self.mStatusPortLayout = QtGui.QHBoxLayout()
      self.mStatusPortLayout.setMargin(0)
      self.mStatusPortLayout.setSpacing(6)
      self.mStatusPortLayout.setObjectName("mStatusPortLayout")
      self.vboxlayout1.addLayout(self.mStatusPortLayout)

      self.mStatusA = ElexolPort.ElexolPort(self.mStatusTab, ElexolPort.ElexolPort.ONOFF)
      self.mStatusA.setObjectName("mStatusA")
      self.mStatusPortLayout.addWidget(self.mStatusA)
      self.mStatusA.setTitle("Status A")

      self.mStatusB = ElexolPort.ElexolPort(self.mStatusTab, ElexolPort.ElexolPort.ONOFF)
      self.mStatusB.setObjectName("mStatusB")
      self.mStatusPortLayout.addWidget(self.mStatusB)
      self.mStatusB.setTitle("Status B")

      self.mStatusC = ElexolPort.ElexolPort(self.mStatusTab, ElexolPort.ElexolPort.ONOFF)
      self.mStatusC.setObjectName("mStatusC")
      self.mStatusPortLayout.addWidget(self.mStatusC)
      self.mStatusC.setTitle("Status C")

      self.mDirectionPortLayout = QtGui.QHBoxLayout()
      self.mDirectionPortLayout.setMargin(0)
      self.mDirectionPortLayout.setSpacing(6)
      self.mDirectionPortLayout.setObjectName("mDirectionPortLayout")
      self.vboxlayout2.addLayout(self.mDirectionPortLayout)
      
      self.mDirectionA = ElexolPort.ElexolPort(self.mDirectionTab, ElexolPort.ElexolPort.DIRECTION)
      self.mDirectionA.setObjectName("mDirectionA")
      self.mDirectionPortLayout.addWidget(self.mDirectionA)
      self.mDirectionA.setTitle("Direction A")

      self.mDirectionB = ElexolPort.ElexolPort(self.mDirectionTab, ElexolPort.ElexolPort.DIRECTION)
      self.mDirectionB.setObjectName("mDirectionB")
      self.mDirectionPortLayout.addWidget(self.mDirectionB)
      self.mDirectionB.setTitle("Direction B")

      self.mDirectionC = ElexolPort.ElexolPort(self.mDirectionTab, ElexolPort.ElexolPort.DIRECTION)
      self.mDirectionC.setObjectName("mDirectionC")
      self.mDirectionPortLayout.addWidget(self.mDirectionC)
      self.mDirectionC.setTitle("Direction C")

      
      # Pull Up
      self.hboxlayout5 = QtGui.QHBoxLayout()
      self.hboxlayout5.setMargin(0)
      self.hboxlayout5.setSpacing(6)
      self.hboxlayout5.setObjectName("hboxlayout5")
      self.vboxlayout3.addLayout(self.hboxlayout5)

      self.mPullUpA = ElexolPort.ElexolPort(self.mPullupTab, ElexolPort.ElexolPort.ONOFF)
      self.mPullUpA.setObjectName("mPullUpA")
      self.hboxlayout5.addWidget(self.mPullUpA)
      self.mPullUpA.setTitle("Pull Up A")

      self.mPullUpB = ElexolPort.ElexolPort(self.mPullupTab, ElexolPort.ElexolPort.ONOFF)
      self.mPullUpB.setObjectName("mPullUpB")
      self.hboxlayout5.addWidget(self.mPullUpB)
      self.mPullUpB.setTitle("Pull Up B")

      self.mPullUpC = ElexolPort.ElexolPort(self.mPullupTab, ElexolPort.ElexolPort.ONOFF)
      self.mPullUpC.setObjectName("mPullUpC")
      self.hboxlayout5.addWidget(self.mPullUpC)
      self.mPullUpC.setTitle("Pull Up C")


      # Threshold
      self.hboxlayout7 = QtGui.QHBoxLayout()
      self.hboxlayout7.setMargin(0)
      self.hboxlayout7.setSpacing(6)
      self.hboxlayout7.setObjectName("hboxlayout7")
      self.vboxlayout4.addLayout(self.hboxlayout7)
      
      self.mThresholdA = ElexolPort.ElexolPort(self.mThresholdTab, ElexolPort.ElexolPort.THRESHOLD)
      self.mThresholdA.setObjectName("mThresholdA")
      self.hboxlayout7.addWidget(self.mThresholdA)
      self.mThresholdA.setTitle("Threshold A")

      self.mThresholdB = ElexolPort.ElexolPort(self.mThresholdTab, ElexolPort.ElexolPort.THRESHOLD)
      self.mThresholdB.setObjectName("mThresholdB")
      self.hboxlayout7.addWidget(self.mThresholdB)
      self.mThresholdB.setTitle("Threshold B")

      self.mThresholdC = ElexolPort.ElexolPort(self.mThresholdTab, ElexolPort.ElexolPort.THRESHOLD)
      self.mThresholdC.setObjectName("mThresholdC")
      self.hboxlayout7.addWidget(self.mThresholdC)
      self.mThresholdC.setTitle("Threshold C")


      # Schmitt Trigger
      self.hboxlayout9 = QtGui.QHBoxLayout()
      self.hboxlayout9.setMargin(0)
      self.hboxlayout9.setSpacing(6)
      self.hboxlayout9.setObjectName("hboxlayout9")
      self.vboxlayout5.addLayout(self.hboxlayout9)

      self.mSchmittA = ElexolPort.ElexolPort(self.mSchmittTriggerTab, ElexolPort.ElexolPort.ONOFF)
      self.mSchmittA.setObjectName("mSchmittA")
      self.hboxlayout9.addWidget(self.mSchmittA)
      self.mSchmittA.setTitle("Schmitt Trigger A")

      self.mSchmittB = ElexolPort.ElexolPort(self.mSchmittTriggerTab, ElexolPort.ElexolPort.ONOFF)
      self.mSchmittB.setObjectName("mSchmittB")
      self.hboxlayout9.addWidget(self.mSchmittB)
      self.mSchmittB.setTitle("Schmitt Trigger B")

      self.mSchmittC = ElexolPort.ElexolPort(self.mSchmittTriggerTab, ElexolPort.ElexolPort.ONOFF)
      self.mSchmittC.setObjectName("mSchmittC")
      self.hboxlayout9.addWidget(self.mSchmittC)
      self.mSchmittC.setTitle("Schmitt Trigger C")


      self.mPullUpA.mInverted = True
      self.mPullUpB.mInverted = True
      self.mPullUpC.mInverted = True
      self.mSchmittA.mInverted = True
      self.mSchmittB.mInverted = True
      self.mSchmittC.mInverted = True
      self.mThresholdA.mInverted = True
      self.mThresholdB.mInverted = True
      self.mThresholdC.mInverted = True

      self.connect(self.mStatusA, QtCore.SIGNAL("valueChanged(int)"), self.onStatusChanged)
      self.connect(self.mStatusB, QtCore.SIGNAL("valueChanged(int)"), self.onStatusChanged)
      self.connect(self.mStatusC, QtCore.SIGNAL("valueChanged(int)"), self.onStatusChanged)
      self.connect(self.mDirectionA, QtCore.SIGNAL("valueChanged(int)"), self.onDirectionChanged)
      self.connect(self.mDirectionB, QtCore.SIGNAL("valueChanged(int)"), self.onDirectionChanged)
      self.connect(self.mDirectionC, QtCore.SIGNAL("valueChanged(int)"), self.onDirectionChanged)
      self.connect(self.mPullUpA, QtCore.SIGNAL("valueChanged(int)"), self.onPullUpChanged)
      self.connect(self.mPullUpB, QtCore.SIGNAL("valueChanged(int)"), self.onPullUpChanged)
      self.connect(self.mPullUpC, QtCore.SIGNAL("valueChanged(int)"), self.onPullUpChanged)
      self.connect(self.mThresholdA, QtCore.SIGNAL("valueChanged(int)"), self.onThresholdChanged)
      self.connect(self.mThresholdB, QtCore.SIGNAL("valueChanged(int)"), self.onThresholdChanged)
      self.connect(self.mThresholdC, QtCore.SIGNAL("valueChanged(int)"), self.onThresholdChanged)
      self.connect(self.mSchmittA, QtCore.SIGNAL("valueChanged(int)"), self.onSchmittChanged)
      self.connect(self.mSchmittB, QtCore.SIGNAL("valueChanged(int)"), self.onSchmittChanged)
      self.connect(self.mSchmittC, QtCore.SIGNAL("valueChanged(int)"), self.onSchmittChanged)

      #self.mPortA = ElexolPort(ElexolPort.DIRECTION, self.widget)
      #self.mPortA.setTitle("Port A")
      #self.mPortB = ElexolPort(ElexolPort.ONOFF, self.widget)
      #self.mPortB.setTitle("Port B")
      #self.mPortC = ElexolPort(ElexolPort.THRESHOLD, self.widget)
      #self.mPortC.setTitle("Port C")
      #self.hboxlayout.removeWidget(self.mBtnA)
      #self.hboxlayout.removeWidget(self.mBtnB)
      #self.hboxlayout.removeWidget(self.mBtnC)
      #self.hboxlayout.addWidget(self.mPortA)
      #self.hboxlayout.addWidget(self.mPortB)
      #self.hboxlayout.addWidget(self.mPortC)

      #self.connect(self.mPortA, QtCore.SIGNAL("valueChanged(int)"), self.onPortAChanged)
      #self.connect(self.mPortB, QtCore.SIGNAL("valueChanged(int)"), self.onPortBChanged)
      #self.connect(self.mPortC, QtCore.SIGNAL("valueChanged(int)"), self.onPortCChanged)

      #self.mPortA.setValue(57)
      #self.mPortA.setEnabled(1)

   def onFindDevices(self):
      print "Finding devices..."
      self.mDevices = Elexol.findDevices()
      self.mDeviceCB.clear()
      for k,v in self.mDevices.iteritems():
         map = {'mac':QtCore.QVariant(str(k)),
                'ip':QtCore.QVariant(str(v[0][0])),
                'port':QtCore.QVariant(int(v[0][1])),
                'version':QtCore.QVariant(str(v[1]))}
         self.mDeviceCB.addItem(k, QtCore.QVariant(map))

   def onDeviceSelected(self, index):
      print self.mDeviceCB.itemText(index)
      user_data = self.mDeviceCB.itemData(index)
      map = user_data.toMap()
      
      mac = map[QtCore.QString('mac')].toString()
      ip = map[QtCore.QString('ip')].toString()
      result = map[QtCore.QString('port')].toInt()
      version = map[QtCore.QString('version')].toString()
      if not result[1]:
         print "Error, could not parse port as an integer."
      port = result[0]

      print "Selected [%s][%s][%s][%s]" % (mac, ip, port, version)

      # Close any existing device.
      if self.mDevice is not None:
         self.mDevice.close()
      # Create new device.
      self.mDevice = Elexol.Elexol((str(ip), port))
      self.mDevice.open()
      self.onStatusRefresh()
      self.onDirectionRefresh()
      self.onPullUpRefresh()
      self.onThresholdRefresh()
      self.onSchmittRefresh()

   def onStatusRefresh(self):
      self.mStatusA.setValue(self.mDevice.readData(Command.ReadPortA))
      self.mStatusB.setValue(self.mDevice.readData(Command.ReadPortB))
      self.mStatusC.setValue(self.mDevice.readData(Command.ReadPortC))

   def onDirectionRefresh(self):
      self.mDirectionA.setValue(self.mDevice.readData(Command.ReadDirectionA))
      self.mDirectionB.setValue(self.mDevice.readData(Command.ReadDirectionB))
      self.mDirectionC.setValue(self.mDevice.readData(Command.ReadDirectionC))

   def onPullUpRefresh(self):
      self.mPullUpA.setValue(self.mDevice.readData(Command.ReadPullUpA))
      self.mPullUpB.setValue(self.mDevice.readData(Command.ReadPullUpB))
      self.mPullUpC.setValue(self.mDevice.readData(Command.ReadPullUpC))

   def onThresholdRefresh(self):
      self.mThresholdA.setValue(self.mDevice.readData(Command.ReadThresholdA))
      self.mThresholdB.setValue(self.mDevice.readData(Command.ReadThresholdB))
      self.mThresholdC.setValue(self.mDevice.readData(Command.ReadThresholdC))

   def onSchmittRefresh(self):
      self.mSchmittA.setValue(self.mDevice.readData(Command.ReadSchmittTriggerA))
      self.mSchmittB.setValue(self.mDevice.readData(Command.ReadSchmittTriggerB))
      self.mSchmittC.setValue(self.mDevice.readData(Command.ReadSchmittTriggerC))

   def onStatusChanged(self, val):
      self.mDevice.writeData(Command.WritePortA, self.mStatusA.getValue())
      self.mDevice.writeData(Command.WritePortB, self.mStatusB.getValue())
      self.mDevice.writeData(Command.WritePortC, self.mStatusC.getValue())

   def onDirectionChanged(self, val):
      self.mDevice.writeData(Command.WriteDirectionA, self.mDirectionA.getValue())
      self.mDevice.writeData(Command.WriteDirectionB, self.mDirectionB.getValue())
      self.mDevice.writeData(Command.WriteDirectionC, self.mDirectionC.getValue())

      # Direction 255 is all inputs
      self.mStatusA.setEnabled(255-self.mDirectionA.getValue())
      self.mStatusB.setEnabled(255-self.mDirectionB.getValue())
      self.mStatusC.setEnabled(255-self.mDirectionC.getValue())
      self.onStatusRefresh()
      

   def onPullUpChanged(self, val):
      self.mDevice.writeData(Command.WritePullUpA, self.mPullUpA.getValue())
      self.mDevice.writeData(Command.WritePullUpB, self.mPullUpB.getValue())
      self.mDevice.writeData(Command.WritePullUpC, self.mPullUpC.getValue())
      self.onStatusRefresh()

   def onThresholdChanged(self, val):
      self.mDevice.writeData(Command.WriteThresholdA, self.mThresholdA.getValue())
      self.mDevice.writeData(Command.WriteThresholdB, self.mThresholdB.getValue())
      self.mDevice.writeData(Command.WriteThresholdC, self.mThresholdC.getValue())
      self.onStatusRefresh()

   def onSchmittChanged(self, val):
      self.mDevice.writeData(Command.WriteSchmittTriggerA, self.mSchmittA.getValue())
      self.mDevice.writeData(Command.WriteSchmittTriggerB, self.mSchmittB.getValue())
      self.mDevice.writeData(Command.WriteSchmittTriggerC, self.mSchmittC.getValue())
      self.onStatusRefresh()

   def onPortAChanged(self, val):
      print "Port A: ", val

   def onPortBChanged(self, val):
      print "Port B: ", val

   def onPortCChanged(self, val):
      print "Port C: ", val


def main():
   app = QtGui.QApplication(sys.argv)

   # Create and display GUI
   v = ElexolEther24GUI()
   v.show()

   sys.exit(app.exec_())

if __name__ == '__main__':
   main()
