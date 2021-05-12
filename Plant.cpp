/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all the logic for a plant. Each plant has its own position, 
				  and animation states despite being spawned at the same time. Plants in this game are 
				  all static, so theyshould all be initialized once the game starts. When the player is 
				  within range, they become activated and begin shooting at the player based on their 
				  position.
--------------------------------------------------------------------------------------------------*/

#include <Windows.h>
#include <stdlib.h>											// For abs
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include <cmath>
#include "Player.h"
#include "ProjectileManager.h"
#include "AudioManager.h"
#include "Plant.h"
#include "Renderer.h"

const GLfloat PlantC::venomUpdateDeltaTime = 1500;
const GLuint PlantC::spriteWidth = 18;
const GLuint PlantC::spriteHeight = 25;
const GLfloat PlantC::range = 200;

// Creates a non-singleton instance of the plant
PlantC* PlantC::CreateInstance()
{
	return new PlantC();
}

// Sets initial positions/states for this instance of the plant 
PlantC* PlantC::Initialize(GLfloat const initialXPosition, GLfloat const initialYPosition, GLuint const textureIndex) noexcept
{
	xPosition = initialXPosition;
	yPosition = initialYPosition;
	texture = textureIndex;

	currentAnimationState = PlantAnimation::IDLE;
	isFacingLeft = true;
	idleTimeCurrent = 0;

	return this;
}

// Updates the plants orientation and potentially its animation
void PlantC::Update(const DWORD milliseconds)
{
	const GLfloat playerXPosition = PlayerC::GetInstance()->GetXPosition();
	SetAnimationState(playerXPosition, milliseconds);

	// The sprite is naturally orientated to the left
	// Set the orientation to the boolean return
	isFacingLeft = (playerXPosition < GetXPosition());
}

// Ensures player is in range, but sets animation change based on a cooldown
void PlantC::SetAnimationState(const GLfloat playerXPosition, const DWORD milliseconds)
{
	const int32_t positionDifference = static_cast<int32_t>(xPosition - playerXPosition);
	if (abs(positionDifference) < range)
	{
		// Update the cooldown and check if it is ready to change animations
		idleTimeCurrent += milliseconds;
		if (idleTimeCurrent > venomUpdateDeltaTime)
		{
			idleTimeCurrent = 0;
			SwapAnimationState(playerXPosition + (PlayerC::GetSpriteWidth() / 2.f));
		}
	}
	else
	{
		currentAnimationState = PlantAnimation::IDLE;
	}
}

// Swaps states from idle to attack, and vise-versa.
// If the plant was attacking, fire its venom
void PlantC::SwapAnimationState(const GLfloat playerXPosition)
{
	if (currentAnimationState == PlantAnimation::IDLE)
	{
		currentAnimationState = PlantAnimation::ATTACK;
	}
	else
	{
		currentAnimationState = PlantAnimation::IDLE;
		FireVenom(playerXPosition);
	}
}

// Calculates the players position using sin/cos and fires a venom with specific velocity
void PlantC::FireVenom(const GLfloat playerXPosition)
{
	// Find the exact point to fire the projectile
	const GLfloat playerCenter = PlayerC::GetInstance()->GetYPosition() + (PlayerC::GetSpriteHeight() / 2.f);
	const GLfloat yDifferential = playerCenter - yPosition;
	const GLfloat xDifferential = playerXPosition - xPosition;
	const GLfloat angle = atan(yDifferential / xDifferential);
	GLfloat venomXVelocity = cos(angle);
	GLfloat venomYVelocity = sin(angle);

	// If the player is behind the plant, flip the velocities
	if (xDifferential < 0)
	{
		venomXVelocity = -venomXVelocity;
		venomYVelocity = -venomYVelocity;
	}

	ProjectileManagerC::GetInstance()->AddVenom(GetXPosition(), GetYPosition(), venomXVelocity, venomYVelocity);
	AudioManagerC::GetInstance()->PlaySpitVenom();
}

// Draw Plant to the screen
void PlantC::Render() noexcept
{
	UVRectangle uvRectangle{ 0,1,0,1 };

	// Build UV rectangle
	// Flip the U and V of the left/right texture coordinates based on the facing direction
	if (GetIsFacingLeft())
	{
		uvRectangle.xLeftCoord = (1.0f / (PlantAnimation::PLANT_MAX_STATES - 1)) * (currentAnimationState - 1);
		uvRectangle.xRightCoord = (1.0f / (PlantAnimation::PLANT_MAX_STATES - 1)) * currentAnimationState;
	}
	else
	{
		uvRectangle.xLeftCoord = (1.0f / (PlantAnimation::PLANT_MAX_STATES - 1)) * currentAnimationState;
		uvRectangle.xRightCoord = (1.0f / (PlantAnimation::PLANT_MAX_STATES - 1)) * (currentAnimationState - 1);
	}

	VertexRectangle vertexRectangle{
		0 + xPosition,
		spriteWidth + xPosition,
		0 + yPosition,
		spriteHeight + yPosition
	};

	RenderQuads(texture, uvRectangle, vertexRectangle);
}