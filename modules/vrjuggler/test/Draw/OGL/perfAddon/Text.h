/****************** <aqua heading BEGIN do not edit this line> *****************
 *
 * Copyright (c) 2004, 2005 Infiscape Corporation. All rights reserved.
 * (See accompanying file LICENSE.txt) 
 *
 ****************** <aqua heading END do not edit this line> ******************/

//C++ header - Blue Marble Viewer - Copyright (C) 2002 Don Burns 
#ifndef BMV_TEXT_H
#define BMV_TEXT_H

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif

#include <string>
#include <map>

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif


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
	void drawTextBlock(std::string text, const float xPosFactor=0.05f, const float yPosFactor=0.05f, 
                           float fontHeight=10.0f, 
	                   float winHeight = 1000.0f, float winWidth = 1.4f * 1000.0f);

	
	static Text *getSingleton();	

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
