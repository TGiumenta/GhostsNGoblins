/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which manages all of the collisions and collision listeners.
				  Creates bounding boxes for each object to test overlap. Once rectangles are created,
				  call a helper function that determines if two rectangles overlap. If it does, consider
				  it a collision. At this point, interact with the two objects (spears/player/enemies) properly.
				  This manager will call the other managers in the solution to interact with those objects
				  in its proper scope. This includes deletion, updating animation states, and creating particle effects.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include <gl\GL.h>
#include "game.h"
#include "EnemyManager.h"
#include "WeaponManager.h"
#include "Player.h"
#include "Spear.h"
#include "Zombie.h"
#include "Ghost.h"
#include "Plant.h"
#include "Devil.h"
#include "Bird.h"
#include "Ghast.h"
#include "Player.h"
#include "Moneybag.h"
#include "Statue.h"
#include "CollectibleManager.h"
#include "Venom.h"
#include "ProjectileManager.h"
#include "Key.h"
#include "KeyManager.h"
#include "ParticleManager.h"
#include "CollisionManager.h"

CollisionManagerC* CollisionManagerC::collisionManagerInstance = nullptr;

// Creates a singleton instance of the manager
CollisionManagerC* CollisionManagerC::CreateInstance()
{
	if (collisionManagerInstance == nullptr)
	{
		collisionManagerInstance = new CollisionManagerC();
	}
	return collisionManagerInstance;
}

// Calls the wrapper to all the collision handlers after calculating the player rectangle
void CollisionManagerC::UpdateCollisions()
{
	const PlayerC* player = PlayerC::GetInstance();
	if (player == nullptr) { return; }

	// Create player rectangle
	CollisionRectangle playerRectangle{
		player->GetXPosition() + player->GetXCollisionOffset(),
		playerRectangle.xLeft + PlayerC::GetSpriteWidth() - player->GetXCollisionOffset(),
		player->GetYPosition(),
		playerRectangle.yBottom + PlayerC::GetSpriteHeight() - player->GetYCollisionOffset()
	};

	if (player->GetIsCrouching())
	{
		playerRectangle.yTop = playerRectangle.yTop - (PlayerC::GetSpriteHeight() / 2.f);
	}

	EnemyCollision(playerRectangle);
	ProjectileCollision(playerRectangle);
	CollectibleCollision(playerRectangle);
	CheckKey(playerRectangle);
}

bool8_t CollisionManagerC::CheckRectangleCollision(const CollisionRectangle& rectangle1, const CollisionRectangle& rectangle2) noexcept
{
	return !((rectangle1.xLeft >= rectangle2.xRight) || (rectangle1.xRight <= rectangle2.xLeft)
		|| (rectangle1.yBottom >= rectangle2.yTop) || (rectangle1.yTop <= rectangle2.yBottom));
}

void CollisionManagerC::EnemyCollision(const CollisionRectangle& playerRectangle)
{
	SpearCollision();
	PlayerCollision(playerRectangle);
}

// The wrapper for all of the collisions to do with the spear
void CollisionManagerC::SpearCollision()
{
	std::vector<SpearC*>& spears = WeaponManagerC::GetInstance()->GetSpears();
	const int32_t size = spears.size();
	for (int32_t spearIndex = 0; spearIndex < size; ++spearIndex)
	{
		// Create the collider for the spear
		CollisionRectangle spearRectangle {
			spears.at(spearIndex)->GetXPosition(),
			spearRectangle.xLeft + SpearC::GetSpriteWidth(),
			spears.at(spearIndex)->GetYPosition(),
			spearRectangle.yBottom + SpearC::GetSpriteHeight(),
		};

		// Check if the rectangles overlap for any and all enemies, ensure to pass the spear pointer properly, indicating it's checking a spear
		if (CheckZombies(spearRectangle, spears.at(spearIndex), spearIndex)) { return; }
		if (CheckGhosts(spearRectangle, spears.at(spearIndex), spearIndex)) { return; }
		if (CheckPlants(spearRectangle, spears.at(spearIndex), spearIndex)) { return; }
		if (CheckDevils(spearRectangle, spears.at(spearIndex), spearIndex)) { return; }
		if (CheckBirds(spearRectangle, spears.at(spearIndex), spearIndex)) { return; }
		if (CheckGhasts(spearRectangle, spears.at(spearIndex), spearIndex)) { return; }
	}
}

// The wrapper for all of the collisions to do with the player
void CollisionManagerC::PlayerCollision(const CollisionRectangle& playerRectangle)
{
	// Check if the rectangles overlap for any and all enemies, ensure to pass null for the spear pointer
	if (CheckZombies(playerRectangle, nullptr, -1)) { return; }
	if (CheckGhosts(playerRectangle, nullptr, -1)) { return; }
	if (CheckPlants(playerRectangle, nullptr, -1)) { return; }
	if (CheckDevils(playerRectangle, nullptr, -1)) { return; }
	if (CheckBirds(playerRectangle, nullptr, -1)) { return; }
	if (CheckGhasts(playerRectangle, nullptr, -1)) { return; }
}

// Determines if something collided with any zombies
bool8_t CollisionManagerC::CheckZombies(const CollisionRectangle& rectangle, const SpearC* spear, const int32_t spearIndex)
{
	std::vector<ZombieC*>& zombies = EnemyManagerC::GetInstance()->GetZombies();
	const int32_t size = zombies.size();
	for (int32_t zombieIndex = 0; zombieIndex < size; ++zombieIndex)
	{
		if (zombies.at(zombieIndex)->GetCurrentAnimationState() >= ZombieC::ZombieAnimation::WALKING_1)
		{
			// Create the collider for the zombie
			CollisionRectangle zombieRectangle{
				zombies.at(zombieIndex)->GetXPosition(),
				zombieRectangle.xLeft + ZombieC::GetSpriteWidth(),
				zombies.at(zombieIndex)->GetYPosition(),
				zombieRectangle.yBottom + ZombieC::GetSpriteHeight()
			};

			if (CheckRectangleCollision(rectangle, zombieRectangle))
			{
				// Collided!
				if (spear)
				{
					EnemyManagerC::GetInstance()->RemoveZombie(zombieIndex, true);
					GenericCollideEnemy(zombieRectangle.xLeft, zombieRectangle.yBottom, spearIndex, true);
					return true;
				}
				else
				{
					PlayerC::GetInstance()->TakeDamage();
					return true;
				}
			}
		}
	}

	return false;
}

// Determines if something collided with any ghosts
bool8_t CollisionManagerC::CheckGhosts(const CollisionRectangle& rectangle, const SpearC* spear, int32_t const spearIndex)
{
	std::vector<GhostC*>& ghosts = EnemyManagerC::GetInstance()->GetGhosts();
	const int32_t size = ghosts.size();
	for (int32_t ghostIndex = 0; ghostIndex < size; ++ghostIndex)
	{
		// Create the collider for the ghost
		CollisionRectangle ghostRectangle{
			ghosts.at(ghostIndex)->GetXPosition(),
			ghostRectangle.xLeft + GhostC::GetSpriteWidth(),
			ghosts.at(ghostIndex)->GetYPosition(),
			ghostRectangle.yBottom + GhostC::GetSpriteHeight(),
		};

		if (CheckRectangleCollision(rectangle, ghostRectangle))
		{
			// Collided!
			if (spear)
			{
				EnemyManagerC::GetInstance()->RemoveGhost(ghostIndex, true);
				GenericCollideEnemy(ghostRectangle.xLeft, ghostRectangle.yBottom, spearIndex, true);
				return true;
			}
			else
			{
				PlayerC::GetInstance()->TakeDamage();
				return true;
			}
		}
	}

	return false;
}

// Determine if something collided with any plants
bool8_t CollisionManagerC::CheckPlants(const CollisionRectangle& rectangle, const SpearC* spear, int32_t const spearIndex)
{
	std::vector<PlantC*>& plants = EnemyManagerC::GetInstance()->GetPlants();
	const int32_t size = plants.size();
	for (int32_t plantIndex = 0; plantIndex < size; ++plantIndex)
	{
		// Create the collider for the plant
		CollisionRectangle plantRectangle{
			plants.at(plantIndex)->GetXPosition(),
			plantRectangle.xLeft + PlantC::GetSpriteWidth(),
			plants.at(plantIndex)->GetYPosition(),
			plantRectangle.yBottom + PlantC::GetSpriteHeight()
		};

		if (CheckRectangleCollision(rectangle, plantRectangle))
		{
			// Collided!
			if (spear)
			{
				EnemyManagerC::GetInstance()->RemovePlant(plantIndex);
				GenericCollideEnemy(plantRectangle.xLeft, plantRectangle.yBottom, spearIndex, true);
				return true;
			}
			else
			{
				PlayerC::GetInstance()->TakeDamage();
				return true;
			}
		}
	}

	return false;
}

// Determine if something collided with any devils
bool8_t CollisionManagerC::CheckDevils(const CollisionRectangle& rectangle, const SpearC* spear, int32_t const spearIndex)
{
	std::vector<DevilC*>& devils = EnemyManagerC::GetInstance()->GetDevils();
	const int32_t size = devils.size();
	for (int32_t devilIndex = 0; devilIndex < size; devilIndex++)
	{
		// Create the collider for the devil
		CollisionRectangle devilRectangle{
			devils.at(devilIndex)->GetXPosition(),
			devilRectangle.xLeft + DevilC::GetSpriteWidth(),
			devils.at(devilIndex)->GetYPosition(),
			devilRectangle.yBottom + DevilC::GetSpriteHeight()
		};

		if (CheckRectangleCollision(rectangle, devilRectangle))
		{
			// Collided!
			if (spear)
			{
				EnemyManagerC::GetInstance()->RemoveDevil(devilIndex);
				GenericCollideEnemy(devilRectangle.xLeft, devilRectangle.yBottom, spearIndex, true);
				return true;
			}
			else
			{
				PlayerC::GetInstance()->TakeDamage();
				return true;
			}
		}
	}

	return false;
}

// Determine if something collided with any birds
bool8_t CollisionManagerC::CheckBirds(const CollisionRectangle& rectangle, const SpearC* spear, int32_t spearIndex)
{
	std::vector<BirdC*>& birds = EnemyManagerC::GetInstance()->GetBirds();
	const int32_t size = birds.size();
	for (int32_t birdIndex = 0; birdIndex < size; ++birdIndex)
	{
		// Create the collider for the bird
		CollisionRectangle birdRectangle{
			birds.at(birdIndex)->GetXPosition(),
			birdRectangle.xLeft + BirdC::GetSpriteWidth(),
			birds.at(birdIndex)->GetYPosition(),
			birdRectangle.yBottom + BirdC::GetSpriteHeight(),
		};

		if (CheckRectangleCollision(rectangle, birdRectangle))
		{
			// Collided!
			if (spear)
			{
				EnemyManagerC::GetInstance()->RemoveBird(birdIndex, true);
				GenericCollideEnemy(birdRectangle.xLeft, birdRectangle.yBottom, spearIndex, false);
				return true;
			}
			else
			{
				PlayerC::GetInstance()->TakeDamage();
				return true;
			}
		}
	}

	return false;
}

// Determine if something collided with any ghasts
bool8_t CollisionManagerC::CheckGhasts(const CollisionRectangle& rectangle, const SpearC* spear, int32_t spearIndex)
{
	std::vector<GhastC*>& ghasts = EnemyManagerC::GetInstance()->GetGhasts();
	const int32_t size = ghasts.size();
	for (int32_t ghastIndex = 0; ghastIndex < size; ++ghastIndex)
	{
		// Create the collider for the ghast
		CollisionRectangle ghastRectangle{
			ghasts.at(ghastIndex)->GetXPosition(),
			ghastRectangle.xLeft + GhastC::GetSpriteWidth(),
			ghasts.at(ghastIndex)->GetYPosition(),
			ghastRectangle.yBottom + GhastC::GetSpriteHeight()
		};

		if (CheckRectangleCollision(rectangle, ghastRectangle))
		{
			// Collided!
			if (spear)
			{
				EnemyManagerC::GetInstance()->RemoveGhast(ghastIndex);
				GenericCollideEnemy(ghastRectangle.xLeft, ghastRectangle.yBottom, spearIndex, false);
				return true;
			}
			else
			{
				PlayerC::GetInstance()->TakeDamage();
				return true;
			}
		}
	}

	return false;
}

// Wrapper for determining if the player collides with any of the projectiles
void CollisionManagerC::ProjectileCollision(const CollisionRectangle& playerRectangle)
{
	if (CheckVenoms(playerRectangle)) { return; }
}

// Determine if the player collided with any projectile
bool8_t CollisionManagerC::CheckVenoms(const CollisionRectangle& playerRectangle)
{
	std::vector<VenomC*>& venoms = ProjectileManagerC::GetInstance()->GetVenoms();
	const int32_t size = venoms.size();

	for (int32_t venomIndex = 0; venomIndex < size; ++venomIndex)
	{
		// Create the collider for the venom
		CollisionRectangle venomRectangle{
			venoms.at(venomIndex)->GetXPosition() + 2,
			venomRectangle.xLeft + VenomC::GetSpriteWidth() - 2,
			venoms.at(venomIndex)->GetYPosition(),
			venomRectangle.yBottom + VenomC::GetSpriteHeight()
		};

		if (CheckRectangleCollision(playerRectangle, venomRectangle))
		{
			// Collided!
			PlayerC::GetInstance()->TakeDamage();
			return true;
		}
	}

	return false;
}

// Wrapper for determining if the player collides with any of the collectibles
void CollisionManagerC::CollectibleCollision(const CollisionRectangle& playerRectangle)
{
	CheckBags(playerRectangle);
	CheckStatues(playerRectangle);
}

// Determine if the player collided with any bags
void CollisionManagerC::CheckBags(const CollisionRectangle& playerRectangle)
{
	std::vector<MoneybagC*>& bags = CollectibleManagerC::GetInstance()->GetBags();
	const int32_t size = bags.size();
	for (int32_t bagIndex = 0; bagIndex < size; ++bagIndex)
	{
		// Create the collider for the bag
		CollisionRectangle bagRectangle{
			bags.at(bagIndex)->GetXPosition(),
			bagRectangle.xLeft + MoneybagC::GetSpriteWidth(),
			bags.at(bagIndex)->GetYPosition(),
			bagRectangle.yBottom + MoneybagC::GetSpriteHeight(),
		};

		if (CheckRectangleCollision(playerRectangle, bagRectangle))
		{
			// Collided!
			CollectibleManagerC::GetInstance()->RemoveBag(bagIndex);
			return;
		}
	}
}

// Detrmine if the player collided with any statues
void CollisionManagerC::CheckStatues(const CollisionRectangle& playerRectangle)
{
	std::vector<StatueC*>& statues = CollectibleManagerC::GetInstance()->GetStatues();
	const int32_t size = statues.size();
	for (int32_t statueIndex = 0; statueIndex < size; ++statueIndex)
	{
		// Create the collider for the statue
		CollisionRectangle statueRectangle {
			statues.at(statueIndex)->GetXPosition(),
			statueRectangle.xLeft + StatueC::GetSpriteWidth(),
			statues.at(statueIndex)->GetYPosition(),
			statueRectangle.yBottom + StatueC::GetSpriteHeight(),
		};

		if (CheckRectangleCollision(playerRectangle, statueRectangle))
		{
			// Collided!
			CollectibleManagerC::GetInstance()->RemoveStatue(statueIndex);
			return;
		}
	}
}

// Determine if the player collided with any keys
void CollisionManagerC::CheckKey(const CollisionRectangle& playerRectangle)
{
	std::vector<KeyC*>& keys = KeyManagerC::GetInstance()->GetKeys();
	const int32_t size = keys.size();

	for (int32_t keyIndex = 0; keyIndex < size; ++keyIndex)
	{
		// Create the collider for the key
		CollisionRectangle keyRectangle{
			keys.at(keyIndex)->GetXPosition(),
			keyRectangle.xLeft + KeyC::GetSpriteWidth(),
			keys.at(keyIndex)->GetYPosition(),
			keyRectangle.yBottom + KeyC::GetSpriteHeight()
		};

		if (CheckRectangleCollision(playerRectangle, keyRectangle))
		{
			// Collided!
			KeyManagerC::GetInstance()->RemoveKey(keyIndex);
			CGame::GetInstance()->EndGame(false);
			return;
		}
	}
}

void CollisionManagerC::GenericCollideEnemy(const GLfloat leftEnemy, const GLfloat bottomEnemy, const int32_t spearIndex, const bool8_t addFireOffset)
{
	CollectibleManagerC::GetInstance()->CalculateDrops(leftEnemy, bottomEnemy);
	WeaponManagerC::GetInstance()->RemoveSpear(spearIndex);

	if (addFireOffset)
	{
		ParticleManagerC::GetInstance()->AddFire(leftEnemy, bottomEnemy + FIRE_DEATH_OFFSET);
	}
	else
	{
		ParticleManagerC::GetInstance()->AddFire(leftEnemy, bottomEnemy);
	}
}