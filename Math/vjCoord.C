#include <config.h>
#include <Math/vjCoord.h>
#include <Math/vjMatrix.h>

vjCoord::vjCoord(vjMatrix mat)
{
   mat.getXYZEuler(orient[0], orient[1], orient[2]);
   mat.getTrans(pos[0], pos[1], pos[2]);
}
