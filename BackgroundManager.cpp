/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which handles all of the background textures. Simply loads and renders the
				  background. Note: This is NOT where the camera moves. The camera is moved every time the 
				  PLAYER moves. The background never moves and is rendered every frame. The level is small 
				  and linear enough to prevent loading too much texture every frame.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>										// Header File For Windows
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include "SOIL.h"
#include "Renderer.h"
#include "game.h"
#include "BackgroundManager.h"

BackgroundManagerC* BackgroundManagerC::backgroundManagerInstance = nullptr;
GLuint BackgroundManagerC::textureIndex = 0;
const GLfloat BackgroundManagerC::backgroundWidth = 2069;
const GLfloat BackgroundManagerC::backgroundHeight = 203;
const GLfloat BackgroundManagerC::bottomPlatformPosition = -75;
const GLfloat BackgroundManagerC::topPlatformPosition = 50;
const GLfloat BackgroundManagerC::leftWallPosition = -205;
const GLfloat BackgroundManagerC::rightWallPosition = 1845;

// Creates a singleton instance of the manager
BackgroundManagerC* BackgroundManagerC::CreateInstance()
{
	if (backgroundManagerInstance == nullptr)
	{
		backgroundManagerInstance = new BackgroundManagerC();
		LoadTextures();
	}
	return backgroundManagerInstance;
}

// Loads the background sprite into memory once, this index will be used every time the background is rendered
void BackgroundManagerC::LoadTextures() noexcept
{
	textureIndex = SOIL_load_OGL_texture("Sprites/BackgroundTest.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

// Draws the background
void BackgroundManagerC::Render() noexcept
{	
	UVRectangle uvRectangle{ 0,1,0,1 };

	VertexRectangle vertexRectangle{
		(0 - CGame::GetInstance()->GetOrthWidth()),
		(backgroundWidth - CGame::GetInstance()->GetOrthWidth()),
		(0 - (backgroundHeight / 2)),
		(backgroundHeight / 2)
	};

	RenderQuads(textureIndex, uvRectangle, vertexRectangle);
}