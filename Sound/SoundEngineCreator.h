#ifndef SOUND_ENGINE_CREATOR
#define SOUND_ENGINE_CREATOR

// abstract mini factory
// used for regging sound engine creators in the SoundFactory.
// seealso: SoundFactory.h
class SoundEngineCreator
{
public:
   SoundEngineCreator() {}
   virtual ~SoundEngineCreator() {}
   virtual SoundEngine* newEngine() = 0;

public:
   // does name match the description of this?
   // to get a handle to a Creator, SoundFactory will ask each Creator if a name matches
   bool matchDescription( const std::string& name )
   {
      for (int x = 0; x < mDescriptors.size(); ++x)
      {
         if (mDescriptors[x] == name)
            return true;
      }
      return false;
   }   
   
   // add a descriptor for this creator
   // this name will be used to match against when 
   // something requests a creator from the SoundFactory.
   void addDescriptor( const std::string& name )
   {
      mDescriptors.push_back( name );
   }   
   
   // returns the number of descriptors available
   int sizeDescs() const
   {
      return mDescriptors.size();  
   }  
    
   // get a numDescriptors so that you can iterate through the names using "whichOne"
   const std::string& descriptor( const int& whichOne = 0 ) const
   {
      assert( whichOne >= 0 && whichOne < mDescriptors.size() );
      return mDescriptors[whichOne];
   }   
   
private:
   std::vector< std::string > mDescriptors;
};

#endif
