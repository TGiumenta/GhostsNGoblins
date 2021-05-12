#pragma once

class MainMenuManagerC
{
public:
	static MainMenuManagerC* CreateInstance();
	static MainMenuManagerC* GetInstance() noexcept { return mainMenuManagerInstance; }
	
	MainMenuManagerC(const MainMenuManagerC& other) = delete;
	MainMenuManagerC(MainMenuManagerC&& other) noexcept = delete;
	MainMenuManagerC& operator=(const MainMenuManagerC& other) = delete;
	MainMenuManagerC& operator=(MainMenuManagerC&& other) noexcept = delete;
	~MainMenuManagerC() { mainMenuManagerInstance = nullptr; }

	void Render();

private:
	MainMenuManagerC() = default;
	static void LoadTextures() noexcept;
	
	static MainMenuManagerC* mainMenuManagerInstance;

	/* Texture Index */
	static GLuint mainMenuTexture;
	static GLuint cheatingTexture;
	static const GLfloat mainMenuWidthOffset;
	static const GLfloat cheatingWidth;
	static const GLfloat cheatingHeight;
	static const GLfloat cheatingYPosition;
};