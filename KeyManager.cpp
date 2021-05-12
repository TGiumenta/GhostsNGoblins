/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which handles all of the keys. Involves instantiating,
				  updating, and rendering all the keys, preventing any of the collectibles from being
				  directly referenced. Specifically contains a reference to each effect which is added to,
				  removed, and received by other managers. Note; there should only ever be 1 key in the
				  scene but the structure is here in the event that keys are meant to be handled separately 
				  other than being a level-ender.
--------------------------------------------------------------------------------------------------*/

#include <Windows.h>
#include <gl/GL.h>
#include "SOIL.h"
#include "Key.h"
#include "KeyManager.h"
#include "Renderer.h"

#pragma warning (disable : 26446)	// Disables warning: "Use gsl::at() instead of unchecked subscript operator"

KeyManagerC* KeyManagerC::keyManagerInstance = nullptr;
GLuint KeyManagerC::keyTexture;
std::vector<KeyC*> KeyManagerC::activeKeys;
GLfloat KeyManagerC::initialKeyLocations[numberStaticKeys] = { 1750 };

// Creates a non-singleton instance of the manager
KeyManagerC* KeyManagerC::CreateInstance()
{
	if (keyManagerInstance == nullptr)
	{
		keyManagerInstance = new KeyManagerC();
		LoadTextures();
		SetupKeys();
	}
	return keyManagerInstance;
}

// Loads the key sprite into memory once, this index will be used every time a key is rendered
void KeyManagerC::LoadTextures() noexcept
{
	keyTexture = SOIL_load_OGL_texture("Sprites/Key.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

// Adds the initial keys to the scene
void KeyManagerC::SetupKeys()
{
	// Place all the needed keys in the world by instantiating the object
	for (int32_t keyIndex = 0; keyIndex < numberStaticKeys; ++keyIndex)
	{
		KeyC* newKey = KeyC::CreateInstance()->Initialize(initialKeyLocations[keyIndex], keyTexture);
		activeKeys.push_back(newKey);
	}
}

void KeyManagerC::Update(const DWORD milliseconds)
{
	const uint32_t size = activeKeys.size();
	for (uint32_t keyIndex = 0; keyIndex < size; ++keyIndex)
	{
		activeKeys.at(keyIndex)->Update(milliseconds);
	}
}

void KeyManagerC::Render()
{
	const uint32_t size = activeKeys.size();
	for (uint32_t keyIndex = 0; keyIndex < size; ++keyIndex)
	{
		activeKeys.at(keyIndex)->Render();
	}
}

// When a key is collected, remove it from the scene
void KeyManagerC::RemoveKey(const int32_t position) noexcept
{
	activeKeys.erase(activeKeys.begin() + position);
}

#pragma warning (disable : 26446)	// Enables warning: "Use gsl::at() instead of unchecked subscript operator"