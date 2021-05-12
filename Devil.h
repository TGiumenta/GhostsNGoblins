#pragma once
#include "baseTypes.h"

class DevilC
{
public:
	static DevilC* CreateInstance();

	DevilC(const DevilC& other) = delete;
	DevilC(DevilC&& other) noexcept = delete;
	DevilC& operator=(const DevilC& other) = delete;
	DevilC& operator=(DevilC&& other) noexcept = delete;
	virtual	~DevilC() = default;

	DevilC*	Initialize(const GLfloat xPosition, const GLfloat yPosition, const GLuint textureIndex) noexcept;
	void Update(const DWORD milliseconds) noexcept;
	void Render() noexcept;

	typedef enum {
		INVALID,
		IDLE,
		FLY_1,
		FLY_2,
		SHOOT_PROJECTILE, 
		DEVIL_MAX_STATES
	}DevilAnimation;

	/* Setters and Getters */
	void SetIsFacingLeft(const bool8_t facingDirection) noexcept { isFacingLeft = facingDirection; };
	bool8_t	GetIsFacingLeft() noexcept { return isFacingLeft; }
	const bool8_t GetIsFacingLeft() const noexcept { return isFacingLeft; }

	void SetFightingMode(const bool8_t fightingMode) noexcept { isInFightingMode = fightingMode; }
	bool8_t	GetIsInFightingMode() noexcept { return isInFightingMode; }
	const bool8_t GetIsInFightingMode() const noexcept { return isInFightingMode; }
	
	void SetIsFlightPathSet(const bool8_t flightPath) noexcept { isFlightPathSet = flightPath; }
	bool8_t	GetIsFlightPathSet() noexcept { return isFlightPathSet; }
	const bool8_t GetIsFlightPathSet() const noexcept { return isFlightPathSet; }

	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat	GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }

	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat	GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }

	void SetCurrentXVelocity(const GLfloat velocity) noexcept { xVelocity = velocity; }
	GLfloat	GetCurrentXVelocity() noexcept { return xVelocity; }
	const GLfloat GetCurrentXVelocity() const noexcept { return xVelocity; }

	void SetYVelocity(const GLfloat velocity) noexcept { yVelocity = velocity; }
	GLfloat	GetYVelocity() noexcept { return yVelocity; }
	const GLfloat GetYVelocity() const noexcept { return yVelocity; }

	void SetCurrentAnimationState(int32_t const animationState) noexcept { currentAnimationState = animationState; }
	int32_t	GetCurrentAnimationState() noexcept { return currentAnimationState; }
	const int32_t GetCurrentAnimationState() const noexcept { return currentAnimationState; }

	static const GLuint GetSpriteWidth() noexcept { return spriteWidth; }
	static const GLuint GetSpriteHeight() noexcept { return spriteHeight; }
	static const int GetScore() noexcept { return score; }

protected:
	DevilC() = default;

private:
	DWORD GenerateRandomIdleTime() noexcept;
	void SetFlightPath(GLfloat const playerXPosition, DWORD const milliseconds) noexcept;
	void DetermineFlightPathEnd(GLfloat const playerXPosition) noexcept;
	void SetPositionsOnFlightPath(GLfloat const playerXPosition, DWORD const milliseconds) noexcept;
	void SetPositionsOffFlightPath(DWORD const milliesconds, GLfloat const playerXPosition, bool const setVelocityInfo) noexcept;
	void AnimationChange(DWORD const milliseconds) noexcept;
	void ResetAnimation() noexcept { currentAnimationState = DevilAnimation::FLY_1; }

	/* Animation State Updates */
	DWORD idleTimeCurrent = 0;
	static const GLfloat minIdleDeltaTime;
	static const GLfloat maxIdleDeltaTime;
	DWORD flyTimeCurrent = 0;
	static const GLfloat flyDeltaTime;
	DWORD randomUpdateDeltaTime = 0;
 
	/* State Info */
	int32_t currentAnimationState = DevilAnimation::IDLE;
	bool8_t isFacingLeft = true;
	bool8_t isInFightingMode = false;
	bool8_t isFlightPathSet = false;

	/* Position and Velocity Info */
	GLfloat xPosition = 0;
	GLfloat yPosition = 0;
	static const GLfloat minXPosition;
	static const GLfloat maxXPosition;

	GLfloat xVelocity = 0.0f;
	GLfloat yVelocity = 0.063f;
	static const GLfloat potentialXVelocity;
	static const GLfloat positionRange;

	static const int32_t score = 1000;

	/* Texture Info */
	GLuint texture;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;
};