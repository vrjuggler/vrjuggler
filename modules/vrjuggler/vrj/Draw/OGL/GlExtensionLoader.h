#ifndef VRJ_EXTENSION_LOADER_H_
#define VRJ_EXTENSION_LOADER_H_

namespace vrj
{

/** Base class for OpenGL extension registration and lookup.
 * This is a the base for a simple helper class for getting
 * at OpenGL/WGL/GLX extensions that we need for VRJ.
 */
class GlExtensionLoader
{
public:
   /** Void type to use when treating extensions the same. */
   typedef void (*VoidExtFunc)(void);

public:
   GlExtensionLoader()
   {;}

   virtual ~GlExtensionLoader()
   {;}

   /** Register common extensions that we may need to use. */
   virtual void registerExtensions()
   {;}

   /** Return pointer to extension if it exists. */
   VoidExtFunc getFunctionByName(const char* name);

};

}  // namespace vrj

#endif
