#ifndef COMPATIBILITY_HEADER_VJ_PF_DRAW_MANAGER
#define COMPATIBILITY_HEADER_VJ_PF_DRAW_MANAGER

#include <vrj/Draw/Pf/PfUtil.h>
#include <deprecated/Math/vjMatrix.h>
#include <deprecated/Math/vjVec3.h>

inline pfMatrix vjGetPfMatrix (vjMatrix& vec)
{
   return vrj::GetPfMatrix(vec);
}

inline vjVec3 vjGetVjVec (pfVec3& vec)
{
   return vrj::GetVjVec(vec);
}

inline pfVec3 vjGetPfVec (vjVec3& vec)
{
   return vrj::GetPfVec(vec);
}

#endif
