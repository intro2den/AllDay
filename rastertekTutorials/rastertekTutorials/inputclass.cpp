////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"

InputClass::InputClass(){
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
	m_hwnd = 0;
}

InputClass::InputClass(const InputClass& other){
}

InputClass::~InputClass(){
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd){
	HRESULT result;
	m_hwnd = hwnd;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the flags associated with mouse input to false
	m_leftMousePressed = false;
	m_rightMousePressed = false;
	m_leftMouseReleased = false;
	m_rightMouseReleased = false;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result)){
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)){
		return false;
	}

	// Set the cooperative level of the keyboard to share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)){
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result)){
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result)){
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result)){
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)){
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result)){
		return false;
	}

	return true;
}

void InputClass::Shutdown(){
	// Release the mouse.
	if (m_mouse){
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if (m_keyboard){
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput){
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool InputClass::Frame(){
	bool result;

	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result){
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result){
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard(){
	HRESULT result;

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result)){
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)){
			m_keyboard->Acquire();
		} else{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse(){
	HRESULT result;
	POINT point;
	
	// Calculate the position of the cursor with respect to the application window
	result = GetCursorPos(&point);
	if (FAILED(result)){
		return false;
	}

	result = ScreenToClient(m_hwnd, &point);
	if (FAILED(result)){
		return false;
	}

	m_mouseX = point.x;
	m_mouseY = point.y;

	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result)){
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)){
			m_mouse->Acquire();
		} else{
			return false;
		}
	}

	return true;
}

void InputClass::ProcessInput(){
	// Update the leftMousePressed and leftMouseReleased flags
	m_leftMouseReleased = false;
	
	if (!m_mouseState.rgbButtons[0] && m_leftMousePressed){
		m_leftMousePressed = false;
		m_leftMouseReleased = true;
	}

	if (m_mouseState.rgbButtons[0]){
		m_leftMousePressed = true;
	}

	// Update the rightMousePressed and rightMouseReleased flags
	m_rightMouseReleased = false;

	if (!m_mouseState.rgbButtons[1] && m_rightMousePressed){
		m_rightMousePressed = false;
		m_rightMouseReleased = true;
	}

	if (m_mouseState.rgbButtons[1]){
		m_rightMousePressed = true;
	}

	return;
}

bool InputClass::IsEscapePressed(){
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_ESCAPE] & 0x80){
		return true;
	}

	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY){
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

bool InputClass::IsLeftMousePressed(){
	return m_leftMousePressed;
}

bool InputClass::IsRightMousePressed(){
	return m_rightMousePressed;
}

bool InputClass::WasLeftMouseClicked(){
	return m_leftMouseReleased;
}

bool InputClass::WasRightMouseClicked(){
	return m_rightMouseReleased;
}

bool InputClass::IsUpPressed(){
	// Check if the up arrow is pressed
	if (m_keyboardState[DIK_UP]){
		return true;
	}

	return false;
}

bool InputClass::IsDownPressed(){
	// Check if the down arrow is pressed
	if (m_keyboardState[DIK_DOWN]){
		return true;
	}

	return false;
}

bool InputClass::IsLeftPressed(){
	// Check if the left arrow is pressed
	if (m_keyboardState[DIK_LEFT]){
		return true;
	}

	return false;
}

bool InputClass::IsRightPressed(){
	// Check if the right arrow is pressed
	if (m_keyboardState[DIK_RIGHT]){
		return true;
	}

	return false;
}
