/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which handles all of the game over menu textures. Simply loads and renders the 
				  game over menu. Takes care of both the win and lose states for the player. Finally, 
				  prints out whether or not the player achieved a new high score on their playthrough.
--------------------------------------------------------------------------------------------------*/


#include <Windows.h>
#include "openGLFramework.h"
#include "game.h"
#include <gl/GLU.h>
#include "SOIL.h"
#include "AudioManager.h"
#include "GameOverManager.h"
#include "Renderer.h"

GameOverManagerC* GameOverManagerC::gameOverManagerInstance = nullptr;
const GLfloat GameOverManagerC::gameEndWidthOffset = -40.f;
const GLfloat GameOverManagerC::highScoreTextWidth = 159.f;
const GLfloat GameOverManagerC::highScoreTextHeight = 10.f;
GLuint GameOverManagerC::gameOverMenuTexture;
GLuint GameOverManagerC::gameWinMenuTexture;
GLuint GameOverManagerC::highScoreTextTexture;

// Creates a singleton instance of the manager
GameOverManagerC* GameOverManagerC::CreateInstance()
{
	if (gameOverManagerInstance == nullptr)
	{
		gameOverManagerInstance = new GameOverManagerC();
		LoadTextures();
	}
	return gameOverManagerInstance;
}

// Loads the UI sprites into memory once, these indexes will be used every time game over is rendered
void GameOverManagerC::LoadTextures() noexcept
{
	gameOverMenuTexture = SOIL_load_OGL_texture("Sprites/GameOver.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	gameWinMenuTexture = SOIL_load_OGL_texture("Sprites/GameWin.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	highScoreTextTexture = SOIL_load_OGL_texture("Sprites/NewHighScoreText.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

// Draw the particular texture based on the status of the player
void GameOverManagerC::Render()
{
	// Reset the position of the camera because the player moved it
	ReshapeGL(CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), 0);

	UVRectangle uvRectangle{ 0,1,0,1 };
	VertexRectangle vertexRectangle{
		0 - CGame::GetInstance()->GetOrthWidth() + gameEndWidthOffset,
		(CGame::GetInstance()->GetScreenWidth() / 2) - CGame::GetInstance()->GetOrthWidth() + gameEndWidthOffset,
		0 - CGame::GetInstance()->GetOrthHeight(),
		(CGame::GetInstance()->GetScreenHeight() / 2) - CGame::GetInstance()->GetOrthHeight()
	};

	// Render a particular ending screen based off the player won or not
	if (CGame::GetInstance()->GetIsPlayerDead())
	{
		RenderQuads(gameOverMenuTexture, uvRectangle, vertexRectangle);
	}
	else
	{
		RenderQuads(gameWinMenuTexture, uvRectangle, vertexRectangle);
	}

	// If high score was just set, add the rendered text to the screen
	if (CGame::GetInstance()->GetNewHighScore())
	{
		RenderHighScoreText();
	}
}

// Draws the "New High Score" if inded it is necessary
void GameOverManagerC::RenderHighScoreText()
{
	// Reset the position of the camera because the player moved it
	ReshapeGL(CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), 0);

	UVRectangle uvRectangle{0,1,0,1};
	VertexRectangle vertexRectangle {
		-highScoreTextWidth / 2,
		-highScoreTextWidth / 2 + highScoreTextWidth,
		0,
		0 + highScoreTextHeight
	};

	RenderQuads(highScoreTextTexture, uvRectangle, vertexRectangle);
}