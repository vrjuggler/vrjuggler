#ifndef SONIX_PLUGINAPI
#define SONIX_PLUGINAPI

// these functions are available when you do a symbol lookup on a loaded 
// shared library (or DLL).  For example, in unix you use dlopen's dlsym 
// function to lookup a symbol.  I.e., to get the func "newPlugin":
//
//   mLibHandle = ::dlopen( libraryFile, f );
//   newPluginFunc newPlugin = ::dlsym( mLibHandle, "newPlugin" );
//
// In sonix, we use xdl, which is a cross platform wrapper for dlopen 
// functionality on win32 and unix.

namespace snx
{
   class ISoundImplementation;
}

/** create the plugin
 * symbol name to lookup from the shared lib is "newPlugin"
 */
typedef snx::ISoundImplementation* (*newPluginFunc)(void);

/** delete the plugin
 * symbol name to lookup from the shared lib is "deletePlugin"
 */
typedef void (*deletePluginFunc)( snx::ISoundImplementation* &p );

/** get the name of the plugin
 * symbol name to lookup from the shared lib is "getName"
 */
typedef char* (*getNameFunc)(void);

/** get the version of sonix plugin was compiled against.
 * symbol name to lookup from the shared lib is "getVersion"
 */
typedef char* (*getVersionFunc)(void);

#endif
