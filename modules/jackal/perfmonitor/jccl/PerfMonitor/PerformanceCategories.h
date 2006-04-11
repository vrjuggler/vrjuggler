/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
#ifndef _JCCL_PERFORMANCE_CATEGORIES_H_
#define _JCCL_PERFORMANCE_CATEGORIES_H_

#include <jccl/jcclConfig.h>
#include <jccl/Util/Debug.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/GUID.h>
#include <vpr/Util/Singleton.h>

namespace jccl
{

class LabeledPerfDataBuffer;

class PerformanceCategory
{
public:
   
   PerformanceCategory (vpr::GUID category, const std::string& name)
      : mCategory (category), mName(name),
        mActive(NULL), mRegistered(false)
   {;}
   
   const vpr::GUID   mCategory;
   const std::string mName;
   /** mActive is a ptr so that we can share the activation state
    *  among all PerformanceCategory objects with the same name.
    *  In addition to saving us from one possible class of errors,
    *  this is helpful for the case where a performance category
    *  is configured before the PerformanceCategory object itself
    *  is registered.
    */
   bool        *mActive;
   bool        mRegistered;

private:
   
};


/** Singleton for category information of all PerfDataBuffers. 
 *  Since it already involves activation & a central registration
 *  of all buffers, this is an increasingly innacurately named
 *  class.  but there'll be time for renaming after it works.
 */
class JCCL_CLASS_API PerformanceCategories
{
private:
   
   typedef std::map<std::string, PerformanceCategory* > category_map_t;
   /** mCategories contains (at least) one PerformanceCategory object
    *  for each name that has been either configured or registered.
    *  Since PerformanceCategories with the same name share the
    *  activation boolean, this is sufficient for configuring all
    *  performance categories.
    */
   category_map_t mCategories; 
   vpr::Mutex mCategoriesMutex;
   
   bool mActive;
    

   std::vector<LabeledPerfDataBuffer*> mBuffers;
   vpr::Mutex mBuffersLock;

protected:

   PerformanceCategories ();
    
public:

   inline void activate ()
   {
      mActive = true;
   }

   
   inline void deactivate ()
   {
      mActive = false;
   }

   
   inline bool isActive () const
   {
      return mActive;
   }

   
   void activateCategory (const std::string& catname);

   void deactivateCategory (const std::string& catname);


   /** Returns whether the given category is active.
    *  Autoregisters the category if necessary.
    *  @return True iff the category is active.
    */
   bool isCategoryActive (PerformanceCategory& category);
    

   void addBuffer (LabeledPerfDataBuffer* buffer);

   void removeBuffer (LabeledPerfDataBuffer* buffer);

   LabeledPerfDataBuffer* getBufferNoLock (const std::string& n);

   void writeAllBuffers (std::ostream& out, const std::string& pad = "");


   vprSingletonHeader (PerformanceCategories);

private:
   // win32 dlls need these for no good reason.
   PerformanceCategories (const PerformanceCategories& o) {;}
   void operator= (const PerformanceCategories& o) {;}
    
};

#ifdef USE_JCCL_PERF
#define jcclTIMESTAMP(cat, id)                                               \
    if (jccl::PerformanceCategories::instance()->isCategoryActive(cat)) {    \
        jccl::PerformanceMonitor::instance()->                               \
             getLabeledTSBuffer()->set (cat, id);                            \
    }                                                                        \
    else {                                                                   \
        ;                                                                    \
    }
#else
#define jcclTIMESTAMP(cat, id) ((void)0)
#endif


} // namespace jccl


// Perf measurement internal to JCCL itself
extern JCCL_DATA_API(jccl::PerformanceCategory) jcclPERF_JACKAL;
extern JCCL_DATA_API(jccl::PerformanceCategory) jcclPERF_ALL;



#endif
