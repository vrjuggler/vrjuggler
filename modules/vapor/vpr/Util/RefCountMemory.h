/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#ifndef VPR_REFERENCE_COUNTABLE_MEMORY
#define VPR_REFERENCE_COUNTABLE_MEMORY
#include <assert.h>


namespace vpr
{
   /**
    * reference countable memory. (no, not radio controlled, sorry)
    *
    * To use reference counting on your objects, inherit from this class.
    *
    * NOTES:
    * - you MUST call unrefDelete or checkDelete to delete, do not call 
    *   operator::delete() yourself!!!!!
    * - on creation, refcount == 0, so if you intend to own the memory 
    *   created, you should call ref() on the object
    *
    * EXAMPLE USAGE:
    *   Object obj;
    *   obj.ref();  // count == 1;
    *   obj.ref();  // count == 2;
    *   int count = obj.getRef();
    *   obj.unrefDelete(); // count == 1, memory is not deleted
    *   obj.unrefDelete(); // count == 0, memory is deleted
    * 
    * class Object : public vpr::RefCountMemory
    *
    * @author Kevin Meinert
    */
   class RefCountMemory
   {
   public:
      enum States
      {
         BAD = -69,
      };
      /**
       * Default constructor.
       * On creation, refcount == 0, so if you intend to own the memory 
       * created, you should ref it
       */
      RefCountMemory();

      /**
       * On creation, refcount == 0, so if you intend to own the memory 
       * created, you should ref it
       */
      RefCountMemory( const RefCountMemory& r ) : ___mNumTimesReferenced( 0 )
       {
          //dont copy ref number.
       }

   protected:
       /**
        * Destructor.
        * You MUST call checkDelete to delete, do not call delete!!!!!
        */
       virtual ~RefCountMemory()
       {
          assert( ___mNumTimesReferenced != BAD && ___mNumTimesReferenced >= 0 && "this data has been dereferenced more times than referenced, someone is probably holding on to the data after they called deref(), or someone called deref too many times. also, make sure you use checkDelete, or unrefDelete." );
          ___mNumTimesReferenced = BAD;
       }

   public:
       RefCountMemory& operator=( const RefCountMemory& r )
       {
          //dont copy ref number.
          return *this;
       }

      /**
       * Increases the reference count by 1, returns the new refcount
       */
      const int& ref();

      /**
       * Decreases the reference count by 1, then calls checkDelete.  This is
       * equivalent to calling RefCountMemory::unref followed by
       * RefCountMemory::checkDelete
       *
       * @return <code>true</code> if deleted; <code>false</code> if not.
       */
      bool unrefDelete();

   public:
      /// Decreases the reference count by 1, return the new refcount.
      const int& unref();

      /// Returns the reference count.
      const int& getRef() const;

      /// Decreases the reference count by 1, return the new refcount.
      const int& unrefGetRef();

      /**
       * If refcount <= 0, then delete the memory.
       * If the object has any refcountable children, then they will
       * have RefCountMemory::unrefDelete() called on them.
       *
       * @return true if deleted, false if not
       */
      virtual bool checkDelete();

   private:
      mutable int ___mNumTimesReferenced;
   };

   /// Default constructor.
   inline RefCountMemory::RefCountMemory() : ___mNumTimesReferenced( 0 )
   {
   }

   /// Gets the reference count.
   inline const int& RefCountMemory::getRef() const
   {
      assert( ___mNumTimesReferenced != BAD && "this data has been deleted, someone is probably holding on to the data after they called deref()" );
      return ___mNumTimesReferenced;
   }
   
   /// Increases the reference count.
   inline const int& RefCountMemory::ref()
   {
      assert( ___mNumTimesReferenced != BAD && "this data has been deleted, someone is probably holding on to the data after they called deref()" );
      ++___mNumTimesReferenced;
      return this->getRef();
   }

   /// Decreases the reference count.
   inline const int& RefCountMemory::unref()
   {
      assert( ___mNumTimesReferenced != BAD && ___mNumTimesReferenced >= 0 && "this data has been dereferenced more times than referenced, someone is probably holding on to the data after they called deref(), or someone called deref too many times. also, make sure you use checkDelete, or unrefDelete." );
      --___mNumTimesReferenced;
      return this->getRef();
   }

   /**
    * If refcount <= 0, then delete the memory.
    * If the object has any refcountable children, then they will
    * have RefCountMemory::unrefDelete() called on them.
    *
    * @return true if deleted, false if not
    */
   inline bool RefCountMemory::checkDelete()
   {
      assert( ___mNumTimesReferenced != BAD && ___mNumTimesReferenced >= 0 && "this data has been dereferenced more times than referenced, someone is probably holding on to the data after they called deref(), or someone called deref too many times. also, make sure you use checkDelete, or unrefDelete." );
      if (___mNumTimesReferenced <= 0)
      {
         delete this;
         return true;
      }
      else
      {
         return false;
      }   
   }
   
   /**
    * Decreases the reference count by 1, then call checkDelete.  This is
    * equivalent to calling RefCountMemory::unref followed by
    * RefCountMemory::checkDelete.
    *
    * @return true if deleted, false if not
    */
   inline bool RefCountMemory::unrefDelete()
   {
      this->unref();
      return this->checkDelete();
   }

   /// Decreases the reference count by 1, return the new refcount.
   inline const int& RefCountMemory::unrefGetRef()
   {
      this->unref();
      return this->getRef();
   }
   
}; //namespace vpr

#endif
