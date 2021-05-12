#define GAME_CPP

#define _CRT_SECURE_NO_WARNINGS									// Allows the use of fopen instead of fopen_s
#include <windows.h>											// Header File For Windows
#include <stdlib.h>												// Header for rand seeding
#include <time.h>												// Header for rand seeding
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include "openglframework.h"	
#include "openGLStuff.h"
#include "game.h"
#include "AudioManager.h"
#include "MainMenuManager.h"
#include "GameOverManager.h"
#include "InputManager.h"
#include "BackgroundManager.h"
#include "Player.h"
#include "WeaponManager.h"
#include "EnemyManager.h"
#include "KeyManager.h"
#include "CollisionManager.h"
#include "CollectibleManager.h"
#include "ProjectileManager.h"
#include "ParticleManager.h"
#include "UIManager.h"

// Declarations
const char8_t CGame::mGameTitle[]="Ghosts 'n Goblins";
CGame* CGame::gameInstance = nullptr;
int32_t CGame::gameState = CGame::GameState::INVALID;
const GLuint CGame::screenWidth = 1024;
const GLuint CGame::screenHeight = 454;
const GLfloat CGame::orthWidth = 215;
const GLfloat CGame::orthHeight = (screenHeight * orthWidth) / screenWidth;
DWORD CGame::currentGameTime = 0;
bool8_t CGame::isPlayerDead = false;
bool8_t CGame::timerEnabled = true;
bool8_t CGame::transitionSet = false;
int32_t CGame::activeHighScore = 0;
int32_t CGame::newHighScore = false;

// Initializes the game
BOOL Initialize (GL_Window* window, Keys* keys)					// Any OpenGL Initialization Goes Here
{
	InitializeOpenGLDrawing(window,keys,0.0f, 0.0f, 0.0f);
	CGame::CreateInstance();
	CGame::SetCurrentGameState(CGame::GameState::MAIN_MENU);
	CGame::GetInstance()->InitializeGame();

	srand(static_cast<unsigned int>(time(0)));				   // Seed random number generator

	return TRUE;						
}

// Creates a non-singleton instance of the game
CGame* CGame::CreateInstance()
{
	gameInstance = new CGame();
	return gameInstance;
}

// Creates all of the managers of the game
void CGame::InitializeGame()
{
	SetInitialStates();

	/* Create Instances */
	MainMenuManagerC::CreateInstance();
	AudioManagerC::CreateInstance();
	BackgroundManagerC::CreateInstance();
	PlayerC::CreateInstance();
	WeaponManagerC::CreateInstance();
	InputManagerC::CreateInstance();			// Do not move, dependency with PlayerC
	EnemyManagerC::CreateInstance();
	KeyManagerC::CreateInstance();
	CollisionManagerC::CreateInstance();
	CollectibleManagerC::CreateInstance();
	ProjectileManagerC::CreateInstance();
	ParticleManagerC::CreateInstance();
	UIManagerC::CreateInstance();
	GameOverManagerC::CreateInstance();
}

// Called every frame, performs all of the updates for the game based on the time since the last frame
void CGame::UpdateFrame(DWORD milliseconds)			
{
	//std::string test = std::to_string(milliseconds);
	//OutputDebugStringA(test.c_str());

	// Kills extra frames that are produced
	if (milliseconds == 0) { return; }

	// If the IN_GAME state was just set, subtract out the length that it took for the game to finish the start theme
	if (GetTransitionOccurrence())
	{
		milliseconds -= AudioManagerC::GetInstance()->GetStartThemeLength();
		transitionSet = false;
	}

	// Reads input from the user
	KeyboardProcessor();

	// Updates the game based on the state of the game
	switch (CGame::gameState) {
	case GameState::MAIN_MENU:
		break;
	case GameState::IN_GAME:
		UpdateTimer(milliseconds);
		ParticleManagerC::GetInstance()->UpdateParticles(milliseconds);
		PlayerC::GetInstance()->Update(milliseconds);
		WeaponManagerC::GetInstance()->Update(milliseconds);
		EnemyManagerC::GetInstance()->UpdateEnemies(milliseconds);
		KeyManagerC::GetInstance()->Update(milliseconds);
		CollisionManagerC::GetInstance()->UpdateCollisions();
		CollectibleManagerC::GetInstance()->UpdateCollectibles(milliseconds);
		ProjectileManagerC::GetInstance()->UpdateProjectiles(milliseconds);
		break;
	case GameState::GAME_OVER:
		break;
	default:
		break;
	}
}

// A hub for all the render functions of each manager
void CGame::DrawScene(const DWORD milliseconds)
{
	// Swaps buffer and begins the drawing process
	StartOpenGLDrawing();

	// Draws the game based on the state of the game
	// Note: This is in a particular order (especially the background being first)
	switch (gameState) {
	case GameState::MAIN_MENU:
		MainMenuManagerC::GetInstance()->Render();
		break;
	case GameState::IN_GAME:
		BackgroundManagerC::GetInstance()->Render();	// Always draw the background first and everything else will be drawn over it
		ParticleManagerC::GetInstance()->RenderParticles();
		EnemyManagerC::GetInstance()->RenderEnemies();	// Enemies rendered behind Players
		PlayerC::GetInstance()->Render(milliseconds);
		WeaponManagerC::GetInstance()->Render();
		KeyManagerC::GetInstance()->Render();
		CollectibleManagerC::GetInstance()->RenderCollectibles();
		ProjectileManagerC::GetInstance()->RenderProjectiles();
		UIManagerC::GetInstance()->RenderHUD();
		break;
	case GameState::GAME_OVER:
		GameOverManagerC::GetInstance()->Render();
		break;
	default:
		break;
	}
}

// Updates the in-game timer based on the time since the last frame
void CGame::UpdateTimer(DWORD milliseconds)
{
	if (timerEnabled)
	{
		currentGameTime -= milliseconds;
		if (currentGameTime <= 0)
		{
			currentGameTime = 0;
			timerEnabled = false;
			PlayerC::GetInstance()->SetArmorTexture(PlayerC::ArmorState::DEAD);
		}
	}
}

// Deletes all the managers
void CGame::DestroyGame() noexcept
{
	delete MainMenuManagerC::GetInstance();
	delete AudioManagerC::GetInstance();
	delete BackgroundManagerC::GetInstance();
	delete ParticleManagerC::GetInstance();
	delete PlayerC::GetInstance();
	delete WeaponManagerC::GetInstance();
	delete KeyManagerC::GetInstance();
	delete InputManagerC::GetInstance();
	delete EnemyManagerC::GetInstance();
	delete CollisionManagerC::GetInstance();
	delete CollectibleManagerC::GetInstance();
	delete ProjectileManagerC::GetInstance();
	delete UIManagerC::GetInstance();
	delete GameOverManagerC::GetInstance();
}

// Sets up the intial boolean states of the game
void CGame::SetInitialStates() noexcept
{
	currentGameTime = maxTime;
	ReadInputFile();
}

// Grab the high score file for reading and loads it
void CGame::ReadInputFile() noexcept
{
	highScore = fopen("Stats/Score.txt", "r");
	const bool8_t readFlag = fscanf(highScore, "%d", &activeHighScore);
	fclose(highScore);
}

// Transitions to the game over state after determining if the player won/lost
void CGame::EndGame(bool8_t deathState)
{
	isPlayerDead = deathState;

	if (deathState)
	{
		AudioManagerC::GetInstance()->PlayEndLoseTheme();
	}
	else
	{
		AudioManagerC::GetInstance()->PlayEndWinTheme();
	}

	// Set new high score
	if (PlayerC::GetInstance()->GetScore() > GetHighScore())
	{
		highScore = fopen("Stats/Score.txt", "w");
		fprintf(highScore, "%d", PlayerC::GetInstance()->GetScore());
		fclose(highScore);
		newHighScore = true;
	}

	gameState = GameState::GAME_OVER;
}

// Resets all the managers so that memory is cleaned up
void CGame::ResetGame()
{
	DestroyGame();
	InitializeGame();
	gameState = GameState::MAIN_MENU;
}