/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifndef _TWEEK_BEAN_DELIVERY_SUBJECT_IMPL_H_
#define _TWEEK_BEAN_DELIVERY_SUBJECT_IMPL_H_

#include <tweek/tweekConfig.h>

#include <map>
#include <vector>
#include <string>

#include <vpr/vpr.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>

#include <tweek/CORBA/SubjectImpl.h>
#include <tweek/idl/BeanDeliverySubject.h>


namespace tweek
{

/** \class BeanDeliverySubjectImpl BeanDeliverySubjectImpl.h tweek/CORBA/BeanDeliverySubjectImpl.h
 *
 * Implementation of tweek::BeanDeliverySubject IDL interface.  This class
 * manages delivery of JavaBeans to clients via standard Java JAR files.  It
 * also allows the C++ code to make a single JavaBean active.  The idea is
 * that the client code can change itself appropriately based on whatever
 * JavaBean is currently active.
 *
 * @date September 17, 2002
 */
class BeanDeliverySubjectImpl : public POA_tweek::BeanDeliverySubject,
                                public tweek::SubjectImpl
{
public:
   /**
    * A handy container for all information relating to a JavaBean.  User code
    * must assign values to all data members except fileContents.  That step
    * can be handled internally by our containing class.
    */
   struct BeanData
   {
      /** A valid path to the JavaBean's JAR file on the local filesystem. */
      std::string filename;

      /**
       * The name of the class within the JAR file that can be used for
       * getting information about the contained JavaBean.
       */
      std::string introspectorClassName;

      /** The complete contents of the named JAR file. */
      std::vector<CORBA::Octet> fileContents;
   };

   /**
    * Default constructor.
    */
   BeanDeliverySubjectImpl() : mHasActiveBean(false)
   {
      ;
   }

   /**
    * Default destructor.
    */
   virtual ~BeanDeliverySubjectImpl()
   {
      ;
   }

   /**
    * Returns a list of all the Beans currently known to this subject.
    * This is part of the IDL interface, and if it is used by C++ code on
    * the server side, that code is responsible for freeing the memory
    * allocated herein.
    */
   virtual BeanNameList* getAllBeanNames() throw(CORBA::SystemException);

   /**
    * Returns all the relevant information for the named JavaBean, including
    * the complete JAR file.  This is part of the IDL interface, and if it is
    * used by C++ code on the server side, that code is responsible for
    * freeing the memory allocated herein.
    *
    * @post If the named JavaBean's JAR file has not been loaded into memory,
    *       it happens now.
    *
    * @note This method does some work to cache the JAR files for the various
    *       JavaBeans.  We do lazy loading of the JAR files to conserve
    *       memory, and once we have a JAR file loaded, we store it in case
    *       it is requested again.  This is done to reduce disk activity
    */
   virtual BeanInfo* getBean(const char* beanName) throw(CORBA::SystemException);

   /**
    * Determines if we have an "active" JavaBean or not.
    *
    * @return true is returned if a JavaBean is active; false otherwise.
    */
   virtual CORBA::Boolean hasActiveBean() throw(CORBA::SystemException)
   {
      vpr::Guard<vpr::Mutex> lock(mActiveBeanLock);
      return mHasActiveBean;
   }

   /**
    * Returns all the relevant information for the current "active" JavaBean.
    *
    * @pre A JavaBean has been marked as active.
    *
    * @see setActiveBean()
    */
   virtual BeanInfo* getActiveBeanInfo() throw(CORBA::SystemException);

   /**
    * Adds the given JavaBean information to the local database.  The JAR
    * file named in the given BeanData object is not loaded into memory.
    * This happens later.
    *
    * @param beanName The unique name of the JavaBean being added.
    * @param beanData Relevant information required for telling observers
    *                 about the named JavaBean.
    *
    * @note Typically, the fileContents member of beanData should be empty
    *       when it is passed in to this method.  The JAR file information
    *       will be loaded on demand by this class when appropriate.
    */
   void addBean(const std::string& beanName, const BeanData& beanData);

   /**
    * Marks the named JavaBean as "active".  Any observers are notified of
    * this change in state, and they can respond accordingly.
    *
    * @pre The named Bean is already known to this subject.
    *
    * @param beanName The name of the JavaBean to mark as active.
    */
   void setActiveBean(const std::string& beanName);

   /**
    * Deactivates the JavaBean that had previously been marked as active.
    * Any observers are notified of this change in state, and they can
    * respond accordingly.
    *
    * @pre A Bean has been marked as active.
    *
    * @see setActiveBean()
    */
   void removeActiveBean();

   tweek::BeanDeliverySubject_ptr _this()
   {
      return POA_tweek::BeanDeliverySubject::_this();
   }

private:
   // These two have to be here because Visual C++ will try to make them
   // exported public symbols.  This causes problems because copying
   // vpr::Mutex objects is not allowed.
   BeanDeliverySubjectImpl(const BeanDeliverySubjectImpl& subj);

   BeanDeliverySubjectImpl& operator=(const BeanDeliverySubjectImpl&)
   {
      return *this;
   }

   /** The collection of all JavaBeans about which we have been informed. */
   std::map<std::string, BeanData> mBeanCollection;
   vpr::Mutex mBeanCollectionLock;

   CORBA::Boolean mHasActiveBean;  /**< State of the "active" JavaBean */
   std::string    mActiveBean;     /**< The name of the "active" JavaBean */
   vpr::Mutex     mActiveBeanLock;
};

} // End of tweek namespace


#endif /* _TWEEK_BEAN_DELIVERY_SUBJECT_IMPL_H_ */
