#include "vjConfig.h" // for DEG2RAD macro
#include <fstream.h>		// for ifstream
#include "KeyFramer.h"
#include "Kernel/vjDebug.h"

namespace kev
{
   class KeyFramerExporter
   {
   public:
      KeyFramerExporter()
      {
      }
   
      void execute( const char* const filename, const kev::KeyFramer& kf )
      {
         #ifdef WIN32
		   ofstream frames_file( filename, ios::out | ios::binary, filebuf::openprot );
		   #else
		   ofstream frames_file( filename, ios::out, filebuf::openprot );
		   #endif

         if (!frames_file.rdbuf()->is_open())
         {
            vjDEBUG(vjDBG_ALL,0) << clrSetNORM(clrYELLOW) << "WARNING: "
                           << clrRESET
                           << "couldn't open keyframe file: " 
                           << filename <<"\n"
                           << vjDEBUG_FLUSH;
            return;
         }
         
         vjDEBUG(vjDBG_ALL,0) << "Writing keyframe(s) to file: "
                <<filename<<"\n"<<vjDEBUG_FLUSH;
         
         std::map<float, kev::KeyFramer::Key>::const_iterator it1 = kf.keys().begin();
         for (; it1 != kf.keys().end(); ++it1)
         {
            const kev::KeyFramer::Key& key = (*it1).second;
            const float& time = key.time();
            const vjVec3& pos = key.position();
            const vjQuat& quat = key.rotation();
            float deg;
            vjVec3 vec;
            quat.getRot( deg, vec[0], vec[1], vec[2] );
            deg = VJ_RAD2DEG( deg ); 
            frames_file<<time<<" "<<pos[0]<<" "<<pos[1]<<" "
                       <<pos[2]<<" "<<deg<<" "<<vec[0]<<" "
                       <<vec[1]<<" "<<vec[2]<<"\n";
         }
         frames_file.close();
      }   
   };
};
