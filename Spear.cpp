/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all the logic for a spear. Each spear  has its own position, 
				  velocity, and direction. The spear's direction is dependent on the player's direction 
				  but maintains a constant velocity for a timed lifetime.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>										// Header File For Windows
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include "SOIL.h"
#include "game.h"
#include "openglframework.h"	
#include "Renderer.h"
#include "Spear.h"
#include "Player.h"

const GLfloat SpearC::xVelocity = .3f;
const GLuint SpearC::spriteWidth = 27;
const GLuint SpearC::spriteHeight = 8;
const GLfloat SpearC::lifetime = 750.f;

// Creates a non-singleton instance of the spear
SpearC* SpearC::CreateInstance()
{
	return new SpearC();
}

// Sets up all the initial positions and boolean states of the spear
SpearC* SpearC::Initialize(const GLuint textureIndex) noexcept
{
	texture = textureIndex;
	SetInitialStates();
	return this;
}

// Sets the direction and the initial position of the spear
void SpearC::SetInitialStates() noexcept
{
	// Gets the direction for the velocity based on the Player's orientation
	direction = PlayerC::GetInstance()->GetIsFacingRight() ? 1.0f : -1.0f;

	// Sets the spear's spawn position which is slightly offset from the player
	constexpr GLfloat xOffsetedPosition = spriteWidth/2.f;
	const GLfloat xInitialVector = direction * xOffsetedPosition;
	xPosition = (PlayerC::GetInstance()->GetXPosition() + xInitialVector);
	yPosition = (PlayerC::GetInstance()->GetYPosition() + (PlayerC::GetSpriteHeight()/2.f));
}

// Updates position and ensure it is not at its end of his life
bool SpearC::Update(const DWORD milliseconds) noexcept
{
	xPosition += static_cast<GLfloat>(direction * (xVelocity * milliseconds));
	currentLifetime += milliseconds;

	// If the lifetime of the spear exceeds the defined lifetime, destroy the spear in the manager
	return (currentLifetime > lifetime);
}

// Draws Spear to the screen
void SpearC::Render() noexcept
{
	UVRectangle uvRectangle{ 0,1,0,1};

	// If direction flipped (backwards), flip the U and the V
	if (direction == -1.f)
	{
		uvRectangle.xLeftCoord = 0;
		uvRectangle.xRightCoord = 1;
	}

	VertexRectangle vertexRectangle{
		0 + xPosition,
		spriteWidth + xPosition,
		0 + yPosition,
		spriteHeight + yPosition
	};

	RenderQuads(texture, uvRectangle, vertexRectangle);
}