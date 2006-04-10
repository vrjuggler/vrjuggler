import struct

# Write Values
WritePortA = 'A'
WritePortB = 'B'
WritePortC = 'C'
# Read Values
ReadPortA = ('a',  2, '1cB')
ReadPortB = ('b',  2, '1cB')
ReadPortC = ('c',  2, '1cB')
# Write Direction Register
WriteDirectionA = '!A'
WriteDirectionB = '!B'
WriteDirectionC = '!C'
# Identify IO24 Units
Find = ('IO24', 12, '4c8B')
ReadDirectionA = ('!a', 3, "2cB")
ReadDirectionB = ('!b', 3, "2cB")
ReadDirectionC = ('!c', 3, "2cB")
# Write Pull Up Register
WritePullUpA = '@A'
WritePullUpB = '@B'
WritePullUpC = '@C'
# Write Threshold Register
WriteThresholdA = '#A'
WriteThresholdB = '#B'
WriteThresholdC = '#C'
# Write Schmitt Trigger Register
WriteSchmittTriggerA = '$A'
WriteSchmittTriggerB = '$B'
WriteSchmittTriggerC = '$C'
# Read Pull Up Register
ReadPullUpA = ('@a', 3, '2cB')
ReadPullUpB = ('@b', 3, '2cB')
ReadPullUpC = ('@c', 3, '2cB')
# Read Threshold Register
ReadThresholdA = ('#a', 3, '2cB')
ReadThresholdB = ('#b', 3, '2cB')
ReadThresholdC = ('#c', 3, '2cB')
# Read Schmitt Trigger Register
ReadSchmittTriggerA = ('$a', 3, '2cB')
ReadSchmittTriggerB = ('$b', 3, '2cB')
ReadSchmittTriggerC = ('$c', 3, '2cB')

def ReadEEPROMWord(address):
   return (struct.pack('2c3B', '\'', 'R', address, 0, 0), 4)

def WriteEEPROMWord(address, msb, lsb):
   return struct.pack('2c3B', '\'', 'W', address, msb, lsb)

def EraseEEPROMWord(address):
   return struct.pack('2c3B', '\'', 'E', address, 0xAA, 0x55)

"""
Three registers for each port control the six different input modes.

Threshold Register - If bit is high then the threshold voltage is 1.4V and if low 2.5V.
Schmitt Trigger Register - Overrides the threshold register when the corresponding bit is low, enabling the Schmitt Trigger input state.
Pull-Up register - Enables the corresponding pull-up resistor when the bit is low.

The default power-up state for all registers is all high bit (TTL Input, No Pull-Up) unless the EEPROM has been programmed to set them up differently.
"""



"""
Command | Command   |Bytes|      Data       |            Function                   | Response |  Response  |  Response Data
 ASCII  |   Hex     |     |                 |                                       |  Bytes   | Identifier |
   A    | 0x41      |  2  |   Port_Value    |          Write Port A                 |    -     |     -      |       -
   B    | 0x42      |  2  |   Port_Value    |          Write Port B                 |    -     |     -      |       -
   C    | 0x43      |  2  |   Port_Value    |          Write Port C                 |    -     |     -      |       -
   a    | 0x61      |  1  |       -         |           Read Port A                 |    2     |     A      |   PortA_Value
   b    | 0x62      |  1  |       -         |           Read Port B                 |    2     |     B      |   PortB_Value
   c    | 0x63      |  1  |       -         |           Read Port C                 |    2     |     C      |   PortC_Value
   !A   | 0x21 0x41 |  3  |    Direction    |    Write Port A Direction Register    |    -     |     -      |       -
   !B   | 0x21 0x42 |  3  |    Direction    |    Write Port B Direction Register    |    -     |     -      |       -
   !C   | 0x21 0x43 |  3  |    Direction    |    Write Port C Direction Register    |    -     |     -      |       -
  IO24  | 0x49 0x4F |  4  |        -        |        Identify IO24 Units            |    12    |    IO24    | 6 Byte MAC Address
        | 0x32 0x34 |     |                 |                                       |          |            | 2 Byte FW Version
   !a   | 0x21 0x61 |  2  |        -        |    Read Port A Direction Register     |    3     |     !A     |   Direction
   !b   | 0x21 0x62 |  2  |        -        |    Read Port B Direction Register     |    3     |     !B     |   Direction
   !c   | 0x21 0x63 |  2  |        -        |    Read Port C Direction Register     |    3     |     !C     |   Direction
   @A   | 0x40 0x41 |  3  |     Pull_Up     |     Write Port A Pull Up Register     |    -     |     -      |       -
   @B   | 0x40 0x42 |  3  |     Pull_Up     |     Write Port B Pull Up Register     |    -     |     -      |       -
   @C   | 0x40 0x43 |  3  |     Pull_Up     |     Write Port C Pull Up Register     |    -     |     -      |       -
   #A   | 0x23 0x41 |  3  |    Threshold    |    Write Port A Threshold Register    |    -     |     -      |       -
   #B   | 0x23 0x42 |  3  |    Threshold    |    Write Port B Threshold Register    |    -     |     -      |       -
   #C   | 0x23 0x43 |  3  |    Threshold    |    Write Port C Threshold Register    |    -     |     -      |       -
   $A   | 0x24 0x41 |  3  |     Schmitt     | Write Port A Schmitt Trigger Register |    -     |     -      |       -
   $B   | 0x24 0x42 |  3  |     Schmitt     | Write Port B Schmitt Trigger Register |    -     |     -      |       -
   $C   | 0x24 0x43 |  3  |     Schmitt     | Write Port C Schmitt Trigger Register |    -     |     -      |       -
   @a   | 0x40 0x61 |  2  |        -        |     Read Port A Pull Up Register      |    3     |     @A     |    Pull_Up
   @b   | 0x40 0x62 |  2  |        -        |     Read Port B Pull Up Register      |    3     |     @B     |    Pull_Up
   @c   | 0x40 0x63 |  2  |        -        |     Read Port C Pull Up Register      |    3     |     @C     |    Pull_Up
   #a   | 0x23 0x61 |  2  |        -        |    Read Port A Threshold Register     |    3     |     #A     |   Threshold
   #b   | 0x23 0x62 |  2  |        -        |    Read Port B Threshold Register     |    3     |     #B     |   Threshold
   #c   | 0x23 0x63 |  2  |        -        |    Read Port C Threshold Register     |    3     |     #C     |   Threshold
   $a   | 0x24 0x61 |  2  |        -        | Read Port A Schmitt Trigger Register  |    3     |     $A     |    Schmitt
   $b   | 0x24 0x62 |  2  |        -        | Read Port B Schmitt Trigger Register  |    3     |     $B     |    Schmitt
   $c   | 0x24 0x63 |  2  |        -        | Read Port C Schmitt Trigger Register  |    3     |     $C     |    Schmitt
   'R   | 0x27 0x52 |  5  |  Address NU NU  |         Read EEPROM Word              |    4     |     R      | Address MSB LSB
   'W   | 0x27 0x57 |  5  | Address MSB LSB |         Write EEPROM Word             |    -     |     -      |       -
   'E   | 0x27 0x45 |  5  | Address $AA $55 |         Erase EEPROM Word             |    -     |     -      |       -
   '0   | 0x27 0x30 |  5  |    NU NU NU     |        Write Disable EEPROM           |    -     |     -      |       -
   '1   | 0x27 0x31 |  5  |   NU $AA $55    |        Write Enable EEPROM            |    -     |     -      |       -
   '@   | 0x27 0x52 |  5  |   NU $AA $55    |            Reset Module               |    -     |     -      |       -
   `    | 0x60      |  2  |      Byte       |      Echo Byte back to sender         |    2     |     `      |      Byte
   *    | 0x2A      |  1  |        -        |      Send Space back to sender        |    1     |  'Space'   |       -
   %    | 0x25      |  1  |        -        |           Send Host Data              |    16    |     %      | 3 byte IO24 Serial
        |           |     |                 |                                       |          |            | 4 byte Sender IP
        |           |     |                 |                                       |          |            | 6 byte Sender MAC
        |           |     |                 |                                       |          |            | 2 byte Sender Port
"""

"""
Address MSB Function                      LSB Function
=========================================================================
0-4   | Reserved (Unwritable)          |  Reserved (Unwritable)
5     | Control Bits 2                 |  Control Bits 1
6     | Fixed IP Address Byte 2        |  Fixed IP Address Byte 1
7     | Fixed IP Address Byte 4        |  Fixed IP Address Byte 3
8     | Preset Port A Value            |  Preset Port A Direction
9     | Preset Port A Pull up          |  Preset Port A Threshold
10    | Preset Port B Direction        |  Preset Port A Schmitt Trigger
11    | Preset Port B Threshold        |  Preset Port B Value
12    | Preset Port B Schmitt Trigger  |  Preset Port B Pull up
13    | Preset Port C Value            |  Preset Port C Direction
14    | Preset Port C Pull up          |  Preset Port C Threshold
15    | Reserved for Future Use        |  Preset Port C Schmitt Trigger
16    | AutoScan Port B Mask           |  AutoScan Port A Mask
17    | AutoScan Filter Count          |  AutoScan Port C Mask
18    | AutoScan Scan Rate MSB         |  AutoScan Scan Rate LSB
19    | AutoScan Target MAC Address 2  |  AutoScan Target MAC Address 1
20    | AutoScan Target MAC Address 4  |  AutoScan Target MAC Address 3
21    | AutoScan Target MAC Address 6  |  AutoScan Target MAC Address 5
22    | AutoScan Target IP Address 2   |  AutoScan Target IP Address 1
23    | AutoScan Target IP Address 4   |  AutoScan Target IP Address 3
24    | AutoScan Target Port MSB       |  AutoScan Target Port LSB
25-47 | Reserved for Future Use        |
48-63 | For your own use               |
"""

"""
Control Bits 1
  Fixed IP function     - 1
     0 - Enable
     1 - Disable
  Preset Port function  - 2
     0 - Reads 8-15 into Port Registers
     1 - Nothing
  AutoScan function     - 4
     0 - Enable
     1 - Disable

  Value to write = 255 - (Enable Fixed IP | Enable Preset Port | Enable AutoScan)
"""

def getLineBit(lineNum):
   return pow(2, (lineNum - 1))

WriteDisableEEPROM = struct.pack('2c3B', '\'', '0', 0, 0, 0)
WriteEnableEEPROM = struct.pack('2c3B', '\'', '1', 0, 0xAA, 0x55)
ResetModule = struct.pack('2c3B', '\'', '@', 0, 0xAA, 0x55) 
EchoByte = ('`', 2)
EchoSpace = ('*', 1)
SendHostData = ('%', 16, '1c3B4B6B1h')
