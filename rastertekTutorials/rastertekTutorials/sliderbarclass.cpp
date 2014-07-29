////////////////////////////////////////////////////////////////////////////////
// Filename: sliderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "sliderbarclass.h"

SliderbarClass::SliderbarClass(){
	m_SliderTexture = 0;
	m_SliderbarTexture = 0;
}

SliderbarClass::SliderbarClass(const SliderbarClass& other){
}

SliderbarClass::~SliderbarClass(){
}

void SliderbarClass::Initialize(float* variable, int sliderbarX, int sliderbarY, int sliderbarWidth, int sliderbarHeight, int sliderWidth, int sliderHeight, BitmapClass* sliderTexture, BitmapClass* sliderbarTexture){
	// Store the pointer to the variable that moving this sliderbar will adjust
	m_variable = variable;

	// Store the sliderbar location
	SetPosition(sliderbarX, sliderbarY);

	// Store the sliderbar and slider dimensions
	SetSliderbarDimensions(sliderbarWidth, sliderbarHeight);
	SetSliderDimensions(sliderWidth, sliderHeight);

	// Store the provided textures for the slider and sliderbar
	m_SliderTexture = sliderTexture;
	m_SliderbarTexture = sliderbarTexture;

	return;
}

void SliderbarClass::SetPosition(int newX, int newY){
	// Store the new sliderbar location
	m_sliderbarX = newX;
	m_sliderbarY = newY;

	return;
}

void SliderbarClass::SetSliderbarDimensions(int newWidth, int newHeight){
	// Store the new sliderbar dimensions
	m_sliderbarWidth = newWidth;
	m_sliderbarHeight = newHeight;

	return;
}

void SliderbarClass::SetSliderDimensions(int newWidth, int newHeight){
	// Store the new slider dimensions
	m_sliderWidth = newWidth;
	m_sliderHeight = newHeight;

	return;
}

bool SliderbarClass::HitCheck(int cursorX, int cursorY){
	// Check if the cursor is currently over this button
	if (cursorX >= m_sliderbarX && cursorX < m_sliderbarX + m_sliderbarWidth && cursorY >= m_sliderbarY && cursorY < m_sliderbarY + m_sliderHeight){
		return true;
	}

	return false;
}

void SliderbarClass::Click(int position){
	// Adjust the value of the variable this sliderbar manages according to the
	// location on the sliderbar that the cursor is over
	*m_variable = float(position - m_sliderbarX) / float(m_sliderbarWidth);

	return;
}

bool SliderbarClass::RenderSliderbar(ID3D11DeviceContext* deviceContext){
	bool result;

	// Ensure the texture bitmap is scaled to the right dimensions
	m_SliderbarTexture->SetDimensions(m_sliderbarWidth - m_sliderWidth / 2, m_sliderbarHeight);

	// Render the sliderbar
	result = m_SliderbarTexture->Render(deviceContext, m_sliderbarX + m_sliderWidth / 4, m_sliderbarY + (m_sliderHeight - m_sliderbarHeight) / 2);
	if (!result){
		return false;
	}

	return true;
}

bool SliderbarClass::RenderSlider(ID3D11DeviceContext* deviceContext){
	bool result;

	// Ensure the texture bitmap is scaled to the right dimensions
	m_SliderTexture->SetDimensions(m_sliderWidth, m_sliderHeight);

	// Render the slider
	result = m_SliderTexture->Render(deviceContext, m_sliderbarX + int((*m_variable) * (m_sliderbarWidth - m_sliderWidth)), m_sliderbarY);
	if (!result){
		return false;
	}

	return true;
}

int SliderbarClass::GetSliderbarIndexCount(){
	return m_SliderbarTexture->GetIndexCount();
}

ID3D11ShaderResourceView* SliderbarClass::GetSliderbarTexture(){
	return m_SliderbarTexture->GetTexture();
}

int SliderbarClass::GetSliderIndexCount(){
	return m_SliderTexture->GetIndexCount();
}

ID3D11ShaderResourceView* SliderbarClass::GetSliderTexture(){
	return m_SliderTexture->GetTexture();
}
