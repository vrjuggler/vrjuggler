
#ifndef AJ_SOUND_API_INFO
#define AJ_SOUND_API_INFO

namespace aj
{

class SoundAPIInfo
{
public:
   SoundAPIInfo() : voices( 65535 ), speakerConfig( STEREO ), sampleRate( STEREO_22050_KHZ ) {}

   int voices; // 0 - n (hardware limited)
   
   enum SpeakerConfig
   {
      MONO, STEREO, QUAD, DOLBY5_1, OCT
   };   

   SpeakerConfig speakerConfig;

   enum SampleRate
   {
      STEREO_22050_KHZ, MONO_22050_KHZ, STEREO_44100_KHZ, MONO_44100_KHZ
   };

   SampleRate sampleRate;
};

}; // end namespace

#endif //AJ_SOUND_API_INFO
