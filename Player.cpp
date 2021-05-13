/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A class which contains all of the logic for the player that the user controls. It 
				  responds to the input manager (and therefore the key processor) to change the players 
				  movement (and therefore animation) state. This class also deliberately moves the camera
				  whenever the player moves, so that the camera follows the user. Handles all of the 
				  attacking logic here as well. Most importantl, all of the player's animations are 
				  handled here, in whichever state they're in. Taking damage, death, as well as the 
				  basic setup for the renderer also lives here.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>										// Header File For Windows
#include "SOIL.h"
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include "game.h"
#include "baseTypes.h"
#include "openglframework.h"	
#include "BackgroundManager.h"
#include "WeaponManager.h"
#include "AudioManager.h"
#include "Player.h"
#include "Renderer.h"

PlayerC* PlayerC::playerInstance = nullptr;
const GLfloat PlayerC::startingXposition = -150;
const GLfloat PlayerC::movementSpeed = 0.067f;
const GLfloat PlayerC::jumpYVelocity = 0.225f;
const GLuint PlayerC::spriteWidth = 22;
const GLuint PlayerC::spriteHeight = 26;
const GLfloat PlayerC::xCollisionOffset = 9;
const GLfloat PlayerC::yCollisionOffset = 8;
const GLfloat PlayerC::downAcceleration = 0.0008f;
const GLfloat PlayerC::runningDeltaTime = 100.f;
const GLfloat PlayerC::attackDeltaTime = 315.f;
const GLfloat PlayerC::attackAnimDeltaTime = 75.f;
const GLfloat PlayerC::invincibilityDeltaTime = CGame::GetMillisecondsToSeconds()*2.f;
const GLfloat PlayerC::deathAnimDeltaTime = 200.f;

// Creates a non-singleton instance of the player
PlayerC* PlayerC::CreateInstance()
{
	if (playerInstance == nullptr)
	{
		playerInstance = new PlayerC();
	}
	return playerInstance;
}

// Sets up position information and initial states
PlayerC::PlayerC()
{
	xPosition = startingXposition;
	yPosition = BackgroundManagerC::GetInstance()->GetBottomPlatformPosition();
	SetInitialStates();

	LoadTextures();
	SetArmorTexture(ArmorState::ARMOR);
}

// Sets all of the boolean states and animation states of the player
void PlayerC::SetInitialStates() noexcept
{
	currentAnimationState = PlayerAnimation::IDLING;
	currentArmorState = ArmorState::ARMOR;
	currentDeathAnimationState = PlayerAnimation::IDLING;
}

// Loads the armor sprites into memory once, this index will be used every time a player is rendered
void PlayerC::LoadTextures() noexcept
{
	armorTexture = SOIL_load_OGL_texture("Sprites/ArmorArthur.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	noArmorTexture = SOIL_load_OGL_texture("Sprites/NoArmorArthur.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	deathTexture = SOIL_load_OGL_texture("Sprites/DeathArthur.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

// Increments player position based on a delta, velocities and handles attack state changes
void PlayerC::Update(const DWORD milliseconds)
{
	// Increase base attack cooldowns
	currentTimeAttack += milliseconds;

	if (isAttacking) { Attack(milliseconds); }

	// Get how far the character moves based on the state changer
	xPosition += HandleStateChange(milliseconds);
	yPosition += yVelocity * milliseconds;

	// Set the jumping animation if the character is in the air
	if (yPosition <= BackgroundManagerC::GetInstance()->GetBottomPlatformPosition())
	{
		isInAir = false;
		yPosition = BackgroundManagerC::GetInstance()->GetBottomPlatformPosition();
		yVelocity = 0;
	}
	else
	{
		if (!isAttacking) 
		{ 
			currentAnimationState = PlayerAnimation::JUMPING; 
		}
		yVelocity -= static_cast<GLfloat>(milliseconds * downAcceleration);
	}
}

// Calculates a delta based on input from the player
GLfloat PlayerC::HandleStateChange(const DWORD milliseconds)
{
	float_t deltaX = 0;
	isCrouching = false;

	if (!isInDeathAnimation)
	{
		// Read states and call helper functions based on if the state is set or not
		for (int32_t index = 0; index < MovementState::PLAYER_MAX_MOVE_STATES; ++index)
		{
			if (movementStates[index])
			{
				switch (index) {
				case IDLE:
					IdleStateChange();
					break;
				case RIGHT:
					MoveRightStateChange(milliseconds, deltaX);
					break;
				case LEFT:
					MoveLeftStateChange(milliseconds, deltaX);
					break;
				case CROUCH:
					CrouchStateChange(deltaX);
					break;
				case JUMP:
					JumpStateChange();
					break;
				case CLIMB:
					break;
				case ATTACK:
					AttackStateChange(deltaX);
					break;
				default:
					break;
				}
			}
		}
	}

	return deltaX;
}

// Handles all the logic for changing the state to idle
void PlayerC::IdleStateChange() noexcept
{ 
	if (CanSwapAnimations()) 
	{ 
		currentAnimationState = PlayerAnimation::IDLING; 
	}
}

// Handles all the logic for changing the state to moving right
void PlayerC::MoveRightStateChange(const DWORD milliseconds, float_t& deltaX)
{
	if (CanSwapAnimations())
	{
		// Update the cooldown and check if it is ready to change animations
		currentTimeRight += milliseconds;
		if (currentTimeRight > runningDeltaTime)
		{
			currentTimeRight = 0;
			SwapRunningAnimationState();
		}
	}

	// If the position is within the bounds of the stage, and the player has the ability to move
	if ((xPosition < (BackgroundManagerC::GetInstance()->GetRightWallPosition() - spriteWidth)) && CanMove())
	{
		deltaX = static_cast<float_t>(milliseconds * movementSpeed);
	}

	isFacingRight = true;
	MoveCamera();
}

// Handles all the logic for changing the state to moving left
void PlayerC::MoveLeftStateChange(const DWORD milliseconds, float_t& deltaX)
{
	if (CanSwapAnimations())
	{
		// Update the cooldown and check if it is ready to change animations
		currentTimeLeft += milliseconds;
		if (currentTimeLeft > runningDeltaTime)
		{
			currentTimeLeft = 0;
			SwapRunningAnimationState();
		}
	}

	// If the position is within the bounds of the stage, and the player has the ability to move
	if ((xPosition > BackgroundManagerC::GetInstance()->GetLeftWallPosition()) && CanMove())
	{
		deltaX = static_cast<float_t>(milliseconds * -movementSpeed);
	}

	isFacingRight = false;
	MoveCamera();
}

// Handles all the logic for changing the state to crouching
void PlayerC::CrouchStateChange(float_t& deltaX) noexcept
{
	if (CanSwapAnimations())
	{
		currentAnimationState = PlayerAnimation::CROUCHED;
		isCrouching = true;
	}

	// Zero out the x movement if crouching
	deltaX = 0;
}

// Handles all the logic for changing the state to jumping
void PlayerC::JumpStateChange()
{
	MoveCamera();
}

// Handles all the logic for changing the state to attacking
void PlayerC::AttackStateChange(float_t& deltaX) noexcept
{
	if (!isAttacking)
	{
		// Update the cooldown and check if it is ready to change animations
		if (currentTimeAttack > attackDeltaTime)
		{
			currentTimeAttack = 0;
			currentTimeAttackAnimation = 0;
			isAttacking = true;

			// Do not zero out the movement if player is jumping
			if (!isInAir) 
			{ 
				deltaX = 0; 
			}
		}
	}
}

// A repeated boolean indicating that the player can move
bool8_t PlayerC::CanMove() noexcept
{
	return (!isAttacking || isInAir);
}

// Moves the orthographic camera in OpenGL dynamically to the Player's position
void PlayerC::MoveCamera()
{
	// Move the camera on the x axis if the player is not near the bounds of the stage
	if ((xPosition > 0) && (xPosition < (BackgroundManagerC::GetInstance()->GetRightWallPosition() - CGame::GetInstance()->GetOrthWidth())))
	{
		ReshapeGL(CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), static_cast<int32_t>(xPosition));
	}
}

// Draw Player to the screen
void PlayerC::Render(const DWORD milliseconds)
{
	// Render a separate animation if the character is dying
	if (GetIsInDeathAnimation())
	{
		RenderDeathAnimation(milliseconds);
		return;
	}

	// Potentially render an invincibility frame if the player recently took damage
	if (GetIsInInvincibleFrame())
	{
		if (RenderIFrame(milliseconds)) 
		{ 
			return; 
		}
	}

	UVRectangle uvRectangle{ 0,1,0,1 };
	VertexRectangle vertexRectangle{
		0 + xPosition,
		spriteWidth + xPosition,
		0 + yPosition,
		spriteHeight + yPosition
	};

	// Build UV rectangle
	// Swap the U and the V coordinate based on Player's orientation
	if (GetIsFacingRight())
	{
		uvRectangle.xLeftCoord = (1.0f / (PlayerAnimation::PLAYER_MAX_ANIMATION_STATES - 1)) * (currentAnimationState - 1);
		uvRectangle.xRightCoord = (1.0f / (PlayerAnimation::PLAYER_MAX_ANIMATION_STATES - 1)) * currentAnimationState;
	}
	else
	{
		uvRectangle.xLeftCoord = (1.0f / (PlayerAnimation::PLAYER_MAX_ANIMATION_STATES - 1)) * currentAnimationState;
		uvRectangle.xRightCoord = (1.0f / (PlayerAnimation::PLAYER_MAX_ANIMATION_STATES - 1)) * (currentAnimationState - 1);
	}

	RenderQuads(currentTexture, uvRectangle, vertexRectangle);
}

// Handles the logic for rendering a invis frames which lasts as long as the cooldown suggests
bool8_t PlayerC::RenderIFrame(const DWORD milliseconds) noexcept
{
	// Update the cooldown and check if it is ready to end iFrames
	currentTimeInvinsibility += milliseconds;
	if (currentTimeInvinsibility > invincibilityDeltaTime)
	{
		// End iFrames
		previousInvincible = false;
		isInInvincibilityFrame = false;
		currentTimeInvinsibility = 0;
	}

	// If the player was rendered last buffer swap, swap invincible
	previousInvincible = !previousInvincible;
	return previousInvincible;
}

// Specific render functions only for the death animation, which requires reading a specific 
// sprite sheet and a different way of determining the animation state
void PlayerC::RenderDeathAnimation(const DWORD milliseconds)
{
	// Update the cooldown and check if it is ready to change animation state
	currentTimeDeathAnimation += milliseconds;
	if (currentTimeDeathAnimation > deathAnimDeltaTime)
	{
		// Increment the sprite to render
		++currentDeathAnimationState;
		currentTimeDeathAnimation = 0;

		// If at the end of the sprite sheet, player died and call the state change
		if (currentDeathAnimationState == PlayerAnimation::PLAYER_MAX_ANIMATION_STATES)
		{
			// Transition screens
			CGame::GetInstance()->EndGame(true);
			return;
		}
	}

	UVRectangle uvRectangle{ 0,1,0,1 };

	// Build UV rectangle
	// Swap the U and the V coordinate based on Player's orientation
	if (GetIsFacingRight())
	{
		uvRectangle.xLeftCoord = (1.0f / (PlayerAnimation::PLAYER_MAX_ANIMATION_STATES - 1)) * (currentDeathAnimationState - 1);
		uvRectangle.xRightCoord = (1.0f / (PlayerAnimation::PLAYER_MAX_ANIMATION_STATES - 1)) * currentDeathAnimationState;
	}
	else
	{
		uvRectangle.xLeftCoord = (1.0f / (PlayerAnimation::PLAYER_MAX_ANIMATION_STATES - 1)) * currentDeathAnimationState;
		uvRectangle.xRightCoord = (1.0f / (PlayerAnimation::PLAYER_MAX_ANIMATION_STATES - 1)) * (currentDeathAnimationState - 1);
	}

	VertexRectangle vertexRectangle{
		0 + xPosition,
		PlayerC::GetSpriteWidth() + xPosition,
		0 + yPosition,
		PlayerC::GetSpriteHeight() + yPosition
	};

	RenderQuads(currentTexture, uvRectangle, vertexRectangle);
}

// Hub for the attacking functionality (both crouching and standing)
void PlayerC::Attack(const DWORD milliseconds)
{
	if (movementStates[CROUCH])
	{
		CrouchAttacking(milliseconds);
	}
	else
	{
		StandAttacking(milliseconds);
	}
}

// Handles the crouching state change between weapon throwing based on a cooldown
void PlayerC::CrouchAttacking(const DWORD milliseconds)
{
	if (currentAnimationState == PlayerAnimation::THROW_WEP_CROUCH_2)
	{
		// Update the cooldown and check if it is ready to change animation state
		currentTimeAttackAnimation += milliseconds;
		if (currentTimeAttackAnimation > attackAnimDeltaTime)
		{
			if (WeaponManagerC::GetInstance()->GetSpears().size() < WeaponManagerC::GetMaxActiveSpears())
			{
				ThrowSpear();
			}
		}
	}
	else
	{
		currentAnimationState = PlayerAnimation::THROW_WEP_CROUCH_1;

		// Update the cooldown and check if it is ready to change animation state
		currentTimeAttackAnimation += milliseconds;
		if (currentTimeAttackAnimation > attackAnimDeltaTime)
		{
			currentTimeAttackAnimation = 0;
			currentAnimationState = PlayerAnimation::THROW_WEP_CROUCH_2;
		}
	}
}

// Handles the standing state change between weapon throwing based on a cooldown
void PlayerC::StandAttacking(const DWORD milliseconds)
{
	if (currentAnimationState == PlayerAnimation::THROW_WEP_STAND_2)
	{
		// Update the cooldown and check if it is ready to change animation state
		currentTimeAttackAnimation += milliseconds;
		if (currentTimeAttackAnimation > attackAnimDeltaTime)
		{
			if (WeaponManagerC::GetInstance()->GetSpears().size() < WeaponManagerC::GetMaxActiveSpears())
			{
				ThrowSpear();
			}
		}
	}
	else
	{
		currentAnimationState = PlayerAnimation::THROW_WEP_STAND_1;

		// Update the cooldown and check if it is ready to change animation state
		currentTimeAttackAnimation += milliseconds;
		if (currentTimeAttackAnimation > attackAnimDeltaTime)
		{
			currentTimeAttackAnimation = 0;
			currentAnimationState = PlayerAnimation::THROW_WEP_STAND_2;
		}
	}
}

// Activates a spear and makes the player stop its attacking animation
void PlayerC::ThrowSpear()
{
	currentAnimationState = PlayerAnimation::IDLING;
	isAttacking = false;
	SetInputAttacking(false);	// Debounces the attack key to prevent player from holding down attack button

	WeaponManagerC::GetInstance()->AddSpear();
	AudioManagerC::GetInstance()->PlayThrowWeapon();
}

// A repeated boolean indicating that the player can change animation states
bool8_t PlayerC::CanSwapAnimations() noexcept
{
	return (!isAttacking && !isInAir);
}

void PlayerC::TakeDamage()
{
	if (!isInInvincibilityFrame && !GetDebugMode())
	{
		isInInvincibilityFrame = true;

		// If dying, don't set any new animation state, leave the dying animation as is
		if (!isInDeathAnimation)
		{
			AudioManagerC::GetInstance()->PlayHitPlayer();
			++currentArmorState;
			SetArmorTexture(currentArmorState);
		}
	}
}

// Handles swapping booleans for death 
void PlayerC::KillPlayer()
{
	isAttacking = false;
	isInDeathAnimation = true;
	AudioManagerC::GetInstance()->PlayLifeLost();
}

void PlayerC::SetInputJumped(const bool8_t jumpState) noexcept
{
	if (jumpState && !isInAir && !isInDeathAnimation)
	{
		// This set of code MUST go here because IsInAir is used multiple times in the state machine check in update
		isInAir = true;
		yVelocity = static_cast<GLfloat>(jumpYVelocity);
		movementStates[MovementState::JUMP] = jumpState;
	}
}

void PlayerC::SetArmorTexture(const int32_t armorState)
{
	switch (armorState)
	{
	case ArmorState::ARMOR:
		currentTexture = armorTexture;
		break;
	case ArmorState::NOARMOR:
		currentTexture = noArmorTexture;
		break;
	case ArmorState::DEAD:
		currentTexture = deathTexture;
		if (!GetIsInDeathAnimation())
		{
			KillPlayer();
		}
		break;
	default:
		break;
	}
}

void PlayerC::SwapRunningAnimationState() noexcept
{
	if (currentAnimationState == PlayerAnimation::RUNNING_1)
	{
		currentAnimationState = PlayerAnimation::RUNNING_2;
	}
	else
	{
		currentAnimationState = PlayerAnimation::RUNNING_1;
	}
}