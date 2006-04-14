#ifndef VRJ_GLX_EXTENSION_LOADER_H_
#define VRJ_GLX_EXTENSION_LOADER_H_

#include <vrj/Draw/OGL/GlExtensionLoader.h>
#include <boost/shared_ptr.hpp>
#include <GL/glx.h>

namespace vrj
{

/** GLX specific class for loading extensions.
 */
class GlxExtensionLoader : public GlExtensionLoader
{
public:
   GlxExtensionLoader();

   /** Register common extensions that we may need to use. */
   virtual void registerExtensions(); 

public:
   /** @name NVidia swap control. */
   //@{
   
   /** Return true if we have support for NV swap group extensions. */
   bool hasSwapGroupNV();

   Bool glXJoinSwapGroupNV(::Display *dpy, GLXDrawable drawable,
                             GLuint group);

   Bool glXBindSwapBarrierNV(::Display *dpy, GLuint group, GLuint barrier);

   Bool glXQuerySwapGroupNV(::Display *dpy, GLXDrawable drawable,
                              GLuint *group, GLuint *barrier);

   Bool glXQueryMaxSwapGroupsNV(::Display *dpy, int screen,
                                  GLuint *maxGroups, GLuint *maxBarriers);

   Bool glXQueryFrameCountNV(::Display *dpy, int screen, GLuint *count);

   Bool glXResetFrameCountNV(::Display *dpy, int screen);
   //@}

protected:
   struct GlxFuncs;

   boost::shared_ptr<GlxFuncs>   mGlxFuncs;  /** Pimpl struct for holding typed functions. */
};

}  // namespace vrj

#endif

