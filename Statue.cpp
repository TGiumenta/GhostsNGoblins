/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all the logic for a statue. This is not to be confused with a
				  Each statue has its own position and velocity. The statue itself should only 
				  ever be affected by a pseudo-gravity. During initialization, there is a randomizer that 
				  helps determine which statue gets rendered from the sprite sheet, which also dictates 
				  the statue's point value in relation to score. This acts like a typical collectible 
				  and is destroyed upon collision.
--------------------------------------------------------------------------------------------------*/

#include <Windows.h>
#include <iostream>
#include "MathHelper.h"
#include "baseTypes.h"
#include "BackgroundManager.h"
#include "Statue.h"
#include "Renderer.h"

#pragma warning (disable : 26446)	// Disables warning: "Use gsl::at() instead of unchecked subscript operator"

const GLfloat StatueC::yVelocity = 0.085f;
const GLuint StatueC::spriteWidth = 17;
const GLuint StatueC::spriteHeight = 16;

// Creates a non-singleton instance of the statue
StatueC* StatueC::CreateInstance()
{
	return new StatueC();
}

// Sets up the the positions and states of the statue
StatueC* StatueC::Initialize(const GLfloat initialXPosition, const GLfloat initialYPosition, const GLuint textureIndex) noexcept
{
	xPosition = initialXPosition;
	yPosition = initialYPosition;
	texture = textureIndex;

	DetermineStatueType(GetRangedRandom(1, 101));

	return this;
}

// Updates Y position based on gravity
void StatueC::Update(const DWORD milliseconds) noexcept
{
	// Simulate a gravitational fall 
	if (yPosition > BackgroundManagerC::GetInstance()->GetBottomPlatformPosition())
	{
		yPosition -= static_cast<GLfloat>(milliseconds * yVelocity);
	}
	else
	{
		yPosition = BackgroundManagerC::GetInstance()->GetBottomPlatformPosition();
	}
}

// Draws the statue
void StatueC::Render() noexcept
{
	// Grab the statue from the sprite sheet dependent on the one randomly selected on initialize
	UVRectangle uvRectangle{
		(1.0f / (StatueType::MAXTYPES - 1)) * (statueType - 1),
		(1.0f / (StatueType::MAXTYPES - 1)) * statueType,
		0,
		1
	};

	VertexRectangle vertexRectangle{
		0 + xPosition,
		spriteWidth + xPosition,
		0 + yPosition,
		spriteHeight + yPosition
	};

	RenderQuads(texture, uvRectangle, vertexRectangle);
}

// Simulate a random percentage selection for statues
void StatueC::DetermineStatueType(const int32_t randomValue) noexcept
{
	// A 33% chance to drop any statue except for the king, which is only a 1% chance
	if (randomValue == 100)
	{
		statueType = StatueType::KING;
	}
	else if (randomValue > 66)
	{
		statueType = StatueType::KNIGHT;
	}
	else if (randomValue > 33)
	{
		statueType = StatueType::DEVIL;
	}
	else
	{
		statueType = StatueType::GHOST;
	}
}

#pragma warning (disable : 26446)	// Enables warning: "Use gsl::at() instead of unchecked subscript operator"