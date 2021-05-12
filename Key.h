#pragma once

#include "baseTypes.h"

class KeyC final
{
public:
	static KeyC* CreateInstance();
	
	KeyC(const KeyC& other) = delete;
	KeyC(KeyC&& other) noexcept = delete;
	KeyC& operator=(const KeyC& other) = delete;
	KeyC& operator=(KeyC&& other) noexcept = delete;
	~KeyC() = default;

	KeyC* Initialize(const GLfloat initialXPosition, const GLuint textureIndex) noexcept;
	void Update(const DWORD milliseconds) noexcept;
	void Render() noexcept;

	// Set of key animations based on the sprite sheet
	typedef enum {
		INVALID,
		WHITE_1,
		WHITE_2,
		RED_1,
		RED_2,
		BROWN_1,
		BROWN_2,
		KEY_MAX_STATES
	}KeyAnimation;

	/* Setters and Getters */
	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat	GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }

	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat	GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }
	
	void SetCurrentAnimationState(const int32_t animationState) noexcept { currentAnimationState = animationState; }
	int32_t GetCurrentAnimationState() noexcept { return currentAnimationState; }
	const int32_t GetCurrentAnimationState() const noexcept { return currentAnimationState; }

	static const GLuint GetSpriteWidth() noexcept { return spriteWidth; }
	static const GLuint GetSpriteHeight() noexcept { return spriteHeight; }

private:
	KeyC() = default;

	/* Animation Update Timers */
	DWORD keyTimeCurrent = 0;
	int32_t currentAnimationState = 0;
	static const GLfloat updateDeltaTime;

	/* Position Info */
	GLfloat xPosition = 0;
	GLfloat yPosition = 0;

	/* Texture Index */
	GLuint texture = 0;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;
};