#pragma once

#include <stdio.h>
#include <gl/GL.h>
#include "baseTypes.h"

class CGame
{
public:
	const char8_t* GetGameTitle() noexcept { return mGameTitle; }
	static CGame *CreateInstance();
	static constexpr CGame *GetInstance() noexcept {return gameInstance; }
	
	CGame(const CGame& other) = delete;
	CGame(CGame&& other) noexcept = delete;
	CGame& operator=(const CGame& other) = delete;
	CGame& operator=(CGame&& other) noexcept = delete;
	~CGame() { gameInstance = nullptr; }

	void InitializeGame();
	void UpdateFrame(DWORD milliseconds);
	void DrawScene(const DWORD milliseconds);

	typedef enum {
		INVALID,
		MAIN_MENU,
		IN_GAME,
		GAME_OVER
	}GameState;

	void DestroyGame() noexcept;
	void ResetGame();
	void EndGame(bool8_t deathState);

	/* Setters and Getters */
	static void	SetCurrentGameState(int32_t state) noexcept { gameState = state; }
	static int32_t GetCurrentState() noexcept { return gameState; }
	static DWORD GetCurrentGameTime() noexcept { return currentGameTime; }

	static void SetIsPlayerDead(bool8_t deathState) noexcept { isPlayerDead = deathState; }
	static bool8_t GetIsPlayerDead() noexcept { return isPlayerDead; }

	static void SetTransitionOccurrence(bool8_t state) noexcept { transitionSet = state; }
	static bool8_t GetTransitionOccurrence() noexcept { return transitionSet; }

	static int32_t GetHighScore() noexcept { return activeHighScore; }

	static void SetNewHighScore(bool8_t highScoreState) noexcept { newHighScore = highScoreState; }
	static bool8_t	GetNewHighScore() noexcept { return newHighScore; }

	static constexpr uint32_t GetMinutesToSeconds() noexcept { return minutesToSeconds; }
	static constexpr uint32_t GetLevelMinutes() noexcept { return levelMinutes; }
	static constexpr uint32_t GetMillisecondsToSeconds() noexcept { return millisecondsToSeconds; }
	static constexpr uint32_t GetMaxTime() noexcept { return maxTime; }
	
	static constexpr GLuint GetScreenWidth() noexcept { return screenWidth; }
	static constexpr GLuint GetScreenHeight() noexcept { return screenHeight; }
	static constexpr GLfloat GetOrthWidth() noexcept { return orthWidth; }
	static constexpr GLfloat GetOrthHeight() noexcept { return orthHeight; }

	static uint32_t const mBitsPerPixel = 32;

private:
	CGame() = default;

	void SetInitialStates() noexcept;
	void ReadInputFile() noexcept;
	void UpdateTimer(DWORD milliseconds);
	void ToggleTimer(bool8_t timerState) noexcept { timerEnabled = timerState; }

	static const char8_t mGameTitle[20];
	static CGame *gameInstance;

	static const uint32_t minutesToSeconds = 60;
	static const uint32_t levelMinutes = 2;
	static const uint32_t millisecondsToSeconds = 1000;
	static const uint32_t maxTime = millisecondsToSeconds * minutesToSeconds * levelMinutes;

	/* Screen Information */
	static const GLuint screenWidth;
	static const GLuint screenHeight;
	static const GLfloat orthWidth;
	static const GLfloat orthHeight;

	/* Game Timers */
	static DWORD currentGameTime;

	/* Game States */
	static int32_t gameState;
	static bool8_t isPlayerDead;
	static bool8_t timerEnabled;
	static bool8_t transitionSet;

	/* High Score information */
	FILE* highScore;
	static int32_t activeHighScore;
	static bool8_t newHighScore;
};