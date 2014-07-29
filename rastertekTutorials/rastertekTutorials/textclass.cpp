///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "textclass.h"

TextClass::TextClass(){
	m_Font = 0;
	m_FontShader = 0;
	m_errorText1 = 0;
	m_errorText2 = 0;
	m_selectedAgent = 0;
	m_cursorXCoordinate = 0;
	m_cursorYCoordinate = 0;
}

TextClass::TextClass(const TextClass& other){
}

TextClass::~TextClass(){
}

bool TextClass::Initialize(FontClass* font, FontShaderClass* fontShader, ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix){
	bool result;

	StaticTextClass::Initialize(font, fontShader, screenWidth, screenHeight, baseViewMatrix);

	// Initialize sentences for the displaying of tooltips
	result = InitializeSentence(&m_tooltipLabel, 20, device);
	if (!result){
		return false;
	}

	result = InitializeSentence(&m_tooltipDescription, 64, device);
	if (!result){
		return false;
	}

	// Both tooltip sentences should be empty to begin with
	result = UpdateSentence(m_tooltipLabel, "", 0, 0, 1.0f, 1.0f, 1.0f, NO_LENGTH_LIMIT, deviceContext);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_tooltipDescription, "", 0, 0, 1.0f, 1.0f, 1.0f, NO_LENGTH_LIMIT, deviceContext);
	if (!result){
		return false;
	}

	// Initialize sentences for the displaying of error messages
	result = InitializeSentence(&m_errorText1, 40, device);
	if (!result){
		return false;
	}

	result = InitializeSentence(&m_errorText2, 40, device);
	if (!result){
		return false;
	}

	// There should be no errors at initialization
	result = ClearErrors(deviceContext);
	if (!result){
		return false;
	}

	// Initialize a sentence to display the currently selected Agent
	result = InitializeSentence(&m_selectedAgent, 16, device);
	if (!result){
		return false;
	}

	// Initially there should be no selected Agent
	result = UpdateSentence(m_selectedAgent, "", 20, m_screenHeight - 80, 1.0f, 1.0f, 1.0f, NO_LENGTH_LIMIT, deviceContext);
	if (!result){
		return false;
	}

	// Initialize 2 sentences to display the cursor coordinates, initialize both to empty strings
	result = InitializeSentence(&m_cursorXCoordinate, 16, device);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_cursorXCoordinate, "", 20, 20, 1.0f, 1.0f, 1.0f, NO_LENGTH_LIMIT, deviceContext);
	if (!result){
		return false;
	}

	result = InitializeSentence(&m_cursorYCoordinate, 16, device);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_cursorYCoordinate, "", 20, 40, 1.0f, 1.0f, 1.0f, NO_LENGTH_LIMIT, deviceContext);
	if (!result){
		return false;
	}

	return true;
}

void TextClass::Shutdown(){
	// Release the static sentences.
	StaticTextClass::Shutdown();

	// Release the tooltip sentences
	ReleaseSentence(&m_tooltipLabel);
	ReleaseSentence(&m_tooltipDescription);

	// Release the error sentences.
	ReleaseSentence(&m_errorText1);
	ReleaseSentence(&m_errorText2);

	// Release the selectedAgent sentence.
	ReleaseSentence(&m_selectedAgent);

	// Release the cursor coordinate sentences.
	ReleaseSentence(&m_cursorXCoordinate);
	ReleaseSentence(&m_cursorYCoordinate);

	return;
}

bool TextClass::Frame(float frameTime, ID3D11DeviceContext* deviceContext){
	// Update the time that each error message has been displayed, if an error message has been
	// displayed for the maximum amount of time, remove it.
	bool result;
	
	// The second error message is only active if it hasn't already expired
	if (m_errorTime2 < MAX_ERROR_TIME){
		m_errorTime2 += frameTime;

		// If the second message has just expired then the first message has also expired, clear all errors and return
		if (m_errorTime2 >= MAX_ERROR_TIME){
			result = ClearErrors(deviceContext);
			if (!result){
				return false;
			}

			return true;
		}
	}

	// The first error message is only active if it hasn't already expired
	if (m_errorTime1 < MAX_ERROR_TIME){
		m_errorTime1 += frameTime;

		// If the first error message has expired, replace it with the second error message and clear
		// the second error message
		if (m_errorTime1 >= MAX_ERROR_TIME){
			// Replace the first error message with the second
			m_errorTime1 = m_errorTime2;
			result = UpdateSentence(m_errorText1, m_secondErrorString, m_screenWidth / 2 - 80, 80, 1.0f, 0.0f, 0.0f, NO_LENGTH_LIMIT, deviceContext);
			if (!result){
				return false;
			}

			// Clear the second error message
			m_errorTime2 = MAX_ERROR_TIME;
			strcpy_s(m_secondErrorString, "");
			result = UpdateSentence(m_errorText2, m_secondErrorString, 0, 0, 1.0f, 0.0f, 0.0f, NO_LENGTH_LIMIT, deviceContext);
			if (!result){
				return false;
			}

		}
	}

	return true;
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix){
	bool result;

	// Draw the sentence indicating which agent is selected
	result = RenderSentence(deviceContext, m_selectedAgent, worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	// Draw the sentence with the X coordinate of the mouse.
	result = RenderSentence(deviceContext, m_cursorXCoordinate, worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	// Draw the sentence with the Y coordinate of the mouse.
	result = RenderSentence(deviceContext, m_cursorYCoordinate, worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	return true;
}

bool TextClass::RenderStaticText(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix){
	// Draw all static text
	bool result;
	
	result = StaticTextClass::Render(deviceContext, worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	return true;
}

bool TextClass::RenderTooltip(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix){
	bool result;

	// Render the tooltip sentences
	result = RenderSentence(deviceContext, m_tooltipLabel, worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	result = RenderSentence(deviceContext, m_tooltipDescription, worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	return true;
}

bool TextClass::RenderErrorText(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix){
	// Draw the text for error messages
	bool result;

	result = RenderSentence(deviceContext, m_errorText1, worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	result = RenderSentence(deviceContext, m_errorText2, worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	return true;
}

bool TextClass::SetTooltipText(int tooltipX, int tooltipY, char* label, char* desc, int tooltipWidth, ID3D11DeviceContext* deviceContext){
	// Update the tooltip sentences with helpful information about the currently highlighted
	// UI object.
	bool result;

	result = UpdateSentence(m_tooltipLabel, label, tooltipX + 3, tooltipY + 3, 1.0f, 1.0f, 1.0f, tooltipWidth - 6, deviceContext);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_tooltipDescription, desc, tooltipX + 3, tooltipY + 16, 1.0f, 1.0f, 1.0f, tooltipWidth - 6, deviceContext);
	if (!result){
		return false;
	}

	return true;
}

bool TextClass::NewErrorMessage(char* text, ID3D11DeviceContext* deviceContext){
	// Update the error sentence objects to display a new error message for a fixed period of time.
	// If there are already 2 error messages, remove the oldest of the two and place this new
	// error in the second error sentence object.
	bool result;

	// If no error messages are currently displayed, this new error is the first
	if (m_errorTime1 >= MAX_ERROR_TIME){
		// Use the first error sentence to display the new error
		m_errorTime1 = 0.0f;
		result = UpdateSentence(m_errorText1, text, m_screenWidth / 2 - 80, 80, 1.0f, 0.0f, 0.0f, NO_LENGTH_LIMIT, deviceContext);
		if (!result){
			return false;
		}
	} else if (m_errorTime2 >= MAX_ERROR_TIME){
		// Only 1 error already exists, use the second error sentence to display the new error
		strcpy_s(m_secondErrorString, text);
		m_errorTime2 = 0.0f;
		result = UpdateSentence(m_errorText2, m_secondErrorString, m_screenWidth / 2 - 80, 96, 1.0f, 0.0f, 0.0f, NO_LENGTH_LIMIT, deviceContext);
		if (!result){
			return false;
		}
	} else{
		// There are already 2 error messages, overwrite the first using the second and use the
		// second error sentence to display the new error
		m_errorTime1 = m_errorTime2;
		result = UpdateSentence(m_errorText1, m_secondErrorString, m_screenWidth / 2 - 80, 80, 1.0f, 0.0f, 0.0f, NO_LENGTH_LIMIT, deviceContext);
		if (!result){
			return false;
		}

		strcpy_s(m_secondErrorString, text);
		m_errorTime2 = 0.0f;
		result = UpdateSentence(m_errorText2, m_secondErrorString, m_screenWidth / 2 - 80, 96, 1.0f, 0.0f, 0.0f, NO_LENGTH_LIMIT, deviceContext);
		if (!result){
			return false;
		}
	}

	return true;
}

bool TextClass::ClearErrors(ID3D11DeviceContext* deviceContext){
	// Clear both error sentence objects
	bool result;

	m_errorTime1 = MAX_ERROR_TIME;
	m_errorTime2 = MAX_ERROR_TIME;
	strcpy_s(m_secondErrorString, "");

	result = UpdateSentence(m_errorText1, m_secondErrorString, 0, 0, 1.0f, 0.0f, 0.0f, NO_LENGTH_LIMIT, deviceContext);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_errorText2, m_secondErrorString, 0, 0, 1.0f, 0.0f, 0.0f, NO_LENGTH_LIMIT, deviceContext);
	if (!result){
		return false;
	}

	return true;
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext){
	char tempString[16];
	char mouseString[16];
	bool result;

	// Convert the mouseX integer to string format.
	_itoa_s(mouseX, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_cursorXCoordinate, mouseString, 20, 20, 1.0f, 1.0f, 1.0f, NO_LENGTH_LIMIT, deviceContext);
	if (!result){
		return false;
	}

	// Convert the mouseY integer to string format.
	_itoa_s(mouseY, tempString, 10);

	// Setup the mouseY string.
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_cursorYCoordinate, mouseString, 20, 40, 1.0f, 1.0f, 1.0f, NO_LENGTH_LIMIT, deviceContext);
	if (!result){
		return false;
	}

	return true;
}

bool TextClass::SetSelectedAgent(char* agentName, ID3D11DeviceContext* deviceContext){
	// Update the selectedAgent string with the name of the currently selected Agent.
	// If a null pointer is provided for the name, empty the string.
	char newString[16];
	bool result;

	if (agentName){
		strcpy_s(newString, agentName);
	} else{
		strcpy_s(newString, "");
	}

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_selectedAgent, newString, 20, m_screenHeight - 80, 1.0f, 1.0f, 1.0f, NO_LENGTH_LIMIT, deviceContext);
	if (!result){
		return false;
	}

	return true;
}
