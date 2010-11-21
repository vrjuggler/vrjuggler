/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include "CrateApp.h"
#include <iostream>
#include <gmtl/Generate.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9math.h>

struct CUSTOMVERTEX {FLOAT X, Y, Z; D3DVECTOR NORMAL; FLOAT U, V;};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

namespace vrjTest
{

// this is the function that puts the 3D models into video RAM
void CrateApp::init_graphics(LPDIRECT3DDEVICE9 renderDevice)
{
    // load the texture we will use
    D3DXCreateTextureFromFile(renderDevice,
                              "wood1.png",
                              &texture_1);

    // create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX t_vert[] =
    {
        // side 1
        { -3.0f, 3.0f, -3.0f, 0, 0, -1, 0, 1, },
        { 3.0f, 3.0f, -3.0f, 0, 0, -1, 1, 1, },
        { -3.0f, -3.0f, -3.0f, 0, 0, -1, 0, 0, },
        { 3.0f, -3.0f, -3.0f, 0, 0, -1, 1, 0, },

        // side 2
        { -3.0f, 3.0f, 3.0f, 0, 0, 1, 0, 1, },
        { -3.0f, -3.0f, 3.0f, 0, 0, 1, 0, 0, },
        { 3.0f, 3.0f, 3.0f, 0, 0, 1, 1, 1, },
        { 3.0f, -3.0f, 3.0f, 0, 0, 1, 1, 0, },

        // side 3
        { -3.0f, 3.0f, 3.0f, 0, 1, 0, 0, 1, },
        { 3.0f, 3.0f, 3.0f, 0, 1, 0, 1, 1, },
        { -3.0f, 3.0f, -3.0f, 0, 1, 0, 0, 0, },
        { 3.0f, 3.0f, -3.0f, 0, 1, 0, 1, 0, },

        // side 4
        { -3.0f, -3.0f, 3.0f, 0, -1, 0, 0, 1, },
        { -3.0f, -3.0f, -3.0f, 0, -1, 0, 0, 0, },
        { 3.0f, -3.0f, 3.0f, 0, -1, 0, 1, 1, },
        { 3.0f, -3.0f, -3.0f, 0, -1, 0, 1, 0, },

        // side 5
        { 3.0f, 3.0f, -3.0f, 1, 0, 0, 1, 0, },
        { 3.0f, 3.0f, 3.0f, 1, 0, 0, 1, 1, },
        { 3.0f, -3.0f, -3.0f, 1, 0, 0, 0, 0, },
        { 3.0f, -3.0f, 3.0f, 1, 0, 0, 0, 1, },

        // side 6
        { -3.0f, 3.0f, -3.0f, -1, 0, 0, 1, 0, },
        { -3.0f, -3.0f, -3.0f, -1, 0, 0, 0, 0, },
        { -3.0f, 3.0f, 3.0f, -1, 0, 0, 1, 1, },
        { -3.0f, -3.0f, 3.0f, -1, 0, 0, 0, 1, },
    };    // that reminds me of programming in binary!

    // create a vertex buffer interface called mTBuffer
    renderDevice->CreateVertexBuffer(24*sizeof(CUSTOMVERTEX),
                               0,
                               CUSTOMFVF,
                               D3DPOOL_MANAGED,
                               &mTBuffer,
                               NULL);

    VOID* pVoid;    // a void pointer

    // lock mTBuffer and load the vertices into it
    mTBuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, t_vert, sizeof(t_vert));
    mTBuffer->Unlock();

    return;
}

// this is the function that sets up the lights and materials
void CrateApp::init_light(LPDIRECT3DDEVICE9 renderDevice)
{
    D3DLIGHT9 light;    // create the light struct
    D3DMATERIAL9 material;    // create the material struct

    ZeroMemory(&light, sizeof(light));    // clear out the struct for use
    light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
    light.Diffuse.r = 0.5f;    // .5 red
    light.Diffuse.g = 0.5f;    // .5 green
    light.Diffuse.b = 0.5f;    // .5 blue
    light.Diffuse.a = 1.0f;    // full alpha (we'll get to that soon)

    D3DVECTOR vecDirection = {-1.0f, -0.3f, -1.0f};    // the direction of the light
    light.Direction = vecDirection;    // set the direction

    renderDevice->SetLight(0, &light);    // send the light struct properties to light #0
    renderDevice->LightEnable(0, TRUE);    // turn on light #0

    ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
    material.Diffuse.r = material.Ambient.r = 1.0f;    // set the material to full red
    material.Diffuse.g = material.Ambient.g = 1.0f;    // set the material to full green
    material.Diffuse.b = material.Ambient.b = 1.0f;    // set the material to full blue
    material.Diffuse.a = material.Ambient.a = 1.0f;    // set the material to full alpha

    renderDevice->SetMaterial(&material);    // set the globably-used material to &material

    return;
}

CrateApp::CrateApp(vrj::Kernel* kernel)
   : vrj::D3dApp(kernel)
   , mTBuffer(NULL)
{}

CrateApp::~CrateApp()
{}

void CrateApp::init()
{
   //std::cout << "---------- App:init() ---------------" << std::endl;
   // Initialize devices
   mWand.init("VJWand");
   mHead.init("VJHead");
   mButton0.init("VJButton0");
   mButton1.init("VJButton1");
   mButton2.init("VJButton2");
   mButton3.init("VJButton3");
   mButton4.init("VJButton4");
   mButton5.init("VJButton5");
}

void CrateApp::contextInit(LPDIRECT3DDEVICE9 renderDevice)
{
   // create the vertices using the CUSTOMVERTEX struct
   CUSTOMVERTEX t_vert[] =
   {
      { 320.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
      { 520.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
      { 120.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
   };

   // create a vertex buffer interface called mTBuffer
   renderDevice->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX),
                              0,
                              CUSTOMFVF,
                              D3DPOOL_MANAGED,
                              &mTBuffer,
                              NULL);

   VOID* pVoid;    // a void pointer

   // lock mTBuffer and load the vertices into it
   mTBuffer->Lock(0, 0, (void**)&pVoid, 0);
   memcpy(pVoid, t_vert, sizeof(t_vert));
   mTBuffer->Unlock();


   init_graphics(renderDevice);    // call the function to initialize the cube
   init_light(renderDevice);    // call the function to initialize the light and material

   renderDevice->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn on the 3D lighting
   renderDevice->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
   renderDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));    // ambient light
}

void CrateApp::draw(LPDIRECT3DDEVICE9 renderDevice)
{
   renderDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
   renderDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

   renderDevice->BeginScene();

   // select which vertex format we are using
   renderDevice->SetFVF(CUSTOMFVF);

   static float index = 0.0f; index+=0.03f;    // an ever-increasing float value
   D3DXMATRIX matRotateY;    // a matrix to store the rotation for each triangle
   D3DXMatrixRotationY(&matRotateY, index);    // the rotation matrix

   // Translate in front of us.
   matRotateY(3, 0) = 0.0f;
   matRotateY(3, 1) = 0.0f;
   matRotateY(3, 2) = 25.0f;

   renderDevice->SetTransform(D3DTS_WORLD, &(matRotateY));    // set the world transform

   // -- Draw box on wand --- //
   //gmtl::Matrix44f wandMatrix = mWand->getData();      // Get the wand matrix
   //renderDevice->SetTransform(D3DTS_WORLD, (const D3DMATRIX*)&wandMatrix.mData);    // set the world transform

   // select the vertex buffer to display
   renderDevice->SetStreamSource(0, mTBuffer, 0, sizeof(CUSTOMVERTEX));

   // set the texture
   renderDevice->SetTexture(0, texture_1);

   // draw the textured square
   renderDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
   renderDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
   renderDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
   renderDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
   renderDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
   renderDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2); 

   renderDevice->EndScene(); 

   return;
}

}
