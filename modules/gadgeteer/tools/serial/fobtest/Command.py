#!/bin/env/python

Angles = 0x57
ChangeValue = 0x50
ExamineValue = 0x4F
FbbReset = 0x2F
Hemisphere = 0x4C
Matrix = 0x58
NextTransmitter = 0x30
Point = 0x42
Position = 0x56
PositionAngles = 0x59
PositionMatrix = 0x5A
PositionQuaternion = 0x5D
Quaternion = 0x5C
ReportRate1 = 0x51
ReportRate2 = 0x52
ReportRate8 = 0x53
ReportRate32 = 0x54
ToFbbNormal = 0xF0
ToFbbExpanded = 0xE0
ToFbbSuperExpanded = 0xA0
Run = 0x46
Sleep = 0x47
Stream = 0x40
StreamStop = 0x3F

# Dictionary mapping examine command to code and return size
ExamineParam = {
   "BirdStatus" : (0x0,2),
   "SoftwareRevision" : (0x01,2),
   "BirdErrorCode" : (0x10,1),
   "ModelIdentification" : (0xF,10),
   "AddressingMode" : (0x13,1),
   "FbbAddress" : (0x15,1),
   "Hemisphere" : (0x16,2),
   "GroupMode" : (0x23,1),
   "FlockSystemStatus" : (0x24,14),
   "FbbAutoConfig" : (0x32,12),
}  