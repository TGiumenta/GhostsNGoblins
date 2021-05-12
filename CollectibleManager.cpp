/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which handles all of the collectibles. Involves instantiating,
				  updating, and rendering all the collectibles, preventing any of the collectibles from being
				  directly referenced. Specifically contains a reference to each effect which is added to, 
				  removed, and received by other managers.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include "SOIL.h"
#include "BackgroundManager.h"
#include "Moneybag.h"
#include "Statue.h"
#include "Player.h"
#include "AudioManager.h"
#include "CollectibleManager.h"

CollectibleManagerC* CollectibleManagerC::collectibleManagerInstance = nullptr;
GLuint CollectibleManagerC::moneybagTexture = 0;
GLuint CollectibleManagerC::statueTexture = 0;
std::vector<MoneybagC*> CollectibleManagerC::activeMoneybags;
std::vector<StatueC*> CollectibleManagerC::activeStatues;
GLfloat CollectibleManagerC::initialBagLocations[numberInitialMoneybags] = { 75, 350, 615, 1015, 1425, 1570};

// Creates a singleton instance of the manager
CollectibleManagerC* CollectibleManagerC::CreateInstance()
{
	if (collectibleManagerInstance == nullptr)
	{
		collectibleManagerInstance = new CollectibleManagerC();
		LoadTextures();
		SetupBags();
	}
	return collectibleManagerInstance;
}

// Loads the collectible sprites into memory once, these indexes will be used every time a collectible is rendered
void CollectibleManagerC::LoadTextures() noexcept
{
	moneybagTexture = SOIL_load_OGL_texture("Sprites/Moneybag.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	statueTexture = SOIL_load_OGL_texture("Sprites/Statues.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

// Adds the static bags to the scene
void CollectibleManagerC::SetupBags()
{
	for (int32_t moneybagIndex = 0; moneybagIndex < numberInitialMoneybags; ++moneybagIndex)
	{
		MoneybagC* newBag = MoneybagC::CreateInstance()->Initialize(initialBagLocations[moneybagIndex], BackgroundManagerC::GetInstance()->GetBottomPlatformPosition(), moneybagTexture);
		activeMoneybags.push_back(newBag);
	}
}

CollectibleManagerC::~CollectibleManagerC()
{
	DeleteAllBags();
	DeleteAllStatues();
	collectibleManagerInstance = nullptr;
}

void CollectibleManagerC::DeleteAllBags()
{
	const uint32_t size = activeMoneybags.size();
	for (uint32_t moneybagIndex = 0; moneybagIndex < size; ++moneybagIndex)
	{
		delete activeMoneybags.at(moneybagIndex);
	}
	activeMoneybags.clear();
}

void CollectibleManagerC::DeleteAllStatues()
{
	const uint32_t size = activeStatues.size();
	for (uint32_t statueIndex = 0; statueIndex < size; ++statueIndex)
	{
		delete activeStatues.at(statueIndex);
	}
	activeStatues.clear();
}

void CollectibleManagerC::UpdateCollectibles(const DWORD milliseconds)
{
	UpdateBags(milliseconds);
	UpdateStatues(milliseconds);
}

// Calls each bag's update method
void CollectibleManagerC::UpdateBags(const DWORD milliseconds)
{
	const int32_t size = activeMoneybags.size();
	for (int32_t moneybagIndex = 0; moneybagIndex < size; ++moneybagIndex)
	{
		activeMoneybags.at(moneybagIndex)->Update(milliseconds);
	}
}

// Calls each statue's update method
void CollectibleManagerC::UpdateStatues(const DWORD milliseconds)
{
	const int32_t size = activeStatues.size();
	for (int32_t statueIndex = 0; statueIndex < size; ++statueIndex)
	{
		activeStatues.at(statueIndex)->Update(milliseconds);
	}
}

void CollectibleManagerC::RenderCollectibles()
{
	RenderBags();
	RenderStatues();
}

// Calls each bag's render method
void CollectibleManagerC::RenderBags()
{
	const int32_t size = activeMoneybags.size();
	for (int32_t moneyIndex = 0; moneyIndex < size; ++moneyIndex)
	{
		activeMoneybags.at(moneyIndex)->Render();
	}
}

// Calls each statue render method
void CollectibleManagerC::RenderStatues()
{
	const int32_t size = activeStatues.size();
	for (int32_t statueIndex = 0; statueIndex < size; ++statueIndex)
	{
		activeStatues.at(statueIndex)->Render();
	}
}

void CollectibleManagerC::AddBag(const GLfloat xPosition, const GLfloat yPosition)
{
	MoneybagC* newBag = MoneybagC::CreateInstance()->Initialize(xPosition, yPosition, moneybagTexture);
	activeMoneybags.push_back(newBag);
}

void CollectibleManagerC::RemoveBag(const int32_t position)
{
	activeMoneybags.erase(activeMoneybags.begin() + position);
	GenericCollection(MoneybagC::GetScore());
}

void CollectibleManagerC::AddStatue(const GLfloat xPosition, const GLfloat yPosition)
{
	StatueC* newStatue = StatueC::CreateInstance()->Initialize(xPosition, yPosition, statueTexture);
	activeStatues.push_back(newStatue);
}

void CollectibleManagerC::RemoveStatue(const int32_t position)
{
	// Calculate the score based on the statue type
	const int32_t scoreToAdd = activeStatues.at(position)->GetScore();
	activeStatues.erase(activeStatues.begin() + position);

	GenericCollection(scoreToAdd);
}

// Called whenever a collectible is collected
void CollectibleManagerC::GenericCollection(const int32_t scoreToAdd)
{
	AudioManagerC::GetInstance()->PlayCollectiblePickup();
	PlayerC::GetInstance()->AddScore(scoreToAdd);
}

// Determines if an item should be dropped
void CollectibleManagerC::CalculateDrops(const GLfloat xPosition, const GLfloat yPosition)
{
	if (CalculateStatueDrop(xPosition, yPosition)) { return; }
	if (CalculateMoneybagDrop(xPosition, yPosition)) { return; }
}

// Calculates the chance to drop a statue
// Note: The actual statue is not determined, just if one should be dropped
bool8_t CollectibleManagerC::CalculateStatueDrop(const GLfloat xPosition, const GLfloat yPosition)
{
	bool8_t shouldDrop = false;

	// Determine if a statue will drop at all
	const int32_t randomChance = (rand() % 100); // Value from 0 to 99
	if (statueDropChance > randomChance)
	{
		AddStatue(xPosition, yPosition);
		shouldDrop = true;
	}

	return shouldDrop;
}

// Calculate the chance to drop a bag
bool8_t CollectibleManagerC::CalculateMoneybagDrop(const GLfloat xPosition, const GLfloat yPosition)
{
	bool8_t shouldDrop = false;

	// Determine if a bag will drop at all
	const int32_t randomChance = (rand() % 100); // Value from 0 to 99
	if (moneybagDropChance > randomChance)
	{
		AddBag(xPosition, yPosition);
		shouldDrop = true;
	}

	return shouldDrop;
}