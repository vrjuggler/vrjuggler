#include <vrj/Sound/pf/pfSoundNode.h>


pfSoundNode::pfSoundNode( const std::string& sound, bool isPositional ) : mIsPositional( true )
{
   this->setPositional( isPositional );
   this->setSound( sound );
   this->setType(classType);  // Set the type
}


// app() - APP traversal function.  This overloads the standard pfDCS
// app() method, which will be called each frame during the APP
// traversal of the scene graph (*only if* needsApp() (below) returns
// TRUE).
// app() is called automatically by Performer; it is not called directly
// by a program.
int pfSoundNode::app(pfTraverser *trav)
{
   // update the sound attributes (position) based on the current
   // position of this node.
   // only do it if the sound is a positional sound.
   if (mSound != "" && mIsPositional == true)
   {
      // get position of this sound in relation to the user's orientation and position
      // NOTE: the sound will change position if the user rotates without translation.
      //       it should orbit the user, so that the audio will pan correctly.
      pfMatrix matrix, traverserMatrix, dcsMatrix;
      matrix.makeIdent();

      //: take the sound from soundspace to userspace.

      // add in any offset due to this DCS node
      // (since the traverser hasn't yet added it to it's matrix stack.)
      // *NOTE: this is the matrix that takes the sound from local(sound)space to modelspace.
      this->getMat( dcsMatrix ); //TODO: don't call this.
      // ...to get the location of the sound in modelspace, you'd do an invertFull here, but we want it in user space..
      matrix.postMult( dcsMatrix );

      // add in any offset due to navigation and any other DCS xforms currently on the traverser's stack.
      // this is the position of the model's origin in relation to a user at position 0,0,0
      // this is just the nav matrix, which should be on the matrix stack right now...
      // *NOTE: this is the matrix that takes the sound from modelspace to userspace
      trav->getMat( traverserMatrix );
      // ...to get the location of the sound in modelspace, you'd do an invertFull here, but we want it in user space..
      matrix.postMult( traverserMatrix );

      pfCoord coord;
      matrix.getOrthoCoord( &coord );
      pfVec3 pf_soundPosition = coord.xyz;

      // set my sound's position.
#ifdef USE_SONIX
      vrj::Vec3 soundPosition = vrj::GetVjVec( pf_soundPosition );
      sonix::instance().setPosition( mSound, soundPosition[0], soundPosition[1], soundPosition[2] );
#endif
      
      // Engine's update should be called by the app's frame process,
      // or in juggler's manager (not both, of course)...

      // use this for debugging the location of the sound reletive to the user.
      //std::cout<<"["<<mSound->getName()<<"] sound in userspace: "<<soundPosition[0]<<" "<<soundPosition[1]<<" "<<soundPosition[2]<<"\n"<<std::flush;
   }
   else
   {
      // redundant (fixme), but make sure it's 0.0f,0.0f,0.0f
      // this makes the sound the same as the observer.
#ifdef USE_SONIX
      sonix::instance().setPosition( mSound, 0.0f, 0.0f, 0.0f );
#endif
   }

   return pfDCS::app( trav );  // call the parent class's app()

}

//---------------------------------------------------------------------//
// Performer type data - this part is required for any class which
// is derived from a Performer class.  It creates a new pfType
// which identifies objects of this class.  All constructors for
// this class must then call setType(classType_).
pfType* pfSoundNode::classType = NULL;

void pfSoundNode::init(void)
{
   vprDEBUG(vprDBG_ALL,1)<<"[pfSoundNode] Registering sound node with performer.\n"<<vprDEBUG_FLUSH;

   if (classType == NULL)
   {
        pfDCS::init();           // Initialize my parent
        classType =  new pfType(pfDCS::getClassType(), "pfSoundNode");  // Create the new type
   }
}
//----------------------------------------------------------------------//
