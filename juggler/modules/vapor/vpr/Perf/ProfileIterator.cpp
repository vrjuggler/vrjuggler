#include <vpr/Perf/ProfileIterator.h>

namespace vpr
{

ProfileIterator::ProfileIterator( ProfileNode* start )
{
	mCurrentParent = start;
	mCurrentChild = mCurrentParent->getChild();
}


void	ProfileIterator::first(void)
{
	mCurrentChild = mCurrentParent->getChild();
}


void	ProfileIterator::next(void)
{
	mCurrentChild = mCurrentChild->getSibling();
}


bool	ProfileIterator::isDone(void)
{
	return mCurrentChild == NULL;
}


void	ProfileIterator::enterChild( int index )
{
	mCurrentChild = mCurrentParent->getChild();
	while ( (mCurrentChild != NULL) && (index != 0) ) {
		index--;
		mCurrentChild = mCurrentChild->getSibling();
	}

	if ( mCurrentChild != NULL ) {
		mCurrentParent = mCurrentChild;
		mCurrentChild = mCurrentParent->getChild();
	}
}


void	ProfileIterator::enterParent( void )
{
	if ( mCurrentParent->getParent() != NULL ) {
		mCurrentParent = mCurrentParent->getParent();
	}
	mCurrentChild = mCurrentParent->getChild();
}


VPR_API(std::ostream&) operator<< (std::ostream& out, ProfileIterator& iter)
{
   while(!iter.isDone())
    {
      out << iter.getCurrentName() << " made " << iter.getCurrentTotalCalls()
          << " calls in " << iter.getCurrentTotalTime() << " total time" << std::endl;
         iter.next();
      }
   return out;
}
} // end vpr namespace
