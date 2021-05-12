/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all the logic for a Zombie. This is not to be confused with a 
				  zombie wave. Each zombie has its own position, velocity, and animation states despite 
				  being spawned at the same time. The zombie cycles through an animation where it rises out 
				  of the ground and then is officially spawned (aka -- allows collision detection) and walks 
				  in a direction that originally faces the player and does not chase them.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include <iostream>
#include "baseTypes.h"
#include "BackgroundManager.h"
#include "Player.h"
#include "Zombie.h"
#include "Renderer.h"

#pragma warning (disable : 26432)	// Disables warning: "If you define or delete any default operation in the type <class>, define or delete them all"
#pragma warning (disable : 26409)	// Disables warning: "Avoid calling new and delete explicitly, use std::make_unique<T> instead"

const GLfloat ZombieC::xVelocity = 0.05f;
const GLuint ZombieC::spriteWidth = 20;
const GLuint ZombieC::spriteHeight = 28;
const GLfloat ZombieC::groundUpdateDeltaTime = 175.f;

// Creates a non-singleton instance of the zombie
ZombieC* ZombieC::CreateInstance()
{
	return new ZombieC();
}

// Sets initial positions/states for this instance of the zombie
ZombieC* ZombieC::Initialize(GLfloat const initialXPosition, GLfloat const initialYPosition, GLuint const textureIndex) noexcept
{
	xPosition = initialXPosition;
	yPosition = initialYPosition;
	texture = textureIndex;

	currentAnimationState = ZombieAnimation::GROUND_1;
	isFacingRight = (PlayerC::GetInstance()->GetXPosition() > GetXPosition());

	return this;
}

// Updates the animation of the zombie and ensures it hasn't fallen off the screen
bool8_t ZombieC::Update(const DWORD milliseconds) noexcept
{
	AnimationStateChange(milliseconds);

	// Check if this zombie hits the left side of the screen
	const bool8_t isPastLeftWall = (xPosition + spriteWidth < BackgroundManagerC::GetInstance()->GetLeftWallPosition());
	const bool8_t isPastRightWall = (xPosition > BackgroundManagerC::GetInstance()->GetRightWallPosition());
	
	return (isPastLeftWall || isPastRightWall);
}

// Holds the animation update backdrop for different states
void ZombieC::AnimationStateChange(const DWORD milliseconds) noexcept
{
	// For states GROUND_1 through GROUND_5, it is simply the animation to rise out of the ground,
	// and are the same functionality for each
	switch (currentAnimationState)
	{
	case ZombieAnimation::GROUND_1:
		UpdateAnimation(milliseconds);
		break;
	case ZombieAnimation::GROUND_2:
		UpdateAnimation(milliseconds);
		break;
	case ZombieAnimation::GROUND_3:
		UpdateAnimation(milliseconds);
		break;
	case ZombieAnimation::GROUND_4:
		UpdateAnimation(milliseconds);
		break;
	case ZombieAnimation::GROUND_5:
		UpdateAnimation(milliseconds);
		break;
	case ZombieAnimation::WALKING_1:
		UpdateAnimation(milliseconds);
		MoveZombie(milliseconds);
		break;
	case ZombieAnimation::WALKING_2:
		UpdateAnimation(milliseconds);
		MoveZombie(milliseconds);
		break;
	default:
		break;
	}
}

// Determines if its appropriate to change animations and does so if needed
void ZombieC::UpdateAnimation(const DWORD milliseconds) noexcept
{
	// Update the cooldown and check if it is ready to change animations
	groundTimeCurrent += milliseconds;
	if (groundTimeCurrent > groundUpdateDeltaTime)
	{
		groundTimeCurrent = 0;

		// If the zombie is at the end of its walking cycle, reset its walking cycle
		if (currentAnimationState == ZombieAnimation::WALKING_2)
		{
			--currentAnimationState;
		}
		else
		{
			++currentAnimationState;
		}
	}
}

// Sets the position based on the facing direction
void ZombieC::MoveZombie(const DWORD milliseconds) noexcept
{
	if (GetIsFacingRight())
	{
		xPosition += static_cast<GLfloat>(milliseconds * xVelocity);
	}
	else
	{
		xPosition += static_cast<GLfloat>(milliseconds * -xVelocity);
	}
}

// Draw Zombie to the screen
void ZombieC::Render() noexcept
{
	UVRectangle uvRectangle{ 0,1,0,1 };

	// Build the UV rectangle
	// Flip the U and V of the left/right texture coordinates based on the facing direction
	if (GetIsFacingRight())
	{
		uvRectangle.xLeftCoord = (1.0f / (ZombieAnimation::ZOMBIE_MAX_STATES - 1)) * (currentAnimationState - 1);
		uvRectangle.xRightCoord = (1.0f / (ZombieAnimation::ZOMBIE_MAX_STATES - 1)) * currentAnimationState;
	}
	else
	{
		uvRectangle.xLeftCoord = (1.0f / (ZombieAnimation::ZOMBIE_MAX_STATES - 1)) * currentAnimationState;
		uvRectangle.xRightCoord = (1.0f / (ZombieAnimation::ZOMBIE_MAX_STATES - 1)) * (currentAnimationState - 1);
	}

	VertexRectangle vertexRectangle{
		0 + xPosition,
		spriteWidth + xPosition,
		heightOffset + yPosition,
		spriteHeight + yPosition + heightOffset
	};

	RenderQuads(texture, uvRectangle, vertexRectangle);
}

#pragma warning (disable : 26432)	// Enables warning: "If you define or delete any default operation in the type <class>, define or delete them all"
#pragma warning (disable : 26409)	// Enables warning: "Avoid calling new and delete explicitly, use std::make_unique<T> instead"