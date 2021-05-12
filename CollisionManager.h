#ifndef COLLISION_MANAGER_H		// Header Guard
#define COLLISION_MANAGER_H

class SpearC;
class PlayerC;

class CollisionManagerC final
{
public:
	static CollisionManagerC* CreateInstance();
	static CollisionManagerC* GetInstance() noexcept { return collisionManagerInstance; }
	
	CollisionManagerC(const CollisionManagerC& other) = delete;
	CollisionManagerC(CollisionManagerC&& other) noexcept = delete;
	CollisionManagerC& operator=(const CollisionManagerC& other) = delete;
	CollisionManagerC& operator=(CollisionManagerC&& other) noexcept = delete;
	~CollisionManagerC() { collisionManagerInstance = nullptr; }

	static void UpdateCollisions();

	typedef struct {
		GLfloat xLeft;
		GLfloat xRight;
		GLfloat yBottom;
		GLfloat yTop;
	}CollisionRectangle;

private:
	CollisionManagerC() = default;

	static bool8_t	CheckRectangleCollision(const CollisionRectangle rectangle1, const CollisionRectangle rectangle2) noexcept;

	static void SpearCollision();
	static void PlayerCollision(CollisionRectangle playerRectangle);

	static void EnemyCollision(const CollisionRectangle playerRectangle);
	static bool8_t CheckZombies(const CollisionRectangle rectangle, const SpearC* spear, const int32_t spearIndex);
	static bool8_t CheckGhosts(const CollisionRectangle rectangle, const SpearC* spear, const int32_t spearIndex);
	static bool8_t CheckPlants(const CollisionRectangle rectangle, const SpearC* spear, const int32_t spearIndex);
	static bool8_t CheckDevils(const CollisionRectangle rectangle, const SpearC* spear, const int32_t spearIndex);
	static bool8_t CheckBirds(const CollisionRectangle rectangle, const SpearC* spear, const int32_t spearIndex);
	static bool8_t CheckGhasts(const CollisionRectangle rectangle, const SpearC* spear, const int32_t spearIndex);

	static void GenericCollideEnemy(const GLfloat leftEnemy, const GLfloat bottomEnemy, const int32_t spearIndex, const bool8_t addFireOffset);

	static void ProjectileCollision(const CollisionRectangle playerRectangle);

	static bool8_t CheckVenoms(const CollisionRectangle playerRectangle);

	static void CollectibleCollision(const CollisionRectangle playerRectangle);
	static void CheckBags(const CollisionRectangle playerRectangle);
	static void CheckStatues(const CollisionRectangle playerRectangle);

	static void CheckKey(const CollisionRectangle playerRectangle);
	
	static CollisionManagerC* collisionManagerInstance;
};

#endif