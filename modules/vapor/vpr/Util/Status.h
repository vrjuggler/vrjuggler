#ifndef STATUS_VPR_ERROR_CODE
#define STATUS_VPR_ERROR_CODE

namespace vpr
{
   class Status
   {
   public:
      enum Code
      {
         Success, Failure, WouldBlock
      };
         
      //: default constructor.  Success is default
      Status() : mStatus( Success )
      {
      }

      //: copy()
      // RESULT: this = status
      void copy( const Status& status )
      {
         mStatus = status.mStatus;
      }
      
      //: set the status code
      // RESULT: set status to code
      void setCode( const Code& code )
      {
         mStatus = code;
      }
      
      //: get status code
      // RESULT: set status to code
      const Code& code() const
      {
         return mStatus;
      }
      
      //: copy constructor
      Status( const Status& status )
      {
         this->copy( status );
      }
      
      //: Status::Code constructor
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
      bool success() { return mStatus == Success; }
      bool failure() { return mStatus == Failure; }
      bool wouldBlock() { return mStatus == WouldBlock; }
      
   private:
      Status::Code mStatus;
   };
}; // namespace vpr

#endif
