/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all the logic for a ghast. Each ghost has its own position, velocity, 
				  and animation states. The ghast spawns and flies linearly on a set path for a set amount of 
				  time. After it finishes that path, it re-evaluates its facing direction to chase the player.
				  Every time a new path is chosen, it moves closr and closer to the y position of the player.
				  It will force the player to interact with it eventually, and the longer it is not dealt with 
				  it is harder to handle.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include <gl/GL.h>
#include "baseTypes.h"
#include "MathHelper.h"
#include "Player.h"
#include "Ghast.h"
#include "Renderer.h"

const GLfloat GhastC::floatUpdateTime = 150;
const GLfloat GhastC::pathUpdateTime = 2050;
const GLfloat GhastC::potentialXVelocity = 0.065f;
const GLuint GhastC::spriteWidth = 26;
const GLuint GhastC::spriteHeight = 13;

// Creates a non-singleton instance of the ghost
GhastC* GhastC::CreateInstance()
{
	return new GhastC();
}

// Sets up thhe positions and animation/boolean states of this instance
GhastC* GhastC::Initialize(const GLfloat randomXOffset, const GLuint textureIndex) noexcept
{
	xPosition = PlayerC::GetInstance()->GetXPosition() + randomXOffset;
	yPosition = static_cast<GLfloat>(GetRangedRandom(-35, -50 + 1)); // Add 1 for inclusivity
	texture = textureIndex;

	return this;
}

// Callsa hub for a state changer
void GhastC::Update(const DWORD milliseconds) noexcept
{
	AnimationStateChange(milliseconds);
}

// HOlds a switch to alter animation based on current state
void GhastC::AnimationStateChange(const DWORD milliseconds) noexcept
{
	// For states SPAWN_1 through SPAWN_2, it is simply the animation to appear out of thin air,
	// and are the same functionality for each
	switch (GetCurrentAnimationState())
	{
	case GhastAnimation::SPAWN_1:
		UpdateAnimation(milliseconds);
		break;
	case GhastAnimation::SPAWN_2:
		UpdateAnimation(milliseconds);
		break;
	case GhastAnimation::ATTACKING_1:
		UpdateAnimation(milliseconds);
		MoveGhast(milliseconds);
		break;
	case GhastAnimation::ATTACKING_2:
		UpdateAnimation(milliseconds);
		MoveGhast(milliseconds);
	default:
		break;
	}
}

// Updates the animation based on a cooldown timer
void GhastC::UpdateAnimation(const DWORD milliseconds) noexcept
{
	// Update the cooldown and check if it is ready to change animations
	flyTimeCurrent += milliseconds;
	if (flyTimeCurrent > floatUpdateTime)
	{
		flyTimeCurrent = 0;

		// If the zombie is at the end of its walking cycle, reset its walking cycle
		if (currentAnimationState == GhastAnimation::ATTACKING_2)
		{
			--currentAnimationState;
		}
		else
		{
			++currentAnimationState;
		}
	}
}

// Moves the ghast towards the player based on their location to the ghast every few seconds
void GhastC::MoveGhast(const DWORD milliseconds) noexcept
{
	flightPathCurrent += milliseconds;
	if (flightPathCurrent > pathUpdateTime)
	{
		flightPathCurrent = 0;

		// Chases the Player
		isFacingLeft = PlayerC::GetInstance()->GetXPosition() < xPosition;
		AdjustYPosition();
	}

	// Move along the x-axis every frame
	if (isFacingLeft)
	{
		xVelocity = static_cast<GLfloat>(milliseconds * -potentialXVelocity);
	}
	else
	{
		xVelocity = static_cast<GLfloat>(milliseconds * potentialXVelocity);
	}

	xPosition += xVelocity;
}

// Alter y position over time if the player does not kill the ghast
void GhastC::AdjustYPosition() noexcept
{
	const GLfloat playerYPosition = PlayerC::GetInstance()->GetYPosition() + PlayerC::GetSpriteHeight() / 2.f;
	if (playerYPosition < yPosition)
	{
		yPosition -= 5;
	}
}

// Draw the ghast to the screen
void GhastC::Render() noexcept
{
	UVRectangle uvRectangle{};
	VertexRectangle vertexRectangle{
		0 + xPosition,
		spriteWidth + xPosition,
		0 + yPosition,
		spriteHeight + yPosition
	};

	// Build UV rectangle
	// Flip the U and V of the left/right texture coordinates based on the facing direction
	if (isFacingLeft)
	{
		uvRectangle.xLeftCoord = (1.0f / (GhastAnimation::GHAST_MAX_STATES - 1)) * (currentAnimationState - 1);
		uvRectangle.xRightCoord = (1.0f / (GhastAnimation::GHAST_MAX_STATES - 1)) * currentAnimationState;
	}
	else
	{
		uvRectangle.xLeftCoord = (1.0f / (GhastAnimation::GHAST_MAX_STATES - 1)) * currentAnimationState;
		uvRectangle.xRightCoord = (1.0f / (GhastAnimation::GHAST_MAX_STATES - 1)) * (currentAnimationState - 1);
	}
	uvRectangle.yBottomTextCoord = 0;
	uvRectangle.yTopTextCoord = 1;

	RenderQuads(texture, uvRectangle, vertexRectangle);
}