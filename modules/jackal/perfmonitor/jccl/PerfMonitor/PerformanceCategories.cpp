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

#include <jccl/jcclConfig.h>
#include <vpr/vpr.h>
#include <jccl/PerfMonitor/PerformanceCategories.h>
#include <jccl/PerfMonitor/LabeledPerfDataBuffer.h>

namespace jccl
{

PerformanceCategories::PerformanceCategories ()
{
   mActive = false;
}



void PerformanceCategories::activateCategory (const std::string& catname)
{
   mCategoriesMutex.acquire();
   
   category_map_t::iterator cat = mCategories.find (catname);
   if (cat != mCategories.end())
   {
      *(cat->second->mActive) = true;
   }
   else
   {
      // the category hasn't been registered yet, but we need to do
      // something to store the fact that it's been activated,
      // so we create a temporary category object.
      PerformanceCategory* tempcat = new PerformanceCategory (vpr::GUID::NullGUID, catname);
      tempcat->mActive = new bool();
      *(tempcat->mActive) = true;
      mCategories[catname] = tempcat;
   }
   
   mCategoriesMutex.release();
}



void PerformanceCategories::deactivateCategory (const std::string& catname)
{
   mCategoriesMutex.acquire();
   
   category_map_t::iterator cat = mCategories.find (catname);
   if (cat != mCategories.end())
   {
      *(cat->second->mActive) = false;
   }

   mCategoriesMutex.release();
}



bool PerformanceCategories::isCategoryActive (PerformanceCategory& category)
{
   if (!mActive) 
   {
      return false;
   }

   if (!category.mRegistered)
   {
      // first time we've encountered this category. register it so that
      // we'll be able to lock/unlock it.
      mCategoriesMutex.acquire();
      if (!category.mRegistered)
      {
         // there might already be a temp category in place, if the
         // cat was configured before it was registered.  Grab the
         // configured activation state from the temp and then destroy
         // it.
         category_map_t::iterator cat = mCategories.find (category.mName);
         if (cat != mCategories.end())
         {
            category.mActive = cat->second->mActive;
            if (cat->second->mCategory == vpr::GUID::NullGUID)
            {
               delete (cat->second);
            }
         }
         else
         {
            category.mActive = new bool();
            *(category.mActive) = false;
         }

         // add in the real category
         mCategories[category.mName] = &category;
         category.mRegistered = true;
      }
      mCategoriesMutex.release();
   }

   vprASSERT (category.mActive != NULL);
   return *(category.mActive);
   
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


} // namespace jccl

jccl::PerformanceCategory jcclPERF_JACKAL("29ecd55b-e68e-40ce-9db2-99e7682b36b4", "PERF_JACKAL");
jccl::PerformanceCategory jcclPERF_ALL("0b6b599c-f90c-43f6-8fbb-08454dd78872", "PERF_ALL");
