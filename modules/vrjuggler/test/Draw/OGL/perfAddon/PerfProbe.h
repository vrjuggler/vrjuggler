#ifndef _PERF_PROBE_H_
#define _PERF_PROBE_H_

#include <GL/gl.h>
#include <deque>
#include <vpr/Util/Interval.h>
#include <sstream>


class PerfProbe
{
public:
   PerfProbe();

public:
   /** Get the stream used for overlay text. */
   std::stringstream& getStream()
   { return mOverlayTextStream;  }

public:
   /** Method to be called once per frame.
    * Used to update data like frame rate.  Call from app::preFrame
    */
   void update();

   /** Draw the performance data.
    * Call from draw.
    */
   void draw();

protected:
   void drawOverlayText();

public:
   std::deque<float>    mFrameTimes;      /**< Used to track most recent set of frame times. */
   vpr::Interval        mLastUpdateTime;  /**< Time of the last update. */
   float                mFrameRate;       /**< Running inst average frame rate. */

   std::stringstream    mOverlayTextStream;  /**< Text to write out each frame. */
};

#endif


