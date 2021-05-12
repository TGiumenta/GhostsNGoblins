#pragma once

#include <gl/GL.h>						// Header File For The OpenGL32 Library
#include "baseTypes.h"

class PlayerC final
{
public:
	static PlayerC*	CreateInstance();
	static PlayerC*	GetInstance() noexcept { return playerInstance; };
	
	PlayerC(const PlayerC& other) = delete;
	PlayerC(PlayerC&& other) noexcept = delete;
	PlayerC& operator=(const PlayerC& other) = delete;
	PlayerC& operator=(PlayerC&& other) noexcept = delete;
	~PlayerC() { playerInstance = nullptr; }

	void Update(const DWORD milliseconds);
	void Render(const DWORD milliseconds);

	// The potential input states from the user
	typedef enum {
		IDLE,
		RIGHT,
		LEFT,
		CROUCH,
		JUMP,
		CLIMB,
		ATTACK,
		PLAYER_MAX_MOVE_STATES
	}MovementState;

	// Set of zombie animations based on the sprite sheet
	typedef enum {
		INVALID,
		IDLING,
		CROUCHED,
		RUNNING_1,
		IDLE_2,
		RUNNING_2,
		JUMPING,
		CLIMB_LEFT_ARM,			// Unused
		CLIMB_RIGHT_ARM,		// Unused
		CLIMB_END,				// Unused
		THROW_WEP_STAND_1,
		THROW_WEP_STAND_2,
		THROW_WEP_CROUCH_1,
		THROW_WEP_CROUCH_2,
		BEAT_BOSS,
		PLAYER_MAX_ANIMATION_STATES
	}PlayerAnimation;

	// Set of armor types
	typedef enum {
		INVALIDARMOR,
		ARMOR,
		NOARMOR,
		DEAD,
		PLAYER_MAX_ARMOR_STATES
	}ArmorState;

	/* Input Handlers */
	void SetInputMovingRight(const bool8_t movementState) noexcept { movementStates[MovementState::RIGHT] = movementState; }
	void SetInputMovingLeft(const bool8_t movementState) noexcept { movementStates[MovementState::LEFT] = movementState; }
	void SetInputCrouched(const bool8_t movementState) noexcept { movementStates[MovementState::CROUCH] = movementState; }
	void SetInputJumped(const bool8_t jumpState) noexcept;
	void SetInputIdle(const bool8_t idleState) noexcept { movementStates[MovementState::IDLE] = idleState; }
	void SetInputAttacking(const bool8_t attackState) noexcept { movementStates[MovementState::ATTACK] = attackState; }

	/* Setters and Getters */
	void SetIsInAir(const bool8_t airState) noexcept { isInAir = airState; };
	bool8_t	GetIsInAir() noexcept { return isInAir; }
	const bool8_t GetIsInAir() const noexcept { return isInAir; }

	void SetCurrentAnimationState(const int32_t state) noexcept { currentAnimationState = state; }
	int32_t GetCurrentAnimationState() noexcept { return currentAnimationState; }
	const int32_t GetCurrentAnimationState() const noexcept { return currentAnimationState; }

	void SetArmorState(const int32_t state) noexcept { currentArmorState = state; }
	int32_t	GetArmorState() noexcept { return currentArmorState; }
	const int32_t GetArmorState() const noexcept { return currentArmorState; }

	void SetArmorTexture(const int32_t armorState);
	GLuint GetArmorTexture() noexcept { return currentTexture; }
	const GLuint GetArmorTexture() const noexcept { return currentTexture; }

	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat	GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }

	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat	GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }

	void SetYVelocity(const GLfloat velocity) noexcept { yVelocity = velocity; }
	GLfloat	GetYVelocity() noexcept { return yVelocity; }
	const GLfloat GetYVelocity() const noexcept { return yVelocity; }

	void SetIsFacingRight(const bool8_t rightState) noexcept { isFacingRight = rightState; }
	bool8_t	GetIsFacingRight() noexcept { return isFacingRight; }
	const bool8_t GetIsFacingRight() const noexcept { return isFacingRight; }

	void SetIsAttacking(const bool8_t attackState) noexcept { isAttacking = attackState; }
	bool8_t	GetIsAttacking() noexcept { return isAttacking; }
	const bool8_t GetIsAttacking() const noexcept { return isAttacking; }

	void SetIsCrouching(const bool8_t crouchState) noexcept { isCrouching = crouchState; }
	bool8_t	GetIsCrouching() noexcept { return isCrouching; }
	const bool8_t GetIsCrouching() const noexcept { return isCrouching; }

	void SetIsInDeathAnimation(const bool8_t deathAnimation) noexcept { isInDeathAnimation = deathAnimation; }
	int32_t	GetIsInDeathAnimation() noexcept { return isInDeathAnimation; }
	const int32_t GetIsInDeathAnimation() const noexcept { return isInDeathAnimation; }

	void SetCurrentDeathAnimationState(int32_t const deathAnimationState) noexcept { currentDeathAnimationState = deathAnimationState; }
	int32_t	GetCurrentDeathAnimationState() noexcept { return currentDeathAnimationState; }
	const int32_t GetCurrentDeathAnimationState() const noexcept { return currentDeathAnimationState; }

	void SetIsInInvincibleFrame(const int32_t invincibleFrameState) noexcept { isInInvincibilityFrame = invincibleFrameState; }
	int32_t	GetIsInInvincibleFrame() noexcept { return isInInvincibilityFrame; }
	const int32_t GetIsInInvincibleFrame() const noexcept { return isInInvincibilityFrame; }

	void SetIsPreviouslyInvincible(const int32_t invincibilityState) noexcept { previousInvincible = invincibilityState; }
	int32_t	GetIsPreviouslyInvincible() noexcept { return previousInvincible; }
	const int32_t GetIsPreviouslyInvincible() const noexcept { return previousInvincible; }

	void ToggleDebugMode() noexcept { debugMode = !debugMode; }
	bool8_t	GetDebugMode() noexcept { return debugMode; }
	const bool8_t GetDebugMode() const noexcept { return debugMode; }
	
	void TakeDamage();
	void SetScore(const int32_t scoreToSet) noexcept { score = scoreToSet; }
	int32_t	GetScore() noexcept { return score; }
	const int32_t GetScore() const noexcept { return score; }
	void AddScore(const int32_t scoreToAdd) noexcept { SetScore(score + scoreToAdd); }

	static const GLuint GetSpriteWidth() noexcept { return spriteWidth; }
	static const GLuint GetSpriteHeight() noexcept { return spriteHeight; }
	static const GLfloat GetXCollisionOffset() noexcept { return xCollisionOffset; }
	static const GLfloat GetYCollisionOffset() noexcept { return yCollisionOffset; }

private:
	PlayerC();
	void SetInitialStates() noexcept;
	void LoadTextures() noexcept;

	GLfloat	HandleStateChange(DWORD const milliseconds);
	void IdleStateChange() noexcept;
	void MoveRightStateChange(DWORD const milliseconds, float_t* deltaX);
	void MoveLeftStateChange(DWORD const milliseconds, float_t* deltaX);
	void CrouchStateChange(float_t* deltaX) noexcept;
	void JumpStateChange();
	void AttackStateChange(float_t* deltaX) noexcept;
	bool8_t	CanMove() noexcept;
	void MoveCamera();

	bool8_t	CanSwapAnimations() noexcept;
	void SwapRunningAnimationState() noexcept;
	void Attack(DWORD const milliseconds);
	void CrouchAttacking(DWORD const milliseconds);
	void StandAttacking(DWORD const milliseconds);
	void ThrowSpear();

	void KillPlayer();

	bool8_t	RenderIFrame(DWORD const milliseconds) noexcept;
	void RenderDeathAnimation(DWORD const milliseconds);
	
	static PlayerC* playerInstance;

	/* State Info */
	int32_t currentAnimationState;			// Indexes into the sprite sheet to get the correct animation
	int32_t currentArmorState;				// Indexes into the enum to get the correct armor type
	int32_t currentDeathAnimationState;		// Indexes into the sprite sheet to get the correct death animation

	bool8_t movementStates[MovementState::PLAYER_MAX_MOVE_STATES] = { false };
	bool8_t isInAir;
	bool8_t isFacingRight = true;
	bool8_t isAttacking = false;
	bool8_t isCrouching;
	bool8_t isInInvincibilityFrame = false;
	bool8_t previousInvincible = false;
	bool8_t isInDeathAnimation = false;
	bool8_t debugMode = false;

	/* Animation Update Timers */
	DWORD currentTimeRight = 0;
	DWORD currentTimeLeft = 0;
	DWORD currentTimeAttack = 0;
	DWORD currentTimeAttackAnimation = 0;
	DWORD currentTimeInvinsibility = 0;
	DWORD currentTimeDeathAnimation = 0;
	static const GLfloat runningDeltaTime;
	static const GLfloat attackDeltaTime;
	static const GLfloat attackAnimDeltaTime;
	static const GLfloat invincibilityDeltaTime;
	static const GLfloat deathAnimDeltaTime;

	/* Position and Velocity Info */
	GLfloat xPosition;
	GLfloat yPosition;
	GLfloat yVelocity;
	
	/* Statistics */
	int32_t score = 0;

	/* Texture Indexes */
	GLuint armorTexture;
	GLuint noArmorTexture;
	GLuint deathTexture;
	GLuint currentTexture;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;
	static const GLfloat xCollisionOffset;
	static const GLfloat yCollisionOffset;

	static const GLfloat startingXposition;
	static const GLfloat movementSpeed;
	static const GLfloat jumpYVelocity;
	static const GLfloat downAcceleration;
};