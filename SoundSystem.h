#pragma once

#include "fmod.hpp"
#include "fmod_errors.h"
#include "baseTypes.h"

typedef FMOD::Sound* Sound;

class SoundSystemC final
{
public:
	static SoundSystemC* CreateInstance();
	static SoundSystemC* GetInstance() noexcept { return soundManagerInstance; }

	SoundSystemC(const SoundSystemC& other) = delete;
	SoundSystemC(SoundSystemC&& other) noexcept = delete;
	SoundSystemC& operator=(const SoundSystemC& other) = delete;
	SoundSystemC& operator=(SoundSystemC&& other) noexcept = delete;
	~SoundSystemC() = default;

	void CreateSound(Sound* pSound, const char8_t* pFile);
	void PlaySound(const Sound pSound, const bool8_t loop);
	void ReleaseSound(const Sound pSound);

private:
	SoundSystemC();
	void SetMasterVolume(const float_t volume);

	static SoundSystemC* soundManagerInstance;
	
	FMOD::System* mSystem;
	const float_t masterVolume = 0.09f;
};