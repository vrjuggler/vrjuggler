#ifndef VPR_PROFILE_MANAGER_H
#define VPR_PROFILE_MANAGER_H


/**
 * mostly taken from:
 * Real-Time Hierarchical Profiling for Game Programming Gems 3
 * by Greg Hjelstrom & Byon Garrabrant
 */

#include <vpr/Perf/ProfileIterator.h>
#include <vpr/Perf/ProfileNode.h>

namespace vpr
{

/*
 * The Manager for the Profile system
 */
class	ProfileManager {
public:

   /**
   * Steps one level deeper into the tree, if a child already exists with the specified name
   * then it accumulates the profiling; otherwise a new child node is added to the profile tree.
   * INPUT: name - name of this profiling record
   * WARNINGS:
   * The string used is assumed to be a static string; pointer compares are used throughout
   * the profiling code for efficiency.
   */
	static	void						startProfile( const char * name );

   /**
    * Stop timing and record the results.
    */
	static	void						stopProfile( void );

   /**
    * Reset the contents of the profiling system
    * 
    * @post Everything is reset except tree structure. Timing data is reset.
    */
	static	void						reset( void );

   /**
    * Increment the frame counter
    *
    * @post Frame counter incremented by one.
    */
	static	void						incrementFrameCounter( void );

   /**
    * @return Returns the number of frames since reset
    */
	static	int						getFrameCountSinceReset( void )	{ return mFrameCounter; }

   /**
    * @return Returns the elapsed time since last reset
    */
	static	float						getTimeSinceReset(void);

   /**
    * @return Returns a new Iterator that is set to the root.
    */
	static	ProfileIterator*	   getIterator( void ){ return new ProfileIterator(&mRoot); }

   /**
    * @post Iterator has been deleted
    */
	static	void						releaseIterator( ProfileIterator* iterator ) { delete iterator; }

private:
	static	ProfileNode			   mRoot;
	static	ProfileNode*			mCurrentNode;
	static	int						mFrameCounter;
	static	vpr::Interval*			mResetTime;
};


/*
 * ProfileSampleClass is a simple way to profile a function's scope
 * Use the PROFILE macro at the start of scope to time
 */
class	ProfileSample {
public:
	ProfileSample( const char * name )
	{ 
		ProfileManager::startProfile( name ); 
	}
	
	~ProfileSample( void )					
	{ 
		ProfileManager::stopProfile(); 
	}
};

#if defined(DEBUG) || defined(_DEBUG)
#define	VPR_PROFILE( name )			vpr::ProfileSample __profile( name )
#else
#define	VPR_PROFILE( name )
#endif

} // end vpr namespace

#endif
