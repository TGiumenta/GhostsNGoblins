#pragma once

#include <gl/GL.h>

class GhastC
{
public:
	static GhastC* CreateInstance();
	
	GhastC(const GhastC& other) = delete;
	GhastC(GhastC&& other) noexcept = delete;
	GhastC& operator=(const GhastC& other) = delete;
	GhastC& operator=(GhastC&& other) noexcept = delete;
	virtual	~GhastC() = default;

	GhastC*	Initialize(GLfloat const randomXOffset, GLuint const textureIndex) noexcept;
	void Update(DWORD const milliseconds) noexcept;
	void Render() noexcept;

	typedef enum {
		INVALID,
		SPAWN_1,
		SPAWN_2,
		ATTACKING_1,
		ATTACKING_2,
		GHAST_MAX_STATES
	}GhastAnimation;

	/* Setters and Getters */
	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat	GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }

	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat	GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }

	void SetXVelocity(const GLfloat velocity) noexcept { xVelocity = velocity; }
	GLfloat GetXVelocity() noexcept { return xVelocity; }
	const GLfloat GetXVelocity() const noexcept { return xVelocity; }

	void SetCurrentAnimationState(const int32_t animationState) noexcept { currentAnimationState = animationState; }
	int32_t	GetCurrentAnimationState() noexcept { return currentAnimationState; }
	const int32_t GetCurrentAnimationState() const noexcept { return currentAnimationState; }

	void SetIsFacingLeft(const bool8_t facingDirection) noexcept { isFacingLeft = facingDirection; }
	bool8_t	GetIsFacingLeft() noexcept { return isFacingLeft; }
	const bool8_t GetIsFacingLeft() const noexcept { return isFacingLeft; }

	static GLuint GetSpriteWidth() noexcept { return spriteWidth; }
	static GLuint GetSpriteHeight() noexcept { return spriteHeight; }
	static int GetScore() noexcept { return score; }

protected:
	GhastC() = default;

private:
	void AnimationStateChange(const DWORD milliseconds) noexcept;
	void UpdateAnimation(const DWORD milliseconds) noexcept;
	void MoveGhast(const DWORD milliseconds) noexcept;
	void AdjustYPosition() noexcept;

	/* Animation Timers */
	DWORD flyTimeCurrent;
	DWORD flightPathCurrent = 0;
	static const GLfloat floatUpdateTime;
	static const GLfloat pathUpdateTime;

	/* Animation States */
	int32_t currentAnimationState = GhastAnimation::SPAWN_1;
	bool8_t isFacingLeft = true;

	/* Position and Velocity Info */
	GLfloat xPosition = 0;
	GLfloat yPosition = 0;
	static const GLfloat potentialXVelocity;
	GLfloat xVelocity;

	/* Texture Index */
	GLuint texture;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;

	static const int32_t score = 100;
};