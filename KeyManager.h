#pragma once

#include <vector>

class KeyC;

class KeyManagerC
{
public:
	static KeyManagerC* CreateInstance();
	static KeyManagerC* GetInstance() noexcept { return keyManagerInstance; };
	
	KeyManagerC(const KeyManagerC& other) = delete;
	KeyManagerC(KeyManagerC&& other) noexcept = delete;
	KeyManagerC& operator=(const KeyManagerC& other) = delete;
	KeyManagerC& operator=(KeyManagerC&& other) noexcept = delete;
	~KeyManagerC() { keyManagerInstance = nullptr; }

	void Update(const DWORD milliseconds);
	void Render();

	std::vector<KeyC*>&	GetKeys() noexcept { return activeKeys; }
	void RemoveKey(const int32_t position) noexcept;

private:
	KeyManagerC() = default;
	static void LoadTextures() noexcept;
	static void SetupKeys();
	
	static KeyManagerC* keyManagerInstance;

	/* Texture Index */
	static GLuint keyTexture;

	/* Dynamic Key Trackers */
	static std::vector<KeyC*> activeKeys;

	/* Static Initial Locations */
	static const int numberStaticKeys = 1;
	static GLfloat initialKeyLocations[numberStaticKeys];
};