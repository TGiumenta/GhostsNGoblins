#pragma once
#include <vector>

class MoneybagC;
class StatueC;

class CollectibleManagerC
{
public:
	static CollectibleManagerC*	CreateInstance();
	static CollectibleManagerC*	GetInstance() noexcept { return collectibleManagerInstance; }

	CollectibleManagerC(const CollectibleManagerC& other) = delete;
	CollectibleManagerC(CollectibleManagerC&& other) noexcept = delete;
	CollectibleManagerC& operator=(const CollectibleManagerC& other) = delete;
	CollectibleManagerC& operator=(CollectibleManagerC&& other) noexcept = delete;
	~CollectibleManagerC();
	
	static void UpdateCollectibles(const DWORD milliseconds);
	static void RenderCollectibles();

	static void AddBag(const GLfloat xPosition, const GLfloat yPosition);
	static std::vector<MoneybagC*>& GetBags() noexcept { return activeMoneybags; }
	static void RemoveBag(const int32_t position);

	static void CalculateDrops(const GLfloat xPosition, const GLfloat yPosition);
	static void AddStatue(const GLfloat xPosition, const GLfloat yPosition);
	static std::vector<StatueC*>& GetStatues() noexcept { return activeStatues; }
	static void RemoveStatue(const int32_t position);

private:
	CollectibleManagerC() = default;

	static void LoadTextures() noexcept;
	static void SetupBags();
		 
	static void UpdateBags(const DWORD milliseconds);
	static void UpdateStatues(const DWORD milliseconds);
 	 
	static void RenderBags();
	static void RenderStatues();

	static void DeleteAllBags();
	static void DeleteAllStatues();
 	 
	static bool8_t CalculateStatueDrop(const GLfloat xPosition, const GLfloat yPosition);
	static bool8_t CalculateMoneybagDrop(const GLfloat xPosition, const GLfloat yPosition);
 
	static void GenericCollection(const int32_t scoreToAdd);

	static CollectibleManagerC* collectibleManagerInstance;

	/* Texture Indexes */
	static GLuint moneybagTexture;
	static GLuint statueTexture;

	/* Dynamic Collectible Trackers */
	static std::vector<MoneybagC*> activeMoneybags;
	static std::vector<StatueC*> activeStatues;
	static const uint32_t moneybagDropChance = 20;
	static const uint32_t statueDropChance = 10;

	/* Static Initial Collectibles */
	static const uint32_t numberInitialMoneybags = 6;
	static GLfloat initialBagLocations[numberInitialMoneybags];
};