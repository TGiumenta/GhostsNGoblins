#pragma once

class PlayerC;

class InputManagerC
{
public:
	static InputManagerC*		CreateInstance();
	static InputManagerC*		GetInstance() noexcept { return inputManagerInstance; };
	
	InputManagerC(const InputManagerC& other) = delete;
	InputManagerC(InputManagerC&& other) noexcept = delete;
	InputManagerC& operator=(const InputManagerC& other) = delete;
	InputManagerC& operator=(InputManagerC&& other) noexcept = delete;
	~InputManagerC() { inputManagerInstance = nullptr; }

	/* Animation Setters */
	void SetIsMovingRight(const bool8_t state) noexcept;
	void SetIsMovingLeft(const bool8_t state) noexcept;
	void SetIsCrouched(const bool8_t state) noexcept;
	void SetJumped(const bool8_t state) noexcept;
	void SetIdle(const bool8_t idleState) noexcept;
	void SetAttacked(const bool8_t attackState) noexcept;

	/* Setters and Getters */
	void SetPreviousSpaceState(const bool8_t spaceState) noexcept;
	bool8_t	GetPreviousSpaceState() noexcept;
	void ToggleDebug() noexcept;

private:
	InputManagerC() = default;
	
	static InputManagerC* inputManagerInstance;
	static PlayerC* playerInstance;

	bool8_t lastSpaceBarState;
};