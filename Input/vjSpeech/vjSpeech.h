/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_SPEECH_H_
#define _VJ_SPEECH_H_


class vjSpeech
{
public:
	//: Construct using chunk
	vjSpeech();

	//: Destructor
	~vjSpeech();
	
	virtual std::string		getWord();
	virtual int			getId();
	virtual void			setMode( Mode mode );
	virtual std::list<asdfjlk>&	getList();
	virtual void			config();

	enum Mode
	{
		DICTATE,COMMAND,ON,OFF
	};


	
	class asdfjlk
	{
		std::string word;
		int id;
	};
	
	virtual char* getDeviceName() { return "vjSpeech";}

       virtual int startSampling() = 0;
       virtual int stopSampling() = 0;
       virtual int sample() = 0;
       virtual void updateData () = 0;
    
protected:
	std::list<asdfjlk>	_wordQueue;
	Mode				_currentMode;

	// Config chunck sets these.
	std::string			_filename;
	std::string			_dragonIP;
	int					_dragonPort;
	std::string			_user;

protected:
	void				startThread();
	static void			controlLoop(vjSpeech& currentInstance);
};


inline std::string vjSpeech::getWord()
{
	std::string word = "";

	if (_wordQueue.size > 0)
	{
		word = (*_wordQueue.begin()).word;
		_wordQueue.pop_front();
	}

	return word;
}

inline int vjSpeech::getId()
{
	int id = -1;

	if (_wordQueue.size > 0)
	{
		id = (*_wordQueue.begin()).id;
		_wordQueue.pop_front();
	}

	return id;
}

inline void vjSpeech::setMode( Mode mode )
{
	_mode = mode;
	// TODO: tell dragon about the mode change.
}

inline std::list<asdfjlk>& vjSpeech::getList()
{
	return _wordQueue;
}

//TODO: take a config chunck here.
inline void vjSpeech::config()
{
	_filename = "somefile.vjs";
	_dragonIP = "129.186.232.70";
	_dragonPort = 5600;
	_user = "kevin";
}


#endif	/* _VJ_SPEECH_H_ */
