/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all the logic for a Venom. Each venom has its own position and velocity.
				  This venom is extremely simple and only ensures that is on the screen before rendering.
--------------------------------------------------------------------------------------------------*/

#include <Windows.h>
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include "baseTypes.h"
#include "BackgroundManager.h"
#include "Venom.h"
#include "Renderer.h"

VenomC* VenomC::venomInstance = nullptr;
const GLuint VenomC::spriteWidth = 11;
const GLuint VenomC::spriteHeight = 11;
const GLfloat VenomC::velocityAdjustment = 15;

// Creates a non-singleton instance of the venom
VenomC* VenomC::CreateInstance()
{
	return new VenomC();
}

// Sets initial positions/states for this instance of the venom
VenomC* VenomC::Initialize(const GLfloat initialXPosition, const GLfloat initialYPosition, const GLfloat initialXVelocity, const GLfloat initialYVelocity, const GLuint textureIndex) noexcept
{
	xPosition = initialXPosition;
	yPosition = initialYPosition;
	xVelocity = initialXVelocity;
	yVelocity = initialYVelocity;
	texture = textureIndex;

	return this;
}

// Update just its velocity every frame
bool8_t VenomC::Update(const DWORD milliseconds) noexcept
{
	// Get the new positions based solely on the velocity, nothing should interupt it
	const GLfloat xPositionOffset = ((xVelocity / velocityAdjustment) * milliseconds);
	const GLfloat yPositionOffset = ((yVelocity / velocityAdjustment) * milliseconds);
	xPosition += xPositionOffset;
	yPosition += yPositionOffset;

	return DetermineIfOffscreen();
}

// A function to determine if the venom has left the map for early deletion
bool8_t VenomC::DetermineIfOffscreen() noexcept
{
	const bool8_t isBelowBottomPlat = (GetYPosition() <= BackgroundManagerC::GetInstance()->GetBottomPlatformPosition());
	const bool8_t isAboveTopPlat = (GetYPosition() >= BackgroundManagerC::GetInstance()->GetTopPlatformPosition());
	const bool8_t isOffLeftSide = (GetXPosition() <= BackgroundManagerC::GetInstance()->GetLeftWallPosition());
	const bool8_t isOffRightSide = (GetXPosition() >= BackgroundManagerC::GetInstance()->GetRightWallPosition());

	return (isBelowBottomPlat || isAboveTopPlat || isOffLeftSide || isOffRightSide);
}

// Draw Venom to the screen
void VenomC::Render() noexcept
{
	UVRectangle uvRectangle{ 0,1,0,1 };
	VertexRectangle vertexRectangle{};

	// Build the vertext rectangle
	vertexRectangle.vertexLeft = 0 + xPosition;
	vertexRectangle.vertexRight = spriteWidth + xPosition;
	vertexRectangle.vertexBottom = 0 + yPosition;
	vertexRectangle.vertexTop = spriteHeight + yPosition;

	RenderQuads(texture, uvRectangle, vertexRectangle);
}