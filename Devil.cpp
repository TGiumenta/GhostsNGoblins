/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all the logic for a Devil. Each zombie has its own position, velocity, 
				  and animation states. The Devil cycles through an animation where it is initially idle, and 
				  then flies through the air. Its position gets updated once the player is officially within 
				  the range of the Devil. Then, the Devil locks itself to the players viewpoint until defeated. 
				  It moves in a parabolic shape (y = x^2) with a specific coefficient which allows it to 
				  follow the player and attack the player at the apex of the curve. 
--------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include <gl/GL.h>
#include "baseTypes.h"
#include "BackgroundManager.h"
#include "game.h"
#include "MathHelper.h"
#include "Player.h"
#include "Devil.h"
#include "Renderer.h"

const GLfloat DevilC::minIdleDeltaTime = 300.f;
const GLfloat DevilC::maxIdleDeltaTime = 650.f;
const GLfloat DevilC::flyDeltaTime = 150.f;
const GLfloat DevilC::minXPosition = -CGame::GetInstance()->GetOrthWidth() + spriteWidth;
const GLfloat DevilC::maxXPosition = CGame::GetInstance()->GetOrthWidth() - spriteWidth - 15;
const GLfloat DevilC::positionRange = 185;
const GLfloat DevilC::potentialXVelocity = 0.17f;
const GLuint DevilC::spriteWidth = 26;
const GLuint DevilC::spriteHeight = 24;

// Creates a non-singleton instance of a devil
DevilC* DevilC::CreateInstance()
{
	return new DevilC();
}

// Sets up the positions and states of the devil
DevilC* DevilC::Initialize(GLfloat const initialXPosition, GLfloat const initialYPosition, GLuint const textureIndex) noexcept
{
	xPosition = initialXPosition;
	yPosition = initialYPosition;
	texture = textureIndex;
	randomUpdateDeltaTime = GenerateRandomIdleTime();

	return this;
}

// Determines if the the player is in range and then sets flight paths based on the player's position
void DevilC::Update(const DWORD milliseconds) noexcept
{
	const GLfloat playerXPosition = PlayerC::GetInstance()->GetXPosition();
	const int32_t positionDifference = static_cast<int32_t>(xPosition - playerXPosition);

	if (!isInFightingMode)
	{
		// Determines if the player is in range
		if (abs(positionDifference) < positionRange)
		{
			isInFightingMode = true; // Devil is now in fighting mode and should constantly lock to the sides of the player's screen
			if (currentAnimationState == DevilAnimation::IDLE)
			{
				ResetAnimation();
			}
		}
	}
	else
	{
		// The devil is in fighting mode and should constantly check/set flight paths
		if (!isFlightPathSet) { SetFlightPath(playerXPosition, milliseconds); }
		SetPositionsOnFlightPath(playerXPosition, milliseconds);
		DetermineFlightPathEnd(playerXPosition);
		AnimationChange(milliseconds);
	}

	// Always determine orientation for rendering
	isFacingLeft = playerXPosition < xPosition;
}

// Update the cooldown and check if it is ready to change animations
void DevilC::SetFlightPath(const GLfloat playerXPosition, const DWORD milliseconds) noexcept
{
	idleTimeCurrent += milliseconds;
	if (idleTimeCurrent > randomUpdateDeltaTime)
	{
		idleTimeCurrent = 0;
		randomUpdateDeltaTime = GenerateRandomIdleTime();

		isFlightPathSet = true;
		SetPositionsOffFlightPath(milliseconds, playerXPosition, true);
	}
	else
	{
		SetPositionsOffFlightPath(milliseconds, playerXPosition, false);
	}
}

// Change both x and y positions every frame if in a path
// Follows a y = x^2 parabola
void DevilC::SetPositionsOnFlightPath(const GLfloat playerXPosition, const DWORD milliseconds) noexcept
{
	xPosition += xVelocity;

	// y = Coefficient*(x-position)^2
	// This sets up parabolic movement + some for a vertex offset on a graph
	const GLfloat xOffset = xPosition - playerXPosition + 5;
	yPosition = (0.05f * 0.05f * xOffset * xOffset);
	yPosition += ((PlayerC::GetSpriteHeight() / 2.f) + BackgroundManagerC::GetInstance()->GetBottomPlatformPosition());
}

// Locks the devil to the sides of the screen
void DevilC::SetPositionsOffFlightPath(const DWORD milliseconds, const GLfloat playerXPosition, const bool setVelocityInfo) noexcept
{
	// Lock xPosition
	if (playerXPosition < GetXPosition())
	{
		if (setVelocityInfo) 
		{ 
			xVelocity = static_cast<GLfloat>(-potentialXVelocity * milliseconds);
		}
		xPosition = ((max(playerXPosition, 0) + maxXPosition));		// Max just in case the player goes all the way to the right, somehow
	}
	else
	{
		if (setVelocityInfo)
		{
			xVelocity = static_cast<GLfloat>(potentialXVelocity * milliseconds);
		}
		xPosition = ((max(playerXPosition, 0) + minXPosition));		// Min just in case the player goes all the way to the left, somehow
	}
}

// Calculates the ends of the path based on where the player is, so that the devil can lock
void DevilC::DetermineFlightPathEnd(const GLfloat playerXPosition) noexcept
{
	// Calculate dynamic position
	const GLfloat maxPosition = static_cast<GLfloat>(playerXPosition + maxXPosition);
	const GLfloat minPosition = static_cast<GLfloat>(playerXPosition + minXPosition);

	// If the devil hits the max position, the path should be unset
	if ((xPosition >= maxPosition) || (xPosition <= minPosition))
	{
		isFlightPathSet = false;
	}
}

// Update the cooldown and check if it is ready to change animations
void DevilC::AnimationChange(DWORD const milliseconds) noexcept
{
	flyTimeCurrent += milliseconds;
	if (flyTimeCurrent > flyDeltaTime)
	{
		flyTimeCurrent = 0;
		++currentAnimationState;
		if (currentAnimationState == DevilAnimation::DEVIL_MAX_STATES)
		{
			ResetAnimation();
		}
	}
}

// Draws a devil to the screen
void DevilC::Render() noexcept
{
	UVRectangle uvRectangle{0,1,0,1};

	// Flip the U and V of the left/right texture coordinates based on the facing direction
	if (isFacingLeft)
	{
		uvRectangle.xLeftCoord = (1.0f / (DevilAnimation::DEVIL_MAX_STATES - 1)) * (currentAnimationState - 1);
		uvRectangle.xRightCoord = (1.0f / (DevilAnimation::DEVIL_MAX_STATES - 1)) * currentAnimationState;
	}
	else
	{
		uvRectangle.xLeftCoord = (1.0f / (DevilAnimation::DEVIL_MAX_STATES - 1)) * currentAnimationState;
		uvRectangle.xRightCoord = (1.0f / (DevilAnimation::DEVIL_MAX_STATES - 1)) * (currentAnimationState - 1);
	}

	VertexRectangle vertexRectangle{
		0 + xPosition,
		spriteWidth + xPosition,
		0 + yPosition,
		spriteHeight + yPosition
	};

	RenderQuads(texture, uvRectangle, vertexRectangle);
}

DWORD DevilC::GenerateRandomIdleTime() noexcept
{
	return static_cast<DWORD>(GetRangedRandom(minIdleDeltaTime, maxIdleDeltaTime + 1)); // Add 1 for maximum inclusitivty
}