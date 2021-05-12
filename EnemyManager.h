#pragma once

#include <gl/gl.h>
#include <vector>

class ZombieC;
class GhostC;
class PlantC;
class DevilC;
class BirdC;
class GhastC;

class EnemyManagerC final
{
public:
	static EnemyManagerC* CreateInstance();
	static EnemyManagerC* GetInstance() noexcept { return enemyManagerInstance; }
	
	EnemyManagerC(const EnemyManagerC& other) = delete;
	EnemyManagerC(EnemyManagerC&& other) noexcept = delete;
	EnemyManagerC& operator=(const EnemyManagerC& other) = delete;
	EnemyManagerC& operator=(EnemyManagerC&& other) noexcept = delete;
	~EnemyManagerC();

	static void UpdateEnemies(DWORD const milliseconds);
 
	static void AddZombie(const GLfloat zombieXPosition, const GLfloat zombieYPosition);
	static std::vector<ZombieC*>& GetZombies() noexcept { return activeZombies; }
	static void RemoveZombie(const int32_t index, const bool8_t killed);
 
	static void AddGhost(const int32_t randomXOffset);
	static std::vector<GhostC*>& GetGhosts() noexcept { return activeGhosts; }
	static void RemoveGhost(const int32_t index, const bool8_t killed);

	static void AddPlant(const GLfloat plantXPosition, const GLfloat plantYPosition);
	static std::vector<PlantC*>& GetPlants() noexcept { return activePlants; }
	static void RemovePlant(int32_t const index);

	static void AddDevil(const GLfloat devilXPosition, const GLfloat devilYPosition);
	static std::vector<DevilC*>& GetDevils() noexcept { return activeDevils; }
	static void RemoveDevil(int32_t index);

	static void AddBird(const GLfloat birdXPosition, const GLfloat birdYPosition);
	static std::vector<BirdC*>& GetBirds() noexcept { return activeBirds; }
	static void RemoveBird(const int32_t index, const bool8_t killed);

	static void AddGhast(const GLfloat ghastXPosition);
	static std::vector<GhastC*>& GetGhasts() noexcept { return activeGhasts; }
	static void RemoveGhast(const int32_t index);

	static void RenderEnemies();

private:
	EnemyManagerC() = default;

	/* Setups */
	static void LoadTextures() noexcept;
	static void SetupZombies() noexcept;
	static void SetupGhosts();
	static void SetupPlants();
	static void SetupDevils();
	static void SetupBirds();
	static void SetupGhasts();

	/* Updates */
	static void UpdateZombies(const DWORD milliseconds);
	static void UpdateGhosts(const DWORD milliseconds);
	static void UpdatePlants(const DWORD milliseconds);
	static void UpdateDevils(const DWORD milliseconds);
	static void UpdateBirds(const DWORD milliseconds);
	static void UpdateGhasts(const DWORD milliseconds);

	/* Spawning Helpers */
	static void CheckZombieSpawns(const DWORD milliseconds);
	static void SpawnZombieWave(const DWORD milliseconds);
	static bool8_t CanSpawnZombie(const GLfloat randomXOffset, const GLfloat playerXPosition) noexcept;
	static void CheckGhostSpawns();
	static void SpawnGhostWave();
	static void CheckGhastSpawns();

	/* Renders */
	static void RenderZombies();
	static void RenderGhosts();
	static void RenderPlants();
	static void RenderDevils();
	static void RenderBirds();
	static void RenderGhasts();

	void DeleteAllZombies();
	void DeleteAllGhosts();
	void DeleteAllPlants();
	void DeleteAllDevils();
	void DeleteAllBirds();
	void DeleteAllGhasts();

	static EnemyManagerC* enemyManagerInstance;

	/* Texture Indexes */
	static GLuint zombieTexture;
	static GLuint ghostTexture;
	static GLuint plantTexture;
	static GLuint devilTexture;
	static GLuint birdTexture;
	static GLuint ghastTexture;

	/* Dynamic Enemy Trackers */
	static std::vector<ZombieC*> activeZombies;
	static std::vector<GhostC*> activeGhosts;
	static std::vector<int32_t> activeGhostSpawnLocations;
	static std::vector<PlantC*> activePlants;
	static std::vector<DevilC*> activeDevils;
	static std::vector<BirdC*> activeBirds;
	static std::vector<GhastC*> activeGhasts;
	static std::vector<int32_t> activeGhastSpawnLocations;

	static const int32_t zombiesToSpawn = 3;
	static const int32_t numberStaticPlants = 4;
	static const int32_t numberStaticDevils = 2;
	static const int32_t numberStaticBirds = 2;
	static const int32_t ghostsToSpawn = 3;

	static const int32_t minZombieXPosition = 65;
	static const int32_t maxZombieXPosition = 135;
	static const int32_t minDistanceBetweenZombies = 5;
	static const int32_t maxDistanceBetweenZombies = 30;
	static const int32_t maxDistanceBetweenGhosts = 100;
	static const int32_t distanceBetweenGhostsMulti = 10;
	static const int32_t minGhastRange = 75;
	static const int32_t maxGhastRange = 100;

	static const float_t spawnTime;

	/* Animation Timers */
	static DWORD currentZombieSpawnTime;

	/* Plant Locations */
	static const GLfloat plantXPositions[numberStaticPlants];
	static const GLfloat plantYPositions[numberStaticPlants];

	/* Devil Locations */
	static const GLfloat devilXPositions[numberStaticDevils];
	static const GLfloat devilYPositions[numberStaticDevils];

	/* Bird Locations */
	static const GLfloat birdXPositions[numberStaticBirds];
	static const GLfloat birdYPositions[numberStaticBirds];
};