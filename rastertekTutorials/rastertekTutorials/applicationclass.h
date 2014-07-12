////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

/////////////
// GLOBALS //
/////////////
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

//////////////////
// UI CONSTANTS //
//////////////////
// NOTE: Will likely replace many of these constants with variables to allow for resolution based resizing
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
const int COMBAT_MENU_BUTTON_COUNT = 4; // Should try to keep this as a multiple of 2 for proper rectangular set of options on the menu bar.
const int COMBAT_MENU_BUTTON_HORIZONTAL_OFFSET = -20 - COMBAT_MENU_BUTTON_WIDTH * (COMBAT_MENU_BUTTON_COUNT / 2); // End the options at the right side of the menu bar
const int COMBAT_MENU_BUTTON_VERTICAL_OFFSET = (COMBAT_MENU_HEIGHT / 2) - COMBAT_MENU_BUTTON_HEIGHT;

// Maximum number of tiles processed per frame when building MovementMap
const int MAX_PATHNODES_PER_FRAME = 1;

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
#include <list>

/////////////
// STRUCTs //
/////////////
struct Pathnode{
	int tileX;
	int tileY;
	int cost;
	bool visited;
	bool optimal;
	Pathnode *prev;
};

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


	//////////////////
	// COMMAND ENUM //
	//////////////////
	enum Command{
		COMMAND_DEFAULT,
		COMMAND_MOVE,
		COMMAND_ATTACK
	};


	///////////////
	// MENU ENUM //
	///////////////
	enum UIMenu{
		UIMENU_NOMENU,
		UIMENU_MAINMENU,
		UIMENU_OPTIONSMENU,
		UIMENU_COMBATMENUBAR
	};

	//////////////////////////////////
	// UIELEMENT ENUMS AND CONSTANT //
	//////////////////////////////////
	const int UIELEMENT_NOELEMENT = -1;

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
		COMBATMENUBUTTON_MOVE = 0,
		COMBATMENUBUTTON_ATTACK,
		COMBATMENUBUTTON_ENDTURN,
		COMBATMENUBUTTON_MENU
	};

public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int, bool);
	void Shutdown();
	bool Frame();

private:
	// Initialization
	bool ReadConfig();

	void FindCurrentUIElement();

	// Handle Input
	bool HandleInput(float);
	bool SelectAgent();
	bool SetSelectedAgent(ActiveAgentClass*);
	bool OrderMove();
	bool OrderAttack();

	// Pathfinding
	void BuildMovementMap();
	void ClearMovementMap();
	void ProcessPathnodes(int);
	void VisitPathnode(int, int);
	int FindCost(int, int);
	int FindDistance(int, int, int, int);

	void NextTurn();
	void EndTurn();
	bool InitializeCombatMap(MapType, int, int);
	void ShutdownCombatMap();

	// Agents
	bool CreateActiveAgent(AgentType, int, int, int, int);
	bool CreateInactiveAgent(AgentType, int, int);
	void ClearAgents();

	// Update
	bool Update(float, bool);
	bool UpdateTooltip();

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
	SpriteClass* m_AgentSprites;

	// State Variables
	MainState m_MainState;
	MenuState m_MenuState;
	bool m_CommandSelected;
	Command m_SelectedCommand;

	// UI Related Variables
	int m_screenWidth, m_screenHeight;
	bool m_fullscreen;
	int m_combatMapWidth, m_combatMapHeight;

	// Cursor Related Variables
	int m_mouseX, m_mouseY;
	bool m_stateChanged;
	UIMenu m_currentUIMenu;
	int m_currentUIElement;
	int m_currentTileX, m_currentTileY;
	int m_currentTileIndex;
	bool m_cursorOverTile;

	// Tooltip Variables
	bool m_displayTooltip;
	float m_tooltipDelay;
	float m_cursorIdleTime;
	int m_tooltipX, m_tooltipY;
	int m_tooltipWidth, m_tooltipHeight;

	// CombatMap Variables, Arrays and Queues
	Pathnode* m_MovementMap;
	std::list <Pathnode*> m_MovementQueue;
	std::list <ActiveAgentClass*> m_ActiveAgents;
	std::list <AgentClass*> m_InactiveAgents;
	ActiveAgentClass* m_SelectedAgent;

	D3DXMATRIX m_UIViewMatrix;
};
