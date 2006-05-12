/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <tweek/tweekConfig.h>

#include <fstream>

#include <vpr/vpr.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>

#include <tweek/Util/Debug.h>
#include <tweek/CORBA/BeanDeliverySubjectImpl.h>


namespace tweek
{

BeanNameList* BeanDeliverySubjectImpl::getAllBeanNames()
   throw(CORBA::SystemException)
{
   BeanNameList_var bean_names = new BeanNameList();

   // Lock down the Bean collection before using it.
   {
      vpr::Guard<vpr::Mutex> lock(mBeanCollectionLock);

      bean_names->length(mBeanCollection.size());

      std::map<std::string, BeanData>::iterator i;
      int j;

      for ( i = mBeanCollection.begin(), j = 0; i != mBeanCollection.end(); ++i, ++j )
      {
         bean_names[j] = CORBA::string_dup((*i).first.c_str());
      }
   }

   return bean_names._retn();
}

BeanInfo* BeanDeliverySubjectImpl::getBean(const char* beanName)
   throw(CORBA::SystemException)
{
   std::string bean_name_str(beanName);
   vprASSERT(mBeanCollection.count(bean_name_str) > 0 && "Unknown Bean requested");

   // Lock down the Bean collection before accessing it.  We basically have to
   // hold the lock for the duration of this method because we get back a
   // reference to data held by the collection (see bean_data below).
   vpr::Guard<vpr::Mutex> lock(mBeanCollectionLock);

   // It's important that this be a reference to avoid copying around the
   // (possibly large) fileContents vector.
   BeanData& bean_data = mBeanCollection[bean_name_str];

   // If the file's byte cache is empty, read everything into bean_data's
   // fileContents data member.
   if ( bean_data.fileContents.empty() )
   {
      vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
         << "Reading JAR file from disk" << std::endl << vprDEBUG_FLUSH;

      std::ifstream bean_file;
      bean_file.open(bean_data.filename.c_str(),
                     std::ios::binary | std::ios::in);

      if ( bean_file.good() )
      {
         // First, we have to size the sequence so that it can hold all the bytes
         // read from the file.
         bean_file.seekg(0, std::ios::end);
         std::streampos file_size = bean_file.tellg();

         // Allocate the space for reading in the file contents.
         bean_data.fileContents.resize(file_size);

         vprDEBUG(tweekDBG_CORBA, vprDBG_VERB_LVL)
            << "JAR file buffer size: " << file_size << std::endl
            << vprDEBUG_FLUSH;
         vprASSERT(bean_data.fileContents.size() == (unsigned long) file_size && "Buffer resize failed");

         bean_file.seekg(0, std::ios::beg);
         vprASSERT((unsigned long) bean_file.tellg() == 0 && "Failed to restore stream pointer to beginning of file");

         // Read in the whole JAR file in one big swoop.
         bean_file.read((char*) &bean_data.fileContents[0],
                        bean_data.fileContents.size());
         bean_file.close();
      }
      else
      {
         vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "ERROR") << ": Could not open file '"
            << mBeanCollection[bean_name_str].filename
            << "' for reading\n" << vprDEBUG_FLUSH;
      }
   }

   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
      << "Using cached copy of JAR file" << std::endl << vprDEBUG_FLUSH;

   // Allocate memory for the object to be returned.
   BeanInfo* bean_info = new BeanInfo();

   // Size bean_info->jarFile so that it can hold the complete JAR file.
   bean_info->jarFile.length(bean_data.fileContents.size());
   vprASSERT(bean_info->jarFile.length() == bean_data.fileContents.size() && "Buffer resize failed");

   // Read the cached JAR file into bean_info->jarFile.
   std::vector<CORBA::Octet>::iterator i = bean_data.fileContents.begin();
   unsigned long j(0);

   for ( ; i != bean_data.fileContents.end(); ++i, ++j )
   {
      bean_info->jarFile[j] = *i;
   }

   // XXX: It's a little weird to send the beanName argument back to the
   // caller...
   bean_info->name = CORBA::string_dup(beanName);
   bean_info->introspectorClassName =
      CORBA::string_dup(bean_data.introspectorClassName.c_str());

   return bean_info;
}

BeanInfo* BeanDeliverySubjectImpl::getActiveBeanInfo()
   throw(CORBA::SystemException)
{
   BeanInfo* bean_info = new BeanInfo();

   // Lock down the data relating to the active Bean.  We basically have to
   // hold the lock for the duration of this method because we get back a
   // reference to data held by the collection (see bean_data below).
   vpr::Guard<vpr::Mutex> lock(mActiveBeanLock);

   // It's important that this be a reference to avoid copying around the
   // (possibly large) fileContents vector unnecessarily.
   BeanData& bean_data = mBeanCollection[mActiveBean];

   bean_info->name                  = CORBA::string_dup(bean_data.filename.c_str());
   bean_info->introspectorClassName = CORBA::string_dup(bean_data.introspectorClassName.c_str());

   // Size bean_info->jarFile so that it can hold the complete JAR file.
   bean_info->jarFile.length(bean_data.fileContents.size());
   vprASSERT(bean_info->jarFile.length() == bean_data.fileContents.size() && "Buffer resize failed");

   // Read the cached JAR file into bean_info->jarFile.
   std::vector<CORBA::Octet>::iterator i = bean_data.fileContents.begin();
   unsigned long j(0);

   for ( ; i != bean_data.fileContents.end(); ++i, ++j )
   {
      bean_info->jarFile[j] = *i;
   }

   return bean_info;
}

void BeanDeliverySubjectImpl::addBean(const std::string& beanName,
                                      const BeanData& beanData)
{
   mBeanCollectionLock.acquire();
   {
      mBeanCollection[beanName] = beanData;
   }
   mBeanCollectionLock.release();

   tweek::SubjectImpl::notify();
}

void BeanDeliverySubjectImpl::setActiveBean(const std::string& beanName)
{
#ifdef TWEEK_DEBUG
   mBeanCollectionLock.acquire();
   {
      vprASSERT(mBeanCollection.count(beanName) > 0 && "Tried to make unknown Bean active");
   }
   mBeanCollectionLock.release();
#endif

   mActiveBeanLock.acquire();
   {
      mHasActiveBean = true;
      mActiveBean    = beanName;
   }
   mActiveBeanLock.release();

   tweek::SubjectImpl::notify();
}

void BeanDeliverySubjectImpl::removeActiveBean()
{
   mActiveBeanLock.acquire();
   {
      vprASSERT(mHasActiveBean && "No active Bean to remove");
      mHasActiveBean = false;
      mActiveBean    = std::string("");
   }
   mActiveBeanLock.release();

   tweek::SubjectImpl::notify();
}

// ============================================================================
// Private methods follow.
// ============================================================================

BeanDeliverySubjectImpl::BeanDeliverySubjectImpl(const BeanDeliverySubjectImpl& subj)
   :
#if defined(TWEEK_USE_OMNIORB)
     omniServant(subj)
   , tweek::_impl_Subject(subj)
   , tweek::_impl_BeanDeliverySubject(subj)
   ,
#elif defined(TWEEK_USE_TAO)
     TAO_Abstract_ServantBase(subj)
   ,
#endif
     PortableServer::ServantBase(subj)
   , POA_tweek::Subject(subj)
   , POA_tweek::BeanDeliverySubject(subj)
   , tweek::SubjectImpl(subj)
{
   /* Do nothing. */ ;
}

}  // End of tweek namespace
