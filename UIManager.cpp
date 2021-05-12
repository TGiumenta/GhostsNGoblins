/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which holds all the logic for rendering the UI. Loads in all of the textures 
				  for the base text (like the score, and time) as well as the render wrappers for the 
				  digits themselves. The digit renderer is reused for both scores as well as the timers.
--------------------------------------------------------------------------------------------------*/

#include <Windows.h>
#include "MathHelper.h"
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include "SOIL.h"
#include "game.h"
#include "Player.h"
#include "BackgroundManager.h"
#include "UIManager.h"
#include "Renderer.h"
#include <vector>

UIManagerC* UIManagerC::uiManagerInstance = nullptr;
GLuint UIManagerC::uiTexture;
GLuint UIManagerC::numbersTexture;
const GLfloat UIManagerC::hudXOffset = 15;
const GLfloat UIManagerC::hudYOffset = 5;
const GLfloat UIManagerC::hudWidth = 1024;
const GLfloat UIManagerC::hudHeight = 167;
const GLfloat UIManagerC::uiXPositionLeft = -172;
const GLfloat UIManagerC::uiXPositionRight = 172;

// Creates a non-singleton instance of the manager
UIManagerC* UIManagerC::CreateInstance()
{
	if (uiManagerInstance == nullptr)
	{
		uiManagerInstance = new UIManagerC();
		LoadTextures();
	}
	return uiManagerInstance;
}

// Loads the UI sprites into memory once, this index will be used every time a UI is rendered
void UIManagerC::LoadTextures() noexcept
{
	uiTexture = SOIL_load_OGL_texture("Sprites/UI.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	numbersTexture = SOIL_load_OGL_texture("Sprites/Numbers.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

// A hub for rendering the entire UI
void UIManagerC::RenderHUD()
{
	// Clamp the player position reference to not push the UI past the boundary of the camera
	const double defaultPlayerXPosition = PlayerC::GetInstance()->GetXPosition();
	const double hudMaxPosition = (BackgroundManagerC::GetInstance()->GetRightWallPosition() - CGame::GetInstance()->GetOrthWidth());
	const GLfloat newPlayerXPosition = static_cast<GLfloat>(Clamp(defaultPlayerXPosition, hudMaxPosition, 0));

	RenderText(newPlayerXPosition);

	int32_t scoreDisplay = PlayerC::GetInstance()->GetScore();
	RenderScores(newPlayerXPosition, uiXPositionLeft, scoreDisplay);
	scoreDisplay = CGame::GetInstance()->GetHighScore();
	RenderScores(newPlayerXPosition, uiXPositionRight, scoreDisplay);

	RenderTime(newPlayerXPosition);
}

// Renders the base HUD which does not contain any scores/timer
void UIManagerC::RenderText(const GLfloat defaultPlayerXPosition) noexcept
{
	UVRectangle uvRectangle{ 0,1,0,1 };
	VertexRectangle vertexRectangle{
		hudXOffset - CGame::GetInstance()->GetOrthWidth() + defaultPlayerXPosition,
		hudXOffset + (hudWidth / 2) - CGame::GetInstance()->GetOrthWidth() + defaultPlayerXPosition,
		hudYOffset - (hudHeight / 2),
		hudYOffset + (hudHeight / 2)
	};

	RenderQuads(uiTexture, uvRectangle, vertexRectangle);
}

// Renders a particular score in a location
void UIManagerC::RenderScores(const GLfloat defaultPlayerXPosition, const GLfloat xPosition, int32_t score) noexcept
{
	int32_t numScoreDigits = NumberDigits(score);
	numScoreDigits = max(numScoreDigits, 5); // Maximum of 5 digits
	
	// Render the digits from the right to left
	for (int32_t digitIndex = 0; digitIndex < numScoreDigits; ++digitIndex)
	{
		const int32_t digitToPrint = score % numberBase;
		score /= numberBase;

		RenderDigit(defaultPlayerXPosition, digitIndex, digitToPrint, scoreHeight, xPosition);
	}
}

// Renders the timer from the game class
void UIManagerC::RenderTime(const GLfloat defaultPlayerXPosition)
{
	// Calculate information needed from the timer
	const DWORD timeDisplay = CGame::GetInstance()->GetCurrentGameTime();
	const GLuint  minutes = ((timeDisplay /CGame::GetMillisecondsToSeconds()) / CGame::GetMinutesToSeconds()); // Assume minutes are less than 10 for now
	const DWORD seconds = max((timeDisplay / CGame::GetMillisecondsToSeconds()) % CGame::GetMinutesToSeconds(), 0);

	// Create an array with the digits to render
	std::vector<int32_t> digitsArray;
	digitsArray.push_back(seconds % numberBase);
	digitsArray.push_back(seconds / numberBase);
	digitsArray.push_back(colonDigitValue);
	digitsArray.push_back(minutes);

	// Print a particular digit based 
	for (int32_t digitIndex = 0; digitIndex < 4; ++digitIndex) // Only render 4 digits
	{
		RenderDigit(defaultPlayerXPosition, digitIndex, digitsArray[digitIndex], timeHeight, uiXPositionLeft);
	}
}

// Handles all of the digit rendering 
void UIManagerC::RenderDigit(const GLfloat defaultPlayerXPosition, const int32_t digitIndex, int32_t const digitToPrint, const GLfloat spriteHeight, const GLfloat xLocation) noexcept
{
	UVRectangle uvRectangle{
		(1.0f / numbersInSheet) * (digitToPrint),
		(1.0f / numbersInSheet) * (digitToPrint + 1),
		0,
		1
	};
	VertexRectangle vertexRectangle{
		xLocation + (numberSpacing * digitIndex) + defaultPlayerXPosition,
		(xLocation + (numberSpacing * digitIndex)) + numberWidth + defaultPlayerXPosition,
		digitOffset + spriteHeight,
		digitOffset + spriteHeight + numberHeight
	};

	RenderQuads(numbersTexture, uvRectangle, vertexRectangle);
}
