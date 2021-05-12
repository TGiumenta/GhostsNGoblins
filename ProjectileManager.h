#pragma once

#include <vector>

class VenomC;

class ProjectileManagerC
{
public:
	static ProjectileManagerC* CreateInstance();
	static ProjectileManagerC* GetInstance() noexcept { return projectileManagerInstance; }
	
	ProjectileManagerC(const ProjectileManagerC& other) = delete;
	ProjectileManagerC(ProjectileManagerC&& other) noexcept = delete;
	ProjectileManagerC& operator=(const ProjectileManagerC& other) = delete;
	ProjectileManagerC& operator=(ProjectileManagerC&& other) noexcept = delete;
	~ProjectileManagerC() { projectileManagerInstance = nullptr; }

	void AddVenom(const GLfloat xPosition, const GLfloat yPosition, const GLfloat venomXVelocity, const GLfloat venomYVelocity);
	std::vector<VenomC*>& GetVenoms() noexcept { return activeVenoms; }
	void RemoveVenom(const int32_t index) noexcept;

	void UpdateProjectiles(const DWORD milliseconds);
	void RenderProjectiles();

private:
	ProjectileManagerC() = default;
	static void LoadTextures() noexcept;
	void UpdateVenoms(const DWORD milliseconds);
	void RenderVenoms();
	
	static ProjectileManagerC* projectileManagerInstance;

	/* Texture Indexes */
	static GLuint venomTexture;

	/* Dynamic Projectile Trackers */
	std::vector<VenomC*> activeVenoms;
};