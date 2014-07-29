////////////////////////////////////////////////////////////////////////////////
// Filename: buttonclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "buttonclass.h"

ButtonClass::ButtonClass(){
	m_Texture = 0;
}

ButtonClass::ButtonClass(const ButtonClass& other){
}

ButtonClass::~ButtonClass(){
}

void ButtonClass::Initialize(ButtonAction buttonAction, int buttonX, int buttonY, int buttonWidth, int buttonHeight, BitmapClass* texture){
	// Store the action that clicking this button will perform
	m_action = buttonAction;

	// Store the button location
	SetPosition(buttonX, buttonY);

	// Store the button dimensions
	SetDimensions(buttonWidth, buttonHeight);

	// Store the provided texture
	m_Texture = texture;

	return;
}

void ButtonClass::SetPosition(int newX, int newY){
	// Store the new button location
	m_buttonX = newX;
	m_buttonY = newY;

	return;
}

void ButtonClass::SetDimensions(int newWidth, int newHeight){
	// Store the new button dimensions
	m_buttonWidth = newWidth;
	m_buttonHeight = newHeight;

	return;
}

bool ButtonClass::HitCheck(int cursorX, int cursorY){
	// Check if the cursor is currently over this button
	if (cursorX >= m_buttonX && cursorX < m_buttonX + m_buttonWidth && cursorY >= m_buttonY && cursorY < m_buttonY + m_buttonHeight){
		return true;
	}

	return false;
}

ButtonAction ButtonClass::Click(){
	// Return the action executed by clicking this button
	return m_action;
}

void ButtonClass::GetTooltipPosition(int& x, int& y){
	// Set the x and y coordinates to the default location for button tooltips
	// ie. Immediately to the right of the button
	x = m_buttonX + m_buttonWidth;
	y = m_buttonY;

	return;
}

bool ButtonClass::Render(ID3D11DeviceContext* deviceContext){
	bool result;

	// Ensure the texture bitmap is scaled to the right dimensions
	m_Texture->SetDimensions(m_buttonWidth, m_buttonHeight);

	// Render the button
	result = m_Texture->Render(deviceContext, m_buttonX, m_buttonY);
	if (!result){
		return false;
	}

	return true;
}

int ButtonClass::GetIndexCount(){
	return m_Texture->GetIndexCount();
}

ID3D11ShaderResourceView* ButtonClass::GetTexture(){
	return m_Texture->GetTexture();
}
