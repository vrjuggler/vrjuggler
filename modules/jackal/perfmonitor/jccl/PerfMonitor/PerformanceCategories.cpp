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

#include <jccl/jcclConfig.h>
#include <vpr/vpr.h>
#include <jccl/PerfMonitor/PerformanceCategories.h>
#include <jccl/PerfMonitor/LabeledPerfDataBuffer.h>

namespace jccl
{

PerformanceCategories::PerformanceCategories ()
{
   mActive = false;

   addCategory (jcclPERF_ALL, "PERF_ALL");
   activateCategory ("PERF_ALL");
}

void PerformanceCategories::addCategory (const vpr::GUID& catId, const std::string& name)
{
   // can't use vprDEBUG here, because this may get called before
   // the debug categories are fully initialized.
//          std::cout << "Adding category named '" << name << "' at " << &catId 
//              << ".\n";
   mCategories.insert( std::pair<const vpr::GUID*,CategoryInfo>(&catId, CategoryInfo(name, false)));
   //updateAllowedCategories();   
}



void PerformanceCategories::activateCategory (const std::string& catname)
{
   category_map_t::iterator cat = mCategories.begin();
   while (cat != mCategories.end())
   {
      if (cat->second.mName == catname)
      {
         cat->second.mActive = true;
         return;
      }
      cat++;
   }
}



void PerformanceCategories::deactivateCategory (const std::string& catname)
{
   category_map_t::iterator cat = mCategories.begin();
   while (cat != mCategories.end())
   {
      if (cat->second.mName == catname)
      {
         cat->second.mActive = false;
         return;
      }
      cat++;
   }
}



bool PerformanceCategories::isCategoryActive (const vpr::GUID& category)
{
   if (!mActive) 
   {
      return false;
   }
   
   category_map_t::iterator cat = mCategories.find(&category);
   vprASSERT(cat != mCategories.end());  // cat is valid
   return (*cat).second.mActive;   
}



void PerformanceCategories::addBuffer (LabeledPerfDataBuffer* buffer)
{
   mBuffersLock.acquire();
   // need to make sure the buffer gets a unique name
   char s[64];
#ifdef HAVE_SNPRINTF
   snprintf (s, 64, "%s [%d]", buffer->getName().c_str(), mBuffers.size());
#else
   sprintf(s, "%s [%d]", buffer->getName().c_str(), mBuffers.size());
#endif
   std::string name(s);
   buffer->setName (name);

//  	std::string base_name = buffer->getName();
//  	if (getBufferNoLock (base_name)) {
//  	    std::string name;
//  	    int i = 1;
//  	    char s[32];
//  	    do {
//  		sprintf (s, "<%d>", i);
//  		name = base_name + s;
//  		i++;
//  	    } while (getBufferNoLock(name));
//  	    buffer->setName (name);
//  	}

   mBuffers.push_back (buffer);
   mBuffersLock.release();
}



void PerformanceCategories::removeBuffer (LabeledPerfDataBuffer* buffer)
{
   mBuffersLock.acquire();
   std::vector<LabeledPerfDataBuffer*>::iterator i = mBuffers.begin();
   while (i != mBuffers.end())
   {
      if (*i == buffer)
      {
         mBuffers.erase(i);
         break;
      }
   }
   mBuffersLock.release();
}


LabeledPerfDataBuffer* PerformanceCategories::getBufferNoLock (const std::string& n)
{
   std::vector<LabeledPerfDataBuffer*>::iterator i = mBuffers.begin();
   while (i != mBuffers.end())
   {
      if ((*i)->getName() == n)
         return *i;
   }
   return NULL;
}


void PerformanceCategories::writeAllBuffers (std::ostream& out, 
                                             const std::string& pad /*=""*/)
{
   out << pad << "<jcclstream>\n";
   mBuffersLock.acquire();
   for (unsigned int i = 0; i < mBuffers.size(); i++)
   {
      mBuffers[i]->write (out, pad);
   }
   mBuffersLock.release();
   out << pad << "</jcclstream>\n";
}


vprSingletonImp (PerformanceCategories);

jcclREGISTER_PERF_CATEGORY(jcclPERF_JACKAL, PERF_JACKAL);


} // namespace jccl
