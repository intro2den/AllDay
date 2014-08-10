////////////////////////////////////////////////////////////////////////////////
// Filename: combatmenubarclass.h
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
// Class name: CombatMenubarClass
////////////////////////////////////////////////////////////////////////////////
class CombatMenubarClass : public MenuClass{
public:
	CombatMenubarClass();
	CombatMenubarClass(const CombatMenubarClass&);
	~CombatMenubarClass();

	bool Initialize(int, int, BitmapClass*, BitmapClass*, TextureShaderClass*, FontClass*, FontShaderClass*, D3DXMATRIX, ID3D11Device*, ID3D11DeviceContext*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	bool SetSelectedAgent(char*, ID3D11Device*, ID3D11DeviceContext*);
	bool SetHighlightedTile(int, int, char*, ID3D11Device*, ID3D11DeviceContext*);

	bool GetTooltip(int, int, int&, int&, int&, int&, ButtonAction&);

private:
	void SetTextOffsets();
	bool SetHighlightedAgent(char*, ID3D11Device*, ID3D11DeviceContext*);

private:
	TextClass* m_SelectedAgentText;
	TextClass* m_HighlightedText;

	int m_textYOffset;
	int m_selectedAgentXOffset;
	int m_highlightedAgentXOffset;
	int m_tileCoordinatesXOffset;
};
