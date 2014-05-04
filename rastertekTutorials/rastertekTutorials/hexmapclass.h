////////////////////////////////////////////////////////////////////////////////
// Filename: hexmapclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>

/////////////
// GLOBALS //
/////////////
const float PI = 3.14159f;
const float HEX_SIZE = 30.0f;
const float HEX_HEIGHT = 2.0f * HEX_SIZE * sin(2.0f*PI/3.0f);


////////////////////
// CLASS INCLUDES //
////////////////////
#include "textureclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: HexMapClass
////////////////////////////////////////////////////////////////////////////////
class HexMapClass{
private:
	struct VertexType{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	HexMapClass();
	HexMapClass(const HexMapClass&);
	~HexMapClass();

	bool Initialize(ID3D11Device*, int, int, CHAR*, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, int*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int, int*);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, CHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	int m_screenWidth, m_screenHeight;
	int m_mapWidth, m_mapHeight;
	int m_mapTiles;
	int m_previousPosX, m_previousPosY;
};
