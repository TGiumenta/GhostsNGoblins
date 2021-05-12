#pragma once

class VenomC final
{
public:
	static VenomC* CreateInstance();

	VenomC(const VenomC& other) = delete;
	VenomC(VenomC&& other) noexcept = delete;
	VenomC& operator=(const VenomC& other) = delete;
	VenomC& operator=(VenomC&& other) noexcept = delete;
	~VenomC() = default;

	VenomC* Initialize(const GLfloat xPosition, const GLfloat yPosition, const GLfloat xVelocity, const GLfloat yVelocity, const GLuint textureIndex) noexcept;
	bool8_t Update(const DWORD milliseconds) noexcept;
	void Render() noexcept;

	/* Getters and Setters */
	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }

	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }

	void SetXVelocity(const GLfloat velocity) noexcept { xVelocity = velocity; }
	GLfloat GetXVelocity() noexcept { return xVelocity; }
	const GLfloat GetXVelocity() const noexcept { return xVelocity; }
	
	void SetYVelocity(const GLfloat velocity) noexcept { yVelocity = velocity; }
	GLfloat GetYVelocity() noexcept { return yVelocity; }
	const GLfloat GetYVelocity() const noexcept { return yVelocity; }

	static const GLuint GetSpriteWidth() noexcept { return spriteWidth; }
	static const GLuint GetSpriteHeight() noexcept { return spriteHeight; }

private:
	VenomC() = default;
	bool8_t DetermineIfOffscreen() noexcept;

	static VenomC* venomInstance;

	/* Position and Velocity Info */
	GLfloat xPosition;
	GLfloat yPosition;
	GLfloat xVelocity;
	GLfloat yVelocity;

	/* Texture Index */
	GLuint texture;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;

	static const GLfloat velocityAdjustment;
};