from PyQt4 import QtGui, QtCore

class ElexolPort(QtGui.QGroupBox):
   DEFAULT = 0
   DIRECTION = 1
   ONOFF = 2
   THRESHOLD = 3
   def __init__(self, parent=None, style = DEFAULT):
      QtGui.QGroupBox.__init__(self, parent)
      
      self.mInverted = False
      (on_pix, off_pix) = self.buildPixmaps(style)
      self.mPin0 = ElexolPin(on_pix, off_pix, self)
      self.mPin1 = ElexolPin(on_pix, off_pix, self)
      self.mPin2 = ElexolPin(on_pix, off_pix, self)
      self.mPin3 = ElexolPin(on_pix, off_pix, self)
      self.mPin4 = ElexolPin(on_pix, off_pix, self)
      self.mPin5 = ElexolPin(on_pix, off_pix, self)
      self.mPin6 = ElexolPin(on_pix, off_pix, self)
      self.mPin7 = ElexolPin(on_pix, off_pix, self)

      self.mLbl0 = QtGui.QLabel('0')
      self.mLbl1 = QtGui.QLabel('1')
      self.mLbl2 = QtGui.QLabel('2')
      self.mLbl3 = QtGui.QLabel('3')
      self.mLbl4 = QtGui.QLabel('4')
      self.mLbl5 = QtGui.QLabel('5')
      self.mLbl6 = QtGui.QLabel('6')
      self.mLbl7 = QtGui.QLabel('7')

      self.mLbl0.setAlignment(QtCore.Qt.AlignCenter)
      self.mLbl1.setAlignment(QtCore.Qt.AlignCenter)
      self.mLbl2.setAlignment(QtCore.Qt.AlignCenter)
      self.mLbl3.setAlignment(QtCore.Qt.AlignCenter)
      self.mLbl4.setAlignment(QtCore.Qt.AlignCenter)
      self.mLbl5.setAlignment(QtCore.Qt.AlignCenter)
      self.mLbl6.setAlignment(QtCore.Qt.AlignCenter)
      self.mLbl7.setAlignment(QtCore.Qt.AlignCenter)
     
      #sp = QtGui.QSizePolicy(QtGui.QSizePolicy.Policy(1),QtGui.QSizePolicy.Policy(1))
      #self.mLbl0.setSizePolicy(sp)
      #self.mLbl1.setSizePolicy(sp)
      #self.mLbl2.setSizePolicy(sp)
      #self.mLbl3.setSizePolicy(sp)
      #self.mLbl4.setSizePolicy(sp)
      #self.mLbl5.setSizePolicy(sp)
      #self.mLbl6.setSizePolicy(sp)
      #self.mLbl7.setSizePolicy(sp)

      sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Policy(1),QtGui.QSizePolicy.Policy(1))
      sizePolicy.setHorizontalStretch(0)
      sizePolicy.setVerticalStretch(0)
      #sizePolicy.setHeightForWidth(self.mPin2.sizePolicy().hasHeightForWidth())
      self.mPin0.setSizePolicy(sizePolicy)
      self.mPin1.setSizePolicy(sizePolicy)
      self.mPin2.setSizePolicy(sizePolicy)
      self.mPin3.setSizePolicy(sizePolicy)
      self.mPin4.setSizePolicy(sizePolicy)
      self.mPin5.setSizePolicy(sizePolicy)
      self.mPin6.setSizePolicy(sizePolicy)
      self.mPin7.setSizePolicy(sizePolicy)


      self.gridlayout = QtGui.QGridLayout(self)
      self.gridlayout.setMargin(9)
      self.gridlayout.setSpacing(6)

      self.gridlayout.addWidget(self.mLbl0,0,0,1,1)
      self.gridlayout.addWidget(self.mLbl1,0,1,1,1)
      self.gridlayout.addWidget(self.mLbl2,0,2,1,1)
      self.gridlayout.addWidget(self.mLbl3,0,3,1,1)
      self.gridlayout.addWidget(self.mPin0,1,0,1,1)
      self.gridlayout.addWidget(self.mPin1,1,1,1,1)
      self.gridlayout.addWidget(self.mPin2,1,2,1,1)
      self.gridlayout.addWidget(self.mPin3,1,3,1,1)
      self.gridlayout.addWidget(self.mLbl4,2,0,1,1)
      self.gridlayout.addWidget(self.mLbl5,2,1,1,1)
      self.gridlayout.addWidget(self.mLbl6,2,2,1,1)
      self.gridlayout.addWidget(self.mLbl7,2,3,1,1)
      self.gridlayout.addWidget(self.mPin4,3,0,1,1)
      self.gridlayout.addWidget(self.mPin5,3,1,1,1)
      self.gridlayout.addWidget(self.mPin6,3,2,1,1)
      self.gridlayout.addWidget(self.mPin7,3,3,1,1)

      self.connect(self.mPin0, QtCore.SIGNAL("clicked(bool)"), self.onClicked)
      self.connect(self.mPin1, QtCore.SIGNAL("clicked(bool)"), self.onClicked)
      self.connect(self.mPin2, QtCore.SIGNAL("clicked(bool)"), self.onClicked)
      self.connect(self.mPin3, QtCore.SIGNAL("clicked(bool)"), self.onClicked)
      self.connect(self.mPin4, QtCore.SIGNAL("clicked(bool)"), self.onClicked)
      self.connect(self.mPin5, QtCore.SIGNAL("clicked(bool)"), self.onClicked)
      self.connect(self.mPin6, QtCore.SIGNAL("clicked(bool)"), self.onClicked)
      self.connect(self.mPin7, QtCore.SIGNAL("clicked(bool)"), self.onClicked)

   def buildPixmaps(self, style):
   
      if ElexolPort.DIRECTION == style:
         highText = "In"
         lowText = "Out"
         highColor = QtGui.QColor(0, 200, 0, 200)
         lowColor = QtGui.QColor(200, 0, 0, 200)
      elif ElexolPort.ONOFF == style:
         highText = "On"
         lowText = "Off"
         highColor = QtGui.QColor(0, 200, 0, 200)
         lowColor = QtGui.QColor(200, 0, 0, 200)
      elif ElexolPort.THRESHOLD == style:
         highText = "2.5V"
         lowText = "1.4V"
         highColor = QtGui.QColor(0, 200, 0, 200)
         lowColor = QtGui.QColor(200, 0, 0, 200)
      else:
         highText = "On"
         lowText = "Off"
         highColor = QtGui.QColor(0, 200, 0, 200)
         lowColor = QtGui.QColor(200, 0, 0, 200)

      fm = QtGui.QFontMetrics(self.font())
      on_size = fm.size(QtCore.Qt.TextSingleLine, highText)
      on_maxsize = max(on_size.width(), on_size.height())
      off_size = fm.size(QtCore.Qt.TextSingleLine, lowText)
      off_maxsize = max(off_size.width(), off_size.height())
      maxsize = max(on_maxsize, off_maxsize)
      size = QtCore.QSize(maxsize, maxsize)

      on_image = QtGui.QImage(size.width(), size.height(), QtGui.QImage.Format_ARGB32_Premultiplied)
      on_image.fill(QtGui.qRgba(0, 0, 0, 0))
      off_image = QtGui.QImage(size.width(), size.height(), QtGui.QImage.Format_ARGB32_Premultiplied)
      off_image.fill(QtGui.qRgba(0, 0, 0, 0))

      font = QtGui.QFont()
      font.setStyleStrategy(QtGui.QFont.ForceOutline)

      painter = QtGui.QPainter()
      painter.begin(on_image)
      painter.setRenderHint(QtGui.QPainter.Antialiasing)
      painter.setBrush(highColor)
      painter.setPen(highColor)
      painter.drawEllipse(QtCore.QRect(QtCore.QPoint(0, 0), size))
      painter.setFont(font)
      painter.setBrush(QtCore.Qt.NoBrush)
      painter.setPen(QtCore.Qt.black)
      painter.drawText(QtCore.QRect(QtCore.QPoint(0, 0), size),
                       QtCore.Qt.AlignCenter, highText)
      painter.end()

      painter.begin(off_image)
      painter.setBrush(lowColor)
      painter.setPen(lowColor)
      painter.drawEllipse(QtCore.QRect(QtCore.QPoint(0, 0), size))
      painter.setFont(font)
      painter.setBrush(QtCore.Qt.NoBrush)
      painter.setPen(QtCore.Qt.black)
      painter.drawText(QtCore.QRect(QtCore.QPoint(0, 0), size),
                       QtCore.Qt.AlignCenter, lowText)

      painter.end()
      on_pix = QtGui.QPixmap.fromImage(on_image)
      off_pix = QtGui.QPixmap.fromImage(off_image)
      return (on_pix, off_pix)


   def setEnabled(self, val):
      if val < 0 or val > 255:
         print "Error: Invalid value [%s]" % (val)
         return
      self.mPin0.setEnabled(val & 0x1)
      self.mPin1.setEnabled(val & (0x1 << 1))
      self.mPin2.setEnabled(val & (0x1 << 2))
      self.mPin3.setEnabled(val & (0x1 << 3))
      self.mPin4.setEnabled(val & (0x1 << 4))
      self.mPin5.setEnabled(val & (0x1 << 5))
      self.mPin6.setEnabled(val & (0x1 << 6))
      self.mPin7.setEnabled(val & (0x1 << 7))
      self.mPin0.setFlat(not(val & 0x1))
      self.mPin1.setFlat(not(val & (0x1 << 1)))
      self.mPin2.setFlat(not(val & (0x1 << 2)))
      self.mPin3.setFlat(not(val & (0x1 << 3)))
      self.mPin4.setFlat(not(val & (0x1 << 4)))
      self.mPin5.setFlat(not(val & (0x1 << 5)))
      self.mPin6.setFlat(not(val & (0x1 << 6)))
      self.mPin7.setFlat(not(val & (0x1 << 7)))


   def setValue(self, val):
      if val < 0 or val > 255:
         print "Error: Invalid value [%s]" % (val)
         return
      if self.mInverted:
         real_val = 255-val
      else:
         real_val = val

      self.mPin0.setChecked(real_val & 0x1)
      self.mPin1.setChecked(real_val & (0x1 << 1))
      self.mPin2.setChecked(real_val & (0x1 << 2))
      self.mPin3.setChecked(real_val & (0x1 << 3))
      self.mPin4.setChecked(real_val & (0x1 << 4))
      self.mPin5.setChecked(real_val & (0x1 << 5))
      self.mPin6.setChecked(real_val & (0x1 << 6))
      self.mPin7.setChecked(real_val & (0x1 << 7))

   def getValue(self):
      val = ((self.mPin0.isChecked() << 0) |
             (self.mPin1.isChecked() << 1) |
             (self.mPin2.isChecked() << 2) |
             (self.mPin3.isChecked() << 3) |
             (self.mPin4.isChecked() << 4) |
             (self.mPin5.isChecked() << 5) |
             (self.mPin6.isChecked() << 6) |
             (self.mPin7.isChecked() << 7))
      if self.mInverted:
         return 255-val
      else:
         return val

   def onClicked(self, checked):
      self.emit(QtCore.SIGNAL("valueChanged(int)"), self.getValue())

class ElexolPin(QtGui.QPushButton):
   def __init__(self, onPix = None, offPix = None, parent = None):
      QtGui.QPushButton.__init__(self, parent)

      self.connect(self, QtCore.SIGNAL("toggled(bool)"), self.onToggle)
      self.setCheckable(True)

      self.mOnIcon = QtGui.QIcon(onPix)
      self.mOnIcon.addPixmap(onPix, QtGui.QIcon.Normal)
      self.mOnIcon.addPixmap(onPix, QtGui.QIcon.Disabled)
      self.mOffIcon = QtGui.QIcon(offPix)
      self.setIcon(self.mOffIcon)
      self.mOffIcon.addPixmap(offPix, QtGui.QIcon.Normal)
      self.mOffIcon.addPixmap(offPix, QtGui.QIcon.Disabled)

   def onToggle(self, clicked):
      if clicked:
         self.setIcon(self.mOnIcon)
      else:
         self.setIcon(self.mOffIcon)

   def onClick(self, clicked):
      print getValue()
