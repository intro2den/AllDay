////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////
// CLASS INCLUDES //
////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "bitmapclass.h"
#include "combatmapclass.h"
#include "hexmapclass.h"
#include "textureshaderclass.h"
#include "textclass.h"
#include "fontshaderclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass{
private:
	enum MainState{
		MAINSTATE_MAINMENU,
		MAINSTATE_COMBATMAP
	};

public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool InitializeCombatMap(MapType, int, int);
	void ShutdownCombatMap();
	//bool HandleInput(float); // Requires timer
	bool RenderGraphics();

private:
	InputClass* m_Input;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	BitmapClass* m_Background;
	BitmapClass* m_MainMenuButton;
	BitmapClass* m_Mouse;
	CombatMap* m_CombatMap;
	HexMapClass* m_TerrainMap;
	TextureShaderClass* m_TextureShader;
	TextClass* m_Text;
	FontShaderClass* m_FontShader;

	MainState m_MainState;
	int m_screenWidth, m_screenHeight;
	int m_combatMapWidth, m_combatMapHeight;
	int m_mouseX, m_mouseY;
};