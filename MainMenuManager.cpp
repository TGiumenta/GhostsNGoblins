/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which handles all of the main menu textures. Simply loads and renders the 
				  main menu. Also, toggles a text that indicates entering debug mode which activates 
				  any and all "cheating".
--------------------------------------------------------------------------------------------------*/

#include <Windows.h>
#include "game.h"
#include "openGLFramework.h"
#include "SOIL.h"
#include "Player.h"
#include "MainMenuManager.h"
#include "Renderer.h"

MainMenuManagerC* MainMenuManagerC::mainMenuManagerInstance = nullptr;
GLuint MainMenuManagerC::mainMenuTexture;
GLuint MainMenuManagerC::cheatingTexture;
const GLfloat MainMenuManagerC::mainMenuWidthOffset = -40;
const GLfloat MainMenuManagerC::cheatingWidth = 53;
const GLfloat MainMenuManagerC::cheatingHeight = 6;
const GLfloat MainMenuManagerC::cheatingYPosition = -45;

// Creates a singleton instance of the manager
MainMenuManagerC* MainMenuManagerC::CreateInstance()
{
	if (mainMenuManagerInstance == nullptr)
	{
		mainMenuManagerInstance = new MainMenuManagerC();
		LoadTextures();
	}
	return mainMenuManagerInstance;
}

// Loads the menu sprites into memory once, this index will be used every time a menu is rendered
void MainMenuManagerC::LoadTextures() noexcept
{
	mainMenuTexture = SOIL_load_OGL_texture("Sprites/MainMenu.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	cheatingTexture = SOIL_load_OGL_texture("Sprites/Cheating.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

// Draws the main menu and adds the cheating text if the player enables debug mode
void MainMenuManagerC::Render()
{
	ReshapeGL(CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), 0);

	UVRectangle uvRectangle{ 0,1,0,1 };
	VertexRectangle vertexRectangle{
		0 - CGame::GetInstance()->GetOrthWidth() + mainMenuWidthOffset,
		(CGame::GetInstance()->GetScreenWidth() / 2) - CGame::GetInstance()->GetOrthWidth() + mainMenuWidthOffset,
		0 - CGame::GetInstance()->GetOrthHeight(),
		(CGame::GetInstance()->GetScreenHeight() / 2) - CGame::GetInstance()->GetOrthHeight()
	};

	RenderQuads(mainMenuTexture, uvRectangle, vertexRectangle);

	if (PlayerC::GetInstance()->GetDebugMode())
	{
		vertexRectangle = {
			-cheatingWidth / 2,
			cheatingWidth / 2,
			cheatingYPosition,
			cheatingYPosition + cheatingHeight
		};

		RenderQuads(cheatingTexture, uvRectangle, vertexRectangle);
	}
}