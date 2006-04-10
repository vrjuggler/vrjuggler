# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'Ether24Base.ui'
#
#      by: PyQt4 UI code generator vsnapshot-20060221
#
# WARNING! All changes made in this file will be lost!

import sys
from PyQt4 import QtCore, QtGui

class Ui_Ether24Base(object):
   def setupUi(self, Ether24Base):
      Ether24Base.setObjectName("Ether24Base")
      Ether24Base.resize(QtCore.QSize(QtCore.QRect(0,0,600,326).size()).expandedTo(Ether24Base.minimumSizeHint()))
      
      self.mCentralWidget = QtGui.QWidget(Ether24Base)
      self.mCentralWidget.setObjectName("mCentralWidget")
      
      self.vboxlayout = QtGui.QVBoxLayout(self.mCentralWidget)
      self.vboxlayout.setMargin(9)
      self.vboxlayout.setSpacing(6)
      self.vboxlayout.setObjectName("vboxlayout")
      
      self.hboxlayout = QtGui.QHBoxLayout()
      self.hboxlayout.setMargin(0)
      self.hboxlayout.setSpacing(6)
      self.hboxlayout.setObjectName("hboxlayout")
      
      self.mDeviceCB = QtGui.QComboBox(self.mCentralWidget)
      self.mDeviceCB.setObjectName("mDeviceCB")
      self.hboxlayout.addWidget(self.mDeviceCB)
      
      self.mFindDevices = QtGui.QToolButton(self.mCentralWidget)
      self.mFindDevices.setObjectName("mFindDevices")
      self.hboxlayout.addWidget(self.mFindDevices)
      self.vboxlayout.addLayout(self.hboxlayout)
      
      self.mStatusWidget = QtGui.QTabWidget(self.mCentralWidget)
      self.mStatusWidget.setTabPosition(QtGui.QTabWidget.North)
      self.mStatusWidget.setTabShape(QtGui.QTabWidget.Rounded)
      self.mStatusWidget.setObjectName("mStatusWidget")
      
      self.mStatusTab = QtGui.QWidget()
      self.mStatusTab.setObjectName("mStatusTab")
      
      self.vboxlayout1 = QtGui.QVBoxLayout(self.mStatusTab)
      self.vboxlayout1.setMargin(9)
      self.vboxlayout1.setSpacing(6)
      self.vboxlayout1.setObjectName("vboxlayout1")
      
      self.hboxlayout1 = QtGui.QHBoxLayout()
      self.hboxlayout1.setMargin(0)
      self.hboxlayout1.setSpacing(6)
      self.hboxlayout1.setObjectName("hboxlayout1")
      
      spacerItem = QtGui.QSpacerItem(40,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Minimum)
      self.hboxlayout1.addItem(spacerItem)
      
      self.mStatusRefresh = QtGui.QPushButton(self.mStatusTab)
      self.mStatusRefresh.setObjectName("mStatusRefresh")
      self.hboxlayout1.addWidget(self.mStatusRefresh)
      self.vboxlayout1.addLayout(self.hboxlayout1)
      self.mStatusWidget.addTab(self.mStatusTab, "")
      
      self.mDirectionTab = QtGui.QWidget()
      self.mDirectionTab.setObjectName("mDirectionTab")
      
      self.vboxlayout2 = QtGui.QVBoxLayout(self.mDirectionTab)
      self.vboxlayout2.setMargin(9)
      self.vboxlayout2.setSpacing(6)
      self.vboxlayout2.setObjectName("vboxlayout2")
      
      self.hboxlayout2 = QtGui.QHBoxLayout()
      self.hboxlayout2.setMargin(0)
      self.hboxlayout2.setSpacing(6)
      self.hboxlayout2.setObjectName("hboxlayout2")
      
      spacerItem1 = QtGui.QSpacerItem(40,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Minimum)
      self.hboxlayout2.addItem(spacerItem1)
      
      self.mDirectionRefresh = QtGui.QPushButton(self.mDirectionTab)
      self.mDirectionRefresh.setObjectName("mDirectionRefresh")
      self.hboxlayout2.addWidget(self.mDirectionRefresh)
      self.vboxlayout2.addLayout(self.hboxlayout2)
      self.mStatusWidget.addTab(self.mDirectionTab, "")
      
      self.mPullupTab = QtGui.QWidget()
      self.mPullupTab.setObjectName("mPullupTab")
      
      self.vboxlayout3 = QtGui.QVBoxLayout(self.mPullupTab)
      self.vboxlayout3.setMargin(9)
      self.vboxlayout3.setSpacing(6)
      self.vboxlayout3.setObjectName("vboxlayout3")
      
      self.hboxlayout3 = QtGui.QHBoxLayout()
      self.hboxlayout3.setMargin(0)
      self.hboxlayout3.setSpacing(6)
      self.hboxlayout3.setObjectName("hboxlayout3")
      
      spacerItem2 = QtGui.QSpacerItem(40,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Minimum)
      self.hboxlayout3.addItem(spacerItem2)
      
      self.mPullUpRefresh = QtGui.QPushButton(self.mPullupTab)
      self.mPullUpRefresh.setObjectName("mPullUpRefresh")
      self.hboxlayout3.addWidget(self.mPullUpRefresh)
      self.vboxlayout3.addLayout(self.hboxlayout3)
      self.mStatusWidget.addTab(self.mPullupTab, "")
      
      self.mThresholdTab = QtGui.QWidget()
      self.mThresholdTab.setObjectName("mThresholdTab")
      
      self.vboxlayout4 = QtGui.QVBoxLayout(self.mThresholdTab)
      self.vboxlayout4.setMargin(9)
      self.vboxlayout4.setSpacing(6)
      self.vboxlayout4.setObjectName("vboxlayout4")
      
      self.hboxlayout4 = QtGui.QHBoxLayout()
      self.hboxlayout4.setMargin(0)
      self.hboxlayout4.setSpacing(6)
      self.hboxlayout4.setObjectName("hboxlayout4")
      
      spacerItem3 = QtGui.QSpacerItem(40,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Minimum)
      self.hboxlayout4.addItem(spacerItem3)
      
      self.mThresholdRefresh = QtGui.QPushButton(self.mThresholdTab)
      self.mThresholdRefresh.setObjectName("mThresholdRefresh")
      self.hboxlayout4.addWidget(self.mThresholdRefresh)
      self.vboxlayout4.addLayout(self.hboxlayout4)
      self.mStatusWidget.addTab(self.mThresholdTab, "")
      
      self.mSchmittTriggerTab = QtGui.QWidget()
      self.mSchmittTriggerTab.setObjectName("mSchmittTriggerTab")
      
      self.vboxlayout5 = QtGui.QVBoxLayout(self.mSchmittTriggerTab)
      self.vboxlayout5.setMargin(9)
      self.vboxlayout5.setSpacing(6)
      self.vboxlayout5.setObjectName("vboxlayout5")
      
      self.hboxlayout5 = QtGui.QHBoxLayout()
      self.hboxlayout5.setMargin(0)
      self.hboxlayout5.setSpacing(6)
      self.hboxlayout5.setObjectName("hboxlayout5")
      
      spacerItem4 = QtGui.QSpacerItem(40,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Minimum)
      self.hboxlayout5.addItem(spacerItem4)
      
      self.mSchmittRefresh = QtGui.QPushButton(self.mSchmittTriggerTab)
      self.mSchmittRefresh.setObjectName("mSchmittRefresh")
      self.hboxlayout5.addWidget(self.mSchmittRefresh)
      self.vboxlayout5.addLayout(self.hboxlayout5)
      self.mStatusWidget.addTab(self.mSchmittTriggerTab, "")
      self.vboxlayout.addWidget(self.mStatusWidget)
      Ether24Base.setCentralWidget(self.mCentralWidget)
      
      self.mMenuBar = QtGui.QMenuBar(Ether24Base)
      self.mMenuBar.setGeometry(QtCore.QRect(0,0,600,29))
      self.mMenuBar.setObjectName("mMenuBar")
      Ether24Base.setMenuBar(self.mMenuBar)
      
      self.mStatusBar = QtGui.QStatusBar(Ether24Base)
      self.mStatusBar.setGeometry(QtCore.QRect(0,304,600,22))
      self.mStatusBar.setObjectName("mStatusBar")
      Ether24Base.setStatusBar(self.mStatusBar)
      
      self.retranslateUi(Ether24Base)
      QtCore.QMetaObject.connectSlotsByName(Ether24Base)
   
   def tr(self, string):
      return QtGui.QApplication.translate("Ether24Base", string, None, QtGui.QApplication.UnicodeUTF8)
   
   def retranslateUi(self, Ether24Base):
      Ether24Base.setWindowTitle(self.tr("Elexol Ether I/O 24"))
      self.mFindDevices.setText(self.tr("..."))
      self.mStatusRefresh.setText(self.tr("&Refresh"))
      self.mStatusWidget.setTabText(self.mStatusWidget.indexOf(self.mStatusTab), self.tr("&Status"))
      self.mDirectionRefresh.setText(self.tr("&Refresh"))
      self.mStatusWidget.setTabText(self.mStatusWidget.indexOf(self.mDirectionTab), self.tr("&Direction"))
      self.mPullUpRefresh.setText(self.tr("&Refresh"))
      self.mStatusWidget.setTabText(self.mStatusWidget.indexOf(self.mPullupTab), self.tr("&Pull Up"))
      self.mThresholdRefresh.setText(self.tr("&Refresh"))
      self.mStatusWidget.setTabText(self.mStatusWidget.indexOf(self.mThresholdTab), self.tr("&Threshold"))
      self.mSchmittRefresh.setText(self.tr("&Refresh"))
      self.mStatusWidget.setTabText(self.mStatusWidget.indexOf(self.mSchmittTriggerTab), self.tr("Sch&mitt Trigger"))

if __name__ == "__main__":
   app = QtGui.QApplication(sys.argv)
   Ether24Base = QtGui.QMainWindow()
   ui = Ui_Ether24Base()
   ui.setupUi(Ether24Base)
   Ether24Base.show()
   sys.exit(app.exec_())
