
//: handle to a sound
//  This class may be used in future versions of juggler, for now ignore it.
//!PUBLIC_API:
class vjSoundHandle
{
public:
   // creates stale by default...
	vjSoundHandle() : mIdentifier( 0 ), mStale( true ), mName( "no name" )
	{
	}
   
   const std::string& name() const { return mName; }
   const int&         id() const { return mIdentifier; }
   const bool&        isStale() const { return mStale; }
   
protected: 
   void               setName( const std::string& name ) { mName = name; }
	void               setId( const int& id ) { mIdentifier = id; }
   void               setStale( const bool& stale ) { mStale = stale; }
   
   friend class vjSoundManager;
   
private:
   bool mStale;
	int mIdentifier;
   std::string mName;
};
