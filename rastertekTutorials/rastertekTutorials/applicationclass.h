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
const int MAX_AGENTS = 512;

//////////////////
// UI CONSTANTS //
//////////////////
const int MAIN_MENU_BUTTON_HORIZONTAL_OFFSET = 50;
const int MAIN_MENU_BUTTON_VERTICAL_OFFSET = 125;
const int MAIN_MENU_BUTTON_WIDTH = 350;
const int MAIN_MENU_BUTTON_HEIGHT = 50;
const int MAIN_MENU_BUTTON_SPACING = 25;
const int MAIN_MENU_BUTTON_COUNT = 2;

const int COMBAT_MENU_HEIGHT = 100;
const int COMBAT_MENU_BUTTON_WIDTH = 100;
const int COMBAT_MENU_BUTTON_HEIGHT = 30;

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
#include "timerclass.h"
#include "positionclass.h"
#include "activeagentclass.h"
#include "spriteclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass{
private:
	enum MainState{
		MAINSTATE_MAINMENU,
		MAINSTATE_COMBATMAP
	};

	enum MainMenuButton{
		MAINMENUBUTTON_ENTERCOMBATMAP = 0,
		MAINMENUBUTTON_EXIT
	};

public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);
	bool SetSelectedAgent(int);
	void NextTurn();
	void EndTurn();
	bool InitializeCombatMap(MapType, int, int);
	void ShutdownCombatMap();
	bool RenderGraphics();

private:
	InputClass* m_Input;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	BitmapClass* m_MainBackground;
	BitmapClass* m_StandardButton;
	BitmapClass* m_Mouse;
	BitmapClass* m_MenuBarBackground;
	CombatMap* m_CombatMap;
	HexMapClass* m_TerrainMap;
	HexMapClass* m_HexHighlight;
	TextureShaderClass* m_TextureShader;
	TextClass* m_Text;
	FontShaderClass* m_FontShader;
	TimerClass* m_Timer;
	PositionClass* m_Position;
	AgentClass** m_Agents;
	SpriteClass* m_AgentSprites;

	MainState m_MainState;
	int m_screenWidth, m_screenHeight;
	int m_combatMapWidth, m_combatMapHeight;
	int m_mouseX, m_mouseY;
	int m_currentTileX, m_currentTileY;
	bool m_cursorOverTile;
	int m_selectedAgent;
	int m_numAgents;

	// These arrays may become obsolete at some point, currently used for
	// the Turn system in the CombatMap
	int m_agentOwner[MAX_AGENTS];
	int m_agentInitiative[MAX_AGENTS];
	bool m_agentBeganTurn[MAX_AGENTS];
	bool m_agentEndedTurn[MAX_AGENTS];

	D3DXMATRIX m_UIViewMatrix;
};
