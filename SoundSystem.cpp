/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A manager which interacts with the FMOD library to produce sounds. Contains multiple 
				  functions which wraps up logic from the AudioManager and interacts with the library.
				  Can both create and remove sounds, as well as play them from specific channels. Also 
				  contains a helper function to adjust the master volume on all sound groups.
--------------------------------------------------------------------------------------------------*/

#include "SoundSystem.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "baseTypes.h"

SoundSystemC* SoundSystemC::soundManagerInstance = nullptr;

SoundSystemC* SoundSystemC::CreateInstance()
{
	if (soundManagerInstance == nullptr)
	{
		soundManagerInstance = new SoundSystemC();
	}

	return soundManagerInstance;
}

// Creates the Sound System
SoundSystemC::SoundSystemC()
{
	if (FMOD::System_Create(&mSystem) != FMOD_OK) { return; }

	// Check if the PC has a sound driver
	int32_t driverCount = 0;
	mSystem->getNumDrivers(&driverCount);
	if (driverCount == 0) { return; }

	// Initialize an instance with 12 channels
	mSystem->init(12, FMOD_INIT_NORMAL, nullptr);

	SetMasterVolume(masterVolume);
};

// Adds the sound to the system memory
void SoundSystemC::CreateSound(Sound* pSound, const char8_t* const pFile)
{
	mSystem->createSound(pFile, FMOD_DEFAULT, 0, pSound);
}

// Plays sound through the default channel
void SoundSystemC::PlaySound(const Sound pSound, const bool8_t loop)
{
	if (pSound == nullptr) { return; }

	if (!loop)
	{
		pSound->setMode(FMOD_LOOP_OFF);
	}
	else
	{
		pSound->setMode(FMOD_LOOP_NORMAL);
		pSound->setLoopCount(-1);
	}

	mSystem->playSound(pSound, 0, false, 0);
}

// Removes the pointer to the memory that points at the sound information
void SoundSystemC::ReleaseSound(const Sound pSound)
{
	if (pSound == nullptr) { return; }
	pSound->release();
}

// Adjusts volume on all channels
void SoundSystemC::SetMasterVolume(const float_t volume)
{
	// Lower volume
	FMOD::SoundGroup* masterGroup{};
	mSystem->getMasterSoundGroup(&masterGroup);
	masterGroup->setVolume(volume);
}