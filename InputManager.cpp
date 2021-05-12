/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which handles all of the input from the key processor handles. This is simply 
				  a buffer between the key processor and the player. This allows the key processor to never
				  have to touch the player class and only ever touch this manager. Here, it simply sets 
				  a boolean in a state changing array which is read and interpreted as changing states.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>										// Header File For Windows
#include <gl\gl.h>
#include "baseTypes.h"
#include "openGLFramework.h"
#include "Player.h"
#include "InputManager.h"

InputManagerC* InputManagerC::inputManagerInstance = nullptr;
PlayerC* InputManagerC::playerInstance = nullptr;

// Creates a singleton instance of the manager
InputManagerC *InputManagerC::CreateInstance()
{
	if (inputManagerInstance == nullptr)
	{
		inputManagerInstance = new InputManagerC();
		playerInstance = PlayerC::GetInstance();
	}
	return inputManagerInstance;
}

void InputManagerC::SetIsMovingRight(const bool8_t state) noexcept
{
	PlayerC::GetInstance()->SetInputMovingRight(state);
}

void InputManagerC::SetIsMovingLeft(const bool8_t state) noexcept
{
	PlayerC::GetInstance()->SetInputMovingLeft(state);
}

void InputManagerC::SetIsCrouched(const bool8_t state) noexcept
{
	PlayerC::GetInstance()->SetInputCrouched(state);
}

void InputManagerC::SetJumped(const bool8_t state) noexcept
{
	PlayerC::GetInstance()->SetInputJumped(state);
}

void InputManagerC::SetIdle(const bool8_t idleState) noexcept
{
	PlayerC::GetInstance()->SetInputIdle(idleState);
}

void InputManagerC::SetAttacked(const bool8_t attackState) noexcept
{
	PlayerC::GetInstance()->SetInputAttacking(attackState);
}

void InputManagerC::SetPreviousSpaceState(const bool8_t spaceState) noexcept
{
	lastSpaceBarState = spaceState;
}

bool8_t InputManagerC::GetPreviousSpaceState() noexcept
{
	return lastSpaceBarState;
}

void InputManagerC::ToggleDebug() noexcept
{
	PlayerC::GetInstance()->ToggleDebugMode();
}