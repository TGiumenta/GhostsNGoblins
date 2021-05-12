#pragma once

#include <vector>

class SpearC;

class WeaponManagerC
{
public:
	static WeaponManagerC* CreateInstance();
	static WeaponManagerC* GetInstance() noexcept { return weaponManagerInstance; }
	
	WeaponManagerC(const WeaponManagerC& other) = delete;
	WeaponManagerC(WeaponManagerC&& other) noexcept = delete;
	WeaponManagerC& operator=(const WeaponManagerC& other) = delete;
	WeaponManagerC& operator=(WeaponManagerC&& other) noexcept = delete;
	~WeaponManagerC();

	void Update(const DWORD milliseconds);
	void Render();

	void AddSpear();
	void RemoveSpear(const int32_t index);
	std::vector<SpearC*>& GetSpears() noexcept { return activeSpears; }

	static const size_t GetMaxActiveSpears() { return maxActiveSpears; }

private:
	WeaponManagerC() = default;
	
	static void	LoadTextures() noexcept;
	void UpdateSpears(const DWORD milliseconds);
	void RenderSpears();
	void DeleteAllSpears();

	static WeaponManagerC* weaponManagerInstance;

	/* Texture Index */
	static GLuint spearTexture;

	/* Dynamic Enemy Trackers */
	std::vector<SpearC*> activeSpears;
	static const size_t maxActiveSpears = 3;
};