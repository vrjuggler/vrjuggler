//----------------------------------------------------------------------------
//--    Copyright (c) 2002 Fuel Tech Inc.  All Rights Reserved.             --
//--    vriPuckDevice.h                                                     --
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

#ifndef fti_vriPuckDevice_h
#define fti_vriPuckDevice_h



#if defined(WIN32) || defined(WIN64)
# include <windows.h>
#else
# include <termios.h> 
#endif

#include <gadget/gadgetParam.h>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Analog.h>
#include <gadget/InputManager.h>

/*
  Note:  this code needs read (or read/write) access to the serial/usb ports
  to access the device.  Accordingly it is not compatible with the drivers
  distributed by 3DConnexion, which need to be turned off for this to work.
*/

//----------------------------------------
/** @name Defines
 */
//@{
/** Byte masks
 */
//@{
#define NIBBLE_MASK      15
#define ODD_NIBBLE_CODE  64
#define EVEN_NIBBLE_CODE 48
//@}
/** Error messages
 */
//@{
#define WRITE_FAIL(x)    "[PuckDevice] ERROR:  Write of " #x " bytes failed"
#define INSUFF_MEM       "[PuckDevice] ERROR:  Insufficent memory"
#define OPEN_PORT	 "[PuckDevice] ERROR:  Could not open serial port "
#define WRONG_SPEED      "[PuckDevice] ERROR:  Only 9600 baud supported by "
#define ILLEGAL_COMMAND  "[PuckDevice] ERROR:  Illegal command byte:  "
#define FRAMING_ERROR    "[PuckDevice] ERROR:  Framing error after character received"
#define GENERIC_ERROR    "[PuckDevice] ERROR..."
//@}
//@}  

/// Forward declaration
class basePuck;

/// Digital data type
typedef std::vector<gadget::DigitalData> digitalData;
/// Analog data type
typedef std::vector<gadget::AnalogData>  analogData;


//----------------------------------------------------------------------------
//--  basePuck   -------------------------------------------------------------
//----------------------------------------------------------------------------
/** Base class provides functionality to specific derived devices.
*/
class basePuck
{
  public:
    ///
    basePuck();
    ///
    virtual ~basePuck();
    /** Methods used to access and configure
    *** the puck device
    */
    //@{
    /// Open, configure and initialize settings
    virtual int puckOpen() = 0;
    /// Close and finalize settings
    virtual int puckClose() = 0;

    /// Pause for specified seconds
    virtual void puckSleep(const float seconds);

    /// Write out the settings
    virtual void writeSettings() = 0;

    /** Called upon a port read, this method should handle filling the
    *** data structures sent in to it.
    *** @return 1 if digital data changed, 2 if analog data changed,
    *** else 0 if neither affected.  Only one buffer will be affected
    *** per call.
    *** @param dig Data structure into which to place the result
    *** @param ana Data structure into which to place the result
    */
    virtual int processBuffer(analogData &ana, digitalData &dig) = 0;
    //@}};

  protected:

#if defined(WIN32) || defined(WIN64)
    /// Generic file descriptor
    typedef HANDLE HandleType;
#else
    /// Generic file descriptor
    typedef int HandleType;
#endif
    

    /** Opens the port
    *** @param name The serial port's name
    *** @param opts Specific options used when opening
    *** @param handle The file handle
    *** @return True if success, else false
    */
    virtual bool portOpen(const char *name, int opts, HandleType &handle) = 0;
    /** Closes the port
    *** @param handle The file handle
    *** @return True if success, else false
    */
    virtual bool portClose(HandleType handle) = 0;



    /// File descriptor for serial port
    HandleType _file;
    /// name of port
    std::string _portName;
    /// device name
    std::string _devName;
    /// version / identifying string
    std::string _version;

};

//----------------------------------------------------------------------------
//--  basePuckSerial   -------------------------------------------------------
//----------------------------------------------------------------------------
class basePuckSerial : public basePuck
{
  public:
    ///
    virtual ~basePuckSerial();
    
  protected:

    /// Returns the device's version number
    virtual std::string puckVersion() { return _version; }
    /// Performs configuration of puck's various modes.
    virtual void puckMode() {};
    /// Process an error 
    virtual void processError(const unsigned char* buffer) {};
    /// Make puck beep
    virtual void puckBeep() {};
    /// Zero the cap at the current position
    virtual void puckZeroCap() {};
    /// Set sensitivity of cap/mouse
    virtual void puckSensitivity() {};
    /// Set null radius (minimum required movement of cap)
    virtual void puckNullRadius() {};
    /// Get the settings
    virtual void querySettings() = 0;

    //----------------------------------------
    /** Device Manipulation
    */
    //@{
#if defined(WIN32) || defined(WIN64)
    /// The port's original timeout settings
    COMMTIMEOUTS _oldTimeouts;
    /// An overlapped io event
    OVERLAPPED _event;
    /// Used to receive notice when an event flag is received
    DWORD _commMask;
#else
    /// Termios struct
    struct termios _tty_struct;
#endif

    /// Read in digital data
    virtual void processDigitalData(const unsigned char* buffer,
				    digitalData &dig) = 0;
    /// Read in analog data
    virtual void processAnalogData(const unsigned char* buffer,
				   analogData &ana) = 0;

    /** Direct port manipulation.
     */
    //@{
    /** Opens the port
    *** @param name The serial port's name
    *** @param opts Specific options used when opening
    *** @param handle The file handle
    *** @return True if success, else false
    */
    virtual bool portOpen(const char *name, int opts, HandleType &handle);
    /** Closes the port
    *** @param handle The file handle
    *** @return True if success, else false
    */
    virtual bool portClose(HandleType handle);
    /** Reads the serial port
    *** @param handle The file handle
    *** @param buffer The buffer to put data into
    *** @param bufferSize The size of the buffer
    *** @return The number of bytes read
    */
    virtual int portRead(HandleType handle, unsigned char* buffer, 
			 int bufferSize);
    /** Writes to the serial port
    *** @param handle The file handle
    *** @param buffer The buffer containing the data
    *** @param bufferSize The size of the buffer
    *** @return The number of bytes written
    */
    virtual int portWrite(HandleType handle, unsigned char* buffer, 
			  int bufferSize);
    //@}

    /** Button masks
     */
    //@{
#define BUTTON_1  0x1 << 0
#define BUTTON_2  0x1 << 1
#define BUTTON_3  0x1 << 2
#define BUTTON_4  0x1 << 3
#define BUTTON_5  0x1 << 4
#define BUTTON_6  0x1 << 5
#define BUTTON_7  0x1 << 6
#define BUTTON_8  0x1 << 7
#define BUTTON_9  0x1 << 8
#define BUTTON_10 0x1 << 9
#define BUTTON_11 0x1 << 10
#define BUTTON_12 0x1 << 11
#define LEFT_MODE 0x1 << 12
    //@}
    /// baud speed
    int _baud;
    //@}
};


//----------------------------------------------------------------------------
//--  basePuckUSB   ----------------------------------------------------------
//----------------------------------------------------------------------------
class basePuckUSB : public basePuck
{
  public:
    ///
    virtual ~basePuckUSB();
    
  protected:

    ///
    virtual int portRead(HandleType handle, int buffer[], int bufferSize);
    ///
    
    /** Direct port manipulation.
     */
    //@{
    /** Opens the port
    *** @param name The serial port's name
    *** @param opts Specific options used when opening
    *** @param handle The file handle
    *** @return True if success, else false
    */
    virtual bool portOpen(const char *name, int opts, HandleType &handle);
    /** Closes the port
    *** @param handle The file handle
    *** @return True if success, else false
    */
    virtual bool portClose(HandleType handle);
    //@}

    //----------------------------------------
    /** @name Local Defines
     */
    //@{
    /** Byte codes
     */
    //@{
    int DIGITAL_CODE;
    int ANALOG_CODE;
    int USB_BUTTON_1;
    int USB_BUTTON_2;
    int USB_BUTTON_3;
    int USB_BUTTON_4;
    int USB_BUTTON_5;
    int USB_BUTTON_6;
    int USB_BUTTON_7;
    int USB_BUTTON_8;
    int USB_BUTTON_9;
    int USB_BUTTON_10;
    int USB_BUTTON_11;
    int USB_BUTTON_12;
    int USB_AXIS_1;
    int USB_AXIS_2;
    int USB_AXIS_3;
    int USB_AXIS_4;
    int USB_AXIS_5;
    int USB_AXIS_6;
    //@}
};


//----------------------------------------------------------------------------
//--  spaceMouse   -----------------------------------------------------------
//----------------------------------------------------------------------------
/** This is the Magellan / SpaceMouse from 3DConnexion, a Logitech Company.
*/
class spaceMouse : public basePuckSerial
{
  public:
    /** Constructor
     */
    spaceMouse(const std::string deviceName, const std::string portName,
	       const int baudRate)
 	{
 	    init(deviceName, portName, baudRate);
 	}
    ///
    virtual ~spaceMouse();
    /// Open
    virtual int puckOpen();
    /// Close
    virtual int puckClose();

    /** Called upon a port read, this method should handle filling the
    *** data structures sent in to it.
    *** @return 1 if digital data changed, 2 if analog data changed,
    *** else 0 if neither affected.  Only one buffer will be affected
    *** per call.
    *** @param dig Data structure into which to place the result
    *** @param ana Data structure into which to place the result
    */
    virtual int processBuffer(analogData &ana, digitalData &dig);

  protected:
    /** Initializes member variables
     */
    void init(const std::string devName, const std::string port,
	      const int baud);

    //----------------------------------------
    /** @name SpaceMouse Manipulation Functions
    *** Methods used to access and configure
    *** the Spacemouse
    */
    //@{
    /// Initialize settings
    virtual void puckInit();
    /// Write out the settings
    virtual void writeSettings();
    /// Get the settings
    virtual void querySettings();
    /** Performs initialization of the SpaceMouse. 
    *** @return 1 if successful, otherwise 0
    */
    virtual int puckConfigure();
    /// Performs needed configuration of serial port
    virtual int portConfigure();
    /** Performs configuration of SpaceMouse; if compress_dom == 1, 
    *** only dominant component is set to non-zero; if compress_trans or
    *** compress_rot == 0, that component is set to zero
    */
    virtual void puckMode();
    /// Process an error 
    virtual void processError(const unsigned char* buffer);
    /// Requests keyboard data from SpaceMouse
    virtual void queryKeyboard();
    /// Requests analog data from SpaceMouse
    virtual void queryMouse();
    /** Make SpaceMouse beep
    *** {\em beep}: beep_duration from 0 (32 ms) to 7 (2000 ms)
    */
    virtual void puckBeep();
    /// Zero the cap at the current position
    virtual void puckZeroCap();
    /** Set sensitivity of cap/mouse from 0 (linear) to 15 (quadratic)
    *** of translational and rotational axes (sens_trans, sens_rot)
    */
    virtual void puckSensitivity();
    /** Set null radius (minimum required movement of cap)
    *** null_radius from 0 (highest) to 15 (lowest)
    */
    virtual void puckNullRadius();
    /// Returns the device's version number
    virtual std::string puckVersion();
    /** Called upon a successful read of the serial port, this method
    *** is responsible for the parsing of the buffer of data read in.
    *** @param buffer Contains the bytes read
    *** @param size Number of bytes read
    *** @param ana The analog data structure
    *** @param dig The digital data structure
    *** @return 1 if digital data changed, 2 if analog data changed,
    *** else 0 if neither affected.  Only one buffer should be affected
    *** per call.
    */
    virtual int handleRead(unsigned char* buffer, int size,
			   analogData &ana, digitalData &dig);
    /// Read in digital data
    virtual void processDigitalData(const unsigned char* buffer,
				    digitalData &dig);
    /// Read in analog data
    virtual void processAnalogData(const unsigned char* buffer,
				   analogData &ana);
    //@}};

    //----------------------------------------
    /** @name SpaceMouse Settings
    *** Settings used to configure the Puck
    *** The various configuration options for the Puck
    */
    //@{
    /** Compressed data mode.
    *** All components in data packet set to 0 except
    *** component with largest magnitude
    */
    int compress_dom;
    /** Translational components of data packet set to
    *** 0 if translation = 0
    */
    int compress_trans;
    /** Rotational components of data packet set to
    *** 0 if translation = 0
    */
    int compress_rot;
    /** Sensitivity settings.
    *** Sets sensitivity of puck:  relationship between
    *** translational/rotational displacements of cap and 
    *** corresponding translational/rotational data transmitted.
    *** For example:  spaceMouse:
    *** 0: linear relationship
    *** 1..15: ballistic (quadratic) function is used
    */
    int sens_trans;
    ///
    int sens_rot;
    /** Defines smallest movement of the cap needed to generate a
    *** non-zero value.  
    *** In case of SpaceMouse, 0 requires smallest movement, 15 largest.
    */
    int null_radius;
    /// Duration of beeps, from 0 (32 ms) to 7 (2000 ms)
    int beep_duration;
    //@}

    //----------------------------------------
    /** @name Local Defines
     */
    //@{
    /** Byte codes
     */
    //@{
    char DIGITAL_CODE;
    char ANALOG_CODE;
    char MODE_CODE;
    char SENS_CODE;
    char BEEP_CODE;
    char ZEROCAP_CODE;
    char NULL_CODE;
    char VERSION_CODE;
    char QUERY_CODE;
    char ERROR_CODE;
    //@}
    /** Packet lengths
     */
    //@{
    int ANALOG_PACKET;
    int DIGITAL_PACKET;
    //@}
    /** Error codes
     */
    //@{
    char ILLEGAL_COMM_CODE;
    char FRAMING_ERROR_CODE;
    //@}
    //@}

    /// Byte/nibble manipulation
    unsigned char returnNibble(const int v);
    /// Nibble Query
    int returnNibble(const unsigned char c);
};




//----------------------------------------------------------------------------
//--  spaceMousePlus   -------------------------------------------------------
//----------------------------------------------------------------------------
/** This is the Magellan / SpaceMouse from 3DConnexion, a Logitech Company.
*** Model Plus XT serial
*/
class spaceMousePlus : public spaceMouse
{
  public:
    ///
    spaceMousePlus(const std::string deviceName, const std::string portName,
		   const int baudRate)
 	: spaceMouse(deviceName, portName, baudRate) 
 	{
 	    init(deviceName, portName, baudRate);
 	}

  protected:
    /** Initializes member variables
     */
    void init(const std::string devName, const std::string port,
	      const int baud);
    ///
    virtual void puckMode();
};



//----------------------------------------------------------------------------
//--  spaceTraveler   --------------------------------------------------------
//----------------------------------------------------------------------------
/** Currently implemented only on Linux
 */
class spaceTraveler : public basePuckUSB
{
  public:
    /// Constructor
    spaceTraveler(const std::string deviceName,
		  const std::string portName);
    ///
    virtual ~spaceTraveler();
    /// Open
    virtual int puckOpen();
    /// Close
    virtual int puckClose();

    /** Called upon a port read, this method should handle filling the
    *** data structures sent in to it.
    *** @return 1 if digital data changed, 2 if analog data changed,
    *** else 0 if neither affected.  Only one buffer will be affected
    *** per call.
    *** @param dig Data structure into which to place the result
    *** @param ana Data structure into which to place the result
    */
    virtual int processBuffer(analogData &ana, digitalData &dig);

  protected:
    /// Write out the settings
    virtual void writeSettings();
    /// Get the settings
    virtual void querySettings();
    /** Initializes member variables
     */
    void init(const std::string devName, const std::string port);
    /** Read in digital data
    *** @param buffer Buffer of data to be processed
    *** @param dig Data structure in which to return the data
    *** @return True if the data has changed from its previous state
    */
    virtual bool processDigitalData(int buffer[], digitalData &dig);
    /** Read in analog data
    *** @param buffer Buffer of data to be processed
    *** @param ana Data structure in which to return the data
    *** @return True if the data has changed from its previous state
    */
    virtual bool processAnalogData(int buffer[], analogData &ana);

    /// Cache the analog values
    std::vector<int> _analogCache;
    ///
    std::vector<int> _digitalCache;
    /// The radius of the null zone
    int _zeroRadius;
    /// Are we auto-zeroing the data?
    bool _autozero;
    /// Cache the auto-zeroing values
    std::vector<int> _zeroValues;
    /// Mark the uninitiated axis readings
    const int _nullVal;

};



//----------------------------------------------------------------------------
//--  spaceBall4000FLX   -----------------------------------------------------
//----------------------------------------------------------------------------
/** This is the SpaceBall 4000 FLX, from Labtec / SpaceTec.
*/
class spaceBall4000FLX : public basePuckSerial
{
  public:
    /** Constructor
     */
    spaceBall4000FLX(const std::string deviceName, const std::string portName,
 		     const int baudRate)
 	{
 	    init(deviceName, portName, baudRate);
 	}
    ///
    virtual ~spaceBall4000FLX();
    /// Open 
    virtual int puckOpen();
    /// Close
    virtual int puckClose();

    /** Called upon a port read, this method should handle filling the
    *** data structures sent in to it.
    *** @return 1 if digital data changed, 2 if analog data changed,
    *** else 0 if neither affected.  Only one buffer will be affected
    *** per call.
    *** @param dig Data structure into which to place the result
    *** @param ana Data structure into which to place the result
    */
    virtual int processBuffer(analogData &ana, digitalData &dig);

  protected:
    /** Initializes member variables
     */
    void init(const std::string devName, const std::string portName,
 	      const int baud);

    //----------------------------------------
    /** @name spaceBall4000FLX Manipulation Functions
    *** Methods used to access and configure
    *** the SpacBall
    */
    //@{
    /// Initialize settings
    virtual void puckInit();
    /// Write out the settings
    virtual void writeSettings();
    /// Get the settings
    virtual void querySettings();
    /** Performs initialization of the SpaceBall.  
    *** @return 1 if successful, otherwise 0
    */
    virtual int puckConfigure();
    /// Performs needed configuration of serial port
    virtual int portConfigure();
    /// Accesses settings of SpaceBall
    virtual void puckMode();
    /// Process an error 
    virtual void processError(const unsigned char* buffer);
    /// Make SpaceBall beep
    virtual void puckBeep();
    /// Zero the cap at the current position
    virtual void puckZeroCap();
    /// Set sensitivity of cap/mouse
    virtual void puckSensitivity();
    /// Set null radius (minimum required movement of cap)
    virtual void puckNullRadius();
    /// Returns the device's version number
    virtual std::string puckVersion();
    /**
    *** Remove control characters from buffer, writing the new
    *** length of buffer into len
    */
    void checkControlChars(unsigned char *buffer, int &len);
    /** Called upon a successful read of the serial port, this method
    *** is responsible for the parsing of the buffer of data read in.
    *** @param buffer Contains the bytes read
    *** @param size Number of bytes read
    *** @param ana The analog data structure
    *** @param dig The digital data structure
    *** @return 1 if digital data changed, 2 if analog data changed,
    *** else 0 if neither affected.  Only one buffer should be affected
    *** per call.
    */
    virtual int handleRead(unsigned char* buffer, int size,
			   analogData &ana, digitalData &dig);
    /// Read in digital data
    virtual void processDigitalData(const unsigned char* buffer,
				    digitalData &dig);
    /// Read in analog data
    virtual void processAnalogData(const unsigned char* buffer,
				   analogData &ana);
    //@}};

    //----------------------------------------
    /** @name SpaceBall Settings
    *** Settings used to configure the SpaceBall
    *** The various configuration options for the SpaceBall
    */
    //@{
    /** Left-handed mode
    *** The Spaceball has a detachable base that can be used left-or-right-
    *** handed. The forward axis (z-direction) needs to be adjusted
    *** accordingly.  If left_mode = 1, the 'ABC' buttons are on the left;
    *** else if left_mode = 0, the number buttons are to the left of the ball.
    */
    int left_mode;
    /** CheckControlChars
    *** True if the input stream needs to be processed to remove special 
    *** control characters.
    */
    bool _checkControlChars;
    //@}    

    //----------------------------------------
    /** @name Local Defines
     */
    //@{
    /** Byte codes
     */
    //@{
    char DIGITAL_CODE;
    char OLD_DIG_CODE;
    char ANALOG_CODE;
    char MODE_CODE;
    char SENS_CODE;
    char BEEP_CODE;
    char ZEROCAP_CODE;
    char NULL_CODE;
    char QUERY_CODE;
    char ERROR_CODE;
    char RESET_CODE;
    char RATE_CODE;
    char MOVEMENT_MODE;
    //@}
    /** Packet lengths
     */
    //@{
    int ANALOG_PACKET;
    int DIGITAL_PACKET;
    //@}
    //@}
};



//----------------------------------------------------------------------------
//--  spaceBall2003B   -------------------------------------------------------
//----------------------------------------------------------------------------
/** This is the SpaceBall 2003, from SpaceTec.
*/
class spaceBall2003B : public spaceBall4000FLX
{
  public:
    /** Constructor
     */
    spaceBall2003B(const std::string deviceName, const std::string portName,
		   const int baudRate)
 	:  spaceBall4000FLX(deviceName, portName, baudRate) 
 	{
 	    init(deviceName, portName, baudRate);
 	}

  protected:
    /** Initializes member variables
     */
    void init(const std::string devName, const std::string portName,
 	      const int baud);

    //----------------------------------------
    /** @name spaceBall2003B Manipulation Functions
    *** Methods used to access and configure
    *** the Spacemouse
    */
    //@{
    /** Performs initialization of the SpaceBall.  Returns 1
    *** if successful, otherwise 0
    */
    virtual int puckConfigure();
    /// Accesses settings of SpaceBall
    virtual void puckMode();
    /// Read in digital data
    virtual void processDigitalData(const unsigned char* buffer,
				    digitalData &dig);
    //@};
};


/** @name Puck Device Driver
*** A custom driver class used for puck-like devices
*** such as SpaceMouse or SpaceBall series
*** @memo Puck driver class for vrjuggler 2.0
*/
//@{

//----------------------------------------------------------------------------
//--  PuckDevice  ------------------------------------------------------------
//----------------------------------------------------------------------------
/** A driver for puck-like devices
*/
class PuckDevice : public gadget::input_digital_analog_t
{
  public:
    /// constructor
    PuckDevice() :
	_buttons(12),
	_axes(6),
	_running(false)
	{
	    setMin(-500.0f);
	    setMax(500.0f);
	    _puck = NULL;
	}
    /// destructor
    virtual ~PuckDevice();

    
    //----------------------------------------
    /** @name Inherited virtual functions
    *** @memo Functions from Device Driver Base Class 
    *** that need to be overridden
    */
    //@{
    /** Device driver starts sampling.
    *** @return 1 if success, else 0
    */
    virtual bool startSampling();
    /** Function accesses and internally stores device state.
    *** @return 1 if success, else 0
    */
    virtual bool sample();
    /** Device driver stops sampling, kill thread.
    *** @return 1 if success, else 0
    */
    virtual bool stopSampling();
    /** Access function returns configuration chunk type.
    *** @return Name of chunk type
    */
    static std::string getElementType();
    /** Performs necessary configuration to open device; system
    *** passes new vjConfigChunk into this function when a
    *** configuration change is detected for this driver.
    *** @args c Ptr. to a configuration object
    *** @return Success of configuration
    */
    virtual bool config(jccl::ConfigElementPtr c);
    /** Swaps data and vjInput:: indexes in triple-buffered data
     */
    virtual void updateData();
    //@}

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

  protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }

    /** Debugging helper functions.
     */
    //@{
    /// Simple function to write out the bits in a byte
    static void writeByte(const unsigned char c);
    /// Simple function to write out the bits in an int
    static void writeInt(const int temp);
    //@}

  private:

    //----------------------------------------
    ///
    void controlLoop();
    /// Initialize the analog and digital buffers
    void initBuffers();
    /// Digital data to be sent to gadget classes
    digitalData _buttons;
    /// Analog data to be sent to gadget classes
    analogData _axes;
    /// Ptr. to specific device class
    basePuck *_puck;
    ///
    bool _running;

};
//@}

#endif
