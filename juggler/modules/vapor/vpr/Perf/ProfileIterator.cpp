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
      ProfileNode* temp;
      temp = mCurrentParent;
      mCurrentParent = mCurrentChild; 
	   mCurrentChild = mCurrentChild->getChild();
      if(mCurrentChild == NULL)
      {
         mCurrentChild = mCurrentParent;
         mCurrentParent = temp;
         mCurrentChild->getSibling();
      }
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
   if(mCurrentChild == NULL)
   {
      enterParent();
      next();
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
      out << iter.getCurrentName() << " total calls: " << iter.getCurrentTotalCalls()
          << " total time: " << iter.getCurrentTotalTime() << " ave: " 
          << iter.getCurrentTotalCalls() /iter.getCurrentTotalTime() << " history: ";

      ProfileNode::NodeHistoryRange p = iter.getNodeHistoryRange();
      for(; p.first != p.second; p.first++)
         { out << *(p.first) << " "; }

      out << std::endl;
      iter.next();
      }
  
   return out;
}

} // end vpr namespace
