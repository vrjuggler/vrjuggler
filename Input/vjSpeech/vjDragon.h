/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_DRAGON_H_
#define _VJ_DRAGON_H_


class vjDragon : public vjSpeech
{
public:
	//: Construct using chunk
	vjDragon();

	//: Destructor
	~vjDragon();
	
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
	
	virtual char* getDeviceName() { return "vjDragon";}

       virtual int startSampling();
       virtual int stopSampling();
       virtual int sample();
       virtual void updateData();

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
	static void			controlLoop(vjDragon& currentInstance);
};

inline int vjDragon::startSampling()
{
    //todo: tell dragon to begin.
}

inline int vjDragon::stopSampling()
{
    //todo: tell dragon to stop sending data.
}

inline int vjDragon::sample()
{
    //todo: recieve all words that dragon has to offer this frame.
}

inline void vjDragon::updateData()
{
    //TODO: find out what the difference between this and Sample() is
}

inline void vjDragon::setMode( Mode mode )
{
	vjSpeech::setMode( mode );
	
	// TODO: tell dragon about the mode change.
}

//TODO: take a config chunck here.
inline void vjDragon::config()
{
	_filename = "somefile.vjs";
	_dragonIP = "129.186.232.70";
	_dragonPort = 5600;
	_user = "kevin";
}


#endif	/* _VJ_DRAGON_H_ */
