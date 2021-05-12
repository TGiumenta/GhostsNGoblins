#pragma once

#include <gl/GL.h>

class ZombieC
{
public:
	static ZombieC* CreateInstance();
	
	ZombieC(const ZombieC& other) = delete;
	ZombieC(ZombieC&& other) noexcept = delete;
	ZombieC& operator=(const ZombieC& other) = delete;
	ZombieC& operator=(ZombieC&& other) noexcept = delete;
	~ZombieC() = default;

	ZombieC* Initialize(const GLfloat initialXPosition, const GLfloat initialYPosition, const GLuint textureIndex) noexcept;
	bool8_t	Update(const DWORD milliseconds) noexcept;
	void Render() noexcept;

	// Set of zombie animations based on the sprite sheet
	typedef enum {
		INVALID,
		GROUND_1,
		GROUND_2,
		GROUND_3,
		GROUND_4,
		GROUND_5,
		WALKING_1,
		WALKING_2,
		ZOMBIE_MAX_STATES
	}ZombieAnimation;

	/* Getters and Setters */
	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat	GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }

	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }

	void SetCurrentAnimationState(const int32_t animationState) noexcept { currentAnimationState = animationState; }
	int32_t GetCurrentAnimationState() noexcept { return currentAnimationState; }
	const int32_t GetCurrentAnimationState() const noexcept { return currentAnimationState; }

	void SetIsFacingRight(const bool8_t rightState) noexcept { isFacingRight = rightState; }
	bool8_t GetIsFacingRight() noexcept { return isFacingRight; }
	const bool8_t GetIsFacingRight() const noexcept { return isFacingRight; }

	static const GLuint GetSpriteWidth() noexcept { return spriteWidth; }
	static const GLuint GetSpriteHeight() noexcept { return spriteHeight; }
	static const int GetScore() noexcept { return score; }

private:
	ZombieC() = default;
	void AnimationStateChange(const DWORD milliseconds) noexcept;
	void MoveZombie(const DWORD milliseconds) noexcept;
	void UpdateAnimation(const DWORD milliseconds) noexcept;

	/* Animation States */
	int32_t currentAnimationState;
	bool8_t isFacingRight;

	/* Animation Update Timers */
	DWORD groundTimeCurrent;
	static const GLfloat groundUpdateDeltaTime;

	/* Position Info */
	GLfloat xPosition;
	GLfloat yPosition;
	static const GLfloat xVelocity;

	/* Texture Index */
	GLuint texture;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;
	static const GLuint heightOffset = 1;

	static const int32_t score = 200;
};