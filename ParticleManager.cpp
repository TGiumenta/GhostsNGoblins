/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which handles all of the particle effects. Involves instantiating, 
				  updating, and rendering all the effects, preventing any of the effects from being directly 
				  referenced. Specifically contains a reference to each effect which is added to, removed, 
				  and received by other managers.
--------------------------------------------------------------------------------------------------*/

#include <Windows.h>
#include <gl/GL.h>
#include "SOIL.h"
#include "baseTypes.h"
#include "FireDeath.h"
#include "ParticleManager.h"

#pragma warning (disable : 26409)	// Disables warning: "Avoid calling new and delete explicitly, use std::make_unique<T> instead"

ParticleManagerC* ParticleManagerC::particleManagerInstance = nullptr;

// Creates a singleton instance of the manager
ParticleManagerC* ParticleManagerC::CreateInstance()
{
	if (particleManagerInstance == nullptr)
	{
		particleManagerInstance = new ParticleManagerC();
	}
	return particleManagerInstance;
}

// Initializes the manager
ParticleManagerC::ParticleManagerC() noexcept
{
	LoadTextures();
	return;
}

// Loads the ffire sprite into memory once, this index will be used every time a fire is rendered
void ParticleManagerC::LoadTextures() noexcept
{
	fireDeathTexture = SOIL_load_OGL_texture("Sprites/Fire.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	return;
}

// Destroys the manager and all its particles
ParticleManagerC::~ParticleManagerC()
{
	DeleteAllFires();

	particleManagerInstance = nullptr;
}

// Called on delete, removes all potential fires from the scene
void ParticleManagerC::DeleteAllFires()
{
	uint32_t const size = activeFireDeaths.size();
	for (uint32_t fireDeathIndex = 0; fireDeathIndex < size; fireDeathIndex++)
	{
		delete activeFireDeaths.at(fireDeathIndex);
	}
	activeFireDeaths.clear();

	return;
}

void ParticleManagerC::AddFire(GLfloat const xPosition, GLfloat const yPosition)
{
	FireDeathC* const newFireDeath = FireDeathC::CreateInstance()->Initialize(xPosition, yPosition, fireDeathTexture);
	activeFireDeaths.push_back(newFireDeath);

	return;
}

std::vector<FireDeathC*> ParticleManagerC::GetFires()
{
	return activeFireDeaths;
}

void ParticleManagerC::RemoveFire(int32_t const position)
{
	delete activeFireDeaths.at(position);
	activeFireDeaths.erase(activeFireDeaths.begin() + position);

	return;
}

void ParticleManagerC::UpdateParticles(DWORD const milliseconds)
{
	UpdateFires(milliseconds);

	return;
}

// Call each fire's update method
void ParticleManagerC::UpdateFires(DWORD const milliseconds)
{
	bool8_t deathFlag = false;
	const int32_t size = activeFireDeaths.size();
	for (int32_t fireIndex = 0; fireIndex < size; fireIndex++)
	{
		// If a fire is at the end of its lifetime, remove it from the game
		deathFlag = activeFireDeaths.at(fireIndex)->Update(milliseconds);
		if (deathFlag)
		{
			RemoveFire(fireIndex);
			return;	// Return early, prevents overflowing into the vector 
		}
	}

	return;
}

void ParticleManagerC::RenderParticles()
{
	RenderFire();
	
	return;
}

// Call each fire's render method
void ParticleManagerC::RenderFire()
{
	const int size = activeFireDeaths.size();
	for (int fireIndex = 0; fireIndex < size; fireIndex++)
	{
		activeFireDeaths.at(fireIndex)->Render();
	}

	return;
}

#pragma warning (disable : 26409)	// Enables warning: "Avoid calling new and delete explicitly, use std::make_unique<T> instead"