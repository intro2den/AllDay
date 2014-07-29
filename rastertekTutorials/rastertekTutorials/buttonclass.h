////////////////////////////////////////////////////////////////////////////////
// Filename: buttonclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

////////////////////
// CLASS INCLUDES //
////////////////////
#include "bitmapclass.h"

//////////////////////////
// Button Command Enums //
//////////////////////////
enum ButtonAction{
	BUTTON_NOBUTTON,
	BUTTON_CLOSEMENU,
	BUTTON_EXITAPPLICATION,
	
	BUTTON_ENTERCOMBAT,
	BUTTON_OPENOPTIONSMENU,

	BUTTON_OPENGAMEOPTIONS,

	BUTTON_SAVESETTINGS,

	BUTTON_ORDERMOVE,
	BUTTON_ORDERATTACK,
	BUTTON_ENDTURN,
	BUTTON_OPENCOMBATMENU,

	BUTTON_EXITCOMBAT
};

////////////////////////////////////////////////////////////////////////////////
// Class name: ButtonClass
////////////////////////////////////////////////////////////////////////////////
class ButtonClass{
public:
	ButtonClass();
	ButtonClass(const ButtonClass&);
	~ButtonClass();

	void Initialize(ButtonAction, int, int, int, int, BitmapClass*);
	void SetPosition(int, int);
	void SetDimensions(int, int);
	bool HitCheck(int, int);
	ButtonAction Click();
	void GetTooltipPosition(int&, int&);

	bool Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	BitmapClass* m_Texture;

	ButtonAction m_action;
	int m_buttonX, m_buttonY;
	int m_buttonWidth, m_buttonHeight;
};
