#include <vrj/vrjConfig.h> // for DEG2RAD macro
#include <fstream.h>        // for ifstream
#include "KeyFramer.h"
#include <vrj/Util/Debug.h>
#include <vrj/Math/Math.h>

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
         float w, x, y, z;
         key.rotation().getRot( w, x, y, z );
         vprDEBUG(vprDBG_ALL,1)<<"KEY "<<key.time()<<": "<<key.position()[0]<<", \t"<<key.position()[1]<<", \t"<<key.position()[2]<<", |#| "<<vrj::Math::rad2deg( w )<<", "<<x<<", "<<y<<", "<<z<<"\n"<<vprDEBUG_FLUSH;
      }

      void execute( const char* const filename, kev::KeyFramer& kf )
      {
         kf.clear();

#ifdef WIN32
         ifstream frames_file( filename, ios::in | ios::nocreate | ios::binary, filebuf::openprot );
#else
         ifstream frames_file( filename, ios::in | ios::nocreate, filebuf::openprot );
#endif

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
         vrj::Vec3 vec;
         vrj::Vec3 pos;
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
            vrj::Quat rot;
            float rad = vrj::Math::deg2rad( deg );
            rot.makeRot( rad, vec[0], vec[1], vec[2] );

            kev::KeyFramer::Key key( time, pos, rot );
            kf.addkey( key );
            output( key );
         }
         frames_file.close();
      }
   };
};
