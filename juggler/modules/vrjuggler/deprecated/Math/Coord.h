#ifndef _VRJ_COORD_H
#define _VRJ_COORD_H

#include <deprecated/Math/vjVec3.h>
#include <deprecated/Math/vjMatrix.h>

namespace vrj
{

class Coord
{
public:
  Coord() { }

  Coord(const vjMatrix& mat)
  {
    mat.getXYZEuler(orient[0], orient[1], orient[2]);
    mat.getTrans(pos[0], pos[1], pos[2]);
  }

public:
  vjVec3 pos;
  vjVec3 orient;
};

}

#endif
