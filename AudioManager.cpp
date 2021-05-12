/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which handles all of the sounds within the game. This manager speaks to the
				  Sound System which prevents having the rest of the game interact with the FMOD system 
				  and the Sound System. This manager creates all the sounds from a directory (which creates 
				  a reference to them to be used in helpers), and then also "releases" them, which gets rid 
				  of that pointer. This manager is what the rest of the game speaks to when trying to play 
				  specific sounds. 
--------------------------------------------------------------------------------------------------*/

#include "AudioManager.h"
#include "baseTypes.h"
#include "SoundSystem.h"

#pragma warning (disable : 26446)	// Disables warning: "Use gsl::at() instead of unchecked subscript operator"
#pragma warning (disable : 26482)	// Disables warning: "Only index into arrays using constant expressions"
#pragma warning (disable : 26409)	// Disables warning: "Avoid calling new and delete explicitly, use std::make_unique<T> instead"

AudioManagerC* AudioManagerC::audioManagerInstance = nullptr;
SoundSystemC* AudioManagerC::soundSystem = nullptr;
Sound AudioManagerC::activeSounds[maxSounds];

// Creates a singleton instance of the manager
AudioManagerC* AudioManagerC::CreateInstance()
{
	if (audioManagerInstance == nullptr)
	{
		audioManagerInstance = new AudioManagerC();
		soundSystem = SoundSystemC::CreateInstance();
		LoadSounds();
	}
	return audioManagerInstance;
}

// A wrapper for loading all the sounds into an array used in the play functions
void AudioManagerC::LoadSounds()
{ 
	// Create references to all the sounds
	soundSystem->CreateSound(&activeSounds[SoundIndexes::START_THEME] , "Sounds/StartTheme.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::STAGE_THEME], "Sounds/StageTheme.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::COLLECTIBLE_PICKUP], "Sounds/PickupCollectible.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::THROW_WEAPON], "Sounds/ThrowWeapon.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::SPAWN_ZOMBIE], "Sounds/SpawnZombie.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::KILL_ENEMY_1], "Sounds/EnemyDeath1.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::KILL_ENEMY_2], "Sounds/EnemyDeath2.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::SPIT_VENOM], "Sounds/SpitVenom.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::MOVE_GHOST], "Sounds/GhostMovement.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::CHASE_GHAST], "Sounds/ChaseGhast.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::HIT_PLAYER], "Sounds/HitPlayer.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::LIFE_LOST], "Sounds/LoseLife.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::END_WIN_THEME], "Sounds/EndWinTheme.wav");
	soundSystem->CreateSound(&activeSounds[SoundIndexes::END_LOSE_THEME], "Sounds/EndLoseTheme.wav");
}

// Destroys the manager
AudioManagerC::~AudioManagerC()
{
	// Release all the sounds
	for (int soundIndex = 0; soundIndex < maxSounds; ++soundIndex)
	{
		soundSystem->ReleaseSound(activeSounds[soundIndex]);
	}

	soundSystem = nullptr;
	audioManagerInstance = nullptr;
}

void AudioManagerC::PlayStartTheme()
{
	soundSystem->PlaySound(activeSounds[SoundIndexes::START_THEME], false);
}

void AudioManagerC::PlayStageTheme()
{
	soundSystem->PlaySound(activeSounds[SoundIndexes::STAGE_THEME], true);
}

void AudioManagerC::PlayCollectiblePickup()
{
	soundSystem->PlaySound(activeSounds[SoundIndexes::COLLECTIBLE_PICKUP], false);
}

void AudioManagerC::PlayThrowWeapon()
{
	soundSystem->PlaySound(activeSounds[SoundIndexes::THROW_WEAPON], false);
}

void AudioManagerC::PlaySpawnZombie()
{
	soundSystem->PlaySound(activeSounds[SoundIndexes::SPAWN_ZOMBIE], false);
}

void AudioManagerC::PlayKillEnemy1()
{
	soundSystem->PlaySound(activeSounds[SoundIndexes::KILL_ENEMY_1], false);
}

void AudioManagerC::PlayKillEnemy2()
{
	soundSystem->PlaySound(activeSounds[SoundIndexes::KILL_ENEMY_2], false);
}

void AudioManagerC::PlaySpitVenom()
{
	soundSystem->PlaySound(activeSounds[SoundIndexes::SPIT_VENOM], false);
}

void AudioManagerC::PlayMoveGhost()
{
	soundSystem->PlaySound(activeSounds[SoundIndexes::MOVE_GHOST], false);
}

void AudioManagerC::PlayChaseGhast()
{
	soundSystem->PlaySound(activeSounds[SoundIndexes::CHASE_GHAST], false);
}

void AudioManagerC::PlayHitPlayer()
{
	soundSystem->PlaySound(activeSounds[SoundIndexes::HIT_PLAYER], false);
}

void AudioManagerC::PlayLifeLost()
{
	soundSystem->ReleaseSound(activeSounds[SoundIndexes::HIT_PLAYER]);		// Don't play the hit track anymore
	soundSystem->ReleaseSound(activeSounds[SoundIndexes::STAGE_THEME]);		// Equivalent to stopping the track
	soundSystem->PlaySound(activeSounds[SoundIndexes::LIFE_LOST], false);
}

void AudioManagerC::PlayEndWinTheme()
{
	soundSystem->ReleaseSound(activeSounds[SoundIndexes::STAGE_THEME]);		// Equivalent to stopping the track
	soundSystem->PlaySound(activeSounds[SoundIndexes::END_WIN_THEME], false);
}

void AudioManagerC::PlayEndLoseTheme()
{
	soundSystem->ReleaseSound(activeSounds[SoundIndexes::STAGE_THEME]);		// Equivalent to stopping the track
	soundSystem->PlaySound(activeSounds[SoundIndexes::END_LOSE_THEME], false);
}

#pragma warning (disable : 26446)	// Enables warning: "Use gsl::at() instead of unchecked subscript operator"
#pragma warning (disable : 26482)	// Enables warning: "Only index into arrays using constant expressions"
#pragma warning (disable : 26409)	// Enables warning: "Avoid calling new and delete explicitly, use std::make_unique<T> instead"