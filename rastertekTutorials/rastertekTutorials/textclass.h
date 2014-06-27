////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

/////////////
// GLOBAlS //
/////////////
const int MAX_ERROR_LENGTH = 40;
const float MAX_ERROR_TIME = 5000.0f;

////////////////////
// CLASS INCLUDES //
////////////////////
#include "fontclass.h"
#include "fontshaderclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////
class TextClass{
private:
	struct SentenceType{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	// NOTE: Currently taking input for displaying menu text in the correct positions at initialization and on updates
	//       Should be possible to provide access without using function input parameters which could help cleanup code
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX, int, int, int ,int);
	void Shutdown();
	bool Frame(float, ID3D11DeviceContext*);
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	bool RenderTooltip(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	bool SetMainMenuText(int, int, int, int, ID3D11DeviceContext*);
	bool SetOptionsMenuText(int, int, int, int, ID3D11DeviceContext*);
	bool SetCombatMapText(ID3D11DeviceContext*);

	bool SetTooltipText(int, int, ID3D11DeviceContext*);

	bool NewErrorMessage(char*, ID3D11DeviceContext*);
	bool ClearErrors(ID3D11DeviceContext*);

	bool SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetSelectedAgent(int, ID3D11DeviceContext*);

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;
	
	SentenceType* m_menuText1;
	SentenceType* m_menuText2;
	SentenceType* m_menuText3;

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
