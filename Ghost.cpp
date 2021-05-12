/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all the logic for a ghost. This is not to be confused with a
				  ghost wave. Each ghost has its own position, velocity, and animation states despite
				  being spawned at the same time. The ghost bounces up and down and oscillates. The ghost 
				  moves linearly and does not chase the player after spawning.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include "BackgroundManager.h"
#include "MathHelper.h"
#include "baseTypes.h"
#include "Player.h"
#include "Ghost.h"
#include "Renderer.h"

const GLfloat GhostC::airUpdateTime = 225.f;
const GLfloat GhostC::xVelocity = -0.06f;
const GLfloat GhostC::potentialYVelocity = 0.125f;
const GLuint GhostC::spriteWidth = 16;
const GLuint GhostC::spriteHeight = 24;

// Creates a non-singleton instance of the ghost
GhostC* GhostC::CreateInstance()
{
	return new GhostC();
}

// Sets upp the intiial positions and animation states of the ghost
GhostC* GhostC::Initialize(const int32_t randomXOffset, const GLuint textureIndex) noexcept
{
	xPosition = static_cast<GLfloat>(PlayerC::GetInstance()->GetXPosition() + 150 + randomXOffset);
	yPosition = static_cast<GLfloat>(GetRangedRandom(-45, 25 + 1)); // Add 1 for inclusivity
	texture = textureIndex;

	airTimeCurrent = 0;
	currentAnimationState = GhostAnimation::AIR_1;

	SetInitialYVelocity();

	return this;
}

// Set the initial movement direction of the ghost, either up or down randomly
void GhostC::SetInitialYVelocity() noexcept
{
	if ((rand() % 2) == 1)
	{
		yVelocity = potentialYVelocity;
	}
	else
	{
		yVelocity = -potentialYVelocity;
	}
}

// Updates the position and velocity based on its current path
bool8_t GhostC::Update(const DWORD milliseconds) noexcept
{
	// Move along the x-axis every frame
	xPosition -= static_cast<GLfloat>(milliseconds * -xVelocity);

	CheckAnimationSwap(milliseconds);
	AttemptYVelocitySwap();

	// Move along the y-axis every frame based on the velocity
	yPosition += milliseconds * yVelocity;

	// Check if this ghost hits the left side of the screen
	if ((xPosition + spriteWidth) < BackgroundManagerC::GetInstance()->GetLeftWallPosition())
	{
		return true;
	}

	return false;
}

// Updates the animation cooldown and ensures the ghost can swap animations
void GhostC::CheckAnimationSwap(const DWORD milliseconds) noexcept
{
	airTimeCurrent += milliseconds;
	if (airTimeCurrent > airUpdateTime)
	{
		airTimeCurrent = 0;
		SwapAnimationState();
	}
}

// Simply swap animations between the only two while the ghost floats
void GhostC::SwapAnimationState() noexcept
{
	if (currentAnimationState == GhostAnimation::AIR_1)
	{
		currentAnimationState = GhostAnimation::AIR_2;
	}
	else
	{
		currentAnimationState = GhostAnimation::AIR_1;
	}
}

// If the ghost is hitting the edges of the defined areas, switch the velocities
void GhostC::AttemptYVelocitySwap() noexcept
{
	// If the positions exceed their bounds
	if (yPosition >= 25)
	{
		yVelocity = -potentialYVelocity;
	}
	if (yPosition <= -100)
	{
		yVelocity = potentialYVelocity;
	}
}

// Draws the ghost to the screen
void GhostC::Render() noexcept
{
	UVRectangle uvRectangle{
		(1.0f / (GhostAnimation::GHOST_MAX_STATES - 1)) * (currentAnimationState - 1),
		(1.0f / (GhostAnimation::GHOST_MAX_STATES - 1)) * currentAnimationState,
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