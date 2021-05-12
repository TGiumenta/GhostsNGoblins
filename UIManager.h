#pragma once

class UIManagerC
{
public:
	static UIManagerC* CreateInstance();
	static UIManagerC* GetInstance() noexcept { return uiManagerInstance; }
	
	UIManagerC(const UIManagerC& other) = delete;
	UIManagerC(UIManagerC&& other) noexcept = delete;
	UIManagerC& operator=(const UIManagerC& other) = delete;
	UIManagerC& operator=(UIManagerC&& other) noexcept = delete;
	~UIManagerC() { uiManagerInstance = nullptr; }

	void RenderHUD();

private:
	UIManagerC() = default;
	
	static void LoadTextures() noexcept;
	void RenderText(const GLfloat defaultPlayerXPosition) noexcept;
	void RenderScores(const GLfloat defaultPlayerXPosition, const GLfloat xLocation, int32_t score) noexcept;
	void RenderTime(const GLfloat defaultPlayerXPosition);
	void RenderDigit(const GLfloat defaultPlayerXPosition, const int32_t digitIndex, const int32_t digitToPrint, const GLfloat spriteHeight, const GLfloat xLocation) noexcept;

	static UIManagerC* uiManagerInstance;

	/* Texture Indexes */
	static GLuint uiTexture;
	static GLuint numbersTexture;

	static const GLfloat hudXOffset;
	static const GLfloat hudYOffset;
	static const GLfloat hudWidth;
	static const GLfloat hudHeight;

	static const GLfloat uiXPositionLeft;
	static const GLfloat uiXPositionRight;

	static const int32_t numberBase = 10;
	static const int32_t numbersInSheet = 11;
	static const int32_t colonDigitValue = 10;

	static const int32_t numberWidth = 12;
	static const int32_t numberHeight = 12;
	static const int32_t numberSpacing = -8;
	static const int32_t scoreHeight = 60;
	static const int32_t timeHeight = 40;

	static const int32_t digitOffset = 10;
};