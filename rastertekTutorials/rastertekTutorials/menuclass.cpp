////////////////////////////////////////////////////////////////////////////////
// Filename: menuclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "menuclass.h"

MenuClass::MenuClass(){
	m_Texture = 0;
	m_TextureShader = 0;
	m_Text = 0;
}

MenuClass::MenuClass(const MenuClass& other){
}

MenuClass::~MenuClass(){
}

void MenuClass::Shutdown(){
	// Shutdown this menu

	// Delete the menu's sliderbars
	while (!m_Sliderbars.empty()){
		delete m_Sliderbars.front();
		m_Sliderbars.pop_front();
	}

	// Delete the menu's buttons
	while (!m_Buttons.empty()){
		delete m_Buttons.front();
		m_Buttons.pop_front();
	}

	// Shutdown the text object for this menu
	if (m_Text){
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	m_TextureShader = 0;
	m_Texture = 0;

	return;
}

void MenuClass::SetPosition(int newX, int newY){
	// Store the new menu location
	m_menuX = newX;
	m_menuY = newY;

	return;
}

void MenuClass::SetDimensions(int newWidth, int newHeight){
	// Store the new menu dimensions
	m_menuWidth = newWidth;
	m_menuHeight = newHeight;

	return;
}

bool MenuClass::HitCheck(int cursorX, int cursorY){
	// Check if the cursor is currently over this menu
	if (cursorX >= m_menuX && cursorX < m_menuX + m_menuWidth && cursorY >= m_menuY && cursorY < m_menuY + m_menuHeight){
		return true;
	}

	return false;
}

ButtonAction MenuClass::LeftMouseClick(int cursorX, int cursorY){
	// If a button in the menu was clicked return the action that should be
	// executed as a result, otherwise return NOBUTTON
	std::list<ButtonClass*>::iterator button;

	for (button = m_Buttons.begin(); button != m_Buttons.end(); ++button){
		if ((*button)->HitCheck(cursorX, cursorY)) return (*button)->Click();
	}

	return BUTTON_NOBUTTON;
}

bool MenuClass::LeftMousePressed(int cursorX, int cursorY){
	// If the mouse is pressed or was clicked over a slider, adjust that slider
	// appropriately
	std::list<SliderbarClass*>::iterator sliderbar;

	for (sliderbar = m_Sliderbars.begin(); sliderbar != m_Sliderbars.end(); ++sliderbar){
		if ((*sliderbar)->HitCheck(cursorX, cursorY)){
			(*sliderbar)->Click(cursorX);
			return true;
		}
	}

	return false;
}

bool MenuClass::GetTooltip(int cursorX, int cursorY, int& tooltipX, int& tooltipY, int& tooltipWidth, int& tooltipHeight, ButtonAction& action){
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

	// If a button is under the cursor, set action and the position of tooltip
	// and return true
	// NOTE: Default tooltip location is to the right of the button it describes
	for (button = m_Buttons.begin(); button != m_Buttons.end(); ++button){
		if ((*button)->HitCheck(cursorX, cursorY)){
			action = (*button)->Click();
			(*button)->GetTooltipPosition(tooltipX, tooltipY);
			return true;
		}
	}

	// If no button was under the cursor return false
	return false;
}

bool MenuClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldmatrix, D3DXMATRIX viewmatrix, D3DXMATRIX orthomatrix){
	bool result;
	std::list<ButtonClass*>::iterator button;
	std::list<SliderbarClass*>::iterator sliderbar;

	// Ensure the texture bitmap is scaled to the right dimensions
	m_Texture->SetDimensions(m_menuWidth, m_menuHeight);

	// Render the menu
	result = m_Texture->Render(deviceContext, m_menuX, m_menuY);
	if (!result){
		return false;
	}

	result = m_TextureShader->Render(deviceContext, m_Texture->GetIndexCount(), worldmatrix, viewmatrix, orthomatrix, m_Texture->GetTexture(), PSTYPE_NORMAL);
	if (!result){
		return false;
	}

	// Render the menu's buttons
	for (button = m_Buttons.begin(); button != m_Buttons.end(); ++button){
		result = (*button)->Render(deviceContext);
		if (!result){
			return false;
		}

		result = m_TextureShader->Render(deviceContext, (*button)->GetIndexCount(), worldmatrix, viewmatrix, orthomatrix, (*button)->GetTexture(), PSTYPE_NORMAL);
		if (!result){
			return false;
		}
	}

	// Render the menu's sliderbars
	for (sliderbar = m_Sliderbars.begin(); sliderbar != m_Sliderbars.end(); ++sliderbar){
		result = (*sliderbar)->RenderSliderbar(deviceContext);
		if (!result){
			return false;
		}

		result = m_TextureShader->Render(deviceContext, (*sliderbar)->GetSliderbarIndexCount(), worldmatrix, viewmatrix, orthomatrix, (*sliderbar)->GetSliderbarTexture(), PSTYPE_NORMAL);
		if (!result){
			return false;
		}

		result = (*sliderbar)->RenderSlider(deviceContext);
		if (!result){
			return false;
		}

		result = m_TextureShader->Render(deviceContext, (*sliderbar)->GetSliderIndexCount(), worldmatrix, viewmatrix, orthomatrix, (*sliderbar)->GetSliderTexture(), PSTYPE_NORMAL);
		if (!result){
			return false;
		}
	}

	// Render the menu's text
	result = m_Text->Render(deviceContext, worldmatrix, orthomatrix);
	if (!result){
		return false;
	}

	return true;
}

bool MenuClass::GetExclusive(){
	return m_exclusive;
}

bool MenuClass::GetCloseOnMiss(){
	return m_closeOnMiss;
}

bool MenuClass::Initialize(int screenWidth, int screenHeight, BitmapClass* texture, TextureShaderClass* textureShader, FontClass* font, FontShaderClass* fontShader, D3DXMATRIX viewMatrix){
	// Universal menu initialization
	bool result;

	// Store the provided texture and texture shader
	m_Texture = texture;
	m_TextureShader = textureShader;

	// Initialize the text object
	result = InitializeText(font, fontShader, screenWidth, screenHeight, viewMatrix);
	if (!result){
		return false;
	}

	return true;
}

void MenuClass::PositionMainMenu(int screenWidth, int screenHeight){
	// Set the position and size of this menu to that of a standard menu within
	// the Main Menus based on the screen resolution

	// NOTE: There should be a better way of sizing menus based on the screen
	//       resolution

	// 800x600 Setup
	if (screenWidth == 800 && screenHeight == 600){
		m_menuWidth = 400;
		m_menuHeight = 350;

		m_menuX = (screenWidth - m_menuWidth) / 2;
		m_menuY = 100;
	} else{
		m_menuWidth = 450;
		m_menuHeight = 400;

		m_menuX = (screenWidth - m_menuWidth) / 2;
		m_menuY = 150;
	}

	return;
}

void MenuClass::PositionMenubar(int screenWidth, int screenHeight){
	// Set the position and size of this menu to that of a menubar along the
	// bottom of the screen based on the screen resolution

	// NOTE: There should be a better way of sizing menus based on the screen
	//       resolution

	// The Combat Menubar is screenWidth x 100 across the bottom of the screen
	m_menuWidth = screenWidth;
	m_menuHeight = 100;

	m_menuX = 0;
	m_menuY = screenHeight - m_menuHeight;

	return;
}

void MenuClass::PositionCombatMainMenu(int screenWidth, int screenHeight){
	// Set the position and size of this menu to that of a standard menu within
	// the Combat Map based on the screen resolution

	// NOTE: There should be a better way of sizing menus based on the screen
	//       resolution

	// 800x600 Setup
	if (screenWidth == 800 && screenHeight == 600){
		m_menuWidth = 300;
		m_menuHeight = 400;

		m_menuX = (screenWidth - m_menuWidth) / 2;
		m_menuY = 125;
	} else{
		m_menuWidth = 350;
		m_menuHeight = 400;

		m_menuX = (screenWidth - m_menuWidth) / 2;
		m_menuY = 150;
	}

	return;
}

bool MenuClass::NewButton(ButtonAction buttonAction, int buttonX, int buttonY, int buttonWidth, int buttonHeight, BitmapClass* standardButtonTexture){
	// Initialize a button with the provided parameters and add it to the menu
	ButtonClass* newButton;

	newButton = new ButtonClass;
	if (!newButton){
		return false;
	}

	newButton->Initialize(buttonAction, buttonX, buttonY, buttonWidth, buttonHeight, standardButtonTexture);

	m_Buttons.push_back(newButton);

	return true;
}

bool MenuClass::NewSliderbar(float* variable, int sliderbarX, int sliderbarY, int sliderbarWidth, int sliderbarHeight, int sliderWidth, int sliderHeight, BitmapClass* sliderTexture, BitmapClass* sliderbarTexture){
	// Create and initialize a new sliderbar with the provided parameters and
	// add it to the menu
	SliderbarClass* newSliderbar;

	newSliderbar = new SliderbarClass;
	if (!newSliderbar){
		return false;
	}

	newSliderbar->Initialize(variable, sliderbarX, sliderbarY, sliderbarWidth, sliderbarHeight, sliderWidth, sliderHeight, sliderTexture, sliderbarTexture);

	m_Sliderbars.push_back(newSliderbar);

	return true;
}

bool MenuClass::InitializeText(FontClass* font, FontShaderClass* fontShader, int screenWidth, int screenHeight, D3DXMATRIX viewMatrix){
	// Initialize the Static Text object to display text on this menu
	m_Text = new StaticTextClass;
	if (!m_Text){
		return false;
	}

	m_Text->Initialize(font, fontShader, screenWidth, screenHeight, viewMatrix);

	return true;
}
