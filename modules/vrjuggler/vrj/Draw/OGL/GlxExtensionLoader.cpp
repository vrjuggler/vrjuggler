#include <vrj/Draw/OGL/GlxExtensionLoader.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <GL/glx.h>
#include <GL/glxext.h>

namespace vrj
{
   // NVIDIA swap control
   typedef Bool ( * PFNGLXJOINSWAPGROUPNVPROC) (::Display *dpy,
                                                GLXDrawable drawable,
                                                GLuint group);
   
   typedef Bool ( * PFNGLXBINDSWAPBARRIERNVPROC) (::Display *dpy,
                                                  GLuint group,
                                                  GLuint barrier);
   
   typedef Bool ( * PFNGLXQUERYSWAPGROUPNVPROC) (::Display *dpy,
                                                 GLXDrawable drawable,
                                                 GLuint *group,
                                                 GLuint *barrier);
   
   typedef Bool ( * PFNGLXQUERYMAXSWAPGROUPSNVPROC) (::Display *dpy,
                                                     int screen,
                                                     GLuint *maxGroups,
                                                     GLuint *maxBarriers);
   
   typedef Bool ( * PFNGLXQUERYFRAMECOUNTNVPROC) (::Display *dpy,
                                                  int screen,
                                                  GLuint *count);
   
   typedef Bool ( * PFNGLXRESETFRAMECOUNTNVPROC) (::Display *dpy, int screen);

struct GlxExtensionLoader::GlxFuncs
{
   GlxFuncs()
   {
      glXJoinSwapGroupNV = NULL;
      glXBindSwapBarrierNV = NULL;
      glXQuerySwapGroupNV = NULL;
      glXQueryMaxSwapGroupsNV = NULL;
      glXQueryFrameCountNV = NULL;
      glXResetFrameCountNV = NULL;
   }

   PFNGLXJOINSWAPGROUPNVPROC        glXJoinSwapGroupNV;
   PFNGLXBINDSWAPBARRIERNVPROC      glXBindSwapBarrierNV;
   PFNGLXQUERYSWAPGROUPNVPROC       glXQuerySwapGroupNV;
   PFNGLXQUERYMAXSWAPGROUPSNVPROC   glXQueryMaxSwapGroupsNV;
   PFNGLXQUERYFRAMECOUNTNVPROC      glXQueryFrameCountNV;
   PFNGLXRESETFRAMECOUNTNVPROC      glXResetFrameCountNV;
};

GlxExtensionLoader::GlxExtensionLoader()
{
   mGlxFuncs = boost::shared_ptr<GlxFuncs>(new GlxFuncs);
}

void GlxExtensionLoader::registerExtensions()
{
   GlExtensionLoader::registerExtensions();

   mGlxFuncs->glXJoinSwapGroupNV       = (PFNGLXJOINSWAPGROUPNVPROC)       (getFunctionByName("glXJoinSwapGroupNV"));
   mGlxFuncs->glXBindSwapBarrierNV     = (PFNGLXBINDSWAPBARRIERNVPROC)     (getFunctionByName("glXBindSwapBarrierNV"));
   mGlxFuncs->glXQuerySwapGroupNV      = (PFNGLXQUERYSWAPGROUPNVPROC)      (getFunctionByName("glXQuerySwapGroupNV"));
   mGlxFuncs->glXQueryMaxSwapGroupsNV  = (PFNGLXQUERYMAXSWAPGROUPSNVPROC)  (getFunctionByName("glXQueryMaxSwapGroupsNV"));
   mGlxFuncs->glXQueryFrameCountNV     = (PFNGLXQUERYFRAMECOUNTNVPROC)     (getFunctionByName("glXQueryFrameCountNV"));
   mGlxFuncs->glXResetFrameCountNV     = (PFNGLXRESETFRAMECOUNTNVPROC)     (getFunctionByName("glXResetFrameCountNV"));
}

// --- NVIDIA Swap Control methods --- //
bool GlxExtensionLoader::hasSwapGroupNV()
{
   return (mGlxFuncs->glXJoinSwapGroupNV != NULL);
}

Bool GlxExtensionLoader::glXJoinSwapGroupNV(::Display *dpy, GLXDrawable drawable,
                             GLuint group)
{
   vprASSERT(mGlxFuncs->glXJoinSwapGroupNV != NULL && "Attemped to call unsupported extension.");
   return mGlxFuncs->glXJoinSwapGroupNV(dpy,drawable,group);
}

Bool GlxExtensionLoader::glXBindSwapBarrierNV(::Display *dpy, GLuint group, GLuint barrier)
{
   vprASSERT(mGlxFuncs->glXBindSwapBarrierNV != NULL && "Attemped to call unsupported extension.");
   return mGlxFuncs->glXBindSwapBarrierNV(dpy,group,barrier);
}

Bool GlxExtensionLoader::glXQuerySwapGroupNV(::Display *dpy, GLXDrawable drawable,
                           GLuint *group, GLuint *barrier)
{
   vprASSERT(mGlxFuncs->glXQuerySwapGroupNV != NULL && "Attemped to call unsupported extension.");
   return mGlxFuncs->glXQuerySwapGroupNV(dpy,drawable,group,barrier);
}

Bool GlxExtensionLoader::glXQueryMaxSwapGroupsNV(::Display *dpy, int screen,
                               GLuint *maxGroups, GLuint *maxBarriers)
{
   vprASSERT(mGlxFuncs->glXQueryMaxSwapGroupsNV != NULL && "Attemped to call unsupported extension.");
   return mGlxFuncs->glXQueryMaxSwapGroupsNV(dpy,screen,maxGroups,maxBarriers);
}

Bool GlxExtensionLoader::glXQueryFrameCountNV(::Display *dpy, int screen, GLuint *count)
{
   vprASSERT(mGlxFuncs->glXQueryFrameCountNV != NULL && "Attemped to call unsupported extension.");
   return mGlxFuncs->glXQueryFrameCountNV(dpy,screen,count);
}

Bool GlxExtensionLoader::glXResetFrameCountNV(::Display *dpy, int screen)
{
   vprASSERT(mGlxFuncs->glXResetFrameCountNV != NULL && "Attemped to call unsupported extension.");
   return mGlxFuncs->glXResetFrameCountNV(dpy,screen);
}

}  // namespace vrj


