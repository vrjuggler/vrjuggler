#!/bin/env/python

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
