//////////////////////////////////////////////////////////////////////////////
//
//      File Name:      isense.h
//      Description:    Header File for InterSense DLL
//      Created:        12/4/98
//      Author:         Yury Altshuler
//
//      Copyright:      InterSense 2002 - All rights Reserved.
//
//      Comments:       This dynamic link library is provided to simplify 
//                      communications with all models of InterSense tracking devices. 
//                      Currently, these include IS-300, IS-600, IS-900, InertiaCube2 and 
//                      InterTrax. It can detect, configure, and get data from up to 4 trackers.
//
//                      DLL provides two methods of configuring the IS series trackers.
//                      You can use the provided function calls and hardcode the 
//                      settings in your application, or use the isenseX.ini files. 
//                      Second method is strongly recommended, as it provides you with 
//                      ability to change the configuration without recompiling your 
//                      application. In the isenseX.ini file name X is a number, starting 
//                      at 1, identifying one tracking system in the order if 
//                      initialization. Use included isense1.ini file as example. 
//                      Only enter the settings you want the program to change.
//
//                      InterTrax requires no configuration. All function calls and 
//                      settings in the isenseX.ini files are ignored.
//                      
//                      
//                      Items not implemented in this release:
//
//                          ISD_OpenConfigWindow
//                          infoScreen
//
//                       
//////////////////////////////////////////////////////////////////////////////
#ifndef _ISD_isenseh
#define _ISD_isenseh


#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__)
    #include <windows.h>
    #include "types.h"
#else
    #include "types.h"
    #ifndef UNIX
        #define UNIX
    #endif
#endif

#ifndef TRUE
    #define TRUE    1
    #define FALSE   0
#endif

//////////////////////////////////////////////////////////////////////////////
//
// This defines the calling conventions of the entrypoints to the
// DLL. This varies depending upon the operating system.
//
//////////////////////////////////////////////////////////////////////////////

#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__)
    #define	DLLEXPORT __declspec(dllexport)
    #define	DLLENTRY __cdecl
    typedef void (* DLL_EP)(void);
    #define	DLL_EP_PTR __cdecl *
#else
    #define	DLLEXPORT
    #define	DLLENTRY
    typedef void (* DLL_EP)(void);
    #define	DLL_EP_PTR *
#endif


//////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
extern "C"
{
#endif


// tracking system type
typedef enum
{
    ISD_NONE = 0,           // none found, or can't identify 
    ISD_PRECISION_SERIES,   // InertiaCube2, IS-300, IS-600, IS-900 and IS-1200 
    ISD_INTERTRAX_SERIES    // InterTrax 

} ISD_SYSTEM_TYPE;


// tracking system model 
typedef enum
{
    ISD_UNKNOWN = 0,          
    ISD_IS300,          // 3DOF system 
    ISD_IS600,          // 6DOF system 
    ISD_IS900,          // 6DOF system   
    ISD_INTERTRAX,      // InterTrax (Serial) 
    ISD_INTERTRAX_2,    // InterTrax (USB) 
    ISD_INTERTRAX_LS,   // InterTraxLS, verification required 
    ISD_INTERTRAX_LC,   // InterTraxLC 
    ISD_ICUBE2,         // InertiaCube2 
    ISD_ICUBE2_PRO,     // InertiaCube2 Pro 
    ISD_IS1200          // 6DOF system   

} ISD_SYSTEM_MODEL;


typedef enum
{
    ISD_INTERFACE_UNKNOWN = 0,
    ISD_INTERFACE_SERIAL,
    ISD_INTERFACE_USB,
    ISD_INTERFACE_ETHERNET_UDP,
    ISD_INTERFACE_ETHERNET_TCP,
    ISD_INTERFACE_IOCARD,
    ISD_INTERFACE_PCMCIA

} ISD_INTERFACE_TYPE;


// for now limited to 8 
#define ISD_MAX_STATIONS       8

#define ISD_MAX_TRACKERS       4 

// orientation format 
#define ISD_EULER              1
#define ISD_QUATERNION         2

// Coordinate frame in which position and orientation data is reported 
#define ISD_DEFAULT_FRAME      1    // InterSense default 
#define ISD_VSET_FRAME         2    // Virtual set frame, use for camera tracker only 

// number of supported stylus buttons 
#define MAX_NUM_BUTTONS        8

// hardware is limited to 10 analog channels per station 
#define MAX_ANALOG_CHANNELS    10


typedef int ISD_TRACKER_HANDLE;


typedef struct
{
    // Following item are for information only and should not be changed 

    float  LibVersion;     // InterSense Library version 

    DWORD  TrackerType;    // IS Precision series or InterTrax. 
                           // TrackerType can be: 
                           // ISD_PRECISION_SERIES for IS-300, IS-600, IS-900 and IS-1200 model trackers, 
                           // ISD_INTERTRAX_SERIES for InterTrax, or 
                           // ISD_NONE if tracker is not initialized 

    DWORD  TrackerModel;   // ISD_UNKNOWN, ISD_IS300, ISD_IS600, ISD_IS900, ISD_INTERTRAX 
    
    DWORD  Port;           // Number of the rs232 port. Starts with 1. 

    // Communications statistics. For information only. 

    DWORD  RecordsPerSec;
    float  KBitsPerSec;    

    // Following items are used to configure the tracker and can be set in
    // the isenseX.ini file 

    DWORD  SyncState;   // 4 states: 0 - OFF, system is in free run 
                        //           1 - ON, hardware genlock frequency is automatically determined
                        //           2 - ON, hardware genlock frequency is specified by the user
                        //           3 - ON, no hardware signal, lock to the user specified frequency  

    float  SyncRate;    // Sync frequency - number of hardware sync signals per second, 
                        // or, if SyncState is 3 - data record output frequency 

    DWORD  SyncPhase;   // 0 to 100 per cent    

    DWORD  Interface;   // hardware interface 

    DWORD  dwReserved2; // reserved for future use  
    DWORD  dwReserved3;  
    DWORD  CallsPerSec;

    float  FirmwareRev; // Firmware revision 
    float  fReserved2;
    float  fReserved3;
    float  fReserved4;

    Bool   bReserved1;
    Bool   bReserved2;
    Bool   bReserved3;
    Bool   bReserved4;

} ISD_TRACKER_INFO_TYPE;


// ISD_STATION_INFO_TYPE can only be used with IS Precision Series tracking devices.
// If passed to ISD_SetStationState or ISD_GetStationState with InterTrax, FALSE is returned. 

typedef struct
{
    DWORD   ID;             // unique number identifying a station. It is the same as that 
                            // passed to the ISD_SetStationState and ISD_GetStationState   
                            // functions and can be 1 to ISD_MAX_STATIONS 

    Bool    State;          // TRUE if ON, FALSE if OFF 

    Bool    Compass;        // 0, 1 or 2 for OFF, PARTIAL and FULL. Older versions of tracker
                            // firmware supported only 0 and 1, which stood for ON or OFF. Please
                            // use the new notation. This API will correctly interpret the settings.
                            // Compass setting is ignored if station is configured for 
                            // Fusion Mode operation. 

    LONG    InertiaCube;    // InertiaCube associated with this station. If no InertiaCube is
                            // assigned, this number is -1. Otherwise, it is a positive number
                            // 1 to 4 

    DWORD   Enhancement;    // levels 0, 1, or 2 
    DWORD   Sensitivity;    // levels 1 to 4 
    DWORD   Prediction;     // 0 to 50 ms 
    DWORD   AngleFormat;    // ISD_EULER or ISD_QUATERNION 
   
    Bool    TimeStamped;    // TRUE if time stamp is requested 
    Bool    GetButtons;     // TRUE if wand or stylus button state is requested 
    Bool    GetEncoderData; // TRUE if raw encoder data is requested 
    Bool    GetAnalogData;  // TRUE if analog joystick data is requested 

    DWORD   CoordFrame;     // coord frame in which position and orientation data is reported  
    DWORD   dwReserved2;    // reserved for future use 
    DWORD   dwReserved3;     
    DWORD   dwReserved4;

    float   fReserved1;  
    float   fReserved2;
    float   fReserved3;
    float   fReserved4;

    Bool    GetCameraData;  // TRUE to get computed FOV, aperature, etc  
    Bool    bReserved2;     
    Bool    bReserved3;
    Bool    bReserved4;

} ISD_STATION_INFO_TYPE;


typedef struct
{
    BYTE    TrackingStatus;   // tracking status byte 
    BYTE    NewData;          
    BYTE    bReserved2;       // pack to 4 byte boundary 
    BYTE    bReserved3;

    float   Orientation[4];   // Supports both Euler and Quaternion formats 
    float   Position[3];      // Always in meters 
    float   TimeStamp;        // Seconds, reported only if requested 
    
    Bool    ButtonState[MAX_NUM_BUTTONS];    // Only if requested 

    // Current hardware is limited to 10 channels, only 2 are used. 
    // The only device using this is the IS-900 wand that has a built-in
    // analog joystick. Channel 1 is x-axis rotation, channel 2 is y-axis
    // rotation 

    short   AnalogData[MAX_ANALOG_CHANNELS]; // only if requested 

    LONG    lReserved1;
    LONG    lReserved2;
    LONG    lReserved3;
    LONG    lReserved4;

    DWORD   dwReserved1;
    DWORD   dwReserved2;
    DWORD   dwReserved3;
    DWORD   dwReserved4;

    float   fReserved1;
    float   fReserved2;
    float   fReserved3;
    float   fReserved4;

} ISD_STATION_STATE_TYPE;


typedef struct
{
    BYTE    TrackingStatus;     // tracking status byte 
    BYTE    bReserved1;         // pack to 4 byte boundary 
    BYTE    bReserved2;
    BYTE    bReserved3;

    DWORD   Timecode;           // timecode, not implemented yet 
    LONG    ApetureEncoder;     // Apeture encoder counts, relative to last reset of power up 
    LONG    FocusEncoder;       // Focus encoder counts 
    LONG    ZoomEncoder;        // Zoom encoded counts 
    DWORD   TimecodeUserBits;   // Time code user bits, not implemented yet 

    float   Apeture;            // Computed Apeture value 
    float   Focus;              // Computed focus value (mm), not implemented yet 
    float   FOV;                // Computed vertical FOV value (degrees) 
    float   NodalPoint;         // Nodal point offset due to zoom and focus (mm) 

    LONG    lReserved1;
    LONG    lReserved2;
    LONG    lReserved3;
    LONG    lReserved4;

    DWORD   dwReserved1;
    DWORD   dwReserved2;
    DWORD   dwReserved3;
    DWORD   dwReserved4;

    float   fReserved1;
    float   fReserved2;
    float   fReserved3;
    float   fReserved4;

} ISD_CAMERA_ENCODER_DATA_TYPE;


typedef struct
{
    ISD_STATION_STATE_TYPE Station[ISD_MAX_STATIONS];

} ISD_TRACKER_DATA_TYPE;


typedef struct
{
    ISD_CAMERA_ENCODER_DATA_TYPE Camera[ISD_MAX_STATIONS];

} ISD_CAMERA_DATA_TYPE;


// Returns -1 on failure. To detect tracker automatically specify 0 for commPort.
// hParent parameter to ISD_OpenTracker is optional and should only be used if 
// information screen or tracker configuration tools are to be used when available 
// in the future releases. If you would like a tracker initialization window to be 
// displayed, specify TRUE value for the infoScreen parameter (not implemented in
// this release). 

DLLEXPORT ISD_TRACKER_HANDLE DLLENTRY ISD_OpenTracker( 
                                                      Hwnd hParent, 
                                                      DWORD commPort, 
                                                      Bool infoScreen, 
                                                      Bool verbose,
                                                      const char *name
                                                      );

DLLEXPORT DWORD DLLENTRY ISD_OpenAllTrackers( 
                                             Hwnd hParent, 
                                             ISD_TRACKER_HANDLE *handle, 
                                             Bool infoScreen, 
                                             Bool verbose,
                                             const char *name
                                             );

// This function call deinitializes the tracker, closes communications port and 
// frees the resources associated with this tracker. If 0 is passed, all currently
// open trackers are closed. When last tracker is closed, program frees the DLL. 

DLLEXPORT Bool DLLENTRY ISD_CloseTracker( ISD_TRACKER_HANDLE handle );


// Get general tracker information, such as type, model, port, etc.
// Also retrieves genlock synchronization configuration, if available. 
// See ISD_TRACKER_INFO_TYPE structure definition above for complete list of items 

DLLEXPORT Bool DLLENTRY ISD_GetTrackerConfig( 
                                             ISD_TRACKER_HANDLE handle, 
                                             ISD_TRACKER_INFO_TYPE *Tracker, 
                                             Bool verbose 
                                             );


// When used with IS Precision Series (IS-300, IS-600, IS-900, IS-1200) tracking devices 
// this function call will set genlock synchronization  parameters, all other fields 
// in the ISD_TRACKER_INFO_TYPE structure are for information purposes only 

DLLEXPORT Bool DLLENTRY ISD_SetTrackerConfig(
                                             ISD_TRACKER_HANDLE handle, 
                                             ISD_TRACKER_INFO_TYPE *Tracker, 
                                             Bool verbose
                                             );


// Get RecordsPerSec and KBitsPerSec without requesting genlock settings from the tracker.
// Use this instead of ISD_GetTrackerConfig to prevent your program from stalling while
// waiting for the tracker response. 

DLLEXPORT Bool DLLENTRY ISD_GetCommInfo( 
                                        ISD_TRACKER_HANDLE handle, 
                                        ISD_TRACKER_INFO_TYPE *Tracker
                                        );


// Configure station as specified in the ISD_STATION_INFO_TYPE structure. Before 
// this function is called, all elements of the structure must be assigned a value. 
// stationID is a number from 1 to ISD_MAX_STATIONS. Should only be used with
// IS Precision Series tracking devices, not valid for InterTrax.  

DLLEXPORT Bool DLLENTRY ISD_SetStationConfig( 
                                             ISD_TRACKER_HANDLE handle, 
                                             ISD_STATION_INFO_TYPE *Station, 
                                             WORD stationID,
                                             Bool verbose 
                                             );


// Fills the ISD_STATION_INFO_TYPE structure with current settings. Function
// requests configuration records from the tracker and waits for the response.
// If communications are interrupted, it will stall for several seconds while 
// attempting to recover the settings. Should only be used with IS Precision Series 
// tracking devices, not valid for InterTrax.
// stationID is a number from 1 to ISD_MAX_STATIONS 

DLLEXPORT Bool DLLENTRY ISD_GetStationConfig(
                                             ISD_TRACKER_HANDLE handle, 
                                             ISD_STATION_INFO_TYPE *Station,
                                             WORD stationID, 
                                             Bool verbose 
                                             );


// Get data from all configured stations. Data is places in the ISD_TRACKER_DATA_TYPE
// structure. Orientation array may contain Euler angles or Quaternions, depending
// on the settings of the AngleFormat field of the ISD_STATION_INFO_TYPE structure.
// TimeStamp is only available if requested by setting TimeStamped field to TRUE. 

DLLEXPORT Bool DLLENTRY ISD_GetData( 
                                    ISD_TRACKER_HANDLE handle, 
                                    ISD_TRACKER_DATA_TYPE *Data 
                                    );


// Get camera encode and other data for all configured stations. Data is places in 
// the ISD_CAMERA_DATA_TYPE structure. This function does not service serial port, so
// ISD_GetData must be called prior to this. 

DLLEXPORT Bool DLLENTRY ISD_GetCameraData( 
                                          ISD_TRACKER_HANDLE handle, 
                                          ISD_CAMERA_DATA_TYPE *Data 
                                          );

// Reset heading to zero 

DLLEXPORT Bool DLLENTRY ISD_ResetHeading( 
                                         ISD_TRACKER_HANDLE handle, 
                                         WORD stationID 
                                         );


// Boresight station using specific reference angles. This is useful when
// you need to apply a specific offset to system output. For example, if
// a sensor is mounted at 40 degrees relative to the HMD, you can 
// enter 0, 40, 0 to get the system to output zero when HMD is horizontal.

DLLEXPORT Bool DLLENTRY ISD_BoresightReferenced( 
                                                ISD_TRACKER_HANDLE handle, 
                                                WORD stationID, 
                                                float yaw,
                                                float pitch, 
                                                float roll 
                                                );

// Boresight, or unboresight a station. If 'set' is TRUE, all angles
// are reset to zero. Otherwise, all boresight settings are cleared,
// including those set by ISD_ResetHeading and ISD_BoresightReferenced

DLLEXPORT Bool DLLENTRY ISD_Boresight( 
                                      ISD_TRACKER_HANDLE handle, 
                                      WORD stationID,
                                      Bool set
                                      );



// Send a configuration script to the tracker. Script must consist of valid 
// commands as described in the interface protocol. Commands in the script 
// should be terminated by the New Line character '\n'. Line Feed character '\r' 
// is added by the function and is not required. 

DLLEXPORT Bool DLLENTRY ISD_SendScript( 
                                       ISD_TRACKER_HANDLE handle, 
                                       char *script 
                                       );


// Number of currently opened trackers is stored in the parameter passed to this
// functions 

DLLEXPORT Bool DLLENTRY ISD_NumOpenTrackers( WORD *num );


// Open tracker configuration win32 interface. Provide tracker handle to 
// configure one specific tracking device, or NULL to get all configured devices. 
// Not implemented in this release. 

DLLEXPORT Hwnd DLLENTRY ISD_OpenConfigWindow( ISD_TRACKER_HANDLE handle );
 

// Platform independent time

DLLEXPORT float DLLENTRY ISD_GetTime( void );

#ifdef __cplusplus
}
#endif

#endif

