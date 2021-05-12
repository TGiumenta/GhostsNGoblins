/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which handles all of the weaponry that the player throws. Involves instantiating, 
				  updating, and rendering all the spears, preventing the spear class from being directly 
				  referenced. Specifically contains a reference to each weapon which is added to, removed, and received
				  by other managers.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include "SOIL.h"
#include "baseTypes.h"
#include "Spear.h"
#include "WeaponManager.h"

WeaponManagerC* WeaponManagerC::weaponManagerInstance;
GLuint WeaponManagerC::spearTexture;

// Creates a singleton instance of the manager
WeaponManagerC* WeaponManagerC::CreateInstance()
{
	if (weaponManagerInstance == nullptr)
	{
		weaponManagerInstance = new WeaponManagerC();
		LoadTextures();
	}
	return weaponManagerInstance;
}

// Loads the spear sprite into memory once, this index will be used every time a spear is rendered
void WeaponManagerC::LoadTextures() noexcept
{
	spearTexture = SOIL_load_OGL_texture("Sprites/Spear.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

// Destroys the manager and all its weapons
WeaponManagerC::~WeaponManagerC()
{
	DeleteAllSpears();
	weaponManagerInstance = nullptr;
}

// Called on delete, removes all potential spears from the scene
void WeaponManagerC::DeleteAllSpears()
{
	const uint32_t size = activeSpears.size();
	for (uint32_t spearIndex = 0; spearIndex < size; ++spearIndex)
	{
		delete activeSpears.at(spearIndex);
	}
	activeSpears.clear();
}

// Adds a spear to the scene and includes it in the vector for updates/renders
void WeaponManagerC::AddSpear()
{
	SpearC* spear = SpearC::CreateInstance()->Initialize(spearTexture);
	activeSpears.push_back(spear);
}

// Deletes a spear to the scene and removes it from the vector for updates/renders
void WeaponManagerC::RemoveSpear(const int32_t index)
{
	delete activeSpears.at(index);
	activeSpears.erase(activeSpears.begin() + index);
}

void WeaponManagerC::Update(DWORD const milliseconds)
{
	UpdateSpears(milliseconds);
}

// Call each spear's update method
void WeaponManagerC::UpdateSpears(const DWORD milliseconds)
{
	bool8_t deathFlag = false;
	const int32_t size = activeSpears.size();
	for (int32_t spearIndex = 0; spearIndex < size; ++spearIndex)
	{
		// If a spear is at the end of its lifetime, delete it from the manager
		deathFlag = activeSpears.at(spearIndex)->Update(milliseconds);
		if (deathFlag)
		{
			RemoveSpear(spearIndex);
			return;	// Return early, assume that two spears are never on the same x-position
		}
	}
}

void WeaponManagerC::Render()
{
	RenderSpears();
}

// Call each spear's render method
void WeaponManagerC::RenderSpears()
{
	const int32_t size = activeSpears.size();
	for (int32_t spearIndex = 0; spearIndex < size; ++spearIndex)
	{
		activeSpears.at(spearIndex)->Render();
	}
}