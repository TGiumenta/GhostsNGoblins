/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which handles all of the enemies and enemy logic within the game.
				  It primarily serves as a hub for spawning in and killing off enemies. It adds all of the 
				  static enemies that start off in the scene, as well as introducing all of the dynamic ones 
				  that get spawned in via a pseduo-collider. This collider is simply an x position, and once 
				  the player passes that position, it spawns in a potential wave of enemies. This class 
				  also contains wrappers to call all of the updates and render functions of each of the 
				  enemies as well as checking if the player should spawn them.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include "SOIL.h"
#include "game.h"
#include "baseTypes.h"
#include "BackgroundManager.h"
#include "AudioManager.h"
#include "MathHelper.h"
#include "Zombie.h"
#include "Ghost.h"
#include "Plant.h"
#include "Devil.h"
#include "Bird.h"
#include "Ghast.h"
#include "Player.h"
#include "EnemyManager.h"

#pragma warning (disable : 26446)	// Disables warning: "Use gsl::at() instead of unchecked subscript operator"
#pragma warning (disable : 26482)	// Disables warning: "Only index into arrays using constant expressions"

EnemyManagerC* EnemyManagerC::enemyManagerInstance = nullptr;
const GLfloat EnemyManagerC::plantXPositions[numberStaticPlants] = { 100, 408, 492, 1530 };
const GLfloat EnemyManagerC::plantYPositions[numberStaticPlants] = { 24, -70, -6, -70 };
const GLfloat EnemyManagerC::devilXPositions[numberStaticDevils] = { 900, 1700 };
const GLfloat EnemyManagerC::devilYPositions[numberStaticDevils] = { 12, 0 };
const GLfloat EnemyManagerC::birdXPositions[numberStaticBirds] = { 327, 686 };
const GLfloat EnemyManagerC::birdYPositions[numberStaticBirds] = { -53, -53 };
GLuint EnemyManagerC::zombieTexture;
GLuint EnemyManagerC::ghostTexture;
GLuint EnemyManagerC::plantTexture;
GLuint EnemyManagerC::devilTexture;
GLuint EnemyManagerC::birdTexture;
GLuint EnemyManagerC::ghastTexture;
std::vector<ZombieC*> EnemyManagerC::activeZombies;
std::vector<GhostC*> EnemyManagerC::activeGhosts;
std::vector<int32_t> EnemyManagerC::activeGhostSpawnLocations;
std::vector<PlantC*> EnemyManagerC::activePlants;
std::vector<DevilC*> EnemyManagerC::activeDevils;
std::vector<BirdC*> EnemyManagerC::activeBirds;
std::vector<GhastC*> EnemyManagerC::activeGhasts;
std::vector<int32_t> EnemyManagerC::activeGhastSpawnLocations;
DWORD EnemyManagerC::currentZombieSpawnTime = 0;
const float_t EnemyManagerC::spawnTime = CGame::GetMillisecondsToSeconds() * 3.75f;

// Creates a singleton instance of the fire
EnemyManagerC* EnemyManagerC::CreateInstance()
{
	if (enemyManagerInstance == nullptr)
	{
		enemyManagerInstance = new EnemyManagerC();
		LoadTextures();
		SetupZombies();
		SetupGhosts();
		SetupPlants();
		SetupDevils();
		SetupBirds();
		SetupGhasts();
	}

	return enemyManagerInstance;
}

// Loads the enemy sprites into memory once, these indexes will be used every time an enemy is rendered
void EnemyManagerC::LoadTextures() noexcept
{
	zombieTexture = SOIL_load_OGL_texture("Sprites/Zombie.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	ghostTexture = SOIL_load_OGL_texture("Sprites/Ghost.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	plantTexture = SOIL_load_OGL_texture("Sprites/Plant.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	devilTexture = SOIL_load_OGL_texture("Sprites/Devil.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	birdTexture = SOIL_load_OGL_texture("Sprites/Bird.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	ghastTexture = SOIL_load_OGL_texture("Sprites/Ghast.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

void EnemyManagerC::SetupZombies() noexcept
{
	currentZombieSpawnTime = 0;
}

// Adds the x position lookups to the spawn location vector
void EnemyManagerC::SetupGhosts()
{
	activeGhostSpawnLocations.clear();
	activeGhostSpawnLocations.push_back(1000);
	activeGhostSpawnLocations.push_back(1200);
	activeGhostSpawnLocations.push_back(1400);
}

// Adds the initial static plants to the scene
void EnemyManagerC::SetupPlants()
{
	for (int plantIndex = 0; plantIndex < numberStaticPlants; ++plantIndex)
	{
		AddPlant(plantXPositions[plantIndex], plantYPositions[plantIndex]);
	}
}

// Adds the initial static devils to the scenes
void EnemyManagerC::SetupDevils()
{
	for (int devilIndex = 0; devilIndex < numberStaticDevils; ++devilIndex)
	{
		AddDevil(devilXPositions[devilIndex], devilYPositions[devilIndex]);
	}
}

// Adds the initial static birds to the scene
void EnemyManagerC::SetupBirds()
{
	for (int birdIndex = 0; birdIndex < numberStaticBirds; ++birdIndex)
	{
		AddBird(birdXPositions[birdIndex], birdYPositions[birdIndex]);
	}
}

// Adds the x position lookups to the spawn location vector
void EnemyManagerC::SetupGhasts()
{
	activeGhastSpawnLocations.clear();
	activeGhastSpawnLocations.push_back(1300);
	activeGhastSpawnLocations.push_back(1600);
}

// Destroys the manager and all of its enemies its tracking
EnemyManagerC::~EnemyManagerC()
{
	enemyManagerInstance = nullptr;

	DeleteAllZombies();
	DeleteAllGhosts();
	DeleteAllPlants();
	DeleteAllDevils();
	DeleteAllBirds();
	DeleteAllGhasts();
}

// Called on delete, removes all potential zombies from the scene
void EnemyManagerC::DeleteAllZombies()
{
	const uint32_t size = activeZombies.size();
	for (uint32_t zombieIndex = 0; zombieIndex < size; ++zombieIndex)
	{
		delete activeZombies.at(zombieIndex);
	}
	activeZombies.clear();
}

// Called on delete, removes all potential ghosts from the scene
void EnemyManagerC::DeleteAllGhosts()
{
	const uint32_t size = activeGhosts.size();
	for (uint32_t ghostIndex = 0; ghostIndex < size; ++ghostIndex)
	{
		delete activeGhosts.at(ghostIndex);
	}
	activeGhosts.clear();
}

// Called on delete, removes all potential plants from the scene
void EnemyManagerC::DeleteAllPlants()
{
	const uint32_t size = activePlants.size();
	for (uint32_t plantIndex = 0; plantIndex < size; ++plantIndex)
	{
		delete activePlants.at(plantIndex);
	}
	activePlants.clear();
}

// Called on delete, removes all potential devils from the scene
void EnemyManagerC::DeleteAllDevils()
{
	const uint32_t size = activeDevils.size();
	for (uint32_t devilIndex = 0; devilIndex < size; ++devilIndex)
	{
		delete activeDevils.at(devilIndex);
	}
	activeDevils.clear();
}

// Called on delete, removes all potential birds from the scene
void EnemyManagerC::DeleteAllBirds()
{
	const uint32_t size = activeBirds.size();
	for (uint32_t birdIndex = 0; birdIndex < size; ++birdIndex)
	{
		delete activeBirds.at(birdIndex);
	}
	activeBirds.clear();
}

// Called on delete, removes all potential ghasts from the scene
void EnemyManagerC::DeleteAllGhasts()
{
	const uint32_t size = activeGhasts.size();
	for (uint32_t ghastIndex = 0; ghastIndex < size; ghastIndex++)
	{
		delete activeGhasts.at(ghastIndex);
	}
	activeGhasts.clear();
}

void EnemyManagerC::AddZombie(const GLfloat zombieXPosition, const GLfloat zombieYPosition)
{
	ZombieC* zombie = ZombieC::CreateInstance()->Initialize(zombieXPosition, zombieYPosition, zombieTexture);
	activeZombies.push_back(zombie);
}

// Remove a zombie from the scene, and ensure it was killed before triggering killed logic
void EnemyManagerC::RemoveZombie(const int32_t index, const bool8_t killed)
{
	activeZombies.erase(activeZombies.begin() + index);

	if (killed)
	{
		PlayerC::GetInstance()->AddScore(ZombieC::GetScore());
		AudioManagerC::GetInstance()->PlayKillEnemy1();
	}
}
void EnemyManagerC::AddGhost(const int32_t randomXOffset)
{
	GhostC* ghost = GhostC::CreateInstance()->Initialize(randomXOffset, ghostTexture);
	activeGhosts.push_back(ghost);
}

// Remove a ghost from the scene, and ensure it was killed before triggering killed logic
void EnemyManagerC::RemoveGhost(const int32_t index, const bool8_t killed)
{
	activeGhosts.erase(activeGhosts.begin() + index);

	if (killed)
	{
		PlayerC::GetInstance()->AddScore(GhostC::GetScore());
		AudioManagerC::GetInstance()->PlayKillEnemy2();
	}
}

void EnemyManagerC::AddPlant(const GLfloat plantXPosition, const GLfloat plantYPosition)
{
	PlantC* plant = PlantC::CreateInstance()->Initialize(plantXPosition, plantYPosition, plantTexture);
	activePlants.push_back(plant);
}

// Remove a plant from the scene
void EnemyManagerC::RemovePlant(const int32_t index)
{
	delete activePlants.at(index);
	activePlants.erase(activePlants.begin() + index);

	AudioManagerC::GetInstance()->PlayKillEnemy2();
	PlayerC::GetInstance()->AddScore(PlantC::GetScore());
}

void EnemyManagerC::AddDevil(const GLfloat devilXPosition, const GLfloat devilYPosition)
{
	DevilC* devil = DevilC::CreateInstance()->Initialize(devilXPosition, devilYPosition, devilTexture);
	activeDevils.push_back(devil);
}

// Remove a devil from the scene
void EnemyManagerC::RemoveDevil(const int32_t index)
{
	delete activeDevils.at(index);
	activeDevils.erase(activeDevils.begin() + index);

	AudioManagerC::GetInstance()->PlayKillEnemy2();
	PlayerC::GetInstance()->AddScore(DevilC::GetScore());
}

void EnemyManagerC::AddBird(const GLfloat birdXPosition, const GLfloat birdYPosition)
{
	BirdC* bird = BirdC::CreateInstance()->Initialize(birdXPosition, birdYPosition, birdTexture);
	activeBirds.push_back(bird);
}

// Remove a bird from the scene, and ensure it was killed before triggering killed logic
void EnemyManagerC::RemoveBird(const int32_t index, const bool8_t killed)
{
	activeBirds.erase(activeBirds.begin() + index);

	if (killed)
	{
		PlayerC::GetInstance()->AddScore(BirdC::GetScore());
		AudioManagerC::GetInstance()->PlayKillEnemy1();
	}
}

void EnemyManagerC::AddGhast(const GLfloat ghastXPosition)
{
	GhastC* ghast = GhastC::CreateInstance()->Initialize(ghastXPosition, ghastTexture);
	activeGhasts.push_back(ghast);
}

 // Remove a ghast from the scene
void EnemyManagerC::RemoveGhast(const int32_t index)
{
	delete activeGhasts.at(index);
	activeGhasts.erase(activeGhasts.begin() + index);

	AudioManagerC::GetInstance()->PlayKillEnemy1();
	PlayerC::GetInstance()->AddScore(GhastC::GetScore());
}

// Hub for updating all the enemies beased on the time passed
void EnemyManagerC::UpdateEnemies(const DWORD milliseconds)
{
	CheckZombieSpawns(milliseconds);
	UpdateZombies(milliseconds);

	CheckGhostSpawns();
	UpdateGhosts(milliseconds);

	UpdatePlants(milliseconds);
	UpdateDevils(milliseconds);
	UpdateBirds(milliseconds);

	CheckGhastSpawns();
	UpdateGhasts(milliseconds);

	return;
}

// Determines if a wave of zombies should be spawned based on the alloted time passed
void EnemyManagerC::CheckZombieSpawns(const DWORD milliseconds)
{
	// Update the cooldown and check if it is ready to spawn a set of zombies
	currentZombieSpawnTime += milliseconds;
	if (currentZombieSpawnTime >= spawnTime)
	{
		SpawnZombieWave(milliseconds);
	}

	return;
}

// Spawn a wave of zombies
void EnemyManagerC::SpawnZombieWave(const DWORD milliseconds)
{
	currentZombieSpawnTime = 0; // Reset the spawn timer
	const GLfloat playerXPosition = PlayerC::GetInstance()->GetXPosition();

	// Only play this sound once to prevent overlap and repeated sound
	AudioManagerC::GetInstance()->PlaySpawnZombie();

	// Generate an initial position for the zombnie that will be offsetted every time one is spawned
	GLfloat zombieSpawnXPosition;
	do
	{
		zombieSpawnXPosition = static_cast<GLfloat>(GetRangedRandom(minZombieXPosition, maxZombieXPosition));
		// Puts Zombie within the bounds of the screen
		zombieSpawnXPosition += playerXPosition;

	} while (!CanSpawnZombie(zombieSpawnXPosition, playerXPosition));

	for (int32_t zombieIndex = 0; zombieIndex < zombiesToSpawn; ++zombieIndex)
	{
		// Add a zombie to the scene
		AddZombie(zombieSpawnXPosition, BackgroundManagerC::GetInstance()->GetBottomPlatformPosition());

		// Calculate the distance between this zombie and the next one
		const int32_t betweenZombieOffset = GetRangedRandom(minDistanceBetweenZombies, maxDistanceBetweenZombies);
		// If the zombie is behind the player, subtract the offset, and if he's in front, add it
		if (zombieSpawnXPosition < 0) { zombieSpawnXPosition -= betweenZombieOffset; }
		else { zombieSpawnXPosition += betweenZombieOffset; }

		if (rand() % 2 == 1)
		{
			const GLfloat playerZombieXDifferential = zombieSpawnXPosition - playerXPosition;
			zombieSpawnXPosition -= playerZombieXDifferential * 2;
		}
	}

	return;
}

// Ensure Zombie doesn't go off screen and isn't on top of the player
bool8_t EnemyManagerC::CanSpawnZombie(const GLfloat randomXOffset, const GLfloat playerXPosition) noexcept
{
	const bool8_t isOffScreen = ((randomXOffset >= BackgroundManagerC::GetInstance()->GetRightWallPosition()) || (randomXOffset <= BackgroundManagerC::GetInstance()->GetLeftWallPosition()));
	const GLfloat playerZombieXDifferential = randomXOffset - playerXPosition;
	const bool8_t isOnPlayer = IsInRange(playerZombieXDifferential, -minZombieXPosition, minZombieXPosition);

	return (!isOffScreen && !isOnPlayer);
}

// Call each zombie's update
void EnemyManagerC::UpdateZombies(const DWORD milliseconds)
{
	bool8_t deathFlag = false;
	const int32_t size = activeZombies.size();
	for (int32_t zombieIndex = 0; zombieIndex < size; ++zombieIndex)
	{
		// If a zombie is at the end of its lifetime, remove it from the game
		deathFlag = activeZombies.at(zombieIndex)->Update(milliseconds);
		if (deathFlag)
		{
			RemoveZombie(zombieIndex, false);
			return;	// Return early, assume that two ghosts are never on the same x-position
		}
	}
}

// Determine if the player is past the point of a ghost spawn
void EnemyManagerC::CheckGhostSpawns()
{
	if (activeGhostSpawnLocations.empty()) { return; }
	const GLfloat playerXPosition = PlayerC::GetInstance()->GetXPosition();

	// If the player passes the barrier to spawn ghosts, spawn that wave
	if (playerXPosition >= activeGhostSpawnLocations.at(0))
	{
		SpawnGhostWave();
	}
}

// Spawn the waves and calculate random distance between them 
void EnemyManagerC::SpawnGhostWave()
{
	// Erase the wave location element
	activeGhostSpawnLocations.erase(activeGhostSpawnLocations.begin());

	for (int32_t ghostCount = 0; ghostCount < ghostsToSpawn; ++ghostCount)
	{
		// Spawn a ghost with varying distance between them
		const int32_t betweenGhostOffset = GetRangedRandom(0, maxDistanceBetweenGhosts + 1)
			+ (ghostCount * distanceBetweenGhostsMulti);	// Add 1 for inclusivity
		AddGhost(betweenGhostOffset);
	}

	// Play the sound only once even though many ghosts could spawn
	AudioManagerC::GetInstance()->PlayMoveGhost();
}

// Call each ghost's update method
void EnemyManagerC::UpdateGhosts(const DWORD milliseconds)
{
	bool8_t deathFlag = false;
	const int32_t size = activeGhosts.size();
	for (int32_t ghostIndex = 0; ghostIndex < size; ++ghostIndex)
	{
		// If a ghost is at the end of its lifetime, remove it from the game
		deathFlag = activeGhosts.at(ghostIndex)->Update(milliseconds);
		if (deathFlag)
		{
			RemoveGhost(ghostIndex, false);
			return;	// Return early, assume that two ghosts are never on the same x-position
		}
	}
}

// Call each plant's update method
void EnemyManagerC::UpdatePlants(const DWORD milliseconds)
{
	const int32_t size = activePlants.size();
	for (int32_t plantIndex = 0; plantIndex < size; ++plantIndex)
	{
		activePlants.at(plantIndex)->Update(milliseconds);
	}
}

// Call each devil's update method
void EnemyManagerC::UpdateDevils(const DWORD milliseconds)
{
	const int32_t size = activeDevils.size();
	for (int32_t devilIndex = 0; devilIndex < size; ++devilIndex)
	{
		activeDevils.at(devilIndex)->Update(milliseconds);
	}
}

// Call each bird's update method
void EnemyManagerC::UpdateBirds(const DWORD milliseconds)
{
	bool8_t deathFlag = false;
	const int32_t size = activeBirds.size();
	for (int32_t birdIndex = 0; birdIndex < size; ++birdIndex)
	{
		// If a bird is at the end of its lifetime, remove it from the game
		deathFlag = activeBirds.at(birdIndex)->Update(milliseconds);
		if (deathFlag)
		{
			RemoveBird(birdIndex, false);
			return;	// Return early, assume that two birds are never on the same x-position
		}
	}
}

// Determine if the player is past the point of a ghast spawn
void EnemyManagerC::CheckGhastSpawns()
{
	if (activeGhastSpawnLocations.empty()) { return; }
	const GLfloat playerXPosition = PlayerC::GetInstance()->GetXPosition();

	// If the player passes the barrier to spawn ghosts, spawn that wave
	if (playerXPosition >= activeGhastSpawnLocations.at(0))
	{
		activeGhastSpawnLocations.erase(activeGhastSpawnLocations.begin());

		// Spawn a ghast at an offset to the player
		const GLfloat spawnOffset = static_cast<GLfloat>(GetRangedRandom(minGhastRange, maxGhastRange + 1)); // Add 1 for inclusivity
		AddGhast(spawnOffset);

		// Play the sound only once even though many ghosts could spawn
		AudioManagerC::GetInstance()->PlayChaseGhast();
	}
}

// Call each ghast's update method
void EnemyManagerC::UpdateGhasts(const DWORD milliseconds)
{
	const int32_t size = activeGhasts.size();
	for (int32_t ghastIndex = 0; ghastIndex < size; ++ghastIndex)
	{
		activeGhasts.at(ghastIndex)->Update(milliseconds);
	}
}

void EnemyManagerC::RenderEnemies()
{
	RenderZombies();
	RenderGhosts();
	RenderPlants();
	RenderDevils();
	RenderBirds();
	RenderGhasts();
}

// Call each zombie's update method
void EnemyManagerC::RenderZombies()
{
	const int32_t size = activeZombies.size();
	for (int32_t zombieIndex = 0; zombieIndex < size; ++zombieIndex)
	{
		activeZombies.at(zombieIndex)->Render();
	}
}

// Call each ghost's update method
void EnemyManagerC::RenderGhosts()
{
	const int32_t size = activeGhosts.size();
	for (int32_t ghostIndex = 0; ghostIndex < size; ++ghostIndex)
	{
		activeGhosts.at(ghostIndex)->Render();
	}
}

// Call each plant's update method
void EnemyManagerC::RenderPlants()
{
	const int32_t size = activePlants.size();
	for (int32_t plantIndex = 0; plantIndex < size; ++plantIndex)
	{
		activePlants.at(plantIndex)->Render();
	}
}

// Call each devil's update method
void EnemyManagerC::RenderDevils()
{
	const int32_t size = activeDevils.size();
	for (int32_t devilIndex = 0; devilIndex < size; ++devilIndex)
	{
		activeDevils.at(devilIndex)->Render();
	}
}

// Call each bird's update method
void EnemyManagerC::RenderBirds()
{
	const int32_t size = activeBirds.size();
	for (int32_t birdIndex = 0; birdIndex < size; ++birdIndex)
	{
		activeBirds.at(birdIndex)->Render();
	}
}

// Call each ghast's update method
void EnemyManagerC::RenderGhasts()
{
	const int32_t size = activeGhasts.size();
	for (int32_t ghastIndex = 0; ghastIndex < size; ++ghastIndex)
	{
		activeGhasts.at(ghastIndex)->Render();
	}
}

#pragma warning (disable : 26446)	// Enables warning: "Use gsl::at() instead of unchecked subscript operator"
#pragma warning (disable : 26482)	// Enables warning: "Only index into arrays using constant expressions"