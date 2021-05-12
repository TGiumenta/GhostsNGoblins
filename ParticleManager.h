#ifndef PARTICLE_MANAGER_HEADER		// Header Guard
#define PARTICLE_MANAGER_HEADER

#include <vector>

/* Positional Info */
#define FIRE_DEATH_OFFSET	5

class FireDeathC;

class ParticleManagerC
{
public:
	static ParticleManagerC* CreateInstance();
	static ParticleManagerC* GetInstance() noexcept { return particleManagerInstance; }
	~ParticleManagerC();

	void							UpdateParticles(DWORD const milliseconds);
	void							RenderParticles();

	void							AddFire(GLfloat const xPosition, GLfloat const yPosition);
	std::vector<FireDeathC*>		GetFires();
	void							RemoveFire(int32_t const position);

private:
	static ParticleManagerC* particleManagerInstance;

	/* Texture Indexes */
	GLuint fireDeathTexture;

	/* Dynamic Particle Tracker */
	std::vector<FireDeathC*> activeFireDeaths;

	ParticleManagerC() noexcept;

	void							LoadTextures() noexcept;
	void							UpdateFires(DWORD const milliseconds);
	void							RenderFire();
	void							DeleteAllFires();
};
#endif