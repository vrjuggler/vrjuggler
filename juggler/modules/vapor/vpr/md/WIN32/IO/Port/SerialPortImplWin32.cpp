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
    : Port(port_name), m_handle(NULL)
{
    openFlag = GENERIC_READ | GENERIC_WRITE;
    pName = port_name.c_str();
    blocking = true;
    parityMark = false;
}

// ----------------------------------------------------------------------------
// Destructor.  If the file handle is non-NULL, its memory is released.
// ----------------------------------------------------------------------------
SerialPortImplWin32::~SerialPortImplWin32 () {
    if ( m_handle != NULL ) {
        delete m_handle;
    }
}

// ----------------------------------------------------------------------------
// Open the serial port and initialize its flags.
// ----------------------------------------------------------------------------
Status SerialPortImplWin32::open () {
    Status status;
    COMMTIMEOUTS gct;
    m_handle = CreateFile( pName,
                     openFlag,
                     0,    // comm devices must be opened w/exclusive-access
                     NULL, // no security attributes
                     OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                     0,    // not overlapped I/O
                     NULL  // hTemplate must be NULL for comm devices
                     );

    if (m_handle == INVALID_HANDLE_VALUE) {  // Handle the error.
        printf ("CreateFile failed with error %d.\n", GetLastError());
        status.setCode(Status::Failure);
    }
    gct.ReadIntervalTimeout =0;
    gct.ReadTotalTimeoutConstant=0;
    gct.ReadTotalTimeoutMultiplier=0;
    gct.WriteTotalTimeoutConstant=0;
    gct.WriteTotalTimeoutMultiplier=0;
    if(!SetCommTimeouts(m_handle,&gct)){
        printf("Timeout initialization failed.");
        status.setCode(Status::Failure);
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
Status SerialPortImplWin32::getBufferSize(Uint16 &size){
    Status s;
    COMMPROP lpCommProp;
    if(!GetCommProperties(m_handle, &lpCommProp) || (int)lpCommProp.dwCurrentTxQueue == 0){
        s.setCode(Status::Failure);
        std::cout << "Maximum buffer size is unavailable.\n";
    }else{
        size = lpCommProp.dwCurrentTxQueue;
    }

    return s;
}

// ----------------------------------------------------------------------------
// Attempt to change the buffer size to the given argument.
// ----------------------------------------------------------------------------
Status SerialPortImplWin32::setBufferSize(const Uint8 size){
    Status s;
    if(!SetupComm(m_handle, (int)size, (int)size)){
        s.setCode(Status::Failure);
        std::cout << "Could not set the minimum buffer size.\n";
    }
    return s;
}

// ----------------------------------------------------------------------------
// Get the value of the timeout (in tenths of a second) to wait for data to
// arrive.  This is only applicable in non-canonical mode.
// ----------------------------------------------------------------------------
Status SerialPortImplWin32::getTimeout (Uint8& timeout) {
    COMMTIMEOUTS t;
    Status retval;
    if(!GetCommTimeouts(m_handle, &t)){
        retval.setCode(Status::Failure);
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
Status SerialPortImplWin32::setTimeout (const Uint8 timeout) {
    COMMTIMEOUTS t;
    Status retval;
    GetCommTimeouts(m_handle, &t);

    t.ReadTotalTimeoutConstant = (int)timeout*100;
    if(!SetCommTimeouts(m_handle, &t)){
        retval.setCode(Status::Failure);
        std::cout << "Could not set timeout value.\n";
    }
//        std::cout << t.ReadIntervalTimeout << " : " << t.ReadTotalTimeoutConstant << " : " << t.ReadTotalTimeoutMultiplier << " : " << t.WriteTotalTimeoutConstant << " : " << t.WriteTotalTimeoutMultiplier << std::endl;
    return retval;
}


// ----------------------------------------------------------------------------
// Get the character size (the bits per byte).
// ----------------------------------------------------------------------------
Status SerialPortImplWin32::getCharacterSize (SerialTypes::CharacterSizeOption& size)
{
    Status retval;
    DCB dcb;
    if(GetCommState(m_handle, &dcb)){
        switch(dcb.ByteSize){
        case 5:
            size = SerialTypes::CS_BITS_5;
            break;
        case 6:
            size = SerialTypes::CS_BITS_6;
            break;
        case 7:
            size = SerialTypes::CS_BITS_7;
            break;
        case 8:
            size = SerialTypes::CS_BITS_8;
            break;
        }
    }else{
        retval.setCode(Status::Failure);
        std::cout << "Error attaining bits/byte.\n";
    }
    return retval;
}

// ----------------------------------------------------------------------------
// Set the current character size (the bits per byte) to the size in the given
// value.  This is used for both reding and writing, and the size does not
// include the parity bit (if any).
// ----------------------------------------------------------------------------
Status
SerialPortImplWin32::setCharacterSize (const SerialTypes::CharacterSizeOption bpb)
{
    Status retval;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    switch(bpb){
    case SerialTypes::CS_BITS_5:
        dcb.ByteSize = 5;
        break;
    case SerialTypes::CS_BITS_6:
        dcb.ByteSize = 6;
        break;
    case SerialTypes::CS_BITS_7:
        dcb.ByteSize = 7;
        break;
    case SerialTypes::CS_BITS_8:
        dcb.ByteSize = 8;
        break;
    }
    if(!SetCommState(m_handle,&dcb)){
        std::cout << GetLastError()<< std::endl<<std::endl;
        retval.setCode(Status::Failure);
        std::cout << "Error setting bits/byte.\n";
    }
    return retval;
}



Status SerialPortImplWin32::getStopBits (Uint8& num_bits) {
    DCB dcb;
    
    Status retval;

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
        retval.setCode(Status::Failure);
    }
    return retval;
}


// ----------------------------------------------------------------------------
// Set the number of stop bits to use.  The value must be either 1 or 2.
// ----------------------------------------------------------------------------
Status SerialPortImplWin32::setStopBits (const Uint8 num_bits) {
    DCB dcb;
    
    Status retval;

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
        retval.setCode(Status::Failure);
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
Status SerialPortImplWin32::enableInputParityCheck () {
    Status s;
    DCB dcb;
    
    if(!GetCommState(m_handle, &dcb))
        s.setCode(Status::Failure);

    dcb.fParity = 1;
    if(!SetCommState(m_handle, &dcb))
        s.setCode(Status::Failure);
    
    return s;
}
    

// ----------------------------------------------------------------------------
// Disable input parity checking.
// ----------------------------------------------------------------------------
Status
SerialPortImplWin32::disableInputParityCheck () {
    Status s;
    DCB dcb;
    GetCommState(m_handle,&dcb);
    dcb.fParity = false;
    SetCommState(m_handle,&dcb);
    return s;
}

// ----------------------------------------------------------------------------
// Get the current parity checking type (either odd or even).
// ----------------------------------------------------------------------------
SerialTypes::ParityType SerialPortImplWin32::getParity () {
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
Status SerialPortImplWin32::setOddParity () {
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
Status SerialPortImplWin32::setEvenParity () {
    Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.Parity = EVENPARITY;
    SetCommState(m_handle, &dcb);
    return s;
}



Status SerialPortImplWin32::write_i (const void* buffer, const size_t length,
                                     ssize_t& bytes_written, const vpr::Interval timeout)
{
    Status s;
    unsigned long bytes;
    
   if(vpr::Interval::NoTimeout != timeout)
       vprDEBUG(0,vprDBG_WARNING_LVL) << "Timeout not supported\n" << vprDEBUG_FLUSH;

   if(!WriteFile(m_handle, buffer, length, &bytes, NULL)){
        s.setCode(Status::Failure);
        bytes_written = bytes;
    }
    return s;

    
}

Status SerialPortImplWin32::read_i(void* buffer, const size_t length,
                                   ssize_t& bytes_read,
                                   const vpr::Interval timeout)
{
    Status s;
    unsigned long bytes;

   if(vpr::Interval::NoTimeout != timeout)
       vprDEBUG(0,vprDBG_WARNING_LVL) << "Timeout not supported\n" << vprDEBUG_FLUSH;

    if(!ReadFile( m_handle, buffer, length, &bytes,NULL)){
        s.setCode(Status::Failure);
    //    bytes_read = bytes;
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
Status SerialPortImplWin32::enableBadByteIgnore () {
    Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.fErrorChar = true;
    if(!SetCommState(m_handle, &dcb)){

        s.setCode(Status::Failure);
    }
    return s;
}

// ----------------------------------------------------------------------------
// Disable ignoring of received bytes with framing errors or parity errors.
// ----------------------------------------------------------------------------
Status
SerialPortImplWin32::disableBadByteIgnore () {
    Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.fErrorChar = false;
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(Status::Failure);
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
Status SerialPortImplWin32::enableParityGeneration () {
    Status s;
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
        s.setCode(Status::Failure);
    }
    return s;
}

// ----------------------------------------------------------------------------
// Disable parity generation for outgoing bytes and parity checking for
// incoming bytes.
// ----------------------------------------------------------------------------
Status SerialPortImplWin32::disableParityGeneration () {
    Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.fParity = false;
    dcb.Parity = NOPARITY;
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(Status::Failure);
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
Status SerialPortImplWin32::enableParityErrorMarking () {
    Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.fErrorChar = true;
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(Status::Failure);
    }
    
    return s;
}

// ----------------------------------------------------------------------------
// Disable parity error and framing error marking.
// ----------------------------------------------------------------------------
Status SerialPortImplWin32::disableParityErrorMarking () {
    Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.fErrorChar = false;
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(Status::Failure);
    }
    return s;
}

// ----------------------------------------------------------------------------
// Get the current input baud rate.
// ----------------------------------------------------------------------------
Status SerialPortImplWin32::getInputBaudRate (Uint32& rate) {
    Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    rate = dcb.BaudRate;
    return s;
}

// ----------------------------------------------------------------------------
// Set the current input baud rate.
// ----------------------------------------------------------------------------
Status SerialPortImplWin32::setInputBaudRate (const Uint32 baud) {
    Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.BaudRate = baud;
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(Status::Failure);
    }
    return s;

}

// ----------------------------------------------------------------------------
// Get the current output baud rate.
// ----------------------------------------------------------------------------
Status SerialPortImplWin32::getOutputBaudRate (Uint32& rate) {
    Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    rate = dcb.BaudRate;
    return s;
}

// ----------------------------------------------------------------------------
// Set the current output baud rate.
// ----------------------------------------------------------------------------
Status SerialPortImplWin32::setOutputBaudRate (const Uint32 baud) {
    Status s;
    DCB dcb;
    GetCommState(m_handle, &dcb);
    dcb.BaudRate =baud;
    if(!SetCommState(m_handle, &dcb)){
        s.setCode(Status::Failure);
    }
    return s;
}


// ----------------------------------------------------------------------------
// Transmit a continuous stream of zero bits for the given duration.  If the
// argument is 0, the transmission will last between 0.25 and 0.5 seconds.
// Otherwise, the duration specfies the number of seconds to send the zero bit
// stream.
// ----------------------------------------------------------------------------
Status SerialPortImplWin32::sendBreak (const Int32 duration) {
    Status s;
    std::cout << "sendBreak Not yet implemented for Win32" << std::endl;
    s.setCode(Status::Failure);
    return s;
}

Status SerialPortImplWin32::drainOutput(void){
    Status s;
    // do nothing
    return s;
}

Status SerialPortImplWin32::controlFlow(SerialTypes::FlowActionOption opt){
    Status s;
    // do nothing
    return s;
}

Status SerialPortImplWin32::flushQueue(SerialTypes::FlushQueueOption queue){
    Status s;
    // do nothing
    return s;
}


bool SerialPortImplWin32::getCanonicalState(void){
    std::cout << "Canonical State not yet implemented, EOF is enabled."
              << std::endl;
    return false;
}

Status SerialPortImplWin32::enableCanonicalInput(void){
    Status s;
    std::cout << "Canoncial State not yet implemented, EOF is enabled."
              << std::endl;
    s.setCode(Status::Failure);
    return s;
}

Status SerialPortImplWin32::disableCanonicalInput(void){
    Status s;
    std::cout << "Canoncial State not yet implemented on Win32, EOF is enabled."
              << std::endl;
    s.setCode(Status::Failure);
    return s;
}

bool SerialPortImplWin32::getBitStripState(void){
    std::cout << "Bit Stripping is not yet implemented on Win32." << std::endl;
    return false;
}

Status SerialPortImplWin32::enableBitStripping(void){
    Status s;
    std::cout << "Bit Stripping is not yet implemented on Win32." << std::endl;
    s.setCode(Status::Failure);
    return s;
}

Status SerialPortImplWin32::disableBitStripping(void){
    Status s;
    std::cout << "Bit Stripping is not yet implemented on Win32." << std::endl;
    s.setCode(Status::Failure);
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

Status SerialPortImplWin32::enableStartStopInput(void){
    Status s;
    std::cout << "Start/Stop Input is not yet implemented on Win32."
              << std::endl;
    s.setCode(Status::Failure);
    return s;
}

Status SerialPortImplWin32::disableStartStopInput(void){
    Status s;
    std::cout << "Start/Stop Input is not yet implemented on Win32."
              << std::endl;
    s.setCode(Status::Failure);
    return s;
}

Status SerialPortImplWin32::enableStartStopOutput(void){
    Status s;
    std::cout << "Start/Stop output is not yet implemented on Win32."
              << std::endl;
    s.setCode(Status::Failure);
    return s;
}

Status SerialPortImplWin32::disableStartStopOutput(void){
    Status s;
    std::cout << "Start/Stop Output is not yet implemented on Win32."
              << std::endl;
    s.setCode(Status::Failure);
    return s;
}

} // End of vpr namespace
