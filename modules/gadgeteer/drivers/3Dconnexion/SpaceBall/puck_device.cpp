//----------------------------------------------------------------------------
//--    Copyright (c) 2002 Fuel Tech Inc.  All Rights Reserved.             --
//--    vriPuckDevice.cpp                                                   --
//--    A custom driver for a puck-like device                              --
//--    (e.g. Logitech SpaceMouse, SpaceBall, SpaceTraveler)                --
//----------------------------------------------------------------------------
//--    Dan R. Harms      10/12/2004                                        --
//--    For Acuitiv, a division of Fuel Tech Inc.                           --
//----------------------------------------------------------------------------
//--  This is free software; you can redistribute it and/or modify          --
//--  it under the terms of the GNU Lesser General Public License as        --
//--  published by the Free Software Foundation; either version 2.1 of      --
//--  the License, or (at your option) any later version.                   --
//--                                                                        --
//--  This is distributed in the hope that it will be useful, but           --
//--  WITHOUT ANY WARRANTY; without even the implied warranty of            --
//--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         --
//--  GNU Lesser General Public License for more details.                   --
//--                                                                        --
//--  You should have received a copy of the GNU Lesser General Public      --
//--  License along with this file; if not, write to the Free Software      --
//--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA              --
//--  02111-1307  USA                                                       --
//----------------------------------------------------------------------------

#include "puck_device.h"

#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/Devices/DriverConfig.h>

#include <stdio.h>
#include <fcntl.h>

#if defined(WIN32) || defined(WIN64)
# include <sys/types.h>
# include <sys/stat.h>
# include <io.h>
#else                 /*  ! defined (WIN32) && ! defined (WIN64)    */
# include <ctype.h>
# include <unistd.h>
# include <sys/time.h>
# if defined(VPR_OS_Linux) || defined(linux_x86_64)
#  include <asm/ioctls.h>		         // for FIONREAD, TCGETA, TCSETA
#  include <linux/input.h>                       // for struct input_event
# endif
# if defined(__sun)
#  include </usr/ucbinclude/sys/ioctl.h>         // for FIONREAD
# endif
# if defined(HP) || defined(HPUX)
#  include <sys/termiox.h>
#  include <sys/modem.h>
# endif
#endif

#include <boost/bind.hpp>


// Using Statements
using std::cout;
using std::endl;
using std::cerr;
using std::string;


//----------------------------------------------------------------------------
//--  PuckDevice  ------------------------------------------------------------
//----------------------------------------------------------------------------
extern "C"
{
    // Register this device with the Input Manager
    GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
    { new gadget::DeviceConstructor<PuckDevice>(inputMgr); }

    // Provide entry point for loading shared object
    GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
	{ return __GADGET_version; }
}

PuckDevice::~PuckDevice()
{
    if (_running) stopSampling(); 
    if (_puck) delete _puck; 
}

void PuckDevice::controlLoop()
{
    while (1)
    {
	sample();
//	vpr::System::sleep(1);
    }
}

void PuckDevice::controlLoopRel()
{
    while (1)
	sampleRel();
}

string PuckDevice::getElementType() 
{
   return string( "puck_device" );
}

bool PuckDevice::startSampling()
{
    if (!_puck || !_puck->puckOpen())
    {
	_running = false;
	return false;
    }
    else 
    {
	_running = true;
    }    

    /* gadget::Analog's internal buffer needs to be initialized here.
     * Otherwise it uses mDefaultValue (private member variable) of 0
     * (non-normalized).  Therefore the navigation class reports all axes
     * as having a normalized value of 0, when it hasn't actually 
     * been normalized.  ( normalize(0) should be 0.5, since mMin = -mMax. )
     */
    initBuffers();    
    // Create a new thread to handle the control

    bool started(false);

    try
    {
	if (_puck->useRelative())
	    mThread = new vpr::Thread(boost::bind(&PuckDevice::controlLoopRel, this));
	else
	    mThread = new vpr::Thread(boost::bind(&PuckDevice::controlLoop, this));
        started = true;
    }
    catch (vpr::Exception& ex)
    {
        vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
	    << clrOutBOLD(clrRED, "ERROR")
	    << ": Failed to spawn thread for 3DConnexion puck_device driver!\n"
	    << vprDEBUG_FLUSH;
        vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
	    << ex.what() << std::endl << vprDEBUG_FLUSH;
        _running = false;	
    }

    return started;
}

bool PuckDevice::stopSampling()
{
    _running = false;
    if (mThread != NULL)
    {
	/* According to vrj, this may not work on all platforms. */
 	mThread->kill();
	mThread->join();
 	delete(mThread);
 	mThread = NULL;
    }
    return _puck->puckClose();
}


bool PuckDevice::sample()
{
    if (!_puck || !_running) return false;
    int result = _puck->processBuffer(_axes, _buttons);
    if (result == 2)
	// analog data changed
    {
	analogData submit(6);
	for (unsigned int i = 0; i < _axes.size(); i++)
	{
	    float normalize;
	    float value = _axes[i].getAnalog();
	    gadget::Analog::normalizeMinToMax(value, normalize);
	    submit[i] = normalize;
	}
	gadget::Analog::addAnalogSample(submit);
	return true;
    }
    else if (result == 1)
	// digital data changed
    {
	gadget::Digital::addDigitalSample(_buttons);
	return true;
    }
    // else no data changed
    return false;
}

bool PuckDevice::sampleRel()
{
    if (!_puck || !_running) return false;
    analogData localAxes(6);
    int result = _puck->processBuffer(localAxes, _buttons);
    if (result & 0x02)
	// analog data changed
    {
	for (int i = 0 ; i < 6 ; i++)
	{
	    if (localAxes[i].getAnalog() != 0.0f)
	    {
		float value;
		float normalize;
		value = localAxes[i].getAnalog();
		gadget::Analog::normalizeMinToMax(value, normalize);
		_axes[i] = _axes[i].getAnalog() + normalize;
		_axesCount[i]++;
	    }
	}

	return true;
    }
    else if (result & 0x01)
	// digital data changed
    {
	gadget::Digital::addDigitalSample(_buttons);
	return true;
    }
    // else no data changed
    return false;
}

void PuckDevice::updateData()
{
    if (_running)
    {
	if (_puck->useRelative())
	{
	    if (_updateCount > 3 ||
		_axesCount[0] >= 2 ||
		_axesCount[1] >= 2 ||
		_axesCount[2] >= 2 ||
		_axesCount[3] >= 2 ||
		_axesCount[4] >= 2 ||
		_axesCount[5] >= 2)
	    {
		analogData submit(6);
		for (int i = 0 ; i < 6 ; i++)
		{
		    if (_axesCount[i] > 0)
			submit[i] = _axes[i].getAnalog() / _axesCount[i];
		    else
			submit[i] = 0.5f;
		    // cout << _axes[i].getAnalog() << " / " << _axesCount[i] << " = " << submit[i].getAnalog() << endl;
		    _axes[i] = 0.0f;
		    _axesCount[i] = 0;
		}
		_updateCount = 0;
		gadget::Analog::addAnalogSample(submit);
	    }
	    _updateCount++;
	}
	gadget::Analog::swapAnalogBuffers();
	gadget::Digital::swapDigitalBuffers();
    }
}

void PuckDevice::initBuffers()
{
    for (unsigned int i = 0; i < _axes.size(); i++)
    {
	float normalize;
	float value = 0;
	gadget::Analog::normalizeMinToMax(value, normalize);
	_axes[i] = normalize;
	_axesCount[i] = 0;
    }
    for (unsigned int i = 0; i < _buttons.size(); i++)
	_buttons[i] = 0;

    _updateCount = 0;

    gadget::Digital::addDigitalSample(_buttons);
    gadget::Analog::addAnalogSample(_axes);
}

bool PuckDevice::config( jccl::ConfigElementPtr c )
{
    // Names of devices
    // Note the SpaceBall 5000 uses the SpaceMouse firmware so 
    // use "SpaceMouse" for it; use "SpaceBall" for
    // SpaceBall 4000 as expected.
    string _spaceTraveler = "SpaceTraveler";
    string _spaceBall4000 = "SpaceBall 4000 FLX";
    // Not well supported
    string _spaceBall2003B = "SpaceBall 2003B";
    string _spaceMouse = "SpaceMouse";
    // Just use SpaceMouse instead, unless you really need the extra
    // buttons of the XT
    string _spaceMousePlus = "SpaceMouse Plus XT";
    
    if (!gadget::Digital::config(c) || !gadget::Analog::config(c)  || !gadget::Input::config(c))
	return false;

    string deviceName = c->getProperty<string>("device");
    string portName = c->getProperty<string>( "port" );
    int baudRate = c->getProperty<int>( "baud" );
    if (baudRate != 9600)
    {
	cerr << WRONG_SPEED << deviceName << endl;
	return false;
    }

    // Check for SpaceTraveler
    const char *p0 = strstr(deviceName.c_str(), _spaceTraveler.c_str());
    if (p0 != NULL)
    {
	_puck = new spaceTraveler(deviceName, portName);
	if (_puck == NULL)
	{
	    cerr << INSUFF_MEM << endl;
	    return false;
	}
	setMin(-500);
	setMax(500);
	return true;
    }
    
    // Check for SpaceMousePlus
    const char *p1 = strstr(deviceName.c_str(), _spaceMousePlus.c_str());
    if (p1 != NULL)
    {
	_puck = new spaceMouse(deviceName, portName, baudRate);
	if (_puck == NULL)
	{
	    cerr << INSUFF_MEM << endl;
	    return false;
	}
	setMin(-500);
	setMax(500);
	return true;
    }


    // Check for SpaceMouse
    const char *p2 = strstr(deviceName.c_str(), _spaceMouse.c_str());
    if (p2 != NULL)
    {
	_puck = new spaceMouse(deviceName, portName, baudRate);
	if (_puck == NULL)
	{
	    cerr << INSUFF_MEM << endl;
	    return false;
	}
	setMin(-500);
	setMax(500);
	return true;
    }

    // Check for SpaceBall4000
    const char *p4 = strstr(deviceName.c_str(), _spaceBall4000.c_str());
    if (p4 != NULL)
    {
	_puck = new spaceBall4000FLX(deviceName, portName, baudRate);
	if (_puck == NULL)
	{
	    cerr << INSUFF_MEM << endl;
	    return false;
	}
	setMin(-2000);
	setMax(2000);
	return true;
    }

    // Check for SpaceBall2003B
    const char *p3 = strstr(deviceName.c_str(), _spaceBall2003B.c_str());
    if (p3 != NULL)
    {
	_puck = new spaceBall2003B(deviceName, portName, baudRate);
	if (_puck == NULL)
	{
	    cerr << INSUFF_MEM << endl;
	    return false;
	}
	setMin(-2000);
	setMax(2000);
	return true;
    }
    else
	// unknown device
	return false;

    return true;
}

void PuckDevice::writeByte(const unsigned char c) 
{
    for (int i = 0; i < 8; i++)
	if ( (c >> (8 - i - 1)) & 1)
	    cout<<"1";
	else
	    cout<<"0";
    cout<<endl;
}

void PuckDevice::writeInt(const int temp)
{
    char buf[32];  
    for (int i = 0; i < 32; i++)  
	if ( (temp >> (32 - i - 1)) & 1)
	    buf[i] = '1';
	else 
	    buf[i] = '0';
    for (int i = 0; i < 32; i++)
        if (i % 8 == 0)
	    printf("x%c", buf[i]);
        else
	    printf("%c", buf[i]);
    printf("\n");
}



//----------------------------------------------------------------------------
//--  basePuck   -------------------------------------------------------------
//----------------------------------------------------------------------------
basePuck::basePuck()
{
    _version = "unknown";
}

basePuck::~basePuck()
{}

void basePuck::puckSleep(const float seconds)
{
#if defined(WIN32) || defined(WIN64)
    Sleep(1000 * seconds);
#else
    usleep(static_cast<int>(1000000 * seconds));
#endif
}


//----------------------------------------------------------------------------
//--  basePuckSerial   -------------------------------------------------------
//----------------------------------------------------------------------------
basePuckSerial::~basePuckSerial()
{
}

bool basePuckSerial::portOpen(const char *name, int opts, HandleType &handle)
{
#if defined(WIN32) || defined(WIN64)

    handle = CreateFile( name, opts, 0, NULL, OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING,
			0);
    if (handle == INVALID_HANDLE_VALUE)
    {
	DWORD d;
	d = GetLastError();
	int a = (int) d;
	return false;
    }
    else
    {
	return true;
    } 

#else            /*    ! defined (WIN32) && ! defined (WIN64)   */

    handle = open(name, opts);
    if (handle < 0)
    {
 	return false;
    }
    else
    {
	return true;
    }

#endif
}

bool basePuckSerial::portClose(HandleType handle)
{
#if defined(WIN32) || defined(WIN64)

    if (!SetCommTimeouts(handle, &_oldTimeouts))
    {
	cerr << GENERIC_ERROR << " can't set comm timeouts" << endl;
    }
    CloseHandle(handle);
    return true;

#else            /*    ! defined (WIN32) && ! defined (WIN64)   */

    // discard pending output to avoid handshaking interference
    tcflush(handle, TCIOFLUSH);
    close(handle);
    return true;

#endif
}

int basePuckSerial::portRead(HandleType handle, unsigned char* buffer, 
			     int bufferSize)
{
#if defined(WIN32) || defined(WIN64)

#if 0
//Check error status...
    {
	COMSTAT comStat;
	DWORD   dwErrors;
	BOOL    fOOP, fOVERRUN, fPTO, fRXOVER, fRXPARITY, fTXFULL;
	BOOL    fBREAK, fDNS, fFRAME, fIOE, fMODE;

	// Get and clear current errors on the port.
	if (!ClearCommError(handle, &dwErrors, &comStat))
	    // Report error in ClearCommError.
	{
	    cout<<"# Error in ClearCommError"<<endl;
	    return 0;
	}
 
	// Get error flags.
	fDNS = dwErrors & CE_DNS;
	if (fDNS) cout<<"fDNS is true"<<endl;
	fIOE = dwErrors & CE_IOE;
	if (fIOE) cout<<"fIOE is true"<<endl;
	fOOP = dwErrors & CE_OOP;
	if (fOOP) cout<<"fOOP is true"<<endl;
	fPTO = dwErrors & CE_PTO;
	if (fPTO) cout<<"fPTO is true"<<endl;
	fMODE = dwErrors & CE_MODE;
	if (fMODE) cout<<"fMODE is true"<<endl;
	fBREAK = dwErrors & CE_BREAK;
	if (fBREAK) cout<<"fBREAK is true"<<endl;
	fFRAME = dwErrors & CE_FRAME;
	if (fFRAME) cout<<"fFRAME is true"<<endl;
	fRXOVER = dwErrors & CE_RXOVER;
	if (fRXOVER) cout<<"fRXOVER is true"<<endl;
	fTXFULL = dwErrors & CE_TXFULL;
	if (fTXFULL) cout<<"fTXFULL is true"<<endl;
	fOVERRUN = dwErrors & CE_OVERRUN;
	if (fOVERRUN) cout<<"fOVERRUN is true"<<endl;
	fRXPARITY = dwErrors & CE_RXPARITY;
	if (fRXPARITY) cout<<"fRXPARITY is true"<<endl;
	else cout<<"No error flags"<<endl;

	// COMSTAT structure contains information regarding
	// communications status.
	if (comStat.fCtsHold)
	    // Tx waiting for CTS signal
	    cout<<" Tx waiting for CTS signal  "<<endl;
	if (comStat.fDsrHold)
	    // Tx waiting for DSR signal
	    cout<<" Tx waiting for DSR signal  "<<endl;
	if (comStat.fRlsdHold)
	    // Tx waiting for RLSD signal
	    cout<<" Tx waiting for RLSD signal  "<<endl;
	if (comStat.fXoffHold)
	    // Tx waiting, XOFF char rec'd
	    cout<<" Tx waiting, XOFF char rec'd  "<<endl;
	if (comStat.fXoffSent)
	    // Tx waiting, XOFF char sent
	    cout<<" Tx waiting, XOFF char sent  "<<endl;    
	if (comStat.fEof)
	    // EOF character received
	    cout<<" EOF character received  "<<endl;    
	if (comStat.fTxim)
	    // Character waiting for Tx; char queued with TransmitCommChar
	    cout<<" Character waiting for Tx; char queued with TransmitCommChar  "<<endl;
	if (comStat.cbInQue)
	    // comStat.cbInQue bytes have been received, but not read
	    cout<<" comStat.cbInQue bytes have been received, but not read  "<<endl;
	if (comStat.cbOutQue)
	    // comStat.cbOutQue bytes are awaiting transfer
	    cout<<" comStat.cbOutQue bytes are awaiting transfer  "<<endl;
	else cout<<"Commstat structure fine"<<endl;
    }

#endif

    if (_event.hEvent == 0)
    {
	_event.hEvent = CreateEvent(NULL, TRUE, FALSE, "PuckDeviceSerialEvent");
	if (_event.hEvent == INVALID_HANDLE_VALUE)
	{
	    cerr 
		<< GENERIC_ERROR << " can't create overlapped serial event" << endl;
	    return 0;
	}
	else if (_event.hEvent == NULL)
	{
	    cerr 
		<< GENERIC_ERROR << " can't create overlapped serial event 2" << endl;
	    return 0;
	}
    }
    MSG message;
    DWORD bRead = 0;
    memset(buffer, 0x00, bufferSize);
    ResetEvent(_event.hEvent);
    WaitCommEvent(handle, &_commMask, &_event);
    switch (MsgWaitForMultipleObjects(1, &_event.hEvent, FALSE,
				      INFINITE, QS_ALLINPUT))
    {
    case WAIT_OBJECT_0+0:
	ReadFile(handle, buffer, bufferSize, &bRead, &_event);
	return bRead;
	break;

    case WAIT_OBJECT_0+1:
	while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
	{
	    TranslateMessage(&message);
	    DispatchMessage(&message);
	}
	memset(buffer, 0x00, bufferSize);
	ReadFile(handle, buffer, bufferSize, &bRead, &_event);
	return bRead;
	break;

    case WAIT_TIMEOUT:
    default:
//	cout<<"WAIT_TIMEOUT"<<endl;
	return 0;
    }

#else            /*    ! defined (WIN32) && ! defined (WIN64)   */

    return read(handle, buffer, bufferSize); 
    // blocks until data w/ CR available

#endif
}

int basePuckSerial::portWrite(HandleType handle, unsigned char* buffer, 
			      int bufferSize)
{
#if defined(WIN32) || defined(WIN64)

    int i;
    for (i = 0; i < bufferSize; i++)
	if (!TransmitCommChar(handle, buffer[i]))
	{
	    puckSleep(0.01f);
	    if (!TransmitCommChar(handle, buffer[i]))
		break;
	}
    return i;

#else            /*    ! defined (WIN32) && ! defined (WIN64)   */

    return write(handle, buffer, bufferSize);

#endif
}


//----------------------------------------------------------------------------
//--  basePuckUSB   ----------------------------------------------------------
//----------------------------------------------------------------------------
basePuckUSB::~basePuckUSB()
{
}

bool basePuckUSB::portOpen(const char *name, int opts, HandleType &handle)
{
#if defined(VPR_OS_Linux) || defined(linux_x86_64)
    handle = open(name, opts);
    if (handle < 0)
    {
 	return false;
    }
    else
    {
	return true;
    }

#else

    return false;
    
#endif              /* defined(VPR_OS_Linux) || defined(linux_x86_64) */
}

bool basePuckUSB::portClose(HandleType handle)
{
#if defined(VPR_OS_Linux) || defined(linux_x86_64)

    // discard pending output to avoid handshaking interference
    tcflush(handle, TCIOFLUSH);
    close(handle);
    return true;

#else

    return false;
    
#endif              /* defined(VPR_OS_Linux) || defined(linux_x86_64) */
}

int basePuckUSB::portRead(HandleType handle, int buffer[], int bufferSize)
{
#if defined(VPR_OS_Linux) || defined(linux_x86_64)
    if (bufferSize < 3)
	return 0;
    struct input_event event;
    read(handle, &event, sizeof(struct input_event));
    buffer[0] = event.type;
    buffer[1] = event.code;
    buffer[2] = event.value;
    return 3;
#else
    return 0;
#endif              /* defined(VPR_OS_Linux) || defined(linux_x86_64) */
}

//----------------------------------------------------------------------------
//--  spaceMouse   -----------------------------------------------------------
//----------------------------------------------------------------------------
spaceMouse::~spaceMouse()
{
}

void spaceMouse::init(const string devName, const string portName,
	      const int baud)
{
    _devName = devName;
    _portName = portName;
    _baud = baud;

    // Byte codes
    DIGITAL_CODE  = 'k';
    ANALOG_CODE   = 'd';
    MODE_CODE     = 'm';
    SENS_CODE     = 'q';
    BEEP_CODE     = 'b';
    ZEROCAP_CODE  = 'z';
    NULL_CODE     = 'n';
    VERSION_CODE  = 'v';
    QUERY_CODE    = 'Q';
    ERROR_CODE    = 'e';

    // Error codes
    ILLEGAL_COMM_CODE  = 'A';
    FRAMING_ERROR_CODE = 'B';

    // Packet lengths
    ANALOG_PACKET  = 26;
    DIGITAL_PACKET = 5;
}

int spaceMouse::puckOpen()
{
    int options;
#if defined(WIN32) || defined(WIN64)
    options = GENERIC_READ | GENERIC_WRITE;
#else
    options = O_RDWR | O_NDELAY;
#endif

    if (!portOpen(_portName.c_str(), options, _file))
     {
 	cerr << OPEN_PORT << _portName << endl;
 	return 0;
     }
 
    puckSleep(1);

    if (!portConfigure())
	return 0;
    if (!puckConfigure())
	return 0;
    return 1;
}

int spaceMouse::puckConfigure()
{
    puckInit();
    puckMode();
    puckSensitivity();
    puckNullRadius();
    writeSettings();
    puckZeroCap();
    return 1;
}

void spaceMouse::puckInit()
{
    _version = puckVersion();
    compress_dom = 0;
    compress_trans = 1;
    compress_rot = 1;
    sens_trans = 0;
    sens_rot = 0;
    null_radius = 15;
    beep_duration = 3;
}

int spaceMouse::processBuffer(analogData &ana, digitalData &dig)
{
    const int size_buffer = 512;
    unsigned char buffer[size_buffer];
    int rc = portRead(_file, buffer, size_buffer);
    buffer[rc] = '\0';
    if (rc != 0)
	return handleRead(buffer, rc, ana, dig);
    return 0;
}

int spaceMouse::handleRead(unsigned char* buffer, int bRead,
			  analogData &ana, digitalData &dig )
{
    if (buffer[0] == ANALOG_CODE && bRead == ANALOG_PACKET)	// analog data
    {
	processAnalogData(buffer + 1, ana);
	return 2;
    }
    else if (buffer[0] == DIGITAL_CODE && bRead == DIGITAL_PACKET)	// digital data
    {
	processDigitalData(buffer + 1, dig);
	return 1;
    }
    if (buffer[0] == ERROR_CODE && bRead == 5)
    {
   	processError(buffer);
    }
    return 0;
}

void spaceMouse::querySettings()
{
    unsigned char buffer[32];

    // Mode
    sprintf((char *) buffer, "%c%c\r", MODE_CODE, QUERY_CODE);
    int n = portWrite(_file, buffer, 3);
    if (n < 0)
 	cerr << WRITE_FAIL(3) << endl;
    n = portRead(_file, buffer, 3);
    buffer[n] = '\0';
    if (buffer[0] == MODE_CODE)
    {
	int val = returnNibble(buffer[1]);
	compress_dom = (val & 4) ? 1 : 0;
	compress_trans = (val & 2) ? 1 : 0;
	compress_rot = (val & 1) ? 1 : 0;
    }
    else if (buffer[0] == ERROR_CODE)
	processError(buffer);
 
    // Sensitivity
    sprintf((char *) buffer, "%c%c\r", SENS_CODE, QUERY_CODE);
    n = portWrite(_file, buffer, 3);
    if (n < 0)
 	cerr << WRITE_FAIL(3) << endl;    
    n = portRead(_file, buffer, 4);
    buffer[n] = '\0';
    if (buffer[0] == SENS_CODE)
    {
	sens_trans = returnNibble(buffer[1]);
	sens_rot = returnNibble(buffer[2]);
    }
    else if (buffer[0] == ERROR_CODE)
	processError(buffer);


    // Null radius
    sprintf((char *) buffer, "%c%c\r", NULL_CODE, QUERY_CODE);
    n = portWrite(_file, buffer, 3);
    if (n < 0)
 	cerr << WRITE_FAIL(3) << endl;
    n = portRead(_file, buffer, 3);
    buffer[n] = '\0';
    if (buffer[0] == NULL_CODE)
    {
	null_radius = returnNibble(buffer[1]);
    }
    else if (buffer[0] == ERROR_CODE)
	processError(buffer);
}

void spaceMouse::writeSettings()
{
    cout << endl 
	 << " //----------------------- @@ Puck Device Driver @@";
    cout << " ---------------------------//" << endl;
    cout << " //--  A " << _devName << " is connected to the computer." << endl;
    cout << " //--  Serial port: \"" << _portName << "\"" << endl;
    cout << " //--        Speed: " << _baud << " bps" << endl;
    cout << " //--      Version: \"" << _version << "\"" << endl;
    cout << " //--     Settings --------------->" << endl;
    cout << " //--                 Mode Dominant: " << compress_dom << endl;
    cout << " //--              Mode Translation: " << compress_trans << endl;
    cout << " //--                 Mode Rotation: " << compress_rot << endl;
    cout << " //--       Sensitivity Translation: " << sens_trans << endl;
    cout << " //--          Sensitivity Rotation: " << sens_rot << endl;
    cout << " //--                   Null Radius: " << null_radius << endl;
    cout << " //--                 Beep Duration: " << beep_duration << endl;
    cout << " //----------------------- @@ Puck Device Driver @@";
    cout << " ---------------------------//" 
	 << endl << endl;
}

void spaceMouse::puckMode()
{
    /// Performs configuration of puck; if compress_dom == 1, 
    /// only dominant component is set to non-zero; if compress_trans or
    /// compress_rot == 0, that component is set to zero
    unsigned char buffer[64];
    int first = 0;
    if (compress_dom) first += 4;
    if (compress_trans) first += 2;
    if (compress_rot) first += 1;
    // report buttons 10, 11 separately
    sprintf((char *) buffer, "%c%c\r", MODE_CODE, 
	    returnNibble(first));
    int n = portWrite(_file, buffer, 3);
    if (n < 0)
	cerr << WRITE_FAIL(3) << endl;
    n = portRead(_file, buffer, 3);
}

void spaceMouse::queryKeyboard()
{
    unsigned char buffer[16];
    sprintf((char *) buffer, "%c%c\r", DIGITAL_CODE, QUERY_CODE);
    int n = portWrite(_file, buffer, 3);
    if (n < 0)
	cerr << WRITE_FAIL(3) << endl;
}

void spaceMouse::queryMouse()
{
    unsigned char buffer[16];
    sprintf((char *) buffer, "%c%c\r", ANALOG_CODE, QUERY_CODE);
    int n = portWrite(_file, buffer, 3);
    if (n < 0)
	cerr << WRITE_FAIL(3) << endl;
}

string spaceMouse::puckVersion()
{
    unsigned char buffer[64];
    sprintf((char *) buffer, "%c%c\r", VERSION_CODE, QUERY_CODE);
    int n = portWrite(_file, buffer, 3);
    if (n < 0)
	cerr << WRITE_FAIL(3) << endl;
    n = portRead(_file, buffer, 64);
				// get rid of carriage return
    buffer[n-1] = '\0';
    string str = (char *) buffer;
    return str;
}

void spaceMouse::puckNullRadius()
{
    /// From 0 (highest) to 15 (lowest)
    if (null_radius < 0 || null_radius > 15)
	return;
    unsigned char buffer[16];
    sprintf((char *) buffer, "%c%c\r", NULL_CODE, returnNibble(null_radius));
    int n = portWrite(_file, buffer, 3);
    if (n < 0)
	cerr << WRITE_FAIL(3) << endl;
    n = portRead(_file, buffer, 3);
}

void spaceMouse::puckZeroCap()
{
    unsigned char buffer[16];
    sprintf((char *) buffer, "%c\r", ZEROCAP_CODE);
    int n = portWrite(_file, buffer, 2);
    if (n < 0)
	cerr << WRITE_FAIL(2) << endl;
    n = portRead(_file, buffer, 2);
}

 
void spaceMouse::puckBeep()
{
    /// From 0 (32 ms) to 7 (2000 ms)
    if (beep_duration < 0 || beep_duration > 7)
	return;
    unsigned char buffer[16];
    sprintf((char *) buffer, "%c%c\r", BEEP_CODE, returnNibble(beep_duration));
    int n = portWrite(_file, buffer, 3);
    if (n < 0)
	cerr << WRITE_FAIL(3) << endl;
    n = portRead(_file, buffer, 2);
}

void spaceMouse::puckSensitivity()
{
    /// Set sensitivity of cap/mouse from 0 (linear) to 15 (quadratic)
    /// of translational and rotational axes
    if (sens_trans < 0 || sens_trans > 15)
	return;
    if (sens_rot < 0 || sens_rot > 15)
	return;
    unsigned char buffer[16];
    sprintf((char *) buffer, "%c%c%c\r", SENS_CODE,
	    returnNibble(sens_trans),
	    returnNibble(sens_rot));
    int n = portWrite(_file, buffer, 4);
    if (n < 0)
	cerr << WRITE_FAIL(4) << endl;
    n = portRead(_file, buffer, 4);
}

int spaceMouse::portConfigure()
{
#if defined(WIN32) || defined(WIN64)

    DCB dcb;
    FillMemory(&dcb, sizeof(dcb), 0);
    dcb.DCBlength = sizeof(dcb);

    // magellan stuff:
    dcb.BaudRate = CBR_9600;
    dcb.fBinary = TRUE;
    dcb.fParity = FALSE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fDsrSensitivity = FALSE;
    dcb.fErrorChar = TRUE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fAbortOnError = FALSE;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = TWOSTOPBITS;
    dcb.ErrorChar = -1;
    dcb.EvtChar = '\r';
    dcb.EofChar = -1;
    dcb.XonChar = -1;
    dcb.XoffChar = -1;
 
    if (!SetCommState(_file, &dcb))
    {
	cerr << GENERIC_ERROR << " can't set comm state" << endl;
	return 0;
    }

    if (!SetCommMask(_file, EV_RXFLAG))
    {
	cerr << GENERIC_ERROR << " can't set comm mask" << endl;
	return 0;
    }
    GetCommMask(_file, &_commMask);
 
    // Initialize overlapped event
    _event.Internal = 0;
    _event.InternalHigh = 0;
    _event.Offset = 0;
    _event.OffsetHigh = 0;
    _event.hEvent = 0;

    // Create temporary overlapped event
    OVERLAPPED EVENT;
    EVENT.Internal = 0;
    EVENT.InternalHigh = 0;
    EVENT.Offset = 0;
    EVENT.OffsetHigh = 0;
    EVENT.hEvent = CreateEvent(NULL, TRUE, FALSE, "PuckDeviceSerialEvent");
    if (EVENT.hEvent == INVALID_HANDLE_VALUE)
    {
	cerr << GENERIC_ERROR << " can't create overlapped serial event" << endl;
	return 0;
    }
    else if (EVENT.hEvent == NULL)
    {
	cerr << GENERIC_ERROR << " can't create overlapped serial event 2" << endl;
	return 0;
    }
 
    if (!GetCommTimeouts(_file, &_oldTimeouts))
    {
	cerr << GENERIC_ERROR << " can't get comm timeouts" << endl;
	return 0;
    }
    COMMTIMEOUTS newTimeouts;
    newTimeouts.ReadIntervalTimeout = MAXDWORD;
    newTimeouts.ReadTotalTimeoutMultiplier = 0;//MAXDWORD;
    newTimeouts.ReadTotalTimeoutConstant = 0;//32000;
    newTimeouts.WriteTotalTimeoutMultiplier = 10;
    newTimeouts.WriteTotalTimeoutConstant = 100;
    if (!SetCommTimeouts(_file, &newTimeouts))
    {
	cerr << GENERIC_ERROR << " can't set comm timeouts" << endl;
	return 0;
    }

    puckSleep(1);
 
    return 1;

#else                                 /* #if !defined(WIN32) && !defined(WIN64) */


#if defined(HP) || defined(HPUX)
    /* Assert Request to Send, Data Terminal Ready, Data Rate Select */
    mflag m = MRTS | MDTR | MDRS;
    ioctl(_file, MCSETA, &m);
    struct termiox tz_struct;
    ioctl(_file, TCGETX, &tz_struct);
    /* 
      The hardware flow control mode needs to enable Clear to Send
      hardware flow control on input to serial port.  This ensures 
      output from computer to device will block until the device 
      asserts CTS (which wasn't happening right after a reboot on HP).
    */
    tz_struct.x_hflag |= CTSXON;
    ioctl(_file, TCSETX, &tz_struct);
#endif

    // Set blocking behavior on read
    fcntl(_file, F_SETFL, 0);

    // get termios structure
    int status = tcgetattr(_file, &_tty_struct);
    // Control options
    // Enable receiver; 8 data bits; 2 stop bits; hangup on last close
    // (drop DTR); local line (keep owner same)
    _tty_struct.c_cflag = CREAD | CS8 | CSTOPB | HUPCL | CLOCAL; 
    // Output options
    // Postprocess output
    _tty_struct.c_oflag = 0;
    // Input options
    // Ignore parity errors and break conditions
    _tty_struct.c_iflag = IGNBRK | IGNPAR ;
    // Local options
    // Enable canonical input; disable echo characters
    //    _tty_struct.c_lflag &= ~( ECHO | ECHOE);
    _tty_struct.c_lflag = ICANON;
    // Control characters
    _tty_struct.c_cc[VEOL] = '\r';
    _tty_struct.c_cc[VERASE] = _POSIX_VDISABLE;
    _tty_struct.c_cc[VKILL] =  _POSIX_VDISABLE;
    // Port speed
    cfsetispeed(&_tty_struct, B9600);
    cfsetospeed(&_tty_struct, B9600);

    // Flush input/output buffers, then make change
    tcsetattr(_file, TCSAFLUSH, &_tty_struct);

    return 1;

#endif
}


unsigned char spaceMouse::returnNibble(const int val)
{
    switch (val)
    {
    case 0: case 3: case 5: case 6: 
    case 9: case 10: case 12: case 15:
	return EVEN_NIBBLE_CODE | val;
	break;
    case 1: case 2: case 4: case 7:
    case 8: case 11: case 13: case 14:
	return ODD_NIBBLE_CODE | val;
	break;
    default:
	cerr << GENERIC_ERROR << "returnNibble()" << endl;
	return 0;
    }
}

int spaceMouse::returnNibble(const unsigned char c)
{
    return c & NIBBLE_MASK;
}

void spaceMouse::processError(const unsigned char *buffer)
{
    if (buffer[1] == ILLEGAL_COMM_CODE)
    {
	char temp = ((NIBBLE_MASK & buffer[2]) << 4 ) | 
	    ( NIBBLE_MASK & buffer[3]);
	cerr << ILLEGAL_COMMAND << temp << endl;
    }
    else if (buffer[1] == FRAMING_ERROR_CODE)
    {
	cerr << FRAMING_ERROR << endl;
    }
    else cerr << GENERIC_ERROR <<  "processError()" << endl;
}

void spaceMouse::processDigitalData(const unsigned char* buffer,
				   digitalData &dig)
{
    int temp = ( (buffer[2] & NIBBLE_MASK) << 8 ) | 
	( (buffer[1] & NIBBLE_MASK) << 4) |
	(buffer[0] & NIBBLE_MASK);

    dig[0] = temp & BUTTON_1;
    dig[1] = temp & BUTTON_2;
    dig[2] = temp & BUTTON_3;
    dig[3] = temp & BUTTON_4;
    dig[4] = temp & BUTTON_5;
    dig[5] = temp & BUTTON_6;
    dig[6] = temp & BUTTON_7;
    dig[7] = temp & BUTTON_8;
    dig[8] = temp & BUTTON_9;
    dig[9] = temp & BUTTON_10;
    dig[10] = temp & BUTTON_11;
    dig[11] = temp & BUTTON_12;
    


    return;
}

void spaceMouse::processAnalogData(const unsigned char *buffer,
				   analogData &ana)
{
    int scalVal[] = { 4096, 256, 16, 1 };
    int ret[6];
    
    for (int i = 0; i < 6; i++)
    {	
	int temp;
	ret[i] = 0;
	for (int k = 0; k < 4; k++)
	{
	    temp = buffer[k + 4*i] & NIBBLE_MASK;
	    ret[i] += scalVal[k] * temp;
	}
	ret[i] -= 32768;
    }

    for (int i = 0; i < 6; i++)
    {
	ana[i] = ret[i];
//	cout<<"Axis["<<i<<"] = "<<ret[i]<<endl;
    }
}

int spaceMouse::puckClose()
{
    return portClose(_file);
}


//----------------------------------------------------------------------------
//--  spaceMousePlus   -------------------------------------------------------
//----------------------------------------------------------------------------
void spaceMousePlus::init(const string devName, const string portName,
	      const int baud)
{
    spaceMouse::init(devName, portName, baud);
    MODE_CODE = 'c';
}

void spaceMousePlus::puckMode()
{
    /// Performs configuration of puck; if compress_dom == 1, 
    /// only dominant component is set to non-zero; if compress_trans or
    /// compress_rot == 0, that component is set to zero
    unsigned char buffer[64];
    int first = 0;
    if (compress_dom) first += 4;
    if (compress_trans) first += 2;
    if (compress_rot) first += 1;
    int second = 2;
    // Turn off quicktip;
    // report buttons 10, 11 separately
    sprintf((char *) buffer, "%c%c%c\r", MODE_CODE, 
	    returnNibble(first), returnNibble(second));
    int n = portWrite(_file, buffer, 4);
    if (n < 0)
	cerr << WRITE_FAIL(4) << endl;
    n = portRead(_file, buffer, 64);
}



//----------------------------------------------------------------------------
//--  spaceTraveler   --------------------------------------------------------
//----------------------------------------------------------------------------
spaceTraveler::spaceTraveler(const std::string deviceName, 
			     const std::string portName)
    : _analogCache(6), _digitalCache(6), _zeroValues(6),
      _nullVal(1000)
{
    init(deviceName, portName);
}

spaceTraveler::~spaceTraveler()
{
}

void spaceTraveler::init(const string devName, const string port)
{
    _devName = devName;
    _portName = port;
    
    // Byte codes
#if defined(VPR_OS_Linux) || defined(linux_x86_64)
    DIGITAL_CODE = EV_KEY;
    ANALOG_CODE = EV_REL;
    SYNC_CODE = EV_SYN;
#endif              /* defined(VPR_OS_Linux) || defined(linux_x86_64) */
    USB_BUTTON_1 = 256;
    USB_BUTTON_2 = 257;
    USB_BUTTON_3 = 258;
    USB_BUTTON_4 = 259;
    USB_BUTTON_5 = 260;
    USB_BUTTON_6 = 261;
    USB_BUTTON_7 = 262;
    USB_BUTTON_8 = 263;
    USB_BUTTON_9 = 264;
    USB_BUTTON_10 = 265;
    USB_BUTTON_11 = 266;
    USB_BUTTON_12 = 267;
    USB_AXIS_1 = 0;
    USB_AXIS_2 = 2;
    USB_AXIS_3 = 1;
    USB_AXIS_4 = 3;
    USB_AXIS_5 = 5;
    USB_AXIS_6 = 4;

    _zeroRadius = 100;
    _autozero = true;
    
    // Init cached values
    for (unsigned int i = 0; i < _analogCache.size(); i++)
    {
	_analogCache[i] = _nullVal;
	_zeroValues[i] = 0;
    }
    for (unsigned int i = 0; i < _digitalCache.size(); i++)
	_digitalCache[i] = -1;
    
}

int spaceTraveler::puckOpen()
{
    int options = O_RDONLY;
    if (!portOpen(_portName.c_str(), options, _file))
    {
	cerr << OPEN_PORT << _portName << endl;
	return 0;
    }
  
    querySettings();
    return 1;
}

int spaceTraveler::puckClose()
{
    return portClose(_file);
}

int spaceTraveler::processBuffer(analogData &ana, digitalData &dig)
{
    const int size_buffer = 16;
    bool done = false;
    int result = 0;
    while (!done)
    {
	int buffer[size_buffer];
	int rc = portRead(_file, buffer, size_buffer);
	if (rc < 1) return 0;

	if (buffer[0] == DIGITAL_CODE)
	{
	    if (processDigitalData(buffer + 1, dig)) // data has changed
	    {
		done = true;
		result = 1;
	    }
	}
	else if (buffer[0] == ANALOG_CODE)
	{
	    if (processAnalogData(buffer + 1, ana))
	    {
		done = true;
		result = 2;
	    }
	}

    }
    
    return result;
}

bool spaceTraveler::processDigitalData(int buffer[], digitalData &dig)
{
    // This is struct input_event.code
    int button = buffer[0];
    // This is struct input_event.value
    int value = buffer[1];
    if (     button == USB_BUTTON_1)
	dig[0] = value;
    else if (button == USB_BUTTON_2)
	dig[1] = value;
    else if (button == USB_BUTTON_3)
	dig[2] = value;
    else if (button == USB_BUTTON_4)
	dig[3] = value;
    else if (button == USB_BUTTON_5)
	dig[4] = value;
    else if (button == USB_BUTTON_6)
	dig[5] = value;
    else if (button == USB_BUTTON_7)
	dig[6] = value;
    else if (button == USB_BUTTON_8)
	dig[7] = value;
    else if (button == USB_BUTTON_9)
	dig[8] = value;
    else if (button == USB_BUTTON_10)
	dig[9] = value;
    else if (button == USB_BUTTON_11)
	dig[10] = value;
    else if (button == USB_BUTTON_12)
	dig[11] = value;
    else cerr << GENERIC_ERROR << "unknown button press" << endl;
    return true;
}

bool spaceTraveler::processAnalogData(int buffer[], analogData &ana)
{
    // This is struct input_event.code
    int axis = buffer[0];
    // This is struct input_event.value
    int value = buffer[1];

    int localIndex;
    
    if (axis == USB_AXIS_1)
    {
	localIndex = 0;
    }
    else if (axis == USB_AXIS_2)
    { 
	localIndex = 1; 
	value = -value;
    }
    else if (axis == USB_AXIS_3)
    {
	localIndex = 2;
    }
    else if (axis == USB_AXIS_4)
    {
	localIndex = 3;
    }
    else if (axis == USB_AXIS_5)
    {
	localIndex = 4; 
	value = -value;
    }
    else if (axis == USB_AXIS_6)
    { 
	localIndex = 5;
    }
    else return false;
    
    ana[localIndex] = value;
    return true;
}

void spaceTraveler::writeSettings()
{

}

void spaceTraveler::querySettings()
{
#if defined(VPR_OS_Linux) || defined(linux_x86_64)
    char name[256] = "unknown";
    if (ioctl(_file, EVIOCGNAME(sizeof(name)), name) < 0)
    {
	cerr << GENERIC_ERROR << " ioctl system call failed" << endl;
	return;
    }
    cout << endl 
	 << " //----------------------- @@ Puck Device Driver @@";
    cout << " ---------------------------//" << endl;
    cout << " //--  A " << _devName << " is connected to the computer." << endl;
    cout << " //--  It reports itself as a \"" << name << "\"" << endl;
    cout << " //--         Port: \"" << _portName << "\"" << endl;
    cout << " //----------------------- @@ Puck Device Driver @@";
    cout << " ---------------------------//" 
	 << endl << endl;
    
#endif              /* defined(VPR_OS_Linux) || defined(linux_x86_64) */
}


//----------------------------------------------------------------------------
//--  spaceBall4000FLX   -----------------------------------------------------
//----------------------------------------------------------------------------
void spaceBall4000FLX::init(const string devName, const string portName,
	      const int baud)
{
    _devName = devName;
    _portName = portName;
    _baud = baud;
 
    // Byte codes
    DIGITAL_CODE  = '.';
    OLD_DIG_CODE  = 'K';
    ANALOG_CODE   = 'D';
    MODE_CODE     = 'C';
    SENS_CODE     = 'F';
    BEEP_CODE    =  'B';
    ZEROCAP_CODE  = 'Z';
    NULL_CODE     = 'N';
    QUERY_CODE    = '?';//'Q';
    ERROR_CODE    = 'E';
    RESET_CODE    = '@';
    RATE_CODE     = 'P';
    MOVEMENT_MODE = 'M';

    // Packet lengths
    ANALOG_PACKET   = 16;
    DIGITAL_PACKET  = 4;

    _checkControlChars = true;
}

spaceBall4000FLX::~spaceBall4000FLX()
{}

int spaceBall4000FLX::puckConfigure()
{
    puckInit();
    puckMode();
    puckSensitivity();
    puckNullRadius();
    puckZeroCap();
//    puckVersion();
    puckBeep();
    writeSettings();
    return 1;
}

int spaceBall4000FLX::puckOpen()
{
    int options;
#if defined(WIN32) || defined(WIN64)
    options = GENERIC_READ | GENERIC_WRITE;
#else
    options = O_RDWR | O_NONBLOCK | O_NOCTTY;
#endif

    if (!portOpen(_portName.c_str(), options, _file))
     {
 	cerr << OPEN_PORT << _portName << endl;
 	return 0;
     }

    puckSleep(1);
    
    if (!portConfigure())
	return 0;
    if (!puckConfigure())
	return 0;
    return 1;
}

void spaceBall4000FLX::puckInit()
{
    int n = portWrite(_file, (unsigned char *) "@\r", 2);
    if (n < 0)
 	cerr << WRITE_FAIL(2) << endl;
    puckSleep(2);
    _version = "Unknown";
    left_mode = 0;
}

void spaceBall4000FLX::querySettings()
{
}

void spaceBall4000FLX::writeSettings()
{
    cout << endl << " //----------------------- @@ Puck Device Driver @@";
    cout << " ---------------------------//" << endl;
    cout << " //--  A " << _devName << " is connected to the computer." << endl;
    cout << " //--  Serial port: \"" << _portName << "\"" << endl;
    cout << " //--        Speed: " << _baud << " bps" << endl;
    cout << " //--      Version: \"" << _version << "\"" << endl;
    cout << " //--     Settings --------------->" << endl;
    cout << " //--              Left-handed Mode: " << left_mode << endl;
    cout << " //----------------------- @@ Puck Device Driver @@";
    cout << " ---------------------------//" 
	 << endl << endl;
}

void spaceBall4000FLX::puckMode()
{
//    int n = write(_file, "CB\rP@r@r\rMSSV\rZ\r", 16);
//    int n = write(_file, "CB\rNT\rFTp\rFRp\rP@r@r\rMSSV\rZ\rBcCcC\r", 33);
//    int n = write(_file, "CB\rNT\rFTp\rFRp\rP32\rMSSV\rZ\rBcCcC\r", 31);
//    int n = write(_file, "CB\rP32\rMSSV\r", 12);
    int n = portWrite(_file, (unsigned char *) "CB\rP32\rMSSV\r", 12);
    if (n < 0)
 	cerr << WRITE_FAIL(12) << endl; 
//    n = portRead(_file, buffer, 256);
}

string spaceBall4000FLX::puckVersion()
{
    unsigned char buffer[256];
    int n = portWrite(_file, (unsigned char *) "@\r\r\r", 4);
//    n = portRead(_file, buffer, 256);
    if (n < 1)
	return "";
    buffer[n-1] = '\0';
    if (buffer[0] == '@' && buffer[1] == '2')
    {
	// get rid of carriage return
	buffer[n-1] = '\0';
	_version = (char*)(buffer+3);
    }
//    n = portRead(_file, buffer, 256);
    if (n < 1)
	return "";
    buffer[n-1] = '\0';
    if (buffer[0] == '@' && buffer[1] == '2')
    {
	// get rid of carriage return
	buffer[n-1] = '\0';
	_version = (char*)(buffer+3);
    }
    return "unknown";
}

void spaceBall4000FLX::puckNullRadius()
{
     int n = portWrite(_file, (unsigned char *) "NT\r", 3);
     if (n < 0)
	 cerr << WRITE_FAIL(3) << endl;
//    n = portRead(_file, buffer, 256);
}

void spaceBall4000FLX::puckZeroCap()
{
    int n = portWrite(_file, (unsigned char *) "Z\r", 2);
    if (n < 0)
	cerr << WRITE_FAIL(2) << endl;
//    n = portRead(_file, buffer, 256);
}

 
void spaceBall4000FLX::puckBeep()
{
     int n = portWrite(_file, (unsigned char *) "BcCcC\r", 6);
     if (n < 0)
	 cerr << WRITE_FAIL(6) << endl;
//    n = portRead(_file, buffer, 256);
}

void spaceBall4000FLX::puckSensitivity()
{
     int n = portWrite(_file, (unsigned char *) "FTp\rFRp\r", 8);
     if (n < 0)
	 cerr << WRITE_FAIL(8) << endl;
//    n = portRead(_file, buffer, 256);
}


int spaceBall4000FLX::portConfigure()
{

#if defined(WIN32) || defined(WIN64)

    DCB dcb;
    FillMemory(&dcb, sizeof(dcb), 0);
    dcb.DCBlength = sizeof(dcb);

    // magellan stuff:
    dcb.BaudRate = CBR_9600;
    dcb.fBinary = TRUE;
    dcb.fParity = FALSE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fDsrSensitivity = TRUE;//FALSE;
    dcb.fErrorChar = TRUE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fAbortOnError = FALSE;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;//TWOSTOPBITS;//
    dcb.ErrorChar = -1;
    dcb.EvtChar = '\r';
    dcb.EofChar = -1;
    dcb.XonChar = -1;
    dcb.XoffChar = -1;
 
    dcb.fNull = FALSE;

    dcb.XonLim = 4096;
    dcb.XoffLim = 0;
    
    if (!SetCommState(_file, &dcb))
    {
	cerr << GENERIC_ERROR << " can't set comm state" << endl;
	return 0;
    }

    if (!SetCommMask(_file, EV_RXFLAG))
    {
	cerr << GENERIC_ERROR << " can't set comm mask" << endl;
	return 0;
    }
    GetCommMask(_file, &_commMask);
 
    // Initialize overlapped event
    _event.Internal = 0;
    _event.InternalHigh = 0;
    _event.Offset = 0;
    _event.OffsetHigh = 0;
    _event.hEvent = 0;

    // Create temporary overlapped event
    OVERLAPPED EVENT;
    EVENT.Internal = 0;
    EVENT.InternalHigh = 0;
    EVENT.Offset = 0;
    EVENT.OffsetHigh = 0;
    EVENT.hEvent = CreateEvent(NULL, TRUE, FALSE, "PuckDeviceSerialEvent");
    if (EVENT.hEvent == INVALID_HANDLE_VALUE)
    {
	cerr << GENERIC_ERROR << " can't create overlapped serial event" << endl;
	return 0;
    }
    else if (EVENT.hEvent == NULL)
    {
	cerr << GENERIC_ERROR << " can't create overlapped serial event 2" << endl;
	return 0;
    }
 
    if (!GetCommTimeouts(_file, &_oldTimeouts))
    {
	cerr << GENERIC_ERROR << " can't get comm timeouts" << endl;
	return 0;
    }
    COMMTIMEOUTS newTimeouts;
    newTimeouts.ReadIntervalTimeout = MAXDWORD;
    newTimeouts.ReadTotalTimeoutMultiplier = 0;//MAXDWORD;
    newTimeouts.ReadTotalTimeoutConstant = 0;//32000;
    newTimeouts.WriteTotalTimeoutMultiplier = 10;
    newTimeouts.WriteTotalTimeoutConstant = 100;
    if (!SetCommTimeouts(_file, &newTimeouts))
    {
	cerr << GENERIC_ERROR << " can't set comm timeouts" << endl;
	return 0;
    }

    puckSleep(1);
    return 1;

#else                              /* #if !defined(WIN32) && !defined(WIN64) */


#if defined(HP) || defined(HPUX)
    mflag m = MRTS | MDTR | MDRS;
    ioctl(_file, MCSETA, &m);
    ioctl(_file, MCSETA, &m);    
#endif

    // Set blocking behavior on read
    fcntl(_file, F_SETFL, 0);

    // get termios structure
    int status = tcgetattr(_file, &_tty_struct);
    // Local options
    // Enable canonical input; disable echo characters
    _tty_struct.c_lflag = ICANON;
//////////////////////    _tty_struct.c_lflag &= ~( ECHO | ECHOE);
    // Input options
    // Ignore parity errors and break conditions
    _tty_struct.c_iflag = IGNBRK | IGNPAR;
    // Output options
    // None
    _tty_struct.c_oflag = 0;
    // Control options
    // Enable receiver; 8 data bits; 1 stop bit; hangup on last close
    // (drop DTR); local line (keep owner same)    
/////////////////////////////    _tty_struct.c_cflag &= ~CSIZE;
    _tty_struct.c_cflag = CREAD | CS8 | HUPCL | CLOCAL; 
//////////////////////////    _tty_struct.c_cflag &= ~CSTOPB;
    // Control characters
    // Disable erase and kill special characters; min & time aren't used
    _tty_struct.c_cc[VEOL] = '\r';
    _tty_struct.c_cc[VERASE] = _POSIX_VDISABLE;
    _tty_struct.c_cc[VKILL] = _POSIX_VDISABLE;
    // Set input = output speed
    cfsetispeed(&_tty_struct, B9600);
    cfsetospeed(&_tty_struct, B9600);

    // Make changes after buffer flush
    tcsetattr(_file, TCSAFLUSH, &_tty_struct);

    return 1;

#endif
}

void spaceBall4000FLX::checkControlChars(unsigned char *buffer, int &len)
{
    for (int i = 0; i < len; i++)
    {
	bool found = false;
	if (buffer[i] == '^')
	{
	    found = true;
	    for (int j = i; j < len-1; j++)
	    {
		if (found)
		{
		    // Make them "un-escaped"
		    buffer[j] = buffer[j+1] &= 31;
		    found = false;
		}
		else
		    buffer[j] = buffer[j+1];
	    }
	    buffer[len-1] = '\r';
	    len--;
	}
    }
}

int spaceBall4000FLX::processBuffer(analogData &ana, digitalData &dig)
{
    const bool checkControlChar = true;

    const int size_buffer = 512;
    unsigned char buffer[size_buffer];
    int rc = portRead(_file, buffer, size_buffer);
    buffer[rc] = '\0';
    if (checkControlChar)
    {
	checkControlChars(buffer, rc);
    }
    if (rc == 0)
	return 0;
    else return handleRead(buffer, rc, ana, dig);
}


int spaceBall4000FLX::handleRead(unsigned char* buffer, int bRead,
				 analogData &ana, digitalData &dig )
{
    if (buffer[0] == ANALOG_CODE && bRead == ANALOG_PACKET)	// analog data
    {
	processAnalogData(buffer + 1, ana);
	return 2;
    }
    else if (buffer[0] == DIGITAL_CODE && bRead == DIGITAL_PACKET)
    {
	processDigitalData(buffer + 1, dig);
	return 1;
    }
    else if (buffer[0] == OLD_DIG_CODE && bRead == (2 * DIGITAL_PACKET) )	// digital data
    {
	processDigitalData(buffer + 5, dig);
	return 1;
    }
    if (buffer[0] == ERROR_CODE)
    {
	processError(buffer);
    }
    return 0;
}

void spaceBall4000FLX::processError(const unsigned char *buffer)
{
}

void spaceBall4000FLX::processDigitalData(const unsigned char* buffer,
					 digitalData &dig)
{
    /* Buttons are represented by following bits:

Bit:         7    6    5    4    3    2    1    0
            --------------------------------------
Meaning:     
(First Byte)
             0    1   <L> <12> <11> <10>  <9>  <8>

(Second Byte)
            <7>   1   <6>  <5>  <4>  <3>  <2>  <1>

e.g. The higher 4 bits of the first byte are always 01xy, where
    x = the 'left-handed' mode of the SpaceBall, and
    y = the on/off state of button 12
    */
    int temp = (buffer[1] & 63) |
	( (buffer[1] & 128) >> 1 ) |
	( (buffer[0] & 63) << 7 );
    left_mode = (temp & LEFT_MODE) >> 12;

    dig[0] = temp & BUTTON_1;
    dig[1] = temp & BUTTON_2;
    dig[2] = temp & BUTTON_3;
    dig[3] = temp & BUTTON_4;
    dig[4] = temp & BUTTON_5;
    dig[5] = temp & BUTTON_6;
    dig[6] = temp & BUTTON_7;
    dig[7] = temp & BUTTON_8;
    dig[8] = temp & BUTTON_9;
    dig[9] = temp & BUTTON_10;
    dig[10] = temp & BUTTON_11;
    dig[11] = temp & BUTTON_12;
    
    return;
}

void spaceBall4000FLX::processAnalogData(const unsigned char *buffer,
					 analogData &ana)
{
    int ret[6];
    // 1st 2 char's give a time signal, axes start with 3rd char.
    for (int i = 0; i < 6; i++)
    {	
	ret[i] = (buffer[2*i+2] << 8) |
	    buffer[2*i+3];
	// Bit 16 is sign bit for upper 16 bits
	if (ret[i] & (1 << 15))
	    ret[i] |= 0xFFFF0000;
	else
	    ret[i] &= 0xFFFF;
    }
    ret[2] = -ret[2];
    // switch roll direction
    ret[5] = -ret[5];

    for (int i = 0; i < 6; i++)
    {
	ana[i] = ret[i];
//	cout<<"Axis["<<i<<"] = "<<ret[i]<<endl;
    }
    return;
}

int spaceBall4000FLX::puckClose()
{
    return portClose(_file);
}




//----------------------------------------------------------------------------
//--  spaceBall2003B   -------------------------------------------------------
//----------------------------------------------------------------------------
void spaceBall2003B::init(const string devName, const string portName,
			 const int baud)
{
    spaceBall4000FLX::init(devName, portName, baud);
    _checkControlChars = false;
    DIGITAL_CODE  = 'K';
}


int spaceBall2003B::puckConfigure()
{
    puckInit();
    puckMode();
    puckSensitivity();
    puckNullRadius();
    puckZeroCap();
    puckVersion();
//    puckBeep();
    writeSettings();
    return 1;
}

void spaceBall2003B::puckMode()
{
//    int n = write(_file, "CB\rP@r@r\rMSSV\rZ\r", 16);
//    int n = write(_file, "CB\rNT\rFTp\rFRp\rP@r@r\rMSSV\rZ\rBcCcC\r", 33);
//    int n = write(_file, "CB\rNT\rFTp\rFRp\rP32\rMSSV\rZ\rBcCcC\r", 31);
// Didn't have much luck with the init sequence... anybody know?
//    int n = write(_file, "CBT\rP@r@r\rMSSVLA\r", 17);
    int n = portWrite(_file, (unsigned char *) "CBT\rP@r@r\rMSSVLA\r", 17);
    if (n < 0)
 	cerr << WRITE_FAIL(17) << endl; 
//    n = portRead(_file, buffer, 256);
}

void spaceBall2003B::processDigitalData(const unsigned char* buffer,
					digitalData &dig)
{

    /* Buttons are represented by following bits:

Bit:         7    6    5    4    3    2    1    0
            --------------------------------------
Meaning:     
(First Byte)
v             0    1    0   <P>  <8>  <7>  <6>  <5>

(Second Byte)
             0    1    0    0   <4>  <3>  <2>  <1>

e.g. The higher 4 bits of the first byte are always 010x, where
    x = the on/off state of the pick button
    */    
    int temp = (buffer[1] & 15) |
	( (buffer[0] & 31) << 4 );
    dig[0] = temp & BUTTON_1;
    dig[1] = temp & BUTTON_2;
    dig[2] = temp & BUTTON_3;
    dig[3] = temp & BUTTON_4;
    dig[4] = temp & BUTTON_5;
    dig[5] = temp & BUTTON_6;
    dig[6] = temp & BUTTON_7;
    dig[7] = temp & BUTTON_8;
    dig[8] = temp & BUTTON_9;
    dig[9] = temp & BUTTON_10;
    dig[10] = temp & BUTTON_11;
    dig[11] = temp & BUTTON_12;
    return;
}


