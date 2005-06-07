/****************** <aqua heading BEGIN do not edit this line> *****************
 *
 * Copyright (c) 2004, 2005 Infiscape Corporation. All rights reserved.
 * (See accompanying file LICENSE.txt) 
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <aqua heading END do not edit this line> ******************/
//C++ header - Blue Marble Viewer - Copyright (C) 2002 Don Burns 
#ifndef BMV_TEXT_H
#define BMV_TEXT_H

#ifdef WIN32

#include <windows.h>

#endif

#include <string>
#include <map>

#include <GL/gl.h>


class Text {
    public:
	enum Font {
	    BitmapFont,
	    FilledFont,
	    OutlineFont,
	    StrokeFont,
		LastFont
	};	
	Text();
	void drawString( Font, std::string );
	static Text *getSingleton();
	void *operator new( size_t size );

    protected:

	static Text *theText;

    private :

	GLuint fontmap[LastFont];
	GLuint loadFont(Font font);

	bool createStrokeFont( GLuint fontBase );
	bool createOutlineFont( GLuint fontBase );
	bool createFilledFont( GLuint fontBase );
	bool createBitmapFont( GLuint fontBase );

};

#endif
