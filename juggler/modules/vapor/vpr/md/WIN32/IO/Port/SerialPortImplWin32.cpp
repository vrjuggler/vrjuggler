/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <vpr/md/WIN32/IO/Port/SerialPortImplWin32.h>
#include <vpr/Util/Debug.h>


namespace vpr{

// ----------------------------------------------------------------------------
// Constructor.  This sets all the default values for the given port name.
// ----------------------------------------------------------------------------
SerialPortImplWin32::SerialPortImplWin32 (const std::string& port_name)
    : m_name(port_name), m_handle(NULL)
{
    openFlag = GENERIC_READ | GENERIC_WRITE;
    m_blocking = true;
    parityMark = false;
}

// ----------------------------------------------------------------------------
// Destructor.  If the file handle is non-NULL, its memory is released.
// ----------------------------------------------------------------------------
SerialPortImplWin32::~SerialPortImplWin32 () {
    if ( m_open ) {
        close();
    }
}

// ----------------------------------------------------------------------------
// Open the serial port and initialize its flags.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::open () {
    vpr::Status status;
    COMMTIMEOUTS gct;
    m_handle = CreateFile( m_name.c_str(),
                     openFlag,
                     0,    // comm devices must be opened w/exclusive-access
                     NULL, // no security attributes
                     OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                     0,    // not overlapped I/O
                     NULL  // hTemplate must be NULL for comm devices
                     );

    if (m_handle == INVALID_HANDLE_VALUE) {  // Handle the error.
        printf ("CreateFile failed with error %d.\n", GetLastError());
        status.setCode(vpr::Status::Failure);
    }
    gct.ReadIntervalTimeout =0;
    gct.ReadTotalTimeoutConstant=0;
    gct.ReadTotalTimeoutMultiplier=0;
    gct.WriteTotalTimeoutConstant=0;
    gct.WriteTotalTimeoutMultiplier=0;
    if(!SetCommTimeouts(m_handle,&gct)){
        printf("Timeout initialization failed.");
        status.setCode(vpr::Status::Failure);
    }
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.ErrorChar = 122;
    SetCommState(m_handle, &dcb);


    m_open = true;
    return status;
}

vpr::SerialTypes::UpdateActionOption getUpdateAction(void){
    std::cout << "Update Action is always NOW in Win32" << std::endl;
    vpr::SerialTypes::UpdateActionOption update;
    update = SerialTypes::NOW;
    return update;
}


void SerialPortImplWin32::setUpdateAction (SerialTypes::UpdateActionOption action)
{
    /* Do Nothing */
    std::cout << "Update action always NOW in Win32" << std::endl;
}

// ----------------------------------------------------------------------------
// Query the serial port for the maximum buffer size.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::getBufferSize(vpr::Uint16 &size){
    vpr::Status s;
    COMMPROP lpCommProp;
    if(!GetCommProperties(m_handle, &lpCommProp) || (int)lpCommProp.dwCurrentTxQueue == 0){
        s.setCode(vpr::Status::Failure);
        std::cout << "Maximum buffer size is unavailable.\n";
    }else{
        size = lpCommProp.dwCurrentTxQueue;
    }

    return s;
}

// ----------------------------------------------------------------------------
// Attempt to change the buffer size to the given argument.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::setBufferSize(const vpr::Uint8 size){
    Status s;
    if(!SetupComm(m_handle, (int)size, (int)size)){
        s.setCode(vpr::Status::Failure);
        std::cout << "Could not set the minimum buffer size.\n";
    }
    return s;
}

// ----------------------------------------------------------------------------
// Get the value of the timeout (in tenths of a second) to wait for data to
// arrive.  This is only applicable in non-canonical mode.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::getTimeout (vpr::Uint8& timeout) {
    COMMTIMEOUTS t;
    vpr::Status retval;
    if(!GetCommTimeouts(m_handle, &t)){
        retval.setCode(vpr::Status::Failure);
        std::cout << "The timeout value is unavailable.\n";
    }
    timeout = (int)t.ReadTotalTimeoutConstant/100;
    return retval;
}

// ----------------------------------------------------------------------------
// Set the value of the timeout to wait for data to arrive.  The value given
// must be in tenths of a second.  This is only applicable in non-canonical
// mode.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::setTimeout (const vpr::Uint8 timeout) {
    COMMTIMEOUTS t;
    vpr::Status retval;
    GetCommTimeouts(m_handle, &t);

    t.ReadTotalTimeoutConstant = (int)timeout*100;
    if(!SetCommTimeouts(m_handle, &t)){
        retval.setCode(vpr::Status::Failure);
        std::cout << "Could not set timeout value.\n";
    }
//        std::cout << t.ReadIntervalTimeout << " : " << t.ReadTotalTimeoutConstant << " : " << t.ReadTotalTimeoutMultiplier << " : " << t.WriteTotalTimeoutConstant << " : " << t.WriteTotalTimeoutMultiplier << std::endl;
    return retval;
}


// ----------------------------------------------------------------------------
// Get the character size (the bits per byte).
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::getCharacterSize (vpr::SerialTypes::CharacterSizeOption& size)
{
    vpr::Status retval;
    DCB dcb;
    if(GetCommState(m_handle, &dcb)){
        switch(dcb.ByteSize){
        case 5:
            size = vpr::SerialTypes::CS_BITS_5;
            break;
        case 6:
            size = vpr::SerialTypes::CS_BITS_6;
            break;
        case 7:
            size = vpr::SerialTypes::CS_BITS_7;
            break;
        case 8:
            size = vpr::SerialTypes::CS_BITS_8;
            break;
        }
    }else{
        retval.setCode(vpr::Status::Failure);
        std::cout << "Error attaining bits/byte.\n";
    }
    return retval;
}

// ----------------------------------------------------------------------------
// Set the current character size (the bits per byte) to the size in the given
// value.  This is used for both reding and writing, and the size does not
// include the parity bit (if any).
// ----------------------------------------------------------------------------
vpr::Status
SerialPortImplWin32::setCharacterSize (const vpr::SerialTypes::CharacterSizeOption bpb)
{
    vpr::Status retval;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    switch(bpb){
    case vpr::SerialTypes::CS_BITS_5:
        dcb.ByteSize = 5;
        break;
    case vpr::SerialTypes::CS_BITS_6:
        dcb.ByteSize = 6;
        break;
    case vpr::SerialTypes::CS_BITS_7:
        dcb.ByteSize = 7;
        break;
    case vpr::SerialTypes::CS_BITS_8:
        dcb.ByteSize = 8;
        break;
    }
    if(!SetCommState(m_handle,&dcb)){
        std::cout << GetLastError()<< std::endl<<std::endl;
        retval.setCode(vpr::Status::Failure);
        std::cout << "Error setting bits/byte.\n";
    }
    return retval;
}



vpr::Status SerialPortImplWin32::getStopBits (vpr::Uint8& num_bits) {
    DCB dcb;
    
    vpr::Status retval;

    if (GetCommState(m_handle, &dcb)) {
        switch (dcb.StopBits) {
          case ONESTOPBIT:
            num_bits = 1;
            break;
          case TWOSTOPBITS:
            num_bits = 2;
            break;
        }
    }else{
        std::cout << "Number of stop bits is unavailable.\n";
        retval.setCode(vpr::Status::Failure);
    }
    return retval;
}


// ----------------------------------------------------------------------------
// Set the number of stop bits to use.  The value must be either 1 or 2.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::setStopBits (const vpr::Uint8 num_bits) {
    DCB dcb;
    
    vpr::Status retval;

    GetCommState(m_handle, &dcb);
    switch((int)num_bits){
    case 1:
        dcb.StopBits = ONESTOPBIT;
        break;
    case 2:
        dcb.StopBits = TWOSTOPBITS;
        break;
    }
    if(!SetCommState(m_handle, &dcb)){
        std::cout << "Error in setting stop bits.\n";
        retval.setCode(vpr::Status::Failure);
    }
    return retval;
}

// ----------------------------------------------------------------------------
// Get the state of parity checking for input.
// ----------------------------------------------------------------------------
bool SerialPortImplWin32::getInputParityCheckState () {
    DCB dcb;
    bool b;
    if(!GetCommState(m_handle, &dcb)){
        std::cout << "Error attaining parity checking state.\n";
    }
    if(dcb.fParity == true){
        b = true;
    }else{
        b = false;
    }
    return b;
}

// ----------------------------------------------------------------------------
// Enable input parity checking.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::enableInputParityCheck () {
    vpr::Status s;
    DCB dcb;
    
    if(!GetCommState(m_handle, &dcb))
        s.setCode(vpr::Status::Failure);

    dcb.fParity = 1;
    if(!SetCommState(m_handle, &dcb))
        s.setCode(vpr::Status::Failure);
    
    return s;
}
    

// ----------------------------------------------------------------------------
// Disable input parity checking.
// ----------------------------------------------------------------------------
vpr::Status
SerialPortImplWin32::disableInputParityCheck () {
    vpr::Status s;
    DCB dcb;
    GetCommState(m_handle,&dcb);
    dcb.fParity = false;
    SetCommState(m_handle,&dcb);
    return s;
}

// ----------------------------------------------------------------------------
// Get the current parity checking type (either odd or even).
// ----------------------------------------------------------------------------
vpr::SerialTypes::ParityType SerialPortImplWin32::getParity () {
    DCB dcb;
    GetCommState(m_handle, &dcb);
    if(dcb.Parity == EVENPARITY){
        return vpr::SerialTypes::PORT_PARITY_EVEN;
    }
    if(dcb.Parity == ODDPARITY){
        return vpr::SerialTypes::PORT_PARITY_ODD;
    }else{
        std::cout << "error in attaining parity type\n";
        return vpr::SerialTypes::PORT_PARITY_ODD;
    }
}

// ----------------------------------------------------------------------------
// Enable odd parity.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::setOddParity () {
    Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.Parity = ODDPARITY;
    SetCommState(m_handle, &dcb);
    return s;
}

// ----------------------------------------------------------------------------
// Enable even parity.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::setEvenParity () {
    vpr::Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.Parity = EVENPARITY;
    SetCommState(m_handle, &dcb);
    return s;
}



vpr::Status SerialPortImplWin32::write_i (const void* buffer,
                                          const vpr::Uint32 length,
                                          vpr::Uint32& bytes_written,
                                          const vpr::Interval timeout)
{
    vpr::Status s;
    unsigned long bytes;
    
   if(vpr::Interval::NoTimeout != timeout)
       vprDEBUG(0,vprDBG_WARNING_LVL) << "Timeout not supported\n" << vprDEBUG_FLUSH;

   if(!WriteFile(m_handle, buffer, length, &bytes, NULL)){
        s.setCode(vpr::Status::Failure);
        bytes_written = bytes;
    }
    return s;

    
}

vpr::Status SerialPortImplWin32::read_i(void* buffer, const vpr::Uint32 length,
                                        vpr::Uint32& bytes_read,
                                        const vpr::Interval timeout)
{
    vpr::Status s;
    unsigned long bytes;

   if(vpr::Interval::NoTimeout != timeout)
       vprDEBUG(0,vprDBG_WARNING_LVL) << "Timeout not supported\n" << vprDEBUG_FLUSH;

    if(!ReadFile( m_handle, buffer, length, &bytes,NULL)){
        s.setCode(vpr::Status::Failure);
        bytes_read = bytes;
    }
    return s;
}

// ----------------------------------------------------------------------------
// Get the current state of ignoring bytes with framing errors (other than a
// BREAK) or parity errors.
// ----------------------------------------------------------------------------
bool SerialPortImplWin32::getBadByteIgnoreState () {
    DCB dcb;
    GetCommState(m_handle, &dcb);
    if(dcb.fErrorChar == true){
        return true;
    }else{
        return false;
    }
}

// ----------------------------------------------------------------------------
// Enable ignoring of received bytes with framing errors or parity errors.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::enableBadByteIgnore () {
    vpr::Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.fErrorChar = true;
    if(!SetCommState(m_handle, &dcb)){

        s.setCode(vpr::Status::Failure);
    }
    return s;
}

// ----------------------------------------------------------------------------
// Disable ignoring of received bytes with framing errors or parity errors.
// ----------------------------------------------------------------------------
vpr::Status
SerialPortImplWin32::disableBadByteIgnore () {
    vpr::Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.fErrorChar = false;
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(vpr::Status::Failure);
    }
    return s;


}

// ----------------------------------------------------------------------------
// Get the current state of parity generation for outgoing bytes and parity
// checking for incoming bytes.
// ----------------------------------------------------------------------------
bool SerialPortImplWin32::getParityGenerationState () {
    DCB dcb;
    GetCommState(m_handle, &dcb);
    if(dcb.fParity = false){
        std::cout << "parity checking is not true\n";
        return false;
    }else if(dcb.Parity != NOPARITY){
        std::cout << "parity generaton not invoked\n";
        return false;
    }else{
        return true;
    }

}

// ----------------------------------------------------------------------------
// Enable parity generation for outgoing bytes and parity checking for
// incoming bytes.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::enableParityGeneration () {
    vpr::Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.fParity = TRUE;
    vpr::SerialTypes::ParityType p = getParity();
    if(dcb.Parity == NOPARITY){
        dcb.Parity = ODDPARITY;
    }else{
        dcb.Parity = p;
    }
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(vpr::Status::Failure);
    }
    return s;
}

// ----------------------------------------------------------------------------
// Disable parity generation for outgoing bytes and parity checking for
// incoming bytes.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::disableParityGeneration () {
    vpr::Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.fParity = false;
    dcb.Parity = NOPARITY;
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(vpr::Status::Failure);
    }
    return s;
}

// ----------------------------------------------------------------------------
// Enable marking of bytes with parity errors or framing errors (except
// BREAKs).  This is only active if input parity and framing error reporting
// is enabled (see getInputParityCheckState() for more information).  The mark
// is the three-byte sequence \377 \0 X where X is the byte received in error.
// If bit stripping is enabled, a valid \377 byte is passed as the two-byte
// sequence \377 \377.
// ----------------------------------------------------------------------------
bool SerialPortImplWin32::getParityErrorMarkingState () {
    return parityMark;
}

// ----------------------------------------------------------------------------
// Enable parity error and framing error marking.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::enableParityErrorMarking () {
    vpr::Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.fErrorChar = true;
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(vpr::Status::Failure);
    }
    
    return s;
}

// ----------------------------------------------------------------------------
// Disable parity error and framing error marking.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::disableParityErrorMarking () {
    vpr::Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.fErrorChar = false;
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(vpr::Status::Failure);
    }
    return s;
}

// ----------------------------------------------------------------------------
// Get the current input baud rate.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::getInputBaudRate (vpr::Uint32& rate) {
    vpr::Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    rate = dcb.BaudRate;
    return s;
}

// ----------------------------------------------------------------------------
// Set the current input baud rate.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::setInputBaudRate (const vpr::Uint32 baud) {
    vpr::Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.BaudRate = baud;
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(vpr::Status::Failure);
    }
    return s;

}

// ----------------------------------------------------------------------------
// Get the current output baud rate.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::getOutputBaudRate (vpr::Uint32& rate) {
    vpr::Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    rate = dcb.BaudRate;
    return s;
}

// ----------------------------------------------------------------------------
// Set the current output baud rate.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::setOutputBaudRate (const vpr::Uint32 baud) {
    vpr::Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.BaudRate =baud;
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(vpr::Status::Failure);
    }
    return s;
}


// ----------------------------------------------------------------------------
// Transmit a continuous stream of zero bits for the given duration.  If the
// argument is 0, the transmission will last between 0.25 and 0.5 seconds.
// Otherwise, the duration specfies the number of seconds to send the zero bit
// stream.
// ----------------------------------------------------------------------------
vpr::Status SerialPortImplWin32::sendBreak (const vpr::Int32 duration) {
    vpr::Status s;
    std::cout << "sendBreak Not yet implemented for Win32" << std::endl;
    s.setCode(vpr::Status::Failure);
    return s;
}

vpr::Status SerialPortImplWin32::drainOutput(void){
    vpr::Status s;
    // do nothing
    return s;
}

vpr::Status SerialPortImplWin32::controlFlow(SerialTypes::FlowActionOption opt){
    vpr::Status s;
    // do nothing
    return s;
}

vpr::Status SerialPortImplWin32::flushQueue(vpr::SerialTypes::FlushQueueOption queue){
    vpr::Status s;
    // do nothing
    return s;
}


bool SerialPortImplWin32::getCanonicalState(void){
    std::cout << "Canonical State not yet implemented, EOF is enabled."
              << std::endl;
    return false;
}

vpr::Status SerialPortImplWin32::enableCanonicalInput(void){
    vpr::Status s;
    std::cout << "Canoncial State not yet implemented, EOF is enabled."
              << std::endl;
    s.setCode(vpr::Status::Failure);
    return s;
}

vpr::Status SerialPortImplWin32::disableCanonicalInput(void){
    vpr::Status s;
    std::cout << "Canoncial State not yet implemented on Win32, EOF is enabled."
              << std::endl;
    s.setCode(vpr::Status::Failure);
    return s;
}

bool SerialPortImplWin32::getBitStripState(void){
    std::cout << "Bit Stripping is not yet implemented on Win32." << std::endl;
    return false;
}

vpr::Status SerialPortImplWin32::enableBitStripping(void){
    vpr::Status s;
    std::cout << "Bit Stripping is not yet implemented on Win32." << std::endl;
    s.setCode(vpr::Status::Failure);
    return s;
}

vpr::Status SerialPortImplWin32::disableBitStripping(void){
    vpr::Status s;
    std::cout << "Bit Stripping is not yet implemented on Win32." << std::endl;
    s.setCode(vpr::Status::Failure);
    return s;
}

bool SerialPortImplWin32::getStartStopInputState(void){
    std::cout << "Start/Stop Input is not yet implemented on Win32."
              << std::endl;
    return false;
}

bool SerialPortImplWin32::getStartStopOutputState(void){
    std::cout << "Start/Stop Output is not yet implemented on Win32."
              << std::endl;
    return false;
}

vpr::Status SerialPortImplWin32::enableStartStopInput(void){
    vpr::Status s;
    std::cout << "Start/Stop Input is not yet implemented on Win32."
              << std::endl;
    s.setCode(vpr::Status::Failure);
    return s;
}

vpr::Status SerialPortImplWin32::disableStartStopInput(void){
    vpr::Status s;
    std::cout << "Start/Stop Input is not yet implemented on Win32."
              << std::endl;
    s.setCode(vpr::Status::Failure);
    return s;
}

vpr::Status SerialPortImplWin32::enableStartStopOutput(void){
    vpr::Status s;
    std::cout << "Start/Stop output is not yet implemented on Win32."
              << std::endl;
    s.setCode(vpr::Status::Failure);
    return s;
}

vpr::Status SerialPortImplWin32::disableStartStopOutput(void){
    vpr::Status s;
    std::cout << "Start/Stop Output is not yet implemented on Win32."
              << std::endl;
    s.setCode(vpr::Status::Failure);
    return s;
}

} // End of vpr namespace
