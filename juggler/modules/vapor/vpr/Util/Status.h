#ifndef STATUS_VPR_ERROR_CODE
#define STATUS_VPR_ERROR_CODE

#include <vpr/vprConfig.h>


namespace vpr
{
   /**
    * Class used to represent the status of a function or method upon its
    * return.
    *
    * @author Kevin Meinert
    */
   class VPR_CLASS_API Status
   {
   public:
      /// Possible status codes
      enum Code
      {
         Success,          /**< Successful status */
         Failure,          /**< Failure status */
         WouldBlock,       /**< Operation would block */
         Timeout,          /**< Timeout occurred */
         InProgress        /**< Operation is still in progress */
      };

      /// Default constructor.  vpr::Status:;Success is default
      Status() : mStatus( Success )
      {
      }

      /**
       * Copies the given object into this object.
       * RESULT: this = status
       *
       * @param status The <code>vpr::Status</code> object to be copied.
       */
      void copy( const Status& status )
      {
         mStatus = status.mStatus;
      }

      /**
       * Sets the status code.
       * RESULT: set status to code
       */
      void setCode( const Code& code )
      {
         mStatus = code;
      }

      /**
       * Gets status code
       */
      const Code& code() const
      {
         return mStatus;
      }

      /// copy constructor
      Status( const Status& status )
      {
         this->copy( status );
      }

      /// Status::Code constructor
      Status( const Code& code )
      {
         this->setCode( code );
      }

      //: operator=
      // RESULT: this = status
      Status& operator=( const Status& status )
      {
         this->copy( status );
         return *this;
      }

      //: operator=
      // RESULT: this = status
      Status& operator=( const Code& code )
      {
         this->setCode( code );
         return *this;
      }

      //: operator==
      // RESULT: return true if equal, false if not
      bool operator==( const Status& status ) const
      {
         return status.mStatus == mStatus;
      }

      //: operator==
      // RESULT: return true if equal, false if not
      bool operator==( const Code& code ) const
      {
         return code == mStatus;
      }

      //: operator!=
      // RESULT: return false if equal, true if not
      bool operator!=( const Status& status ) const
      {
         return status.mStatus != mStatus;
      }

      //: operator!=
      // RESULT: return false if equal, true if not
      bool operator!=( const Code& code ) const
      {
         return code != mStatus;
      }

   public:
      bool success() const { return mStatus == Success; }
      bool failure() const { return mStatus == Failure; }
      bool wouldBlock() const { return mStatus == WouldBlock; }
      bool inProgress() const { return mStatus == InProgress; }
      bool timeout() const { return mStatus == Timeout; }

   private:
      Status::Code mStatus;
   };
}; // namespace vpr

#endif
