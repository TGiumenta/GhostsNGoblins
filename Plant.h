#pragma once

class PlantC final
{
public:
	static PlantC* CreateInstance();

	PlantC(const PlantC& other) = delete;
	PlantC(PlantC&& other) noexcept = delete;
	PlantC& operator=(const PlantC& other) = delete;
	PlantC& operator=(PlantC&& other) noexcept = delete;
	~PlantC() = default;

	PlantC*	Initialize(const GLfloat initialXPosition, const GLfloat initialYPosition, const GLuint textureIndex) noexcept;
	void Update(const DWORD milliseconds);
	void Render() noexcept;

	typedef enum {
		INVALID,
		IDLE,
		ATTACK,
		PLANT_MAX_STATES
	}PlantAnimation;

	/* Setters and Getters */
	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat	GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }

	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat	GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }

	void SetIsFacingLeft(const bool8_t leftState) noexcept { isFacingLeft = leftState; }
	bool8_t	GetIsFacingLeft() noexcept { return isFacingLeft; }
	const bool8_t GetIsFacingLeft() const noexcept { return isFacingLeft; }

	void SetCurrentAnimationState(const int32_t animationState) noexcept { currentAnimationState = animationState; }
	int32_t	GetCurrentAnimationState() noexcept { return currentAnimationState; }
	const int32_t GetCurrentAnimationState() const noexcept { return currentAnimationState; }

	static const GLuint GetSpriteWidth() noexcept { return spriteWidth; }
	static const GLuint GetSpriteHeight() noexcept { return spriteHeight; }
	static const GLfloat GetRange() noexcept { return range; }
	static const int GetScore() noexcept { return score; }

private:
	PlantC() = default;

	/* Private State Helpers */
	void SetAnimationState(const GLfloat playerXPosition, const DWORD milliseconds);
	void SwapAnimationState(const GLfloat playerXPosition);
	void FireVenom(const GLfloat playerXPosition);

	/* State Info */
	bool8_t isFacingLeft;

	/* Animation State Updates */
	DWORD idleTimeCurrent;
	int32_t currentAnimationState;
	static const GLfloat venomUpdateDeltaTime;

	/* Position Info */
	GLfloat xPosition;
	GLfloat yPosition;

	/* Texture Index */
	GLuint texture;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;
	static const GLfloat range;

	static const int32_t score = 100;
};