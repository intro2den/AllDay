////////////////////////////////////////////////////////////////////////////////
// Filename: gameoptionsmenuclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "gameoptionsmenuclass.h"

GameOptionsMenuClass::GameOptionsMenuClass(){
	m_TooltipDelayText = 0;
}

GameOptionsMenuClass::GameOptionsMenuClass(const GameOptionsMenuClass& other){
}

GameOptionsMenuClass::~GameOptionsMenuClass(){
}

bool GameOptionsMenuClass::Initialize(int screenWidth, int screenHeight, BitmapClass* texture, BitmapClass* standardButtonTexture, BitmapClass* sliderTexture, BitmapClass* sliderbarTexture, TextureShaderClass* textureShader, FontClass* font, FontShaderClass* fontShader, D3DXMATRIX viewMatrix, ID3D11Device* device, ID3D11DeviceContext* deviceContext, float delay, float* tooltipSlider){
	// Initialize a Game Options Menu
	bool result;
	int buttonWidth, buttonHeight;
	int buttonXOffset, buttonYOffset, buttonSpacing;
	int textXOffset, textYOffset;

	// Do standard menu initialization
	result = MenuClass::Initialize(screenWidth, screenHeight, texture, textureShader, font, fontShader, viewMatrix);
	if (!result){
		return false;
	}

	// Initialize a text object for the tooltip delay text
	m_TooltipDelayText = new TextClass;
	if (!m_TooltipDelayText){
		return false;
	}

	m_TooltipDelayText->Initialize(font, fontShader, screenWidth, screenHeight, viewMatrix);

	// Set the size and position of the menu
	PositionMainMenu(screenWidth, screenHeight);

	// The Game Options Menu is exclusive and does not close on a miss
	m_exclusive = true;
	m_closeOnMiss = false;

	buttonWidth = 300;
	buttonHeight = 50;
	buttonXOffset = (m_menuWidth - buttonWidth) / 2;
	buttonYOffset = 75;
	buttonSpacing = buttonHeight + 25;

	// Add buttons to the Game Options Menu
	result = NewButton(BUTTON_SAVESETTINGS, m_menuX + buttonXOffset, m_menuY + buttonYOffset, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	// Add a sliderbar to control the tooltip delay
	result = NewSliderbar(tooltipSlider, m_menuX + 200, m_menuY + 30, 165, 20, 16, 32, sliderTexture, sliderbarTexture);
	if (!result){
		return false;
	}

	// Add text for the Game Options Menu's buttons
	textXOffset = buttonXOffset + 15;
	textYOffset = buttonYOffset + (buttonHeight / 2) - 4;

	result = m_Text->AddText(m_menuX + textXOffset, m_menuY + textYOffset, "Back", 0.0f, 0.0f, 0.0f, buttonWidth - 30, device, deviceContext);
	if (!result){
		return false;
	}

	// Add text for the tooltip delay
	result = UpdateTooltipDelayLabel(delay, device, deviceContext);
	if (!result){
		return false;
	}

	return true;
}

void GameOptionsMenuClass::Shutdown(){
	// Shutdown the Tooltip Delay text and then shutdown as a normal menu
	if (m_TooltipDelayText){
		m_TooltipDelayText->Shutdown();
		delete m_TooltipDelayText;
		m_TooltipDelayText = 0;
	}

	MenuClass::Shutdown();

	return;
}

bool GameOptionsMenuClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldmatrix, D3DXMATRIX viewmatrix, D3DXMATRIX orthomatrix){
	// Render as a normal menu and also render additional text
	bool result;

	result = MenuClass::Render(deviceContext, worldmatrix, viewmatrix, orthomatrix);
	if (!result){
		return false;
	}

	// Render the tooltip delay label
	result = m_TooltipDelayText->Render(deviceContext, worldmatrix, orthomatrix);
	if (!result){
		return false;
	}

	return true;
}

bool GameOptionsMenuClass::UpdateTooltipDelayLabel(float delay, ID3D11Device* device, ID3D11DeviceContext* deviceContext){
	// Replace the Tooltip Delay Label text with text displaying the provided
	// delay
	bool result;
	int textXOffset, textYOffset;
	char newString[30];
	char tempString[2];

	// Remove previous text
	m_TooltipDelayText->ClearText();

	// Calculate the offset for the text
	// NOTE: Currently hardcoded, this will need to change
	textXOffset = 55;
	textYOffset = 43;

	// Create the new string to be displayed
	strcpy_s(newString, "Tooltip Delay = ");

	// Extract the delay in seconds and tenths of a second from the provided floating point value
	_itoa_s(int(delay / 1000.0f), tempString, 10);
	strcat_s(newString, tempString);

	strcat_s(newString, ".");

	_itoa_s(int(delay / 100.0f) % 10, tempString, 10);
	strcat_s(newString, tempString);

	strcat_s(newString, " seconds");

	// Add the new text
	result = m_TooltipDelayText->AddText(m_menuX + textXOffset, m_menuY + textYOffset, newString, 0.0f, 0.0f, 0.0f, m_menuWidth - textXOffset, device, deviceContext);
	if (!result){
		return false;
	}

	return true;
}