/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _SCENE_H_
#define _SCENE_H_


class Scene
{
public:
    Scene();
    virtual ~Scene();

public:
    void init();
    
    //: These functions call the displaylists 
    //  created by init()
    void drawFloor();
    void drawSphere( const float& width, 
		    const float& height,
		    const float& depth, 
		    const bool& highlighted );
    void drawCone( const float& width, 
		    const float& height,
		    const float& depth, 
		    const bool& highlighted );
    void drawCube( const float& _width, 
		    const float& _height,
		    const float& _depth, 
		    const bool& highlighted );
    void drawTable();  
    void renderWoodTexture();
    void renderRainbowTexture();
    
    
//:  Scene components: table, floor, and some objects.    
private:
    //: these functions render the geometry using opengl functions
    void _drawTable();  
    void _drawFloor();
    void _drawSphere( const float& _width, 
		    const float& _height,
		    const float& _depth, 
		    const bool& highlighted );
    void _drawCone( const float& _width, 
		    const float& _height,
		    const float& _depth, 
		    const bool& highlighted );
    void _drawCube( const float& _width, 
		    const float& _height,
		    const float& _depth, 
		    const bool& highlighted );
    void _renderTexture(char bitmap[16*3]);
    
    // these hold the display list IDs
    int _tableList;
    int _floorList;
    
    int _coneList;
    int _cubeList;
    int _sphereList;
    
    int _coneHlList;
    int _cubeHlList;
    int _sphereHlList; 
    unsigned int _woodBitmapBindId;
    unsigned int _rainbowBitmapBindId;
    
    char woodBitmap[16*3]; 
    char rainbowBitmap[16*3]; 
};


#endif
