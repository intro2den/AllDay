////////////////////////////////////////////////////////////////////////////////
// Filename: gameoptionsmenuclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>

////////////////////
// CLASS INCLUDES //
////////////////////
#include "menuclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GameOptionsMenuClass
////////////////////////////////////////////////////////////////////////////////
class GameOptionsMenuClass : public MenuClass{
public:
	GameOptionsMenuClass();
	GameOptionsMenuClass(const GameOptionsMenuClass&);
	~GameOptionsMenuClass();

	bool Initialize(int, int, BitmapClass*, BitmapClass*, BitmapClass*, BitmapClass*, TextureShaderClass*, FontClass*, FontShaderClass*, D3DXMATRIX, ID3D11Device*, ID3D11DeviceContext*, float, float*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	bool UpdateTooltipDelayLabel(float, ID3D11Device*, ID3D11DeviceContext*);

private:
	TextClass* m_TooltipDelayText;

};
