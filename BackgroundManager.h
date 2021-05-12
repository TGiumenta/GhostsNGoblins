#pragma once

#include <gl\gl.h>					// Header File For The OpenGL32 Library

class BackgroundManagerC final
{
public:
	static BackgroundManagerC*	CreateInstance();
	static BackgroundManagerC*	GetInstance() noexcept { return backgroundManagerInstance; };
	
	BackgroundManagerC(const BackgroundManagerC& other) = delete;
	BackgroundManagerC(BackgroundManagerC&& other) noexcept = delete;
	BackgroundManagerC& operator=(const BackgroundManagerC& other) = delete;
	BackgroundManagerC& operator=(BackgroundManagerC&& other) noexcept = delete;
	~BackgroundManagerC() { backgroundManagerInstance = nullptr; }

	void Render() noexcept;

	GLfloat GetBottomPlatformPosition() noexcept { return bottomPlatformPosition; }
	GLfloat GetTopPlatformPosition() noexcept { return topPlatformPosition; }
	GLfloat GetLeftWallPosition() noexcept { return leftWallPosition; }
	GLfloat GetRightWallPosition() noexcept { return rightWallPosition; }

private:
	BackgroundManagerC() = default;
	static void LoadTextures() noexcept;

	static BackgroundManagerC* backgroundManagerInstance;
	static GLuint textureIndex;

	static const GLfloat backgroundWidth;
	static const GLfloat backgroundHeight;

	static const GLfloat bottomPlatformPosition;
	static const GLfloat topPlatformPosition;
	static const GLfloat leftWallPosition;
	static const GLfloat rightWallPosition;
};