#include <gmtl/Math.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>

#include <snx/SoundImplementation.h>


namespace snx
{

void SoundImplementation::copy( const SoundImplementation& si )
{
   // copy over the current state
   mName = si.mName;
   mSounds = si.mSounds;
   mListenerPos = si.mListenerPos;
   mSoundAPIInfo = si.mSoundAPIInfo;
}

void SoundImplementation::configure( const std::string& alias, const snx::SoundInfo& description )
{
   this->unbind( alias );
   snx::SoundInfo temp = mSounds[alias];
   mSounds[alias] = description; // TODO: put is_playing within the SoundInfo.

   // restore fields that should be preserved on a reconfigure...
   mSounds[alias].triggerOnNextBind = temp.triggerOnNextBind;
   //std::cout<<"DEBUG: triggerOnNextBind = "<<mSounds[alias].triggerOnNextBind<<"\n"<<std::flush;

   if (this->isStarted())
   {
      this->bind( alias );
   }
}

void SoundImplementation::remove(const std::string& alias)
{
   if (this->isStarted())
   {
      this->unbind( alias );
   }
   mSounds.erase( alias );
}

void SoundImplementation::bindAll()
{
   std::map< std::string, snx::SoundInfo >::iterator it;
   for( it = mSounds.begin(); it != mSounds.end(); ++it)
   {
      //std::cout<<"DEBUG: loading alias: "<<(*it).first<<"\n"<<std::flush;
      this->bind( (*it).first );
   }
}   

void SoundImplementation::unbindAll()
{
   std::map< std::string, snx::SoundInfo >::iterator it;
   for( it = mSounds.begin(); it != mSounds.end(); ++it)
   {
      //std::cout<<"DEBUG: loading alias: "<<(*it).first<<"\n"<<std::flush;
      this->unbind( (*it).first );
   }
   /*
   std::map< std::string, AlSoundInfo >::iterator it;
   for( it = mBindLookup.begin(); it != mBindLookup.end(); ++it)
   {
      this->unbind( (*it).first );
   }

   assert( mBindLookup.size() == 0 && "unbindAll failed" );
   */
}

} // End of snx namespace
