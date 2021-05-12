#pragma once

#include <gl\GL.h>
#include "baseTypes.h"

class StatueC
{
public:
	static StatueC* CreateInstance();
	
	StatueC(const StatueC& other) = delete;
	StatueC(StatueC&& other) noexcept = delete;
	StatueC& operator=(const StatueC& other) = delete;
	StatueC& operator=(StatueC&& other) noexcept = delete;
	~StatueC() = default;

	StatueC* Initialize(const GLfloat initialXPosition, const GLfloat initialYPosition, const GLuint textureIndex) noexcept;
	void Update(const DWORD milliseconds) noexcept;
	void Render() noexcept;

	int	GetStatueType() noexcept { return statueType; }
	const int GetStatueType() const noexcept { return statueType; }

	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }

	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }

	static const GLuint GetSpriteWidth() noexcept { return spriteWidth; }
	static const GLuint GetSpriteHeight() noexcept { return spriteHeight; }

	int	GetScore() noexcept { return pointValues[statueType]; }
	const int GetScore() const noexcept { return pointValues[statueType]; }

private:
	StatueC() = default;
	void DetermineStatueType(const int32_t randomValue) noexcept;

	/* Used for UVs when indexing into statue sheet */
	typedef enum
	{
		INVALID,
		GHOST,
		DEVIL,
		KNIGHT,
		KING,
		MAXTYPES
	}StatueType;

	int32_t statueType = 0;
	int32_t pointValues[MAXTYPES] = {
		0, 400, 400, 400, 10000 }; // 0 for an invalid score for enum simplicity

	/* Position and Velocity Info */
	GLfloat xPosition = 0;
	GLfloat yPosition = 0;
	static const GLfloat yVelocity;

	/* Texture Index */
	GLuint texture = 0;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;
};