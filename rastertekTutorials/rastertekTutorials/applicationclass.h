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
// Main Menu
const int MAIN_MENU_BUTTON_WIDTH = 350;
const int MAIN_MENU_BUTTON_HEIGHT = 50;
const int MAIN_MENU_BUTTON_SPACING = 25;
const int MAIN_MENU_BUTTON_COUNT = 3;
const int MAIN_MENU_BUTTON_HORIZONTAL_OFFSET = 50;
const int MAIN_MENU_BUTTON_VERTICAL_OFFSET = 125;

// Options Menu
// NOTE: Currently using Main Menu constants in place of constants specific to the Options Menu as they should look the same
const int OPTIONS_MENU_BUTTON_COUNT = 1;

// CombatMap Menu Bar
// Buttons/Options from the Menu Bar are in a double row from the right side of the menu bar
const int COMBAT_MENU_HEIGHT = 100;
const int COMBAT_MENU_BUTTON_WIDTH = 100; // Will resize when text labels replaced by icons and tooltips
const int COMBAT_MENU_BUTTON_HEIGHT = 32;
const int COMBAT_MENU_BUTTON_COUNT = 2; // Should try to keep this as a multiple of 2 for proper rectangular set of options on the menu bar.
const int COMBAT_MENU_BUTTON_HORIZONTAL_OFFSET = -20 - COMBAT_MENU_BUTTON_WIDTH * (COMBAT_MENU_BUTTON_COUNT / 2); // End the options at the right side of the menu bar
const int COMBAT_MENU_BUTTON_VERTICAL_OFFSET = (COMBAT_MENU_HEIGHT / 2) - COMBAT_MENU_BUTTON_HEIGHT;

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
	/////////////////
	// STATE ENUMs //
	/////////////////
	enum MainState{
		MAINSTATE_MAINMENU,
		MAINSTATE_COMBATMAP
	};

	enum MenuState{
		MENUSTATE_MAINMENU,
		MENUSTATE_OPTIONMENU,
		MENUSTATE_NOMENU
	};


	///////////////////////
	// MENU BUTTON ENUMS //
	///////////////////////
	enum MainMenuButton{
		MAINMENUBUTTON_ENTERCOMBATMAP = 0,
		MAINMENUBUTTON_OPTIONS,
		MAINMENUBUTTON_EXIT
	};

	enum OptionsMenuButton{
		OPTIONSMENUBUTTON_BACK = 0
	};

	enum CombatMenuBarButton{
		COMBATMENUBUTTON_ENDTURN = 0,
		COMBATMENUBUTTON_MENU
	};

public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	// Initialization
	bool ReadConfig();

	// Handle Input
	bool HandleInput(float);
	bool SetSelectedAgent(int);
	void NextTurn();
	void EndTurn();
	bool InitializeCombatMap(MapType, int, int);
	void ShutdownCombatMap();

	// Update
	bool Update(float, bool);

	// Render
	bool RenderGraphics();

private:
	InputClass* m_Input;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	BitmapClass* m_MainBackground;
	BitmapClass* m_StandardButton;
	BitmapClass* m_Mouse;
	BitmapClass* m_MenuBackground;
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
	MenuState m_MenuState;
	int m_screenWidth, m_screenHeight;
	int m_combatMapWidth, m_combatMapHeight;
	int m_mouseX, m_mouseY;
	int m_currentTileX, m_currentTileY;
	bool m_cursorOverTile;
	int m_selectedAgent;
	int m_numAgents;

	bool m_displayTooltip;
	float m_tooltipDelay;
	float m_cursorIdleTime;
	int m_tooltipX, m_tooltipY;
	int m_tooltipWidth, m_tooltipHeight;

	// These arrays may become obsolete at some point, currently used for
	// the Turn system in the CombatMap
	int m_agentOwner[MAX_AGENTS];
	int m_agentInitiative[MAX_AGENTS];
	bool m_agentBeganTurn[MAX_AGENTS];
	bool m_agentEndedTurn[MAX_AGENTS];

	D3DXMATRIX m_UIViewMatrix;
};
