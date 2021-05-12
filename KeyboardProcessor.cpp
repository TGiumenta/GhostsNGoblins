/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class that interacts directly with both the game and the input manager. It specifically 
				  reads the player's keypresses and sends messages to the input manager. That manager will 
				  then set up a state changing array to talk to the player. This key manager handles key 
				  presses differently dependent on the state of the game.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>													// Header File For The Windows Library
#include "game.h"
#include <time.h>
#include "openglframework.h"											// Header File For The NeHeGL Basecode
#include "AudioManager.h"
#include "InputManager.h"

// Handles keyboard processing
void KeyboardProcessor()
{
	if (g_keys->keyDown[VK_ESCAPE])								// Is ESC Being Pressed?
	{
		TerminateApplication (g_window);						// Terminate The Program
	}
	
	// Keys will do different things dependending on the game state
	switch (CGame::GetCurrentState())
	{
		case CGame::GameState::MAIN_MENU:
			MainMenuKeys();
			break;
		case CGame::GameState::IN_GAME:
			InGameKeys();
			break;
		case CGame::GameState::GAME_OVER:
			GameOverKeys();
			break;
		default:
			break;
	}
}

// Input when in the main menu
void MainMenuKeys()
{
	// Indicates starting the game
	if (g_keys->keyDown[VK_RETURN])
	{
		AudioManagerC::GetInstance()->PlayStartTheme();

		Sleep(AudioManagerC::GetInstance()->GetStartThemeLength());

		CGame::GetInstance()->SetTransitionOccurrence(true);
		CGame::GetInstance()->SetCurrentGameState(CGame::GameState::IN_GAME);
		AudioManagerC::GetInstance()->PlayStageTheme();
		g_keys->keyDown[VK_RETURN] = false; // Debounce
	}

	// Debug key
	if (g_keys->keyDown[VK_END])
	{
		InputManagerC::GetInstance()->ToggleDebug();
		g_keys->keyDown[VK_END] = false;	// Debounce
	}
}

// Input when in the game
void InGameKeys() noexcept
{
	// Get the status of each key every frame and call its respective function in the input manager
	InputManagerC* instance = InputManagerC::GetInstance();
	if (instance == nullptr) { return; }

	// Set individual states
	instance->SetIsMovingRight(g_keys->keyDown[VK_RIGHT] || g_keys->keyDown['D']);
	instance->SetIsMovingLeft(g_keys->keyDown[VK_LEFT] || g_keys->keyDown['A']);
	instance->SetIsCrouched(g_keys->keyDown[VK_DOWN] || g_keys->keyDown['S']);
	instance->SetJumped(g_keys->keyDown[VK_UP] || g_keys->keyDown['W']);

	// Debounces the attack key
	if (!instance->GetPreviousSpaceState()) { instance->SetAttacked(g_keys->keyDown[VK_SPACE]); }
	instance->SetPreviousSpaceState(g_keys->keyDown[VK_SPACE]);

	instance->SetIdle(DetermineIfIdle());
}

// If the player didn't touch any of the input keys in the game, ensure that they get set to idle
bool DetermineIfIdle() noexcept
{
	return !(g_keys->keyDown[VK_RIGHT] || g_keys->keyDown['D'] ||
		g_keys->keyDown[VK_LEFT] || g_keys->keyDown['A'] ||
		g_keys->keyDown[VK_DOWN] || g_keys->keyDown['S'] ||
		g_keys->keyDown[VK_UP] || g_keys->keyDown['W'] ||
		g_keys->keyDown[VK_SPACE]);
}

// Input when in the game over screen
void GameOverKeys()
{
	// When on the end menu, pressing enter will reset the system
	if (g_keys->keyDown[VK_RETURN])
	{
		CGame::GetInstance()->ResetGame();
		g_keys->keyDown[VK_RETURN] = false; // Debounce
	}
}