// Aster
// asteroids... first person shooter style.
//
// kevin meinert - kevin@vrsource.org
// Public domain
//
// Features:
// - mouselook (no pointer with warp) rotate camera
// - straef
// - air hockey physics
// - alternate keyboard controls (works without mouse)
// - 2 gun turrets on either side...
//
// Usage/Keys:
//
//    -------------------------------------------
//        W                    accel
//      A S D          straef  decel  straef
//    -------------------------------------------
//        up                   accel
//    left  right      rotate         rotate
//       down                  decel
//    -------------------------------------------
//    mouse(x,y) axes          rotate
//    left mouse button        fire
//    -------------------------------------------
//    <SPACEBAR>               fire
//    -------------------------------------------
//         f             fullscreen toggle
//    -------------------------------------------
//
// TODO:
//   - pretty models and textures... (use a model loader)
//   - keep score
//   - keep track of lives (ships)
//   - die when collide with asteroid (no collision performed yet)
//   - add a top-down 2D map or radar screen
//   - let the top down 2D map be one additional way to play the game
//

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <snx/sonix.h>    // interface
#include <snx/SoundHandle.h>

#include "StopWatch.h" // for time measurment...
#include "cubeGeometry.h" // for cheezy cube geometry

///////////////////////////////////////////////////////////
// A few useful math routines
///////////////////////////////////////////////////////////
namespace Math
{
   inline float deg2rad( float deg )
   {
      return (deg * 3.1459f) / 180.0f;
   }
   inline float rad2deg( float rad )
   {
      return (rad * 180.0f) / 3.1459f;
   }
   inline float abs( float a )
   {
      return (a < 0.0f) ? -a : a;
   }

   //: return a random number between 0.0f and 1.0f
   //! RETURNS: random number between 0 and 1
   inline float random()
   {
      // ::rand() returns int from  0 - RAND_MAX
      const float rand_max = RAND_MAX;
      float r = static_cast<float>( ::rand() );
      r /= rand_max;
      return r;
   }

   //: return a random number between x1 and x2
   //! RETURNS: random number between x1 and x2
   inline float random( float x1, float x2 )
   {
      float r = Math::random();
      float size = x2 - x1;
      return r * size + x1;
   }
   
   // dot product.  sqrt of this gives dist between points..
   inline float dot( float pt1[3], float pt2[3] )
   {
      float d = 0.0f;
      d += pt1[0] * pt2[0];
      d += pt1[1] * pt2[1];
      d += pt1[2] * pt2[2];
      return d;
   }
   // subtract two vectors, store result in res
   inline void sub( float res[3], float vec1[3], float vec2[3] )
   {
      res[0] = vec1[0] - vec2[0];
      res[1] = vec1[1] - vec2[1];
      res[2] = vec1[2] - vec2[2];
   }
   // add two vectors, store result in res
   inline void add( float res[3], float vec1[3], float vec2[3] )
   {
      res[0] = vec1[0] + vec2[0];
      res[1] = vec1[1] + vec2[1];
      res[2] = vec1[2] + vec2[2];
   }
   // scale a vector
   inline void mul( float res[3], float vec1[3], float scalar )
   {
      res[0] = vec1[0] * scalar;
      res[1] = vec1[1] * scalar;
      res[2] = vec1[2] * scalar;
   }
   // copy a vector
   inline void copy( float res[3], float vec1[3] )
   {
      res[0] = vec1[0];
      res[1] = vec1[1];
      res[2] = vec1[2];
   }
   // normalize a vector
   inline void norm( float vec[3] )
   {
      float d = Math::dot( vec, vec );
      if (d <= 0.0001f) // some epsillon (d should be > 0)
         return;
      
      float n = sqrtf( d );
      float n_inv = 1.0f / n;
      Math::mul( vec, vec, n_inv );
   }
   // distance
   inline float distance( float vec1[3], float vec2[3]  )
   {
      float temp[3];
      Math::sub( temp, vec1, vec2 );
      return sqrtf( Math::dot( temp, temp ) );
   }
   
   // compute perfectly elastic reflection using available vectors... 
   inline void reflect( float reflection[3], float incoming[3], float surfacenormal[3] )
   {
      Math::mul( reflection, incoming, -1.0f );
      float d = Math::dot( reflection, surfacenormal );
      Math::mul( reflection, surfacenormal, d );
      Math::mul( reflection, reflection, 2.0f );
      Math::add( reflection, reflection, incoming );
   }   
}

///////////////////////////////////////////////////////////
// a few game objects
///////////////////////////////////////////////////////////

class BaseObject
{
public:
   BaseObject() : size( 1.0f ), rotation( 0 ), angularVel( 0 ), timeToLive( -1 )
   {
      this->setPos( 0.0f, 0.0f, 0.0f );
      this->setVel( 0.0f, 0.0f, 0.0f );
   }
   void draw() 
   { 
      glPushMatrix();
      glTranslatef( position[0], position[1], position[2] );
      glRotatef( rotation, 0,1,0 );
      glScalef( size * 0.5f, size * 0.5f, size * 0.5f );
      cube.render(); 
      glPopMatrix();
   }
   void update( float dt )
   {
      position[0] += velocity[0] * dt;
      position[1] += velocity[1] * dt;
      position[2] += velocity[2] * dt;
      rotation += angularVel * dt;
      if (timeToLive != -1) 
         timeToLive -= dt;
   }
   void setPos( float x, float y, float z )
   {
      position[0] = x;
      position[1] = y;
      position[2] = z;
   }   
   void setVel( float x, float y, float z )
   {
      velocity[0] = x;
      velocity[1] = y;
      velocity[2] = z;
   }   
   float size;
   float rotation;
   float angularVel;
   float timeToLive; // -1 is infinite == don't ever die;
   float velocity[3];
   float position[3];
   cubeGeometry cube;
};

class Projectile : public BaseObject
{
public:
};
class Roid : public BaseObject
{
public:
};
class Ship : public BaseObject
{
public:
};

class Grid
{
public:
   Grid()
   {
      size = 10;
      this->setPos( 0.0f, 0.0f, 0.0f );
      this->setColor( 0.3f, 0.2f, 0.6f );
   }
   void draw()
   {
      glColor3f( color[0], color[1], color[2] );
      glPushMatrix();
      glTranslatef( position[0], position[1], position[2] );
      glBegin( GL_LINES );
         for ( float x = -size; x < size; ++x)
         {
            glVertex3f( -size, 0, x );
            glVertex3f(  size, 0, x );
            glVertex3f( x, 0, -size );
            glVertex3f( x, 0,  size );
         }
      glEnd();
      glPopMatrix();
   }
   float size;
   void setPos( float x, float y, float z )
   {
      position[0] = x;
      position[1] = y;
      position[2] = z;
   }
   void setColor( float r, float g, float b )
   {
      color[0] = r;
      color[1] = g;
      color[2] = b;
   }   
   float position[3];
   float color[3];
};

///////////////////////////////////////////////////////////
// the asteroids application
///////////////////////////////////////////////////////////
class App
{
public:
   App() : width( 0 ), height( 0 ), mainWin_contextID( -1 ), 
      boardSize( 15 ), 
      projVelocity( 20.0f ), projSize( 0.1f ), projTimeToLive( 0.75f ), 
      rotVelocity( 60.0f ), shipAccel( 10.0f ), 
      roidSize( 3.0f ), roidMaxSpeed( 3.0f ), 
      mousesens( 0.15f ), 
      rotLeftButton( false ), rotRightButton( false ), 
      straefLeftButton( false ), straefRightButton( false ), 
      fireButton( false ), accelButton( false ), decelButton( false )
   {
      stopwatch.pulse();
      Roid r; 
      r.size = roidSize;
      r.setPos( -5.0f, 0.0f, -5.0f );
      r.setVel( Math::random() * roidMaxSpeed, 0, Math::random() * roidMaxSpeed );
      roids.push_back( r );
      r.setPos( -5.0f, 0.0f, 5.0f );
      r.setVel( Math::random() * roidMaxSpeed, 0, Math::random() * roidMaxSpeed );
      roids.push_back( r );
      r.setPos(  5.0f, 0.0f, -5.0f );
      r.setVel( Math::random() * roidMaxSpeed, 0, Math::random() * roidMaxSpeed );
      roids.push_back( r );
      r.setPos(  5.0f, 0.0f, 5.0f );
      r.setVel( Math::random() * roidMaxSpeed, 0, Math::random() * roidMaxSpeed );
      roids.push_back( r );
      
      grid.setPos( 0.0f, -1.0f, 0.0f );
      grid.size = boardSize;
      
      sonix::instance()->changeAPI( "OpenAL" );
      snx::SoundInfo si;
      si.retriggerable = true;
      si.filename = "fire.wav";
      si.ambient = false;
      si.datasource = snx::SoundInfo::FILESYSTEM;
      mFireSound.init( "fire" );
      mFireSound.configure( si );
//      mFireSound.setPitchBend( 1 );
      si.filename = "music.wav";
      si.datasource = snx::SoundInfo::FILESYSTEM;
      mApproaching.init( "approaching" );
      mApproaching.configure( si );
      mApproaching.trigger( -1 );
      mApproaching.setCutoff( 0.1 );
      mApproaching.setPitchBend( 0.1 );
      si.filename = "motion.wav";
      si.datasource = snx::SoundInfo::FILESYSTEM;
      mMoving.init( "moving" );
      mMoving.configure( si );
      mMoving.trigger( -1 );
      mMoving.setCutoff( 0.1 );
      mMoving.setPitchBend( 0.1 );
      si.filename = "explode.wav";
      si.datasource = snx::SoundInfo::FILESYSTEM;
      mExplode.init( "explode" );
      mExplode.configure( si );
   }

   void draw()
   {
      // camera xform (this does the first person view navigation transform)
      glRotatef( -ship.rotation, 0.0f, 1.0f, 0.0f );
      glTranslatef( -ship.position[0], -ship.position[1], -ship.position[2] );
      
      unsigned int x;
      for (x = 0; x < roids.size(); ++x)
      {
         roids[x].draw();
      }  
      for (x = 0; x < projs.size(); ++x)
      {
         projs[x].draw();
      }  
      grid.draw();    
   }   
   
   void update( float dt )
   {
      ::glutWarpPointer( width / 2, height / 2 );
      
      
      ///////////
      // PHYSICS
      ///////////
      ship.update( dt );

      unsigned int x;
      for (x = 0; x < roids.size(); ++x)
      {
         roids[x].update( dt );
      }  
      for (x = 0; x < projs.size(); ++x)
      {
         projs[x].update( dt );
      }
      
      
      ///////////
      // INPUT: 
      ///////////
      
      // find out what dir, the ship is pointing.
      // - flip the x axis to support right-hand rule
      // - add 90deg so that the identity rotation faces out of the screen
      // - then negate it all because we actually face forward (in 0,0,-1)
      float shipForward[3];
      shipForward[0] = cosf( Math::deg2rad( ship.rotation + 90 ) ); 
      shipForward[1] = 0;                                           
      shipForward[2] = -sinf( Math::deg2rad( ship.rotation + 90 ) ); 
      
      float shipRight[3];
      shipRight[0] = cosf( Math::deg2rad( ship.rotation ) ); 
      shipRight[1] = 0;                                           
      shipRight[2] = -sinf( Math::deg2rad( ship.rotation ) ); 
      
      float shipUp[3] = { 0.0f, 1.0f, 0.0f };
      
      
      // handle button input
      // poll each button state, do appropriate behaviour
      if (rotLeftButton)
         ship.angularVel = rotVelocity;
      else if (rotRightButton)
         ship.angularVel = -rotVelocity;
      else
         ship.angularVel = 0;
      
      if (fireButton)
      {
         mFireSound.trigger();
         
         // fire in the direction ship faces
         // use flip flop to give effect of two side mounted gun turrets...
         Projectile proj;
         static float flip_flop = -1;
         flip_flop = -flip_flop;
         
         // local position on the ship
         float local_pos[3] = { 0.2f * flip_flop, -0.2f, 0.4f };
         
         // convert the local position to world coord
         float world_offset[3];
         world_offset[0] = shipRight[0] * local_pos[0] + shipUp[0] * local_pos[1] + shipForward[0] * local_pos[2];
         world_offset[1] = shipRight[1] * local_pos[0] + shipUp[1] * local_pos[1] + shipForward[1] * local_pos[2];
         world_offset[2] = shipRight[2] * local_pos[0] + shipUp[2] * local_pos[1] + shipForward[2] * local_pos[2];
         
         // set the projectile start position (relative to the ships world position)
         proj.setPos( ship.position[0] + world_offset[0], 
                      ship.position[1] + world_offset[1], 
                      ship.position[2] + world_offset[2] );
         proj.setVel( shipForward[0] * projVelocity, 
                      shipForward[1] * projVelocity, 
                      shipForward[2] * projVelocity );
         proj.size = projSize;
         proj.timeToLive = projTimeToLive;
         projs.push_back( proj );
      }
      if (accelButton)
         ship.setVel( ship.velocity[0] + (shipForward[0] * shipAccel * dt), 
                      ship.velocity[1] + (shipForward[1] * shipAccel * dt), 
                      ship.velocity[2] + (shipForward[2] * shipAccel * dt) );
      
      if (decelButton)
         ship.setVel( ship.velocity[0] - (shipForward[0] * shipAccel * dt), 
                      ship.velocity[1] - (shipForward[1] * shipAccel * dt), 
                      ship.velocity[2] - (shipForward[2] * shipAccel * dt) );
      if (straefRightButton)
         ship.setVel( ship.velocity[0] + (shipRight[0] * shipAccel * dt), 
                      ship.velocity[1] + (shipRight[1] * shipAccel * dt), 
                      ship.velocity[2] + (shipRight[2] * shipAccel * dt) );
      
      if (straefLeftButton)
         ship.setVel( ship.velocity[0] - (shipRight[0] * shipAccel * dt), 
                      ship.velocity[1] - (shipRight[1] * shipAccel * dt), 
                      ship.velocity[2] - (shipRight[2] * shipAccel * dt) );
      
      // reset the buttons (unless you want them to keep retriggering)
      //rotLeftButton = false;
      //rotRightButton = false;
      fireButton = false;
      //accelButton = false;
      //decelButton = false;
      
      float zero[3] = { 0,0,0 };
      float d = (Math::distance( ship.velocity, zero ) / 6.0f);
      if (d < 0.001) d = 0.001;
      mMoving.setPitchBend( d );
      mMoving.setCutoff( d );
      
      
      //////////////
      // GAME LOGIC
      // collisions, roid bounce, removing, etc..
      //////////////


      // do asteroid bounce with the walls of the arena.
      std::vector<Roid>::iterator itr;
      //const float eps = 0.01f;
      for (itr = roids.begin(); itr != roids.end(); ++itr)
      {
         if ((*itr).position[0] > boardSize || (*itr).position[0] < -boardSize)
            (*itr).velocity[0] = -(*itr).velocity[0];
         if ((*itr).position[1] > boardSize || (*itr).position[1] < -boardSize)
            (*itr).velocity[1] = -(*itr).velocity[1];
         if ((*itr).position[2] > boardSize || (*itr).position[2] < -boardSize)
            (*itr).velocity[2] = -(*itr).velocity[2];
      }
      
      // do bounce on the ship with the edges of the arena.
      float outside = 10.0f;// ship can go this much outside the arena before bouncing
      float dampen = 0.8f;// dampen the elastic collision with wall
      if (ship.position[0] > (boardSize+outside) || ship.position[0] < -(boardSize+outside))
         ship.velocity[0] = -ship.velocity[0] * dampen;
      if (ship.position[1] > (boardSize+outside) || ship.position[1] < -(boardSize+outside))
         ship.velocity[1] = -ship.velocity[1] * dampen;
      if (ship.position[2] > (boardSize+outside) || ship.position[2] < -(boardSize+outside))
         ship.velocity[2] = -ship.velocity[2] * dampen;

      // remove projectiles that are too old
      std::vector<Projectile>::iterator pitr;
      std::vector< std::vector<Projectile>::iterator > proj_remove_queue;
      for (pitr = projs.begin(); pitr != projs.end(); ++pitr)
      {
         if ((*pitr).timeToLive != -1 && (*pitr).timeToLive <= 0)
            proj_remove_queue.push_back( pitr );
      }
      x = proj_remove_queue.size();
      while (x--)
         projs.erase( proj_remove_queue[x] );
      
      // do collision between ship and asteroids...
      float closest_dist = 9999;
      for (itr = roids.begin(); itr != roids.end(); ++itr)
      {
         // get dist from ship to center of roid (treat roid as sphere for simplicity)
         float dist = Math::distance( ship.position, (*itr).position );
         float min_dist_between_ship_and_roid = ((*itr).size + ship.size);

         if ((dist - min_dist_between_ship_and_roid) < closest_dist)
            closest_dist = (dist - min_dist_between_ship_and_roid);
         
         // if too close, then collision...
         if (dist < min_dist_between_ship_and_roid)
         {
            // get vector from roid to ship (normal to collision angles)
            float normal[3];
            Math::sub( normal, ship.position, (*itr).position );
            Math::norm( normal );

            // reflect velocity only if moving toward each other
            if (Math::dot( ship.velocity, normal ) < 0)
            {
               // compute perfectly elastic reflection
               float reflectionVector[3];
               Math::reflect( reflectionVector, ship.velocity, normal );

               // dampen resulting reflection
               Math::mul( ship.velocity, reflectionVector, dampen );
               
               mExplode.trigger();
            }
            
            // asteroids can push the ship around... (affects position)
            float vec[3], newposition[3];
            Math::sub( vec, ship.position, (*itr).position );
            Math::norm( vec );
            Math::mul( vec, vec, min_dist_between_ship_and_roid );
            Math::add( newposition, (*itr).position, vec );
            
            // set the new ship position and velocity
            float additional_velocity_from_impact[3];
            Math::sub( additional_velocity_from_impact, newposition, ship.position );
            Math::add( ship.velocity, ship.velocity, additional_velocity_from_impact );
            Math::copy( ship.position, newposition );            
         }
      }
      
      d = 1.0f - (closest_dist / 16.0f);
      if (d < 0.001) d = 0.001;
      mApproaching.setPitchBend( d );
      //mApproaching.setCutoff( d );
      
      // do collision between projectile and asteroids...
      for (itr = roids.begin(); itr != roids.end(); ++itr)
      {
         for (pitr = projs.begin(); pitr != projs.end(); ++pitr)
         {
            // get dist from bullet to center of roid (treat roid as sphere for simplicity)
            float dist = Math::distance( (*pitr).position, (*itr).position );
            
            // consider the size of the roid (smaller is harder to hit because the distance will need to be much closer)
            // the dist between object centers should be less than the sum of the two radii 
            if (dist < ((*itr).size + (*pitr).size))
            {
               Roid aster = (*itr); 
               
               // remove the hit asteroid, and the hit projectile
               roids.erase( itr );
               projs.erase( pitr );
                  
               // if it was big enough, replace it with two smaller ones...
               if (aster.size > 0.5f) // .25 is smallest size roid can break into
               {
                  Roid r; 
                  r.size = aster.size / 2.0f;
                  r.setPos( aster.position[0], aster.position[1], aster.position[2] );
                  r.setVel( Math::random() * roidMaxSpeed, 0, Math::random() * roidMaxSpeed );
                  roids.push_back( r );
                  
                  r.setPos( aster.position[0], aster.position[1], aster.position[2] );
                  r.setVel( Math::random() * roidMaxSpeed, 0, Math::random() * roidMaxSpeed );
                  roids.push_back( r );
                  
                  mExplode.trigger();
               }
               
               return; // WARNING: don't put any code after this for loop!
            }       
         }
      }
      
      
   }   
      
   // window state
   int width, height;
   int mainWin_contextID;
   
   // game setup (configuration options)
   const int boardSize;
   const float projVelocity, projSize, projTimeToLive;
   const float rotVelocity, shipAccel;
   const float roidSize, roidMaxSpeed;
   const float mousesens;
   
   // ship state
   Ship ship;
   bool rotLeftButton, rotRightButton;
   bool straefLeftButton, straefRightButton;
   bool fireButton, accelButton, decelButton;
   
   // projectile state
   std::vector<Projectile> projs;
   
   // aster-roids state
   std::vector<Roid> roids;

   // other game objects...
   Grid grid;
   
   // frame time measurement
   StopWatch stopwatch;
   
   snx::SoundHandle mFireSound, mApproaching, mMoving, mExplode;
};
App app;


///////////////////////////////////////////////////////////
// a bunch of glut stuff (drives our game application object)
///////////////////////////////////////////////////////////




//////////////////////////////////////////////////
// This is called on a glutPostRedisplay
//////////////////////////////////////////////////
static void OnRedisplay()
{ 
   glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
   glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
   glEnable( GL_DEPTH_TEST );
   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 
   
   
   // set up the projection matrix
   glMatrixMode( GL_PROJECTION );
        glLoadIdentity();                     
        gluPerspective( 100.0f, app.width / app.height, 0.01f, 1000.0f );
                           
   // initialize your matrix stack used for transforming your models
    glMatrixMode( GL_MODELVIEW );
      glLoadIdentity();      

 
   
   
   app.draw();
   
   // swaps the front and back frame buffers.
   // hint: you've been drawing on the back, offscreen, buffer.  
   // This command then brings that framebuffer onscreen.
   glutSwapBuffers();
}

//////////////////////////////////////////////////
// This is called repeatedly, as fast as possible
//////////////////////////////////////////////////
static void OnIdle()
{
   // According to the GLUT specification, the current window is
   // undefined during an idle callback.  So we need to explicitly change
   // it if necessary
   if ( glutGetWindow() != app.mainWin_contextID )
           glutSetWindow( app.mainWin_contextID );

   // tell glut to call redisplay (which then calls OnRedisplay)
   glutPostRedisplay();
   
   
   // non graphics calculations...
   app.stopwatch.pulse();
   app.update( app.stopwatch.timeDelta() );
   sonix::instance()->step( app.stopwatch.timeDelta() );
   
   
}

/////////////////////////////////////////////
// This is called on a Resize of the glut window
/////////////////////////////////////////////
static void OnReshape( int width, int height ) 
{
   // save these params in case your app needs them
   app.width = width;
   app.height = height;
   
   // set your viewport to the extents of the window
   glViewport( 0, 0, width, height );
   
   // let the app run idle, while resizing, 
   // glut does not do this for us automatically, so call OnIdle explicitly.
   OnIdle();
}

////////////////////////////////
// This is called on a Down Keypress        
////////////////////////////////
static void OnKeyboardDown( unsigned char k, int x, int y )
{ 
   switch (k)
   {
   // If user pressed 'q' or 'ESC', then exit the app.
   // this is really ungraceful, but necessary since GLUT does a while(1)
   // as it's control loop.  There is no GLUT method to exit, unfortunately.
   case 'q':
   case 27:
      exit( 0 );
      break;

   case ' ':
      app.fireButton = true;
      break;
   case 'f':
   case 'F':
      static bool full = false;
      static int old_width, old_height;
      if (full == false)
      {
         glutFullScreen();
         old_width = app.width;
         old_height = app.height;
         std::cout<<"full"<<std::endl;
      }
      else
      {
         glutReshapeWindow( old_width, old_height );
         std::cout<<"win"<<std::endl;
      }
      full = !full;
      std::cout<<full<<std::endl;
      break;
   case 'A':
   case 'a':
     app.straefLeftButton = true;
      break;
   case 'D':
   case 'd':
     app.straefRightButton = true;
      break;
   case 'W':
   case 'w':
      app.accelButton = true;
      break;
   case 'S':
   case 's':
      app.decelButton = true;
      break;
   default:
      // do nothing if no key is pressed
      break;
   }
}
////////////////////////////////
// This is called on a Up Keypress        
////////////////////////////////
static void OnKeyboardUp( unsigned char k, int x, int y ) 
{
   switch (k)
   {
   case ' ':
      //app.fireButton = false; // deselect this in the disp loop 
                                // so were sure to get the message
      break;
   case 'A':
   case 'a':
     app.straefLeftButton = false;
      break;
   case 'D':
   case 'd':
     app.straefRightButton = false;
      break;
   case 'W':
   case 'w':
      app.accelButton = false;
      break;
   case 'S':
   case 's':
      app.decelButton = false;
      break;
   default:
      // do nothing if no key is pressed
      break;
   }
}

////////////////////////////////
// This is called on a Down Keypress 
// of a "special" key such as the grey arrows.
////////////////////////////////
static void OnSpecialKeyboardDown(int k, int x, int y) 
{
   switch (k)
   {
   case GLUT_KEY_LEFT:
      app.rotLeftButton = true;
      break;
   case GLUT_KEY_RIGHT:
      app.rotRightButton = true;
      break;
   case GLUT_KEY_UP:
      app.accelButton = true;
      break;
   case GLUT_KEY_DOWN:
      app.decelButton = true;
      break;
   default:
      // do nothing if no special key pressed
      break;
   }
}

////////////////////////////////
// This is called on a Up Keypress        
////////////////////////////////
static void OnSpecialKeyboardUp( int k, int x, int y ) 
{
   switch (k)
   {
   case GLUT_KEY_LEFT:
      app.rotLeftButton = false;
      break;
   case GLUT_KEY_RIGHT:
      app.rotRightButton = false;
      break;
   case GLUT_KEY_UP:
      app.accelButton = false;
      break;
   case GLUT_KEY_DOWN:
      app.decelButton = false;
      break;
   default:
      // do nothing if no special key pressed
      break;
   }
}

////////////////////////////////
// This is called when mouse changes position
// x and y are the screen position 
// in your 2D window's coordinate frame
////////////////////////////////
static void OnMousePos( int x, int y ) 
{
   static bool once_only = true;
   if (once_only)
   {
      once_only = false;
      return;
   }
   
   // this implements a 1st person mouselook (similar to quake)
   // this requires that you call glutWarpPointer each frame to center of screen.
   float center[2];
   center[0] = (float)app.width * 0.5f;
   center[1] = (float)app.height * 0.5f;
   float dx = center[0] - (float)x;//, dy = center[1] - (float)y;
   app.ship.rotation += dx * app.mousesens;
}

////////////////////////////////
// This is called when mouse clicks
////////////////////////////////
static void OnMouseClick( int button, int state, int xpos, int ypos)
{
   // for button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
   // for state: GLUT_DOWN, GLUT_UP
   
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
      app.fireButton = true;
   //else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
   //   app.fireButton = false; // deselect this in the disp loop 
                                // so were sure to get the message
}


// Initialize the application
// initialize the state of your app here if needed...
static void OnApplicationInit()
{
   
}


int main( int argc, char* argv[] )
{
    // Initialize the application
    // initialize the state of your app here if needed...
    OnApplicationInit();
   
    // Set the window's initial size
    ::glutInitWindowSize( 640, 480 );
    ::glutInit( &argc, argv );
  
    // Set to double buffer to reduce flicker
    ::glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    
    // Set the window title
    app.mainWin_contextID = ::glutCreateWindow( "GLUT application" );
    
    std::cout<<"\n"<<std::flush;
    std::cout<<"aster - by Kevin Meinert - subatomic@vrsource.org\n"<<std::flush;
    std::cout<<" Usage/Keys:                                   \n"<<std::flush;
    std::cout<<"                                               \n"<<std::flush;
    std::cout<<"    -------------------------------------------\n"<<std::flush;
    std::cout<<"        W                    accel             \n"<<std::flush;
    std::cout<<"      A S D          straef  decel  straef     \n"<<std::flush;
    std::cout<<"    -------------------------------------------\n"<<std::flush;
    std::cout<<"        up                   accel             \n"<<std::flush;
    std::cout<<"    left  right      rotate         rotate     \n"<<std::flush;
    std::cout<<"       down                  decel             \n"<<std::flush;
    std::cout<<"    -------------------------------------------\n"<<std::flush;
    std::cout<<"    mouse(x,y) axes          rotate            \n"<<std::flush;
    std::cout<<"    left mouse button        fire              \n"<<std::flush;
    std::cout<<"    -------------------------------------------\n"<<std::flush;
    std::cout<<"    <SPACEBAR>               fire              \n"<<std::flush;
    std::cout<<"    -------------------------------------------\n"<<std::flush;
    std::cout<<"        f             fullscreen toggle        \n"<<std::flush;
    std::cout<<"    -------------------------------------------\n"<<std::flush;
    std::cout<<"\n"<<std::flush;
    
   // display callbacks.
   ::glutReshapeFunc( OnReshape );
   ::glutIdleFunc( OnIdle );
   ::glutDisplayFunc( OnRedisplay );

   // tell glut to not call the keyboard callback repeatedly 
   // when holding down a key. (uses edge triggering, like the mouse)
   ::glutIgnoreKeyRepeat( 1 );

   // keyboard callback functions.
   ::glutKeyboardFunc( OnKeyboardDown );
   ::glutKeyboardUpFunc( OnKeyboardUp );
   ::glutSpecialFunc( OnSpecialKeyboardDown );
   ::glutSpecialUpFunc( OnSpecialKeyboardUp );

   // mouse callback functions...
   ::glutMouseFunc( OnMouseClick );
   ::glutMotionFunc( OnMousePos );
   ::glutPassiveMotionFunc( OnMousePos );
   
   // with the warp pointer, we don't want to see cursor flashing
   // turn it off...
   ::glutSetCursor( GLUT_CURSOR_NONE );
   
    // start the application loop, your callbacks will now be called
    // time for glut to sit and spin.
    ::glutMainLoop();
    return 1;
}

