/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all the logic for a single key. If there is ever a need to do 
				  more than just update the animation based on a cooldown, there is a place for it. 
				  Currently, it only cycles through the animation states until collected.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include <gl/GL.h>
#include "baseTypes.h"
#include "BackgroundManager.h"
#include "Key.h"
#include "Renderer.h"

const GLfloat KeyC::updateDeltaTime = 100;
const GLuint KeyC::spriteWidth = 16;
const GLuint KeyC::spriteHeight = 14;

// Creates a non-singleton instance of the key
KeyC* KeyC::CreateInstance()
{
	return new KeyC();
}

// Initializes the positions and animations of the key
KeyC* KeyC::Initialize(const GLfloat initialXPosition, const GLuint textureIndex) noexcept
{
	xPosition = initialXPosition;
	yPosition = BackgroundManagerC::GetInstance()->GetBottomPlatformPosition();
	texture = textureIndex;

	currentAnimationState = KeyAnimation::WHITE_1;

	return this;
}

// Update the cooldown and check if it is ready to change animations
void KeyC::Update(const DWORD milliseconds) noexcept
{
	keyTimeCurrent += milliseconds;
	if (keyTimeCurrent > updateDeltaTime)
	{
		keyTimeCurrent = 0;
		currentAnimationState = (((currentAnimationState + 1) % (KeyAnimation::KEY_MAX_STATES - 1)) + 1);
	}
}

// Draw the Key to the screen
void KeyC::Render() noexcept
{
	UVRectangle uvRectangle{
		uvRectangle.xLeftCoord = (1.0f / (KeyAnimation::KEY_MAX_STATES - 1)) * (currentAnimationState - 1),
		uvRectangle.xRightCoord = (1.0f / (KeyAnimation::KEY_MAX_STATES - 1)) * currentAnimationState,
		0,
		1
	};
	VertexRectangle vertexRectangle{
		vertexRectangle.vertexLeft = 0 + xPosition,
		vertexRectangle.vertexRight = spriteWidth + xPosition,
		vertexRectangle.vertexBottom = 0 + yPosition,
		vertexRectangle.vertexTop = spriteWidth + yPosition
	};

	RenderQuads(texture, uvRectangle, vertexRectangle);
}