#pragma once

class GameOverManagerC final
{
public:
	static GameOverManagerC* CreateInstance();
	static GameOverManagerC* GetInstance() noexcept { return gameOverManagerInstance; }
	
	GameOverManagerC(const GameOverManagerC& other) = delete;
	GameOverManagerC(GameOverManagerC&& other) noexcept = delete;
	GameOverManagerC& operator=(const GameOverManagerC& other) = delete;
	GameOverManagerC& operator=(GameOverManagerC&& other) noexcept = delete;
	~GameOverManagerC() { gameOverManagerInstance = nullptr; }

	void Render();

private:
	GameOverManagerC() = default;
	static void	LoadTextures() noexcept;
	void RenderHighScoreText();

	static GameOverManagerC* gameOverManagerInstance;

	static const GLfloat gameEndWidthOffset;
	static const GLfloat highScoreTextWidth;
	static const GLfloat highScoreTextHeight;

	/* Texture Indexes */
	static GLuint gameOverMenuTexture;
	static GLuint gameWinMenuTexture;
	static GLuint highScoreTextTexture;
};