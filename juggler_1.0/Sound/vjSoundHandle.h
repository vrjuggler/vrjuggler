

class vjSoundHandle
{
public:
	vjSoundHandle() : mIdentifier( 0 )
	{
	}

	void setId( const int& id )
	{
		mIdentifier = id;
	}

	int mIdentifier;
};
