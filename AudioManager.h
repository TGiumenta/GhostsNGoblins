#pragma once

#include "SoundSystem.h"

class AudioManagerC final
{
public:
	static AudioManagerC* CreateInstance();
	static AudioManagerC* GetInstance() noexcept { return audioManagerInstance; }
	
	AudioManagerC(const AudioManagerC& other) = delete;
	AudioManagerC(AudioManagerC&& other) noexcept = delete;
	AudioManagerC& operator=(const AudioManagerC& other) = delete;
	AudioManagerC& operator=(AudioManagerC&& other) noexcept = delete;
	~AudioManagerC();

	typedef enum {
		START_THEME,
		STAGE_THEME,
		COLLECTIBLE_PICKUP,
		THROW_WEAPON,
		SPAWN_ZOMBIE,
		KILL_ENEMY_1,
		KILL_ENEMY_2,
		SPIT_VENOM,
		MOVE_GHOST,
		CHASE_GHAST,
		HIT_PLAYER,
		LIFE_LOST,
		END_WIN_THEME,
		END_LOSE_THEME
	}SoundIndexes;

	/* Sound Players */
	void PlayStartTheme();
	void PlayStageTheme();
	void PlayCollectiblePickup();
	void PlayThrowWeapon();
	void PlaySpawnZombie();
	void PlayKillEnemy1();
	void PlayKillEnemy2();
	void PlaySpitVenom();
	void PlayMoveGhost();
	void PlayChaseGhast();
	void PlayHitPlayer();
	void PlayLifeLost();
	void PlayEndWinTheme();
	void PlayEndLoseTheme();

	uint32_t GetStartThemeLength() noexcept { return startThemeLength; }

private:
	AudioManagerC() = default;
	static void LoadSounds();
	
	static const uint32_t startThemeLength = 1000 * 5; // 5 seconds
	static const uint32_t maxSounds = 14;
	static AudioManagerC* audioManagerInstance;
	static SoundSystemC* soundSystem; 
	static Sound activeSounds[maxSounds];
};