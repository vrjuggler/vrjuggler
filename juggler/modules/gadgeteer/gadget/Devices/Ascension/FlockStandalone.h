/////////////////////////////////////////////////////////////////////////
// aFlock.h
//
// Flock of birds tracking class
//
////////////////////////////////////////////////////////////////////////
#ifndef _ASCENSION_FLOCKOFBIRD_H_
#define _ASCENSION_FLOCKOFBIRD_H_

#define POSITION_RANGE 12.0f
#define ANGLE_RANGE   180.0f
#define MAX_SENSORS    128

typedef struct {
        float xmin, ymin, zmin;
        float xmax, ymax, zmax;
        float xloc[15][10][15],yloc[15][10][15],zloc[15][10][15];
} CalStruct;

enum BIRD_HEMI  {FRONT_HEM, AFT_HEM, UPPER_HEM, LOWER_HEM, LEFT_HEM, RIGHT_HEM};
enum BIRD_FILT  {AC_NARROW, AC_WIDE, DC_FILTER};

//----------------------------------------------------------------------------
//: class for running a Flock of Birds.
//
//  aFlock is a positional device driver for the Flock of Birds, the config
//  chunk in the constructor should set up all the settings, for these to be
//  changed the Flock has to be deleted and a new instance created with an
//  updated configchunk.
//  <br>
//  Note: Some functions still remain for changing the options of
//    the flock when its not in Sampling mode, but in order to stay
//    consistent with the vjInput/vjPosition functionality these
//    are only left for building apps without vjConfigChunks
//---------------------------------------------------------------------------
//!PUBLIC_API
class aFlock
{
public:
	//: Configure Constructor
	// Give:
	//   port - such as "/dev/ttyd3"
	//   baud - such as 38400, 19200, 9600, 14400, etc...
	//   sync - sync type.
	//   block - blocking
	//   numBrds - number of birds in flock,
	//   transmit - transmitter unit number,
	//   hemi - hemisphere to track from,
	//   filt - filtering type,
	//   report -
	//   calfile - a calibration file, if "", then use none.
	// 
	// Result: configures internal data members, 
	//         doesn't actually talk to the FOB yet.
	aFlock(const char* const port = "/dev/ttyd3", 
		const int& baud = 38400, 
		const int& sync = 1, 
		const int& block = 0, 
		const int& numBrds = 3, 
		const int& transmit = 3, 
		const BIRD_HEMI& hemi = LOWER_HEM, 
		const BIRD_FILT& filt = AC_NARROW, 
		const char& report = 'R', 
		const char* const calfile = "");
	~aFlock();

	
	int	    StartSampling();
	int	    StopSampling();
	int	    Sample();
	void	    UpdateData();
	
	vjMatrix&   GetMatrix( int dev = 0 );
		
	void	    SetPort( const char* const serialPort );
	const char* const GetPort();
	
	void	    SetBaudRate( const int& baud );
	inline int  GetBaudRate() { return _baud;}
   
	void	    SetTransmitter( const int& Transmit );
	inline int  GetTransmitter() { return _xmitterUnitNumber; }

	void	    SetNumBirds( const int& n );
	inline int  GetNumBirds() { return _numBirds; }

	void	    SetSync( const int& sync );
	inline int  GetSync() {return _syncStyle; }

	void	    SetBlocking( const int& blVal );
	inline int  GetBlocking() { return _blocking; }

	void	    SetFilters( const BIRD_FILT& f );
	inline int  GetFilters() { return _filter; }

	void	    SetHemisphere( const BIRD_HEMI& h );
	inline int  GetHemisphere() {return _hemisphere; }

	void	    SetReportRate( const char& rRate );
        inline char GetReportRate() {return _reportRate; }

private:
	void        Position_Correct( float& x, float& y, float& z );
	void        InitCorrectionTable( const char* const );
	CalStruct   caltable;
	char        _reportRate;
	BIRD_HEMI   _hemisphere;
	BIRD_FILT   _filter;
	
	std::string _port;
	int	    _baud;
	int	    _syncStyle;
	int	    _blocking;
	int	    _numBirds;
	int	    _xmitterUnitNumber;
	std::string _calibrationFileName;
	bool	    _usingCorrectionTable;
	
	vjMatrix* theData;
	
	bool _active;
	
// bird declarations.
private:	
	int   getReading(int i, vjMatrix* data, int port);
	float rawToFloat(char& r1, char& r2);
	void  pickBird(int sensor, int port_id);
	int   open_port(char* serialPort, int baud);
	void  set_blocking(int port, int blocking);
	void  set_sync(int port, int sync);
	void  set_hemisphere(int port, BIRD_HEMI hem, int transmitter, int numbirds);
	void  set_rep_and_stream(int port, char repRate);
	void  set_pos_angles(int port, int transmitter, int numbirds);
	void  set_filter(int port, BIRD_FILT filter);
	void  set_transmitter(int port, int transmitter);
	void  set_autoconfig(int port, int numbirds);
	void  set_group(int port);
};




#endif

	
