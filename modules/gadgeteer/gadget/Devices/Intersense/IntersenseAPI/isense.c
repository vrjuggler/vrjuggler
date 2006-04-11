//////////////////////////////////////////////////////////////////////////////
//
//      File Name:      isense.c
//      Description:    DLL access point
//      Created:        12/7/98
//      Author:         Yury Altshuler
//
//      Copyright:      InterSense 2002 - All rights Reserved.
//
//      Comments:        
//
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "isense.h"


#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__)
//#define ISD_LIB_NAME "isense.dll"
#else
#include <dlfcn.h>
//#define ISD_LIB_NAME "libisense"
#endif


typedef void DLL;
DLL *hLib = NULL;


static DLL_EP  dll_entrypoint( DLL *dll, const char *name );
static DLL    *dll_load( const char *name );
static void    dll_unload( DLL *dll );


typedef ISD_TRACKER_HANDLE (*ISD_OPEN_FN)           ( Hwnd, DWORD, Bool, Bool );
typedef DWORD              (*ISD_OPEN_ALL_FN)       ( Hwnd, ISD_TRACKER_HANDLE *, Bool, Bool );
typedef Bool               (*ISD_COMMAND_FN)        ( ISD_TRACKER_HANDLE );
typedef Bool               (*ISD_COMM_INFO_FN)      ( ISD_TRACKER_HANDLE, ISD_TRACKER_INFO_TYPE * );
typedef Bool               (*ISD_SYSTEM_CONFIG_FN)  ( ISD_TRACKER_HANDLE, ISD_TRACKER_INFO_TYPE *, Bool );
typedef Bool               (*ISD_STATION_CONFIG_FN) ( ISD_TRACKER_HANDLE, ISD_STATION_INFO_TYPE *, WORD, Bool );
typedef Bool               (*ISD_DATA_FN)           ( ISD_TRACKER_HANDLE, ISD_TRACKER_DATA_TYPE * );
typedef Bool               (*ISD_CAMERA_DATA_FN)    ( ISD_TRACKER_HANDLE, ISD_CAMERA_DATA_TYPE * );
typedef Bool               (*ISD_SCRIPT_FN)         ( ISD_TRACKER_HANDLE, char * );
typedef Bool               (*ISD_COUNT_FN)          ( WORD * );
typedef Bool               (*ISD_RESET_HEADING_FN)  ( ISD_TRACKER_HANDLE, WORD );
typedef Bool               (*ISD_BORESIGHT_FN)      ( ISD_TRACKER_HANDLE, WORD, Bool );
typedef Bool               (*ISD_BORESIGHT_REF_FN)  ( ISD_TRACKER_HANDLE, WORD, float, float, float );
typedef Hwnd               (*ISD_WINDOW_FN)         ( ISD_TRACKER_HANDLE );
typedef float              (*ISD_GET_TIME)          ( void );


/* dll function pointers */
ISD_OPEN_FN             _ISD_OpenTracker          = NULL;
ISD_OPEN_ALL_FN         _ISD_OpenAllTrackers      = NULL;
ISD_COMMAND_FN          _ISD_CloseTracker         = NULL;
ISD_COMM_INFO_FN        _ISD_GetCommInfo          = NULL;
ISD_SYSTEM_CONFIG_FN    _ISD_GetTrackerConfig     = NULL;
ISD_SYSTEM_CONFIG_FN    _ISD_SetTrackerConfig     = NULL;
ISD_STATION_CONFIG_FN   _ISD_GetStationConfig     = NULL;
ISD_STATION_CONFIG_FN   _ISD_SetStationConfig     = NULL;
ISD_DATA_FN             _ISD_GetData              = NULL;   
ISD_CAMERA_DATA_FN      _ISD_GetCameraData        = NULL;   
ISD_SCRIPT_FN           _ISD_SendScript           = NULL;
ISD_COUNT_FN            _ISD_NumOpenTrackers      = NULL;
ISD_WINDOW_FN           _ISD_OpenConfigWindow     = NULL;
ISD_RESET_HEADING_FN    _ISD_ResetHeading         = NULL; 
ISD_BORESIGHT_FN        _ISD_Boresight            = NULL;
ISD_BORESIGHT_REF_FN    _ISD_BoresightReferenced  = NULL;
ISD_GET_TIME            _ISD_GetTime              = NULL;



/*****************************************************************************
*
*   functionName:   load_ISLIB
*   Description:    loads isense.dll
*   Created:        12/7/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
static DLL *load_ISLIB(  const char *driver_name )
{
    if( (hLib = dll_load( driver_name )) )
    {
        _ISD_OpenTracker         = ( ISD_OPEN_FN )           dll_entrypoint( hLib, "ISD_OpenTracker" );
        _ISD_OpenAllTrackers     = ( ISD_OPEN_ALL_FN )       dll_entrypoint( hLib, "ISD_OpenAllTrackers" );
        _ISD_CloseTracker        = ( ISD_COMMAND_FN )        dll_entrypoint( hLib, "ISD_CloseTracker" );
        _ISD_GetCommInfo         = ( ISD_COMM_INFO_FN )      dll_entrypoint( hLib, "ISD_GetCommInfo" );
        _ISD_GetTrackerConfig    = ( ISD_SYSTEM_CONFIG_FN )  dll_entrypoint( hLib, "ISD_GetTrackerConfig" );
        _ISD_SetTrackerConfig    = ( ISD_SYSTEM_CONFIG_FN )  dll_entrypoint( hLib, "ISD_SetTrackerConfig" );
        _ISD_GetStationConfig    = ( ISD_STATION_CONFIG_FN ) dll_entrypoint( hLib, "ISD_GetStationConfig" );
        _ISD_SetStationConfig    = ( ISD_STATION_CONFIG_FN ) dll_entrypoint( hLib, "ISD_SetStationConfig" );
        _ISD_GetData             = ( ISD_DATA_FN )           dll_entrypoint( hLib, "ISD_GetData" );
        _ISD_GetCameraData       = ( ISD_CAMERA_DATA_FN )    dll_entrypoint( hLib, "ISD_GetCameraData" );
        _ISD_SendScript          = ( ISD_SCRIPT_FN )         dll_entrypoint( hLib, "ISD_SendScript" );
        _ISD_NumOpenTrackers     = ( ISD_COUNT_FN )          dll_entrypoint( hLib, "ISD_NumOpenTrackers" );
        _ISD_OpenConfigWindow    = ( ISD_WINDOW_FN )         dll_entrypoint( hLib, "ISD_OpenConfigWindow" );
        _ISD_ResetHeading        = ( ISD_RESET_HEADING_FN )  dll_entrypoint( hLib, "ISD_ResetHeading" );
        _ISD_Boresight           = ( ISD_BORESIGHT_FN )      dll_entrypoint( hLib, "ISD_Boresight" );
        _ISD_BoresightReferenced = ( ISD_BORESIGHT_REF_FN )  dll_entrypoint( hLib, "ISD_BoresightReferenced" );
        _ISD_GetTime             = ( ISD_GET_TIME )          dll_entrypoint( hLib, "ISD_GetTime" );
    }

    if( hLib == NULL )
    {
        printf( "Could not load %s\n", driver_name );
    }

    return hLib;
}


/*****************************************************************************
*
*   functionName:   ISD_FreeLib
*   Description:    frees isense.dll
*   Created:        12/8/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
static void ISD_FreeLib( void )
{
    _ISD_OpenTracker         = NULL;
    _ISD_OpenAllTrackers     = NULL;
    _ISD_CloseTracker        = NULL;
    _ISD_GetCommInfo         = NULL;
    _ISD_GetTrackerConfig    = NULL;
    _ISD_SetTrackerConfig    = NULL;
    _ISD_GetStationConfig    = NULL;
    _ISD_SetStationConfig    = NULL;
    _ISD_GetData             = NULL;  
    _ISD_GetCameraData       = NULL;
    _ISD_SendScript          = NULL;
    _ISD_NumOpenTrackers     = NULL;
    _ISD_OpenConfigWindow    = NULL;
    _ISD_ResetHeading        = NULL;
    _ISD_Boresight           = NULL;
    _ISD_BoresightReferenced = NULL;
    _ISD_GetTime             = NULL;
   
    if( hLib )
    {
    	dll_unload( hLib );  /* free the dll */
        hLib = NULL;
    }
}


/******************************************************************************/
ISD_TRACKER_HANDLE ISD_OpenTracker( 
                                   Hwnd hParent,
                                   DWORD commPort, 
                                   Bool infoScreen, 
                                   Bool verbose,
                                   const char *name
                                   )
{
    if( !_ISD_OpenTracker ) /* this will be NULL if dll not loaded */
    {
        if( !hLib ) load_ISLIB( name );

        if( !hLib )  /* failed to load dll */
        {
            return 0;
        }
    }
    if( _ISD_OpenTracker )
    {
        return( (*_ISD_OpenTracker)( hParent, commPort, infoScreen, verbose ) );
    }
    return FALSE;
}


/******************************************************************************/
DWORD ISD_OpenAllTrackers( 
                          Hwnd hParent, 
                          ISD_TRACKER_HANDLE *handle, 
                          Bool infoScreen, 
                          Bool verbose,
                          const char *name
                          )
{
    if( !_ISD_OpenAllTrackers ) /* this will be NULL if dll is not loaded */
    {
        if( !hLib ) load_ISLIB( name );

        if( !hLib )  /* failed to load dll */
        {
            return 0;
        }
    }
    if( _ISD_OpenAllTrackers )
    {
        return( (*_ISD_OpenAllTrackers)( hParent, handle, infoScreen, verbose ) );
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_CloseTracker( ISD_TRACKER_HANDLE handle )
{
    Bool ret;
    WORD num;

    if( _ISD_CloseTracker )
    {
        ret = (*_ISD_CloseTracker)( handle );

        /* if all trackers are closed the dll can be freed */
        if( ISD_NumOpenTrackers( &num ) )
        {
            if(num == 0)
            {
                ISD_FreeLib( );
            }
        }
        return ret;
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_NumOpenTrackers( WORD *num )
{
    if( _ISD_NumOpenTrackers ) 
    {
        return( (*_ISD_NumOpenTrackers)( num ) );
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_GetCommInfo( 
                     ISD_TRACKER_HANDLE handle, 
                     ISD_TRACKER_INFO_TYPE *Tracker 
                     )
{
    if( _ISD_GetCommInfo )
    {
        return( (*_ISD_GetCommInfo)( handle, Tracker ) );
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_GetTrackerConfig( 
                          ISD_TRACKER_HANDLE handle, 
                          ISD_TRACKER_INFO_TYPE *Tracker, 
                          Bool verbose 
                          )
{
    if( _ISD_GetTrackerConfig )
    {
        return( (*_ISD_GetTrackerConfig)( handle, Tracker, verbose ) );
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_SetTrackerConfig( 
                          ISD_TRACKER_HANDLE handle, 
                          ISD_TRACKER_INFO_TYPE *Tracker,
                          Bool verbose 
                          )
{
    if( _ISD_SetTrackerConfig )
    {
        return( (*_ISD_SetTrackerConfig)( handle, Tracker, verbose ) );
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_SetStationConfig( 
                          ISD_TRACKER_HANDLE handle, 
                          ISD_STATION_INFO_TYPE *Station, 
                          WORD stationNum, 
                          Bool verbose 
                          )
{
    if( _ISD_SetStationConfig )
    {
        return( (*_ISD_SetStationConfig)( handle, Station, stationNum, verbose ) );
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_GetStationConfig( 
                          ISD_TRACKER_HANDLE handle, 
                          ISD_STATION_INFO_TYPE *Station, 
                          WORD stationNum, 
                          Bool verbose 
                          )
{
    if( _ISD_GetStationConfig )
    {
        return( (*_ISD_GetStationConfig)( handle, Station, stationNum, verbose ) );
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_GetData( 
                 ISD_TRACKER_HANDLE handle, 
                 ISD_TRACKER_DATA_TYPE *Data 
                 )
{
    if( _ISD_GetData )
    {
        return( (*_ISD_GetData)( handle, Data ) );
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_GetCameraData( 
                       ISD_TRACKER_HANDLE handle, 
                       ISD_CAMERA_DATA_TYPE *Data 
                       )
{
    if( _ISD_GetCameraData )
    {
        return( (*_ISD_GetCameraData)( handle, Data ) );
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_SendScript( 
                    ISD_TRACKER_HANDLE handle, 
                    char *script 
                    )
{
    if( _ISD_SendScript )
    {
        return( (*_ISD_SendScript)( handle, script ) );
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_ResetHeading( 
                      ISD_TRACKER_HANDLE handle, 
                      WORD stationNum 
                      )
{
    if( _ISD_ResetHeading ) 
    {
        return( (*_ISD_ResetHeading)( handle, stationNum ) );
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_Boresight( 
                   ISD_TRACKER_HANDLE handle, 
                   WORD stationNum,
                   Bool set
                   )
{
    if( _ISD_Boresight ) 
    {
        return( (*_ISD_Boresight)( handle, stationNum, set ) );
    }
    return FALSE;
}


/******************************************************************************/
Bool ISD_BoresightReferenced( 
                             ISD_TRACKER_HANDLE handle, 
                             WORD stationNum,
                             float yaw, 
                             float pitch,
                             float roll 
                             )
{
    if( _ISD_BoresightReferenced ) 
    {
        return( (*_ISD_BoresightReferenced)( handle, stationNum, yaw, pitch, roll ) );
    }
    return FALSE;
}


/******************************************************************************/
Hwnd ISD_OpenConfigWindow( ISD_TRACKER_HANDLE handle )
{
    if( _ISD_OpenConfigWindow )
    {
        return( (*_ISD_OpenConfigWindow)( handle ) );
    }
    return (Hwnd) NULL;
}


/******************************************************************************/
float ISD_GetTime( void )
{
    if( _ISD_GetTime )
    {
        return( (*_ISD_GetTime)() );
    }
    return 0.0f;
}


/******************************************************************************/
DLL_EP dll_entrypoint( DLL *dll, const char *name )
{
#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__)

    FARPROC proc;

    proc = GetProcAddress( (HMODULE) dll, (LPCSTR) name );
    if( proc == NULL )
    {
        printf( "Failed to load %s. Error code %d\n", name, GetLastError() );
    }
    return (DLL_EP) proc;

#else // UNIX

	void *handle = (void *) dll;
	DLL_EP ep;
	ep = (DLL_EP) dlsym(handle, name);
	return ( dlerror() == 0 ) ? ep : (DLL_EP) NULL;

#endif
}	

/******************************************************************************/
DLL *dll_load( const char *name )
{
#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__)

    return (DLL *) LoadLibrary( name );

#else // UNIX

	DLL *dll;

    char dllname[200+1+2+1];
	strcpy(dllname, name);
	strcat(dllname, ".so");
	dll = dlopen(dllname, RTLD_NOW);
	if(!dll) printf("%s\n", (char *) dlerror());
	return dll;

#endif
}

/******************************************************************************/
void dll_unload( DLL *dll )
{
#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__)

    HINSTANCE hInst = (HINSTANCE) dll;
    FreeLibrary( hInst );

#else // UNIX

	void *handle = (void *) dll;
	dlclose(handle);

#endif
}


