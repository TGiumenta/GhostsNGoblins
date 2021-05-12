#pragma once

class GhostC
{
public:
	static GhostC* CreateInstance();
	
	GhostC(const GhostC& other) = delete;
	GhostC(GhostC&& other) noexcept = delete;
	GhostC& operator=(const GhostC& other) = delete;
	GhostC& operator=(GhostC&& other) noexcept = delete;
	virtual ~GhostC() = default;

	GhostC*	Initialize(const int32_t randomXOffset, const GLuint textureIndex) noexcept;
	bool8_t	Update(const DWORD milliseconds) noexcept;
	void Render() noexcept;

	typedef enum {
		INVALID,
		AIR_1,
		AIR_2,
		GHOST_MAX_STATES
	}GhostAnimation;

	/* Setters and Getters */
	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }

	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat	GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }

	static const GLfloat GetXVelocity() noexcept { return xVelocity; }
	void SetYVelocity(const GLfloat velocity) noexcept { yVelocity = velocity; }
	GLfloat	GetYVelocity() noexcept { return yVelocity; }
	const GLfloat GetYVelocity() const noexcept { return yVelocity; }

	void SetCurrentAnimationState(const int32_t animationState) noexcept { currentAnimationState = animationState; }
	int32_t	GetCurrentAnimationState() noexcept { return currentAnimationState; }
	const int32_t GetCurrentAnimationState() const noexcept { return currentAnimationState; }

	static GLuint GetSpriteWidth() noexcept { return spriteWidth; }
	static GLuint GetSpriteHeight() noexcept { return spriteHeight; }
	static int GetScore() noexcept { return score; }

protected:
	GhostC() = default;

private:
	void SetInitialYVelocity() noexcept;
	void CheckAnimationSwap(const DWORD milliseconds) noexcept;
	void SwapAnimationState() noexcept;
	void AttemptYVelocitySwap() noexcept;

	/* Animation States */
	int32_t currentAnimationState;

	/* Animation Timers */
	DWORD airTimeCurrent;
	static const GLfloat airUpdateTime;

	/* Position Info */
	GLfloat xPosition = 0;
	GLfloat yPosition = 0;
	static const GLfloat xVelocity;
	static const GLfloat potentialYVelocity;
	GLfloat yVelocity = 0;

	/* Texture Index */
	GLuint texture = 0;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;

	static const int32_t score = 100;
};