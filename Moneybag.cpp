/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all the logic for a moneybag. This is not to be confused with a
				  Each bag has its own position and velocity. The moneybag itself should only 
				  ever be affected by a pseudo-gravity. This acts like a typical collectible and is 
				  destroyed upon collection.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>		// Header File For Windows
#include <gl/GL.h>			// Header File For The OpenGL32 Library
#include "BackgroundManager.h"
#include "Moneybag.h"
#include "Renderer.h"

const GLfloat MoneybagC::moneybagDropSpeed = 0.095f;
const GLuint MoneybagC::spriteWidth = 13;
const GLuint MoneybagC::spriteHeight = 11;

// Creates a non-singleton instance of the bag
MoneybagC* MoneybagC::CreateInstance()
{
	return new MoneybagC();
}

// Sets up the position of the bag
MoneybagC* MoneybagC::Initialize(const GLfloat initialXPosition, const GLfloat initialYPosition, const GLuint textureIndex) noexcept
{
	xPosition = initialXPosition;
	yPosition = initialYPosition;
	texture = textureIndex;

	return this;
}

// Updates Y position based on gravity
void MoneybagC::Update(const DWORD milliseconds) noexcept
{
	// Simulate a gravitational fall 
	if (yPosition > BackgroundManagerC::GetInstance()->GetBottomPlatformPosition())
	{
		const GLfloat positionOffset = static_cast<GLfloat>(milliseconds * moneybagDropSpeed);
		yPosition -= positionOffset;
	}
	else
	{
		yPosition = BackgroundManagerC::GetInstance()->GetBottomPlatformPosition();
	}
}

// Draws bag to the screen
void MoneybagC::Render() noexcept
{
	UVRectangle uvRectangle{ 0,1,0,1 };
	VertexRectangle vertexRectangle{
		0 + xPosition,
		spriteWidth + xPosition,
		0 + yPosition,
		spriteHeight + yPosition
	};

	RenderQuads(texture, uvRectangle, vertexRectangle);
}