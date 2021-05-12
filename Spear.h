#pragma once

#include <gl/GL.h>

class SpearC final
{
public:
	static SpearC*	CreateInstance();
	
	SpearC(const SpearC& other) = delete;
	SpearC(SpearC&& other) noexcept = delete;
	SpearC& operator=(const SpearC& other) = delete;
	SpearC& operator=(SpearC&& other) noexcept = delete;
	~SpearC() = default;

	SpearC*	Initialize(const GLuint textureIndex) noexcept;
	bool Update(const DWORD milliseconds) noexcept;
	void Render() noexcept;

	void SetXPosition(const GLfloat position) noexcept { xPosition = position; }
	GLfloat GetXPosition() noexcept { return xPosition; }
	const GLfloat GetXPosition() const noexcept { return xPosition; }

	void SetYPosition(const GLfloat position) noexcept { yPosition = position; }
	GLfloat GetYPosition() noexcept { return yPosition; }
	const GLfloat GetYPosition() const noexcept { return yPosition; }

	static const GLuint GetSpriteWidth() noexcept { return spriteWidth; }
	static const GLuint GetSpriteHeight() noexcept { return spriteHeight; }

private:
	SpearC() = default;

	/* Animation Update Timers */
	DWORD currentLifetime = 0;

	/* Position Info */
	GLfloat xPosition = 0;
	GLfloat yPosition = 0;
	static const GLfloat xVelocity;
	GLfloat direction = 0;

	/* Texture Index */
	GLuint texture = 0;
	static const GLuint spriteWidth;
	static const GLuint spriteHeight;
	static const GLfloat lifetime;

	void SetInitialStates() noexcept;
};