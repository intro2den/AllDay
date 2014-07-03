///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "textclass.h"

TextClass::TextClass(){
	m_Font = 0;
	m_FontShader = 0;
	m_menuText1 = 0;
	m_menuText2 = 0;
	m_menuText3 = 0;
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

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix, int horizontalOffset, int verticalOffset, int buttonHeight, int buttonSpacing){
	bool result;
	
	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;

	// Create the font object.
	m_Font = new FontClass;
	if (!m_Font){
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(device, "../rastertekTutorials/data/fontdata.txt", "../rastertekTutorials/data/font.dds");
	if (!result){
		MessageBox(hwnd, "Could not initialize the font object.", "Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader){
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if (!result){
		MessageBox(hwnd, "Could not initialize the font shader object.", "Error", MB_OK);
		return false;
	}

	// Initialize menu text for the Main Menu
	result = InitializeSentence(&m_menuText1, 16, device);
	if (!result){
		return false;
	}

	result = InitializeSentence(&m_menuText2, 16, device);
	if (!result){
		return false;
	}

	result = InitializeSentence(&m_menuText3, 16, device);
	if (!result){
		return false;
	}

	result = SetMainMenuText(horizontalOffset, verticalOffset, buttonHeight, buttonSpacing, deviceContext);
	if (!result){
		return false;
	}

	// Initialize sentences for the displaying of tooltips
	result = InitializeSentence(&m_tooltipLabel, 16, device);
	if (!result){
		return false;
	}

	result = InitializeSentence(&m_tooltipDescription, 32, device);
	if (!result){
		return false;
	}

	// Both tooltip sentences should be empty to begin with
	result = UpdateSentence(m_tooltipLabel, "", 0, 0, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_tooltipDescription, "", 0, 0, 1.0f, 1.0f, 1.0f, deviceContext);
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
	result = UpdateSentence(m_selectedAgent, "", 20, m_screenHeight - 80, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result){
		return false;
	}

	// Initialize 2 sentences to display the cursor coordinates, initialize both to empty strings
	result = InitializeSentence(&m_cursorXCoordinate, 16, device);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_cursorXCoordinate, "", 20, 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result){
		return false;
	}

	result = InitializeSentence(&m_cursorYCoordinate, 16, device);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_cursorYCoordinate, "", 20, 40, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result){
		return false;
	}

	return true;
}

void TextClass::Shutdown(){
	// Release the menu sentences.
	ReleaseSentence(&m_menuText1);
	ReleaseSentence(&m_menuText2);
	ReleaseSentence(&m_menuText3);

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

	// Release the font shader object.
	if (m_FontShader){
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if (m_Font){
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

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
			result = UpdateSentence(m_errorText1, m_secondErrorString, m_screenWidth / 2 - 80, 80, 1.0f, 0.0f, 0.0f, deviceContext);
			if (!result){
				return false;
			}

			// Clear the second error message
			m_errorTime2 = MAX_ERROR_TIME;
			strcpy_s(m_secondErrorString, "");
			result = UpdateSentence(m_errorText2, m_secondErrorString, 0, 0, 1.0f, 0.0f, 0.0f, deviceContext);
			if (!result){
				return false;
			}

		}
	}

	return true;
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix){
	bool result;

	// Draw the menu related sentences
	result = RenderSentence(deviceContext, m_menuText1, worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	result = RenderSentence(deviceContext, m_menuText2, worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	result = RenderSentence(deviceContext, m_menuText3, worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

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

	// Draw the error sentences
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

bool TextClass::SetMainMenuText(int horizontalOffset, int verticalOffset, int buttonHeight, int buttonSpacing, ID3D11DeviceContext* deviceContext){
	// Update all menuText strings to display labels for each option on the Main Menu
	bool result;

	result = UpdateSentence(m_menuText1, "Enter CombatMap", horizontalOffset + 15, verticalOffset + (buttonHeight / 2) - 4, 0.0f, 0.0f, 0.0f, deviceContext);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_menuText2, "Options", horizontalOffset + 15, verticalOffset + (buttonHeight / 2) + (buttonHeight + buttonSpacing) - 4, 0.0f, 0.0f, 0.0f, deviceContext);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_menuText3, "Exit Application", horizontalOffset + 15, verticalOffset + (buttonHeight / 2) + 2 * (buttonHeight + buttonSpacing) - 4, 0.0f, 0.0f, 0.0f, deviceContext);
	if (!result){
		return false;
	}

	return true;
}

bool TextClass::SetOptionsMenuText(int horizontalOffset, int verticalOffset, int buttonHeight, int buttonSpacing, ID3D11DeviceContext* deviceContext){
	// Update all menuText strings to display labels for each option on the Options Menu
	bool result;

	// NOTE: Positioning is currently fixed and does not change with other elements in the UI
	//       such as the button the text is meant to be a part of. This will have to change,
	//       particularly when we support multiple resolutions (Text size may also need to change)

	result = UpdateSentence(m_menuText1, "Back", horizontalOffset + 15, verticalOffset + (buttonHeight / 2) - 4, 0.0f, 0.0f, 0.0f, deviceContext);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_menuText2, "", 0, 0, 0.0f, 0.0f, 0.0f, deviceContext);
	if (!result){
		return false;
	}
	
	result = UpdateSentence(m_menuText3, "", 0, 0, 0.0f, 0.0f, 0.0f, deviceContext);
	if (!result){
		return false;
	}

	return true;
}

bool TextClass::SetCombatMapText(ID3D11DeviceContext* deviceContext){
	// Update all menuText strings to display labels for each option on the CombatMap Menu Bar
	bool result;

	// NOTE: Positioning is currently fixed and does not change with other elements in the UI
	//       such as the button the text is meant to be a part of. This will have to change,
	//       particularly when we support multiple resolutions (Text size may also need to change)

	result = UpdateSentence(m_menuText1, "End Turn", m_screenWidth - 115, m_screenHeight - 70, 0.0f, 0.0f, 0.0f, deviceContext);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_menuText2, "Main Menu", m_screenWidth - 115, m_screenHeight - 38, 0.0f, 0.0f, 0.0f, deviceContext);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_menuText3, "", 0, 0, 0.0f, 0.0f, 0.0f, deviceContext);
	if (!result){
		return false;
	}

	return true;
}

bool TextClass::SetTooltipText(int tooltipX, int tooltipY, ID3D11DeviceContext* deviceContext){
	// Update the tooltip sentences with helpful information about the currently highlighted
	// UI object.
	bool result;

	result = UpdateSentence(m_tooltipLabel, "Tooltip Label", tooltipX + 3, tooltipY + 3, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_tooltipDescription, "Helpful tooltip text", tooltipX + 3, tooltipY + 16, 1.0f, 1.0f, 1.0f, deviceContext);
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
		result = UpdateSentence(m_errorText1, text, m_screenWidth / 2 - 80, 80, 1.0f, 0.0f, 0.0f, deviceContext);
		if (!result){
			return false;
		}

	} else if (m_errorTime2 >= MAX_ERROR_TIME){
		// Only 1 error already exists, use the second error sentence to display the new error
		strcpy_s(m_secondErrorString, text);
		m_errorTime2 = 0.0f;
		result = UpdateSentence(m_errorText2, m_secondErrorString, m_screenWidth / 2 - 80, 96, 1.0f, 0.0f, 0.0f, deviceContext);
		if (!result){
			return false;
		}

	} else{
		// There are already 2 error messages, overwrite the first using the second and use the
		// second error sentence to display the new error
		m_errorTime1 = m_errorTime2;
		result = UpdateSentence(m_errorText1, m_secondErrorString, m_screenWidth / 2 - 80, 80, 1.0f, 0.0f, 0.0f, deviceContext);
		if (!result){
			return false;
		}

		strcpy_s(m_secondErrorString, text);
		m_errorTime2 = 0.0f;
		result = UpdateSentence(m_errorText2, m_secondErrorString, m_screenWidth / 2 - 80, 96, 1.0f, 0.0f, 0.0f, deviceContext);
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

	result = UpdateSentence(m_errorText1, m_secondErrorString, 0, 0, 1.0f, 0.0f, 0.0f, deviceContext);
	if (!result){
		return false;
	}

	result = UpdateSentence(m_errorText2, m_secondErrorString, 0, 0, 1.0f, 0.0f, 0.0f, deviceContext);
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
	result = UpdateSentence(m_cursorXCoordinate, mouseString, 20, 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result){
		return false;
	}

	// Convert the mouseY integer to string format.
	_itoa_s(mouseY, tempString, 10);

	// Setup the mouseY string.
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_cursorYCoordinate, mouseString, 20, 40, 1.0f, 1.0f, 1.0f, deviceContext);
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
	result = UpdateSentence(m_selectedAgent, newString, 20, m_screenHeight - 80, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result){
		return false;
	}

	return true;
}

bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device){
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!*sentence){
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices){
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices){
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType)* (*sentence)->vertexCount));

	// Initialize the index array.
	for (i = 0; i<(*sentence)->indexCount; i++){
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result)){
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result)){
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = 0;

	return true;
}

bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext* deviceContext){
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if (numLetters > sentence->maxLength){
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices){
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType)* sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)){
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)* sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence){
	if (*sentence){
		// Release the sentence vertex buffer.
		if ((*sentence)->vertexBuffer){
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if ((*sentence)->indexBuffer){
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}

bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix){
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix, m_Font->GetTexture(), pixelColor);
	if (!result){
		false;
	}

	return true;
}
