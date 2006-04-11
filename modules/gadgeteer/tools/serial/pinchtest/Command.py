#!/bin/env/python

#Angles = 0x57
#ChangeValue = 0x50
#ExamineValue = 0x4F
#FbbReset = 0x2F
#Hemisphere = 0x4C
#Matrix = 0x58
#NextTransmitter = 0x30
#Point = 0x42
#Position = 0x56
#PositionAngles = 0x59
#PositionMatrix = 0x5A
#PositionQuaternion = 0x5D
#Quaternion = 0x5C
#ReportRate1 = 0x51
#ReportRate2 = 0x52
#ReportRate8 = 0x53
#ReportRate32 = 0x54
#ToFbbNormal = 0xF0
#ToFbbExpanded = 0xE0
#ToFbbSuperExpanded = 0xA0
#Run = 0x46
#Sleep = 0x47
#Stream = 0x40
#StreamStop = 0x3F

START_BYTE_NTS = 0x80
START_BYTE_TS = 0x81
START_BYTE = 0x82
END_BYTE = 0x8F

# Dictionary mapping examine command to code and return size
ExamineParam = {
   "LeftVersion" : (0x43,0x4C),
   "RightVersion" : (0x43,0x52),
   "ProcessorCode" : (0x43, 0x50),
   "TickLength" : (0x43, 0x54),
   "TurnTimeStampsOn" : (0x54,0x31),
   "TurnTimeStampsOff" : (0x54,0x30),
   "DataFormatVersion" : (0x56,0x31),
}  
