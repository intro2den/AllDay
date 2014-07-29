////////////////////////////////////////////////////////////////////////////////
// Filename: combatmainmenuclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "combatmainmenuclass.h"

CombatMainMenuClass::CombatMainMenuClass(){
}

CombatMainMenuClass::CombatMainMenuClass(const CombatMainMenuClass& other){
}

CombatMainMenuClass::~CombatMainMenuClass(){
}

bool CombatMainMenuClass::Initialize(int screenWidth, int screenHeight, BitmapClass* texture, BitmapClass* standardButtonTexture, TextureShaderClass* textureShader, FontClass* font, FontShaderClass* fontShader, D3DXMATRIX viewMatrix, ID3D11Device* device, ID3D11DeviceContext* deviceContext){
	// Initialize a Combat Main Menu
	bool result;
	int buttonWidth, buttonHeight;
	int buttonXOffset, buttonYOffset, buttonSpacing;
	int textXOffset, textYOffset;

	// Do standard menu initialization, create a text object
	result = MenuClass::Initialize(screenWidth, screenHeight, texture, textureShader, font, fontShader, viewMatrix);
	if (!result){
		return false;
	}

	// Set the size and position of the menu
	PositionCombatMainMenu(screenWidth, screenHeight);

	// The Combat Main Menu is exclusive and does not close on a miss
	m_exclusive = true;
	m_closeOnMiss = false;

	buttonWidth = 250;
	buttonHeight = 50;
	buttonXOffset = (m_menuWidth - buttonWidth) / 2;
	buttonYOffset = 25;
	buttonSpacing = buttonHeight + 25;

	// Add buttons to the Combat Main Menu
	result = NewButton(BUTTON_CLOSEMENU, m_menuX + buttonXOffset, m_menuY + buttonYOffset, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	result = NewButton(BUTTON_EXITCOMBAT, m_menuX + buttonXOffset, m_menuY + buttonYOffset + buttonSpacing, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	result = NewButton(BUTTON_EXITAPPLICATION, m_menuX + buttonXOffset, m_menuY + buttonYOffset + 2 * buttonSpacing, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	// Add text for the Combat Main Menu's buttons
	textXOffset = buttonXOffset + 15;
	textYOffset = buttonYOffset + (buttonHeight / 2) - 4;

	result = m_Text->AddStaticText(m_menuX + textXOffset, m_menuY + textYOffset, "Close Menu", 0.0f, 0.0f, 0.0f, buttonWidth - 30, device, deviceContext);
	if (!result){
		return false;
	}

	result = m_Text->AddStaticText(m_menuX + textXOffset, m_menuY + textYOffset + buttonSpacing, "Exit to Main Menu", 0.0f, 0.0f, 0.0f, buttonWidth - 30, device, deviceContext);
	if (!result){
		return false;
	}

	result = m_Text->AddStaticText(m_menuX + textXOffset, m_menuY + textYOffset + 2 * buttonSpacing, "Exit Application", 0.0f, 0.0f, 0.0f, buttonWidth - 30, device, deviceContext);
	if (!result){
		return false;
	}

	return true;
}
