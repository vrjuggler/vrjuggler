
#ifndef KEY_FRAMER
#define KEY_FRAMER

#include <map>
#include <vrj/Math/Vec3.h>
#include <vrj/Math/Quat.h>
#include <vrj/Math/Matrix.h>
#include <vrj/Math/Math.h>

namespace kev
{
   class KeyFramer
   {
   public:
         
      //: One Key for the KeyFramer
      class Key
      {
      public:
         Key() : mTime( 0.0f ), mRot(), mPos()
         {
            mRot.identity();
         }
         Key( const Key& key ) : mTime( key.mTime ), mRot( key.mRot ), mPos( key.mPos )
         {
            
         }
         
         Key& operator=( const Key& key ) 
         {
            mTime = key.mTime;
            mRot = key.mRot;
            mPos = key.mPos;
            return *this;
         }  
                
         // matrix constructor
         Key( const float& timeVal, const vrj::Matrix& mat )
         {
            assert( timeVal >= 0 && "Keyframes always have positive time value" );
            mTime = timeVal;
            mRot.makeRot( mat );
            mPos = mat.getTrans();
         }
         // pos/quat constructor
         Key( const float& timeVal, const vrj::Vec3& pos, const vrj::Quat& rot )
         {
            assert( timeVal >= 0 && "Keyframes always have positive time value" );
            mTime = timeVal;
            mRot = rot;
            mPos = pos;
         }
         // accessors
         const float&    time() const { return mTime; }
         const vrj::Quat&   rotation() const { return mRot; }
         const vrj::Vec3&   position() const { return mPos; }
         float&          time() { return mTime; }
         vrj::Quat&         rotation() { return mRot; }
         vrj::Vec3&         position() { return mPos; }
         
         // quat/pos -> matrix conversion
         void getMatrix( vrj::Matrix& mat ) const
         {
            vrj::Matrix rot;
            vrj::Matrix pos;
            
            rot.makeQuaternion( mRot );
            pos.makeTrans( mPos );
            
            // why does this work in performer?  (why does it work at all?)
            // in opengl I did the logical thing: mat = pos * rot; which worked
            // but doesn't work in performer.
            mat = rot * pos;
         }         
         
      private:
         float       mTime;
         vrj::Quat      mRot;
         vrj::Vec3      mPos;
      };
   
      //: default constructor
      KeyFramer() : mCurrentKey()
      {
         // initial state.
         this->clear();
         this->stop();
      }
      
      //: returns animation time in seconds
      float length() 
      { 
         if (mKeys.size() == 0) 
            return 0.0f;
         
         return (*(mKeys.rbegin())).first; 
      }
      
      //: play forward
      // resume from a pause(), restart from a stop(), 
      // change directions from an rplay()
      void play() 
      {
         mIsPlaying = true;
         mReverse = false;
      }
      
      //: play backward
      // resume from a pause(), restart from a stop(), 
      // change directions from a play()
      void rplay()
      {
         mIsPlaying = true;
         mReverse = true;
      }
      
      //: pause to stop, play or rplay to resume
      void pause()
      {
         mIsPlaying = false;
      }
      
      //: jump to begin
      void rewind()
      {
         mCurrentTime = 0.0f;
         mFutureTime = 0.0f;
      }
      
      //: jump to end
      void fastforward()
      {
         mCurrentTime = this->length();
         mFutureTime = this->length();
      }
      
      //: stop anim, resetting to beginning.
      void stop()
      {
         mIsPlaying = false;
         mCurrentTime = 0.0f;
         mFutureTime = 0.0f;
         mLoopsLeft = mLoops;
      }
      
      void clear()
      {
         mKeys.clear();
         mPlayFactor = 1.0f;
         mLoops = 1;
         mCurrentTime = 0.0f;
         mFutureTime = 0.0f;
         mLoopsLeft = 1;
      }  
          
      //: set the speed at which to play (factor > 0)
      // normal play:  1.0
      // double speed: 2.0
      // half speed:   0.5
      void setSpeed( float factor = 1.0f )
      {
         assert( factor > 0.0f && "use reverseplay() and play() to change direction (if factor < 0), use stop() to stop (if factor == 0)" );
         mPlayFactor = factor;
      }
      
      //: speed of play (factor > 0)
      // normal play:  1.0
      // double speed: 2.0
      // half speed:   0.5
      float speed() const
      {
         return mPlayFactor;
      }
      
      //: number of times to loop
      //  loop once:      1 (default) 
      //  loop infinite: -1
      //  loop n times:   n > 0
      // NOTE: enter 0 and the animation will never run...
      void loop( int numTimesToLoop = 1 )
      {
         mLoops = numTimesToLoop;
         mLoopsLeft = mLoops;
      }
      
      //: add a key to the keyframer.
      void addkey( const Key& key )
      {
         assert( key.time() >= 0 && "Keyframes always have positive time value" );
         mKeys[key.time()] = key;
      }
      
      //: get the current interpolated key
      const Key& key() const { return mCurrentKey; }
      
      //: get the current interpolated key's quaternion rotation
      const vrj::Quat& rotation() const { return mCurrentKey.rotation(); }
      
      //: get the current interpolated key's position
      const vrj::Vec3& position() const { return mCurrentKey.position(); }
      
      //: get the time of the current interpolated key
      const float& time() const
      {
         //assert( mCurrentTime == mCurrentKey.time() && "should be equal" );
         return mCurrentKey.time();
      }
      
      //: get the current interpolated key's matrix transform
      void getMatrix( vrj::Matrix& mat ) const { mCurrentKey.getMatrix( mat ); }
      
      //: update func.
      void update( float timeOfKey )
      {
         if (mIsPlaying)
         {         
            mCurrentTime = mFutureTime;
            
            // if time is out of bounds,
            if (mCurrentTime > this->length() || mCurrentTime < 0)
            {
               // if we're counting loops, then decrement counter.
               if (mLoopsLeft > 0)
                  --mLoopsLeft;

               // and there are loops left, then rewind
               if (mLoopsLeft != 0)
               {
                  if (mReverse == false)
                        this->rewind();
                     else
                        this->fastforward();
               }
               // otherwise there are no loops left, so stop
               else
               {
                  this->stop();
                  return;
               }               
            }
            
            this->jump( mCurrentTime );
            
            // time passes in the direction of play (forward|reverse)
            // multiply play speed by the set factor...
            if (mReverse == false)
               mFutureTime += timeOfKey * mPlayFactor;
            else
               mFutureTime -= timeOfKey * mPlayFactor;            
         }
      }      
      
private:
      // util func
      //void output( KeyFramer::Key& key )
      //{
      //   printf( "KEY %f: %4.1f, \t%4.1f, \t%4.1f, |#| %4.1f, %4.1f, %4.1f, %4.1f\n", key.time(), key.position()[0], key.position()[1], key.position()[2], key.rotation()[0], key.rotation()[1], key.rotation()[2], key.rotation()[3] );
      //}
public:

      //: jump to arbitrary time
      // O( lg n )
      // seekToTime: time that you want to jump to in the animation 
      //             must be in the range: [0 <= x <= length()]
      // post: time() returns time
      void jump( float seekToTime )
      {
         if (mKeys.size() <= 0)
            return;

         assert( seekToTime <= this->length() );
         
         // grab the two keys that come before and after the requested time.
         std::map<float, Key>::iterator first, second;
         second = mKeys.lower_bound( seekToTime );// will always return a valid it because of the assert above.
         first = second;
         --first;
         if (first == mKeys.end())
         {
            first = second;
         }         
         
         // if out of bounds, then fix it...
         float a = (*first).first, b = (*second).first;
         if (seekToTime < a || seekToTime > b)
         {
            seekToTime = (*first).first;
         }
         
         // interpolate (set the current key)
         const Key& key_one = (*first).second; assert( key_one.time() == (*first).first && "not good" );
         const Key& key_two = (*second).second;  assert( key_two.time() == (*second).first && "not good" );
         this->interpolate( key_one, key_two, seekToTime, mCurrentKey );
         
         //output( mCurrentKey );
         
         // set the current time.
         mCurrentTime = seekToTime;
      }
      
      //: interpolate between two Keys
      void interpolate( const Key& keyOne, const Key& keyTwo, const float& timeNeeded, Key& resultingKey ) const
      {
         float time_needed( timeNeeded );
         assert( (time_needed >= keyTwo.time() && time_needed <= keyOne.time()) ||
                 (time_needed >= keyOne.time() && time_needed <= keyTwo.time()) && "time_needed need to be in between the two times" );
         float size = vrj::Math::abs( keyTwo.time() - keyOne.time() );

         // degenerate case where you're interpolating between two keys of the same size..
         if (size <= 0.00001f)
         {
            resultingKey = keyTwo;
            return;
         }

         // find the distance between [keyOneTime, time_needed, keyTwoTime] to  [0, normalize, 1]
         time_needed -= vrj::Math::Min( keyOne.time(), keyTwo.time() );
         float normalize = time_needed / size;

         // interpolate.
         vrj::Math::lerp( resultingKey.time(), normalize, keyOne.time(), keyTwo.time() );
         resultingKey.position().lerp( normalize, keyOne.position(), keyTwo.position() );
	      resultingKey.rotation().slerp( normalize, keyOne.rotation(), keyTwo.rotation() );
      }
      
      std::map<float, Key>& keys() { return mKeys; }
      const std::map<float, Key>& keys() const { return mKeys; }
         
private:
      // map a time to a key (binary tree, O(lg n) lookup)
      std::map<float, Key>  mKeys; 
      bool           mIsPlaying, mReverse;
      
      // how fast to play
      float          mPlayFactor;
      
      // -1 is continuous, 0 is none, n is number of times.
      int            mLoops, mLoopsLeft;
      
      // position in animation...
      float          mCurrentTime, mFutureTime;
      
      // the current key (interpolated)
      Key            mCurrentKey;
   };
}; //namespace kev


#endif
