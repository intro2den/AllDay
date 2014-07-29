////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

/////////////
// GLOBAlS //
/////////////
const int MAX_ERROR_LENGTH = 40;

////////////////////
// CLASS INCLUDES //
////////////////////
#include "statictextclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////
class TextClass : public StaticTextClass{
private:
	///////////////
	// CONSTANTS //
	///////////////
	const float MAX_ERROR_TIME = 5000.0f;

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	// NOTE: Currently taking input for displaying menu text in the correct positions at initialization and on updates
	//       Should be possible to provide access without using function input parameters which could help cleanup code
	bool Initialize(FontClass*, FontShaderClass*, ID3D11Device*, ID3D11DeviceContext*, int, int, D3DXMATRIX);
	void Shutdown();
	bool Frame(float, ID3D11DeviceContext*);
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	bool RenderStaticText(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	bool RenderTooltip(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	bool RenderErrorText(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	bool SetTooltipText(int, int, char*, char*, int, ID3D11DeviceContext*);

	bool NewErrorMessage(char*, ID3D11DeviceContext*);
	bool ClearErrors(ID3D11DeviceContext*);

	bool SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetSelectedAgent(char*, ID3D11DeviceContext*);

private:
	SentenceType* m_tooltipLabel;
	SentenceType* m_tooltipDescription;

	SentenceType* m_errorText1;
	SentenceType* m_errorText2;
	char m_secondErrorString[MAX_ERROR_LENGTH];
	float m_errorTime1, m_errorTime2;

	SentenceType* m_selectedAgent;
	SentenceType* m_cursorXCoordinate;
	SentenceType* m_cursorYCoordinate;
};
