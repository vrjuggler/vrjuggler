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

#ifndef RETURN_STATUS_VPR_ERROR_CODE
#define RETURN_STATUS_VPR_ERROR_CODE

#include <vpr/vprConfig.h>


namespace vpr
{
   /**
    * Class used to represent the status of a function or method upon its
    * return.
    *
    * @author Kevin Meinert
    */
   class VPR_CLASS_API ReturnStatus
   {
   public:
      /// Possible status codes
      enum Code
      {
         Succeed,          /**< Succeedful status */
         Fail,          /**< Fail status */
         WouldBlock,       /**< Operation would block */
         Timeout,          /**< Timeout occurred */
         InProgress        /**< Operation is still in progress */
      };

      /// Default constructor.  vpr::ReturnStatus:;Succeed is default
      ReturnStatus() : mReturnStatus( vpr::ReturnStatus::Succeed )
      {
      }

      /**
       * Copies the given object into this object.
       * RESULT: this = status
       *
       * @param status The <code>vpr::ReturnStatus</code> object to be copied.
       */
      void copy( const ReturnStatus& status )
      {
         mReturnStatus = status.mReturnStatus;
      }

      /**
       * Sets the status code.
       * RESULT: set status to code
       */
      void setCode( const Code& code )
      {
         mReturnStatus = code;
      }

      /**
       * Gets status code
       */
      const Code& code() const
      {
         return mReturnStatus;
      }

      /// copy constructor
      ReturnStatus( const ReturnStatus& status )
      {
         this->copy( status );
      }

      /// ReturnStatus::Code constructor
      ReturnStatus( const Code& code )
      {
         this->setCode( code );
      }

      //: operator=
      // RESULT: this = status
      ReturnStatus& operator=( const ReturnStatus& status )
      {
         if((&status) != this)
         {
            this->copy( status );
         }
         return *this;
      }

      //: operator=
      // RESULT: this = status
      ReturnStatus& operator=( const Code& code )
      {
         this->setCode( code );
         return *this;
      }

      //: operator==
      // RESULT: return true if equal, false if not
      bool operator==( const ReturnStatus& status ) const
      {
         return status.mReturnStatus == mReturnStatus;
      }

      //: operator==
      // RESULT: return true if equal, false if not
      bool operator==( const Code& code ) const
      {
         return code == mReturnStatus;
      }

      //: operator!=
      // RESULT: return false if equal, true if not
      bool operator!=( const ReturnStatus& status ) const
      {
         return status.mReturnStatus != mReturnStatus;
      }

      //: operator!=
      // RESULT: return false if equal, true if not
      bool operator!=( const Code& code ) const
      {
         return code != mReturnStatus;
      }

   public:
      bool success() const { return mReturnStatus == vpr::ReturnStatus::Succeed; }
      bool failure() const { return mReturnStatus == vpr::ReturnStatus::Fail; }
      bool wouldBlock() const { return mReturnStatus == vpr::ReturnStatus::WouldBlock; }
      bool inProgress() const { return mReturnStatus == vpr::ReturnStatus::InProgress; }
      bool timeout() const { return mReturnStatus == vpr::ReturnStatus::Timeout; }

   private:
      ReturnStatus::Code mReturnStatus;
   };
}; // namespace vpr

#endif
