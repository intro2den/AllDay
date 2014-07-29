////////////////////////////////////////////////////////////////////////////////
// Filename: optionsmenuclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "optionsmenuclass.h"

OptionsMenuClass::OptionsMenuClass(){
}

OptionsMenuClass::OptionsMenuClass(const OptionsMenuClass& other){
}

OptionsMenuClass::~OptionsMenuClass(){
}

bool OptionsMenuClass::Initialize(int screenWidth, int screenHeight, BitmapClass* texture, BitmapClass* standardButtonTexture, TextureShaderClass* textureShader, FontClass* font, FontShaderClass* fontShader, D3DXMATRIX viewMatrix, ID3D11Device* device, ID3D11DeviceContext* deviceContext){
	// Initialize an Options Menu
	bool result;
	int buttonWidth, buttonHeight;
	int buttonXOffset, buttonYOffset, buttonSpacing;
	int textXOffset, textYOffset;

	// Do standard menu initialization
	result = MenuClass::Initialize(screenWidth, screenHeight, texture, textureShader, font, fontShader, viewMatrix);
	if (!result){
		return false;
	}

	// Set the size and position of the menu
	PositionMainMenu(screenWidth, screenHeight);

	// The Options Menu is exclusive and does not close on a miss
	m_exclusive = true;
	m_closeOnMiss = false;

	buttonWidth = 300;
	buttonHeight = 50;
	buttonXOffset = (m_menuWidth - buttonWidth) / 2;
	buttonYOffset = 25;
	buttonSpacing = buttonHeight + 25;

	// Add buttons to the Options Menu
	result = NewButton(BUTTON_OPENGAMEOPTIONS, m_menuX + buttonXOffset, m_menuY + buttonYOffset, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	result = NewButton(BUTTON_CLOSEMENU, m_menuX + buttonXOffset, m_menuY + buttonYOffset + buttonSpacing, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	// Add text for the Options Menu's buttons
	textXOffset = buttonXOffset + 15;
	textYOffset = buttonYOffset + (buttonHeight / 2) - 4;

	result = m_Text->AddStaticText(m_menuX + textXOffset, m_menuY + textYOffset, "Game Options", 0.0f, 0.0f, 0.0f, buttonWidth - 30, device, deviceContext);
	if (!result){
		return false;
	}

	result = m_Text->AddStaticText(m_menuX + textXOffset, m_menuY + textYOffset + buttonSpacing, "Back", 0.0f, 0.0f, 0.0f, buttonWidth - 30, device, deviceContext);
	if (!result){
		return false;
	}

	return true;
}
