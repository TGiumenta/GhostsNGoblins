#pragma once

#include "baseTypes.h"

class FireDeathC final
{
public:
	static FireDeathC* CreateInstance(); 

	FireDeathC(const FireDeathC& other) = delete;
	FireDeathC(FireDeathC&& other) noexcept = delete;
	FireDeathC& operator=(const FireDeathC& other) = delete;
	FireDeathC& operator=(FireDeathC&& other) noexcept = delete;
	~FireDeathC() = default;

	FireDeathC*	Initialize(const GLfloat initialXPosition, const GLfloat initialYPosition, const GLuint textureIndex) noexcept;
	bool8_t	Update(const DWORD milliseconds) noexcept;
	void Render() noexcept;

	// Set of fire animations based on the sprite sheet
	typedef enum {
		INVALID_FIRE,
		FIRE_DEATH_1,
		FIRE_DEATH_2,
		FIRE_DEATH_3,
		FIRE_DEATH_4,
		FIRE_DEATH_5,
		FIRE_DEATH_6,
		FIRE_MAX_STATES
	}FireDeathAnimation;

	/* Setters and Getters */
	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat	GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }

	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat	GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }

	void SetCurrentAnimationState(const int32_t animationState) noexcept { currentAnimationState = animationState; }
	int32_t	GetCurrentAnimationState() noexcept { return currentAnimationState; }
	const int32_t GetCurrentAnimationState() const noexcept { return currentAnimationState; }

private:
	FireDeathC() = default;

	/* Animation Update Timers */
	DWORD currentTimeAnimation;
	int32_t currentAnimationState;

	/* Position Info */
	GLfloat xPosition;
	GLfloat yPosition;

	/* Texture Index */
	GLuint texture;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;
	static const GLuint deathUpdateTimer;
};