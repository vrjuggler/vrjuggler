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
   mHistorySize = 1;
   mHistory.resize(mHistorySize);
   mStartTime.secf(0);
	reset();
}

ProfileNode::ProfileNode( const char * name, ProfileNode * parent, const unsigned int queue_size) :
	mName( name ),
	mTotalCalls( 0 ),
	mTotalTime( 0 ),
	mRecursionCounter( 0 ),
	mParent( parent ),
	mChild( NULL ),
	mSibling( NULL )
{
   mHistorySize = queue_size;
   mHistory.resize(mHistorySize);
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

ProfileNode* ProfileNode::getSubNode( const char* name, const unsigned int queue_size )
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
	ProfileNode* node = new ProfileNode( name, this, queue_size);
	node->mSibling = mChild;
	mChild = node;
	return node;
}

void ProfileNode::printTree(ProfileNode* node)
{
   if(node == NULL) return;

   std::cout << getName() << " total calls: " << getTotalCalls()
          << " total time: " << getTotalTime() << " ave: " 
          << getTotalCalls() /getTotalTime() << " history: ";

   NodeHistoryRange p = getNodeHistoryRange();
      for(; p.first != p.second; p.first++)
         { std::cout << *(p.first) << " "; }

      std::cout << std::endl;

   printTree(node->getChild());
   printTree(node->getSibling());
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
      mHistory.push_front((float)(time.secf() / profileGetTickRate().secf()));
      mHistory.resize(mHistorySize);
		mTotalTime += mHistory.front();
	}
	return ( mRecursionCounter == 0 );
}

ProfileNode	   ProfileManager::mRoot( "Root", NULL );
ProfileNode*	ProfileManager::mCurrentNode = &ProfileManager::mRoot;
int				ProfileManager::mFrameCounter = 0;
vpr::Interval*	ProfileManager::mResetTime = 0;



} // end vpr namespace


