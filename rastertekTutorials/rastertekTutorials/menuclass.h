////////////////////////////////////////////////////////////////////////////////
// Filename: menuclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>

////////////////////
// CLASS INCLUDES //
////////////////////
#include "bitmapclass.h"
#include "textureshaderclass.h"
#include "buttonclass.h"
#include "sliderbarclass.h"
#include "textclass.h"
#include <list>

////////////////////////////////////////////////////////////////////////////////
// Class name: MenuClass
////////////////////////////////////////////////////////////////////////////////
class MenuClass{
public:
	MenuClass();
	MenuClass(const MenuClass&);
	~MenuClass();

	virtual void Shutdown();
	void SetPosition(int, int);
	void SetDimensions(int, int);
	bool HitCheck(int, int);
	ButtonAction LeftMouseClick(int, int);
	bool LeftMousePressed(int, int);

	virtual bool GetTooltip(int, int, int&, int&, int&, int&, ButtonAction&);

	virtual bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	bool GetExclusive();
	bool GetCloseOnMiss();

protected:
	bool Initialize(int, int, BitmapClass*, TextureShaderClass*, FontClass*, FontShaderClass*, D3DXMATRIX);

	void PositionMainMenu(int, int);
	void PositionMenubar(int, int);
	void PositionCombatMainMenu(int, int);

	bool NewButton(ButtonAction, int, int, int, int, BitmapClass*);
	bool NewSliderbar(float*, int, int, int, int, int, int, BitmapClass*, BitmapClass*);

	bool InitializeText(FontClass*, FontShaderClass*, int, int, D3DXMATRIX);

private:
	BitmapClass* m_Texture;
	TextureShaderClass* m_TextureShader;

protected:
	TextClass* m_Text;

	int m_menuX, m_menuY;
	int m_menuWidth, m_menuHeight;
	bool m_exclusive, m_closeOnMiss;

	std::list <ButtonClass*> m_Buttons;
	std::list <SliderbarClass*> m_Sliderbars;
};
