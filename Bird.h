#pragma once

class BirdC
{
public:
	static BirdC* CreateInstance();

	BirdC(const BirdC& other) = delete;
	BirdC(BirdC&& other) noexcept = delete;
	BirdC& operator=(const BirdC& other) = delete;
	BirdC& operator=(BirdC&& other) noexcept = delete;
	virtual ~BirdC() = default;

	BirdC* Initialize(const GLfloat initialXPosition, const GLfloat initialYPosition, const GLuint textureIndex) noexcept;
	bool8_t Update(const DWORD milliseconds) noexcept;
	void Render() noexcept;

	typedef enum {
		INVALID,
		IDLE,
		SCREAM,
		FLY_1,
		FLY_2,
		BIRD_MAX_STATES
	}BirdAnimation;

	/* Setters and Getters */
	void SetIsInRange (const bool8_t inRange) noexcept { isInRange = inRange; }
	bool8_t	GetInRange() noexcept { return isInRange; };
	const bool8_t GetInRange() const noexcept { return isInRange; };
	
	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat	GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }
	
	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat	GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }
	
	void SetCurrentAnimationState(const int32_t newState) noexcept { currentAnimationState = newState; }
	int32_t	GetCurrentAnimationState() noexcept { return currentAnimationState; }
	const int32_t GetCurrentAnimationState() const noexcept { return currentAnimationState; }

	static const GLuint GetSpriteWidth() noexcept { return spriteWidth; }
	static const GLuint GetSpriteHeight() noexcept { return spriteHeight; }
	static const int GetScore() noexcept { return score; }

protected:
	BirdC() = default;

private:
	void CheckPlayerInRange(const GLfloat playerXPosition) noexcept;
	void MoveBird(const DWORD milliseconds) noexcept;
	void CheckSwapAnimation(const DWORD milliseconds) noexcept;
	void SwapAnimationState() noexcept;

	/* Animation State Updates */
	DWORD idleTimeCurrent = 0;
	DWORD idleTimeUpdate = 0;
	static const GLfloat flyUpdateTime;
	int32_t currentAnimationState = BirdAnimation::IDLE;
	bool8_t isInRange = false;

	/* Position and Velocity Info */
	GLfloat xPosition = 0;
	GLfloat yPosition = 0;
	static const GLfloat minYPosition;
	static const GLfloat xVelocity;
	static const GLfloat yVelocity;
	static const GLfloat positionRange;

	/* Sprite Info */
	GLuint texture = 0;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;

	static const int32_t score = 100;
};