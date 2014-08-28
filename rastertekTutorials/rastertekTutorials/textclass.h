////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

////////////////////
// CLASS INCLUDES //
////////////////////
#include "fontclass.h"
#include "fontshaderclass.h"
#include <list>

////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////
class TextClass{
protected:
	/////////////
	// STRUCTS //
	/////////////
	struct SentenceType{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	///////////////
	// CONSTANTS //
	///////////////
	const int NO_LENGTH_LIMIT = -1;

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	void Initialize(FontClass*, FontShaderClass*, int, int, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	void ClearText();
	bool AddText(int, int, char*, float, float, float, int, ID3D11Device*, ID3D11DeviceContext*);

protected:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, int, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);

protected:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;

	std::list <SentenceType*> m_Sentences;
};
