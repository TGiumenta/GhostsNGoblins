/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all the logic for a fire effect. This effect cycles through an
				  animation which gets displayed after an enemy dies. It never changes position after its
				  initial position is set, and only ever alters its animation on a cycle based on a cooldown.
--------------------------------------------------------------------------------------------------*/

#include <Windows.h>
#include <gl/GL.h>
#include "FireDeath.h"
#include "Renderer.h"

const GLuint FireDeathC::spriteWidth = 18;
const GLuint FireDeathC::spriteHeight = 16;
const GLuint FireDeathC::deathUpdateTimer = 100;

// Creates a non-singleton instance of the fire
FireDeathC* FireDeathC::CreateInstance()
{
	return new FireDeathC();
}

// Initializes the instance of the fire
FireDeathC* FireDeathC::Initialize(const GLfloat initialXPosition, const GLfloat initialYPosition, const GLuint textureIndex) noexcept
{
	xPosition = initialXPosition;
	yPosition = initialYPosition;
	texture = textureIndex;

	currentAnimationState = FireDeathAnimation::FIRE_DEATH_1;
	currentTimeAnimation = 0;

	return this;
}

// Updates the animation of the fire bsaed on the cooldown
bool8_t FireDeathC::Update(const DWORD milliseconds) noexcept
{
	// Update the cooldown and check if it is ready to change animations
	currentTimeAnimation += milliseconds;
	if (currentTimeAnimation > deathUpdateTimer)
	{
		currentTimeAnimation = 0;
		++currentAnimationState;
		if (currentAnimationState > FireDeathAnimation::FIRE_DEATH_6)
		{
			return true;
		}
	}

	return false;
}

// Renders the current animation of the fire
void FireDeathC::Render() noexcept
{
	UVRectangle uvRectangle{
		(1.0f / (FireDeathAnimation::FIRE_MAX_STATES - 1)) * currentAnimationState,
		(1.0f / (FireDeathAnimation::FIRE_MAX_STATES - 1)) * (currentAnimationState - 1),
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