#include <vrj/vrjConfig.h> // for DEG2RAD macro
#include <fstream>        // for ifstream
#include "KeyFramer.h"
#include <vrj/Util/Debug.h>
#include <gmtl/Math.h>
#include <gmtl/Output.h>

namespace kev
{
   class KeyFramerImporter
   {
   public:
      KeyFramerImporter()
      {
      }

      void output( kev::KeyFramer::Key& key )
      {
         gmtl::AxisAnglef axisAngle;
         gmtl::set( axisAngle, key.rotation() );
         vprDEBUG(vprDBG_ALL,1)
            << "KEY " << key.time() << ": " << key.position()[0] <<", \t"
            << key.position()[1] << ", \t" << key.position()[2] << ", |#| "
            << gmtl::Math::rad2Deg( axisAngle.getAngle() ) << ", " 
            << axisAngle.getAxis() << "\n" <<vprDEBUG_FLUSH;
      }

      void execute( const char* const filename, kev::KeyFramer& kf )
      {
         kf.clear();

         std::ifstream frames_file( filename, std::ios::in );

         if (!frames_file.rdbuf()->is_open())
         {
            vprDEBUG(vprDBG_ALL,0) << clrSetNORM(clrYELLOW) << "WARNING: "
                           << clrRESET
                           << "couldn't open keyframe file: "
                           << filename <<"\n"
                           << vprDEBUG_FLUSH;
            return;
         }

         vprDEBUG(vprDBG_ALL,0) << "Reading keyframe(s) in file: "
                <<filename<<"\n"<<vprDEBUG_FLUSH;

         float time;
         float deg;
         gmtl::Vec3f vec;
         gmtl::Vec3f pos;
         while (!frames_file.eof())
         {
            frames_file>>time;

            // if EOF then return...
            if (time == -1)
            {
               vprDEBUG(vprDBG_ALL,0)<<"time==-1: "<<time<<"\n"<<vprDEBUG_FLUSH;
               return;
            }
            frames_file>>pos[0]>>pos[1]>>pos[2]>>deg>>vec[0]>>vec[1]>>vec[2];

            // convert [TWIST, VEC] to Quat
            float rad = gmtl::Math::deg2Rad( deg );
            gmtl::Quatf rot;
            gmtl::AxisAnglef axisAngle( rad, vec );
            gmtl::set( rot, axisAngle );

            kev::KeyFramer::Key key( time, pos, rot );
            kf.addkey( key );
            output( key );
         }
         frames_file.close();
      }
   };
};
