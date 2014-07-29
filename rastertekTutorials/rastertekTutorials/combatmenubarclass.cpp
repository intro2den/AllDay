////////////////////////////////////////////////////////////////////////////////
// Filename: combatmenubarclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "combatmenubarclass.h"

CombatMenubarClass::CombatMenubarClass(){
}

CombatMenubarClass::CombatMenubarClass(const CombatMenubarClass& other){
}

CombatMenubarClass::~CombatMenubarClass(){
}

bool CombatMenubarClass::Initialize(int screenWidth, int screenHeight, BitmapClass* texture, BitmapClass* standardButtonTexture, TextureShaderClass* textureShader, FontClass* font, FontShaderClass* fontShader, D3DXMATRIX viewMatrix, ID3D11Device* device, ID3D11DeviceContext* deviceContext){
	// Initialize a Combat Menubar
	bool result;
	int buttonWidth, buttonHeight;
	int buttonXOffset, buttonYOffset, buttonSpacing;

	// Do standard menu initialization, create a text object
	result = MenuClass::Initialize(screenWidth, screenHeight, texture, textureShader, font, fontShader, viewMatrix);
	if (!result){
		return false;
	}

	// Set the size and position of the menu
	PositionMenubar(screenWidth, screenHeight);

	// The Combat Menubar is not exclusive and does not close on a miss
	m_exclusive = false;
	m_closeOnMiss = false;

	buttonWidth = 32;
	buttonHeight = 32;
	buttonXOffset = screenWidth - int(2.5f * buttonWidth);
	buttonYOffset = (m_menuHeight - 2 * buttonHeight) / 2;
	buttonSpacing = buttonHeight;

	// Add buttons to the Combat Menubar
	result = NewButton(BUTTON_ORDERMOVE, m_menuX + buttonXOffset, m_menuY + buttonYOffset, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	result = NewButton(BUTTON_ORDERATTACK, m_menuX + buttonXOffset, m_menuY + buttonYOffset + buttonSpacing, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	result = NewButton(BUTTON_ENDTURN, m_menuX + buttonXOffset + buttonWidth, m_menuY + buttonYOffset, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	result = NewButton(BUTTON_OPENCOMBATMENU, m_menuX + buttonXOffset + buttonWidth, m_menuY + buttonYOffset + buttonSpacing, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	return true;
}

bool CombatMenubarClass::GetTooltip(int cursorX, int cursorY, int& tooltipX, int& tooltipY, int& tooltipWidth, int& tooltipHeight, ButtonAction& action){
	// Return false if no button is under the cursor, otherwise set the position
	// and dimensions of the tooltip, set action as the ButtonAction returned by
	// left clicking on the button and return true
	std::list<ButtonClass*>::iterator button;

	// Set action to NOBUTTON before checking for a button
	action = BUTTON_NOBUTTON;

	// Set the default tooltip dimensions
	// NOTE: Currently proof of concept, fixed dimensions for tooltips
	//       At minimum. tooltip height should probably scale as needed to fit
	tooltipWidth = 200;
	tooltipHeight = 48;

	// Set the menubar tooltip position
	// NOTE: Menubar tooltips appear above the right end of the menubar
	tooltipX = m_menuWidth - tooltipWidth;
	tooltipY = m_menuY - tooltipHeight;

	// If a button is under the cursor, set action and the position of tooltip
	// and return true
	// NOTE: Default tooltip location is to the right of the button it describes
	for (button = m_Buttons.begin(); button != m_Buttons.end(); ++button){
		if ((*button)->HitCheck(cursorX, cursorY)){
			action = (*button)->Click();
			return true;
		}
	}

	// If no button was under the cursor return false
	return false;
}
