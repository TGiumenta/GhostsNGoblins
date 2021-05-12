#pragma once

#include "baseTypes.h"

class MoneybagC
{
public:
	static MoneybagC* CreateInstance();

	MoneybagC(const MoneybagC& other) = delete;
	MoneybagC(MoneybagC&& other) noexcept = delete;
	MoneybagC& operator=(const MoneybagC& other) = delete;
	MoneybagC& operator=(MoneybagC&& other) noexcept = delete;
	~MoneybagC() = default;

	MoneybagC* Initialize(const GLfloat initialXPosition, const GLfloat initialYPosition, const GLuint textureIndex) noexcept;
	void Update(const DWORD milliseconds) noexcept;
	void Render() noexcept;
	
	/* Setters and Getters */
	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat	GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }

	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat	GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }

	static GLuint GetSpriteWidth() noexcept { return spriteWidth; }
	static GLuint GetSpriteHeight() noexcept { return spriteHeight; }
	static int GetScore() noexcept { return score; }

private:
	MoneybagC() = default;

	/* Position Info */
	GLfloat xPosition;
	GLfloat yPosition;
	static const GLfloat moneybagDropSpeed;

	/* Texture Index */
	GLuint texture;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;

	static const int32_t score = 500;
};