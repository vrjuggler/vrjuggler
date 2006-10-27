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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _VPR_RETURN_STATUS_H_
#define _VPR_RETURN_STATUS_H_

#include <vpr/vprConfig.h>
#include <iostream>


namespace vpr
{
   /** \class ReturnStatus ReturnStatus.h vpr/Util/ReturnStatus.h
    *
    * Class used to represent the status of a function or method upon its
    * return.
    */
   class ReturnStatus
   {
   public:
      /** Possible status codes. */
      enum Code
      {
         Succeed,          /**< Successful status */
         Fail,             /**< Failure status */
         WouldBlock,       /**< Operation would block */
         Timeout,          /**< Timeout occurred */
         InProgress,       /**< Operation is still in progress */
         NotConnected,     /**< The given I/O device is closed */
         ConnectionAborted /**< Connection was aborted. */
      };

      /** Default constructor.  vpr::ReturnStatus::Succeed is default. */
      ReturnStatus() : mReturnStatus(vpr::ReturnStatus::Succeed)
      {
      }

      /**
       * Copies the given object into this object.
       *
       * @post This object is a copy of status.
       *
       * @param status The vpr::ReturnStatus object to be copied.
       */
      void copy(const ReturnStatus& status)
      {
         mReturnStatus = status.mReturnStatus;
      }

      /**
       * Sets the status code.
       *
       * @post Set status to \p c.
       */
      void setCode(const Code& c)
      {
         mReturnStatus = c;
      }

      /**
       * Gets status code.
       */
      const Code& code() const
      {
         return mReturnStatus;
      }

      /** Copy constructor. */
      ReturnStatus(const ReturnStatus& status)
      {
         this->copy(status);
      }

      /** vpr::ReturnStatus::Code constructor. */
      ReturnStatus(const Code& c)
      {
         this->setCode(c);
      }

      /**
       * Assignment operator for vpr::ReturnStatus objects.
       *
       * @post This object is a copy of status.
       */
      ReturnStatus& operator=(const ReturnStatus& status)
      {
         if((&status) != this)
         {
            this->copy( status );
         }
         return *this;
      }

      /**
       * Assignment operator for vpr::ReturnStatus::Code values.
       *
       * @post this->mReturnStatus == c.
       */
      ReturnStatus& operator=(const Code& c)
      {
         this->setCode(c);
         return *this;
      }

      /**
       * Equality operator for comparison with a vpr::ReturnStatus object.
       *
       * @return \c true if equal, \c false if not.
       */
      bool operator==(const ReturnStatus& status) const
      {
         return status.mReturnStatus == mReturnStatus;
      }

      /**
       * Equality operator for comparison with a vpr::ReturnStatus::Code value.
       *
       * @return \c true if equal, \c false if not.
       */
      bool operator==(const Code& c) const
      {
         return c == mReturnStatus;
      }

      /**
       * Inequality operator for comparison with a vpr::ReturnStatus object.
       *
       * @return \c false if equal, \c true if not.
       */
      bool operator!=(const ReturnStatus& status) const
      {
         return status.mReturnStatus != mReturnStatus;
      }

      /**
       * Inequality operator for comparison with a vpr::ReturnStatus::Code
       * values.
       *
       * @return \c false if equal, \c true if not.
       */
      bool operator!=(const Code& c) const
      {
         return c != mReturnStatus;
      }

   public:
      /** @name State test helpers */
      //@{
      bool success() const
      {
         return mReturnStatus == vpr::ReturnStatus::Succeed;
      }

      bool failure() const
      {
         return mReturnStatus == vpr::ReturnStatus::Fail;
      }

      bool wouldBlock() const
      {
         return mReturnStatus == vpr::ReturnStatus::WouldBlock;
      }

      bool inProgress() const
      {
         return mReturnStatus == vpr::ReturnStatus::InProgress;
      }

      bool timeout() const
      {
         return mReturnStatus == vpr::ReturnStatus::Timeout;
      }

      /**
       * @since 1.0.2
       */
      bool connectionAborted() const
      {
         return mReturnStatus == vpr::ReturnStatus::ConnectionAborted;
      }
      //@}

   private:
      ReturnStatus::Code mReturnStatus;
   };
} // namespace vpr

inline std::ostream& operator<<(std::ostream& out, const vpr::ReturnStatus& ret)
{
   switch(ret.code())
   {
   case vpr::ReturnStatus::Succeed:
      out << "Succeed";
      break;
   case vpr::ReturnStatus::Fail:             /**< Failure status */
      out << "Fail";
      break;
   case vpr::ReturnStatus::WouldBlock:       /**< Operation would block */
      out << "WouldBlock";
      break;
   case vpr::ReturnStatus::Timeout:          /**< Timeout occurred */
      out << "Timeout";
      break;
   case vpr::ReturnStatus::InProgress:       /**< Operation is still in progress */
      out << "InProgress";
      break;
   case vpr::ReturnStatus::NotConnected:
      out << "NotConnected";
      break;
   default:
      out << "Unknown";
      break;
   }

   return out;
}

#endif
