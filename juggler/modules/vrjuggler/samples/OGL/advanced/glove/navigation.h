#ifndef NAVIGATION_H
#define NAVIGATION_H

//typedef  Vector3<float> Vec3;
//typedef  Matrix4f Matrix4;

typedef  vjVec3 Vec3;
typedef  vjMatrix Matrix4;

class TrackedInfo
{
public:
    //: call this once per frame with your tracker's matrix.
    void updateWithMatrix( const Matrix4& matrix );
    
    const Vec3& vector() const { return _vec; }
    const Vec3& rotDelta() const { return _rotDelta; }
    const Vec3& rotation() const { return _rot; }
    
    
private:
    Vec3 _vec, _rot, _rotOld;
    Vec3 _rotDelta;
};

class UserInfo
{
public:
    //: set the "velocity per frame" once each frame.
    //  required - call this before you use any 'update' functions.
    void  setVelocity( const float& velocity );
    
    //: set the "angular velocity per frame" once each frame.
    //  required - call this before you use any 'update' functions.
    //  give - aVelocity, a value from [0,1]
    void  setAngularVelocity( const float& aVelocity );

    //: call this once per frame with the tracker's TrackerInfo
    //  this will update user data such as position, velocity
    //  NOTE: if in "weightless" mode, 
    //        then pass (0,0,0) in for gravity
    void  update( const TrackedInfo& tracker, const Vec3& gravity );
    
    //: get the transform to put the scene from the user's point of view
    //  from the user's info, calculate, then return, the  
    //  transform to put the scene into the user's point of view
    void  getSceneTransform( Matrix4& sceneMatrtix ) const;
    
private:

    void _updateWithTracker( const TrackedInfo& tracker );
    void _updateWithGravity( const Vec3& gravity );

    // current and old position
    Vec3  _pos, _posOld;
    
    // current and old rotations
    Vec3  _rot, _rotOld;
    
    // velocity vector
    Vec3  _velocityVec;
    
    // velocity per frame
    float _velocity;
    
    // angular velocity
    float _angularVelocity;
};

#endif
