#include "vpr/Perf/ProfileManager.h"
#include "vpr/Perf/ProfileNode.h"


namespace vpr
{


ProfileNode::ProfileNode( const char * name, ProfileNode * parent ) :
	mName( name ),
	mTotalCalls( 0 ),
	mTotalTime( 0 ),
	mRecursionCounter( 0 ),
	mParent( parent ),
	mChild( NULL ),
	mSibling( NULL )
{
   mStartTime.secf(0);
	reset();
}


ProfileNode::~ProfileNode( void )
{
	delete mChild;
	delete mSibling;
}


ProfileNode* ProfileNode::getSubNode( const char* name )
{
	// Try to find this sub node
	ProfileNode* child = mChild;
	while ( child ) {
		if ( child->mName == name ) {
			return child;
		}
		child = child->mSibling;
	}

	// We didn't find it, so add it
	ProfileNode* node = new ProfileNode( name, this );
	node->mSibling = mChild;
	mChild = node;
	return node;
}


void	ProfileNode::reset( void )
{
	mTotalCalls = 0;
	mTotalTime = 0.0f;

	if ( mChild ) {
		mChild->reset();
	}
	if ( mSibling ) {
		mSibling->reset();
	}
}


void	ProfileNode::call( void )
{
	mTotalCalls++;
	if (mRecursionCounter++ == 0) {
		profileGetTicks(&mStartTime);
	}
}


bool	ProfileNode::Return( void )
{
	if ( --mRecursionCounter == 0 && mTotalCalls != 0 ) { 
      vpr::Interval time;
		profileGetTicks(&time);
		time-=mStartTime;
		mTotalTime += (float)(time.secf() / profileGetTickRate().secf());
	}
	return ( mRecursionCounter == 0 );
}




ProfileNode	   ProfileManager::mRoot( "Root", NULL );
ProfileNode*	ProfileManager::mCurrentNode = &ProfileManager::mRoot;
int				ProfileManager::mFrameCounter = 0;
vpr::Interval*	ProfileManager::mResetTime = 0;



} // end vpr namespace


