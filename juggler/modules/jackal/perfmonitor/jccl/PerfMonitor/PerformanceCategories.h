/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
#include <jccl/Plugins/PerformanceMonitor/TimeStamp.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/GUID.h>
#include <jccl/Util/Debug.h>
#include <vpr/Util/Singleton.h>

namespace jccl {

class LabeledPerfDataBuffer;

/** Singleton for category information of all PerfDataBuffers. 
 *  Since it already involves activation & a central registration
 *  of all buffers, this is an increasingly innacurately named
 *  class.  but there'll be time for renaming after it works.
 */
class PerformanceCategories {
private:

    struct CategoryInfo
    {
        CategoryInfo(std::string name, bool active)
            : mName(name), mActive(active)
        {;}
        
        std::string mName;
        bool        mActive;
    };

    typedef std::map<vpr::GUID, CategoryInfo > category_map_t;
    category_map_t mCategories; 
    
    bool mActive;
    

    std::vector<LabeledPerfDataBuffer*> mBuffers;
    vpr::Mutex mBuffersLock;

protected:

    PerformanceCategories ();
    
public:

    inline void activate () {
        mActive = true;
    }
    
    inline void deactivate () {
        mActive = false;
    }

    inline bool isActive () const {
        return mActive;
    }

    void addCategory (const vpr::GUID& catId, const std::string& name);

    void activateCategory (const std::string& catname);

    void deactivateCategory (const std::string& catname);


    /** Returns whether the given category is active.
     *  @return True iff the category is active.
     */
    bool isCategoryActive (const vpr::GUID& category);
    

    void addBuffer (LabeledPerfDataBuffer* buffer);

    void removeBuffer (LabeledPerfDataBuffer* buffer);

    void writeAllBuffers (std::ostream& out, const std::string& pad = "");


    vprSingletonHeader (PerformanceCategories);

private:
    // win32 dlls need these for no good reason.
    PerformanceCategories (const PerformanceCategories& o) {;}
    void operator= (const PerformanceCategories& o) {;}
    
};


#define jcclTIMESTAMP(cat, id)                                               \
    if (jccl::PerformanceCategories::instance()->isCategoryActive(cat)) {    \
        jccl::PerformanceMonitor::instance()->                               \
             getLabeledTSBuffer()->set (cat, id);                            \
    }                                                                        \
    else {                                                                   \
        ;                                                                    \
    }


/** Utility class for auto-registering a category.  Use the macros
 *  below instead of using this class directly.
 */
struct PerfCatRegistrator
{
    PerfCatRegistrator(const vpr::GUID& catGuid, const std::string& catName)
    {
      PerformanceCategories::instance()->addCategory(catGuid, catName);
    }
};


/** Helper macro for registering category
 * Defines a (file) unique variable that create a registrator 
 * in the file prive namespace
 * @param NAME  String name of the category (as used in the 
 *              environment variable).  Note: This is not in 
 *              string ("str") form
 * @param CAT   GUID id of the category
 * Use this in the .cpp files to register the actually token with 
 * jccl::
 */
#define jcclREGISTER_PERF_CATEGORY(CAT, NAME) jccl::PerfCatRegistrator NAME ## _registrator (CAT, #NAME);

}; // namespace jccl

// Perf measurement internal to jackal itself
const vpr::GUID jcclPERF_JACKAL ("29ecd55b-e68e-40ce-9db2-99e7682b36b4");
const vpr::GUID jcclPERF_ALL ("0b6b599c-f90c-43f6-8fbb-08454dd78872");

#endif
