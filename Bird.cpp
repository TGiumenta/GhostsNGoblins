/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all the logic for a Bird. Each zombie has its own position, velocity, 
				  and animation states. The Bird cycles through an animation where it flies towards you 
				  after slightly adjusting his y position to move off of the initial position and follow the 
				  player. After it gets activated, it will fly in a linear direction and not chase the player.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include <gl/GL.h>
#include "baseTypes.h"
#include "BackgroundManager.h"
#include "Player.h"
#include "Bird.h"
#include "Renderer.h"

const GLfloat BirdC::flyUpdateTime = 150;
const GLfloat BirdC::minYPosition = -65;
const GLfloat BirdC::xVelocity = 0.08f;
const GLfloat BirdC::yVelocity = 0.007f;
const GLfloat BirdC::positionRange = 165;
const GLuint BirdC::spriteWidth = 20;
const GLuint BirdC::spriteHeight = 16;

// Creates a non-singleton instance of the bird
BirdC* BirdC::CreateInstance()
{
	return new BirdC();
}

// Sets up the position and animations/booleans of the bird
BirdC* BirdC::Initialize(const GLfloat initialXPosition, const GLfloat initialYPosition, const GLuint textureIndex) noexcept
{
	xPosition = initialXPosition;
	yPosition = initialYPosition;
	texture = textureIndex;

	return this;
}

// Moves the bird if it is within range and determines if it is outside the game scene to destroy
bool8_t BirdC::Update(const DWORD milliseconds) noexcept
{
	const GLfloat playerXPosition = PlayerC::GetInstance()->GetXPosition();
	if (isInRange) 
	{ 
		MoveBird(milliseconds);
	}
	else
	{
		CheckPlayerInRange(playerXPosition); 
	}

	// Check if this bird hits the left side of the screen
	return ((xPosition + spriteWidth) < BackgroundManagerC::GetInstance()->GetLeftWallPosition());
}

// Determine if the player is in range of the bird to activate it
void BirdC::CheckPlayerInRange(const GLfloat playerXPosition) noexcept
{
	const int32_t positionDifference = static_cast<int32_t>(xPosition - playerXPosition);
	if (abs(positionDifference) < positionRange)
	{
		isInRange = true;
		currentAnimationState = BirdAnimation::SCREAM;
	}
}

// Adjusts the x position based on time-based velocity
void BirdC::MoveBird(const DWORD milliseconds) noexcept
{
	xPosition = xPosition - static_cast<GLfloat>(xVelocity * milliseconds);
	yPosition = max(static_cast<GLfloat>(yPosition - (yVelocity * milliseconds)), minYPosition);
	CheckSwapAnimation(milliseconds);
}

// Update the cooldown and check if it is ready to change animations
void BirdC::CheckSwapAnimation(const DWORD milliseconds) noexcept
{
	idleTimeCurrent += milliseconds;
	if ((idleTimeCurrent - idleTimeUpdate) > flyUpdateTime)
	{
		idleTimeUpdate = idleTimeCurrent;
		SwapAnimationState();
	}

	return;
}

void BirdC::SwapAnimationState() noexcept
{
	if (currentAnimationState == BirdAnimation::FLY_1)
	{
		currentAnimationState = BirdAnimation::FLY_2;
	}
	else
	{
		currentAnimationState = BirdAnimation::FLY_1;
	}

	return;
}

// Draws the bird to the screen
void BirdC::Render() noexcept
{
	UVRectangle uvRectangle{
		(1.0f / (BirdAnimation::BIRD_MAX_STATES - 1)) * (currentAnimationState - 1),
		(1.0f / (BirdAnimation::BIRD_MAX_STATES - 1)) * currentAnimationState,
		0,
		1
	};

	VertexRectangle vertexRectangle{
		0 + xPosition,
		spriteWidth + xPosition,
		0 + yPosition,
		spriteHeight + yPosition,
	};

	RenderQuads(texture, uvRectangle, vertexRectangle);
}