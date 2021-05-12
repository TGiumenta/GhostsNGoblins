/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which handles all of the projectiles that the enemies throw. Involves instantiating, 
				  updating, and rendering all the venoms, preventing the venoms class from being directly 
				  referenced. Specifically contains a reference to each venom which is added to, removed, and received
				  by other managers.
--------------------------------------------------------------------------------------------------*/

#include <Windows.h>
#include "SOIL.h"
#include <gl/GL.h>
#include "baseTypes.h"
#include "Venom.h"
#include "ProjectileManager.h"

#pragma warning (disable : 26409)	// Disables warning: "Avoid calling new and delete explicitly, use std::make_unique<T> instead"

ProjectileManagerC* ProjectileManagerC::projectileManagerInstance = nullptr;
GLuint ProjectileManagerC::venomTexture;

// Creates a singleton instance of the manager
ProjectileManagerC* ProjectileManagerC::CreateInstance()
{
	if (projectileManagerInstance == nullptr)
	{
		projectileManagerInstance = new ProjectileManagerC();
		LoadTextures();
	}
	return projectileManagerInstance;
}

// Loads the venom sprite into memory once, this index will be used every time a venom is rendered
void ProjectileManagerC::LoadTextures() noexcept
{
	venomTexture = SOIL_load_OGL_texture("Sprites/Venom.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

void ProjectileManagerC::AddVenom(const GLfloat xPosition, const GLfloat yPosition, const GLfloat xVelocity, const GLfloat yVelocity)
{
	VenomC* venom = VenomC::CreateInstance()->Initialize(xPosition, yPosition, xVelocity, yVelocity, venomTexture);
	activeVenoms.push_back(venom);
}

void ProjectileManagerC::RemoveVenom(const int32_t index) noexcept
{
	activeVenoms.erase(activeVenoms.begin() + index);
}

void ProjectileManagerC::UpdateProjectiles(const DWORD milliseconds)
{
	UpdateVenoms(milliseconds);
}

// Updates the position of all the venoms and ensures none are out of bounds
void ProjectileManagerC::UpdateVenoms(const DWORD milliseconds)
{
	bool8_t deathFlag = false;
	const int32_t size = activeVenoms.size();
	for (int32_t venomIndex = 0; venomIndex < size; ++venomIndex)
	{
		// If the venom is meant to be destroyed based on its position offscreen, remove it from the game
		deathFlag = activeVenoms.at(venomIndex)->Update(milliseconds);
		if (deathFlag)
		{
			RemoveVenom(venomIndex);
			return;	// Return early, prevents overflowing into the vector 
		}
	}
}

void ProjectileManagerC::RenderProjectiles()
{
	RenderVenoms();
}

// Call each venoms's render method 
void ProjectileManagerC::RenderVenoms()
{
	const int size = activeVenoms.size();
	for (int32_t venomIndex = 0; venomIndex < size; ++venomIndex)
	{
		activeVenoms.at(venomIndex)->Render();
	}
}

#pragma warning (disable : 26409)	// Enables warning: "Avoid calling new and delete explicitly, use std::make_unique<T> instead"