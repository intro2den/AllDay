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

////////////////////
// CLASS INCLUDES //
////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"

#include "bitmapclass.h"
#include "textureshaderclass.h"

#include "fontclass.h"
#include "fontshaderclass.h"

#include "mainmenuclass.h"
#include "optionsmenuclass.h"
#include "gameoptionsmenuclass.h"
#include "combatmenubarclass.h"
#include "combatmainmenuclass.h"

#include "combatmapclass.h"
#include "hexmapclass.h"

#include "textclass.h"
#include "errortextclass.h"

#include "timerclass.h"
#include "positionclass.h"

#include "activeagentclass.h"
#include "spriteclass.h"
#include "abilityclass.h"

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

	//////////////////
	// COMMAND ENUM //
	//////////////////
	enum Command{
		COMMAND_DEFAULT,
		COMMAND_MOVE,
		COMMAND_ABILITY
	};

	// Maximum number of tiles processed per frame when building MovementMap
	const int MAX_PATHNODES_PER_FRAME = 1;

public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int, bool);
	void Shutdown();
	bool Frame();

private:
	void ReadConfig();
	void WriteConfig();
	bool InitializeMenus();

	bool EnterMainMenu();

	bool HandleInput(float);

	bool UpdateSliderVariables();
	void StateChanged();
	void DeselectCommand();

	void SetHighlightedAgent();
	bool SelectAgent();
	bool SetSelectedAgent(ActiveAgentClass*);
	bool OrderMove();
	bool OrderAbility();

	// Pathfinding
	void BuildMovementMap();
	void ClearMovementMap();
	void ProcessPathnodes(int);
	void VisitPathnode(int, int);
	int FindCost(int, int);
	int FindDistance(int, int, int, int);

	void NextTurn();
	void EndTurn();
	bool EnterCombatMap();
	bool InitializeCombatMap(MapType, int, int);
	void ShutdownCombatMap();

	// Agents
	bool CreateActiveAgent(AgentType, int, int, int, int);
	bool CreateInactiveAgent(AgentType, int, int);
	void ClearAgents();

	// Update
	bool Update(float, bool);
	bool UpdateTooltip();
	bool SetTooltip(char*, char*);
	void ResetTooltip();

	bool NewError(char*);
	void ClearErrors();

	// Render
	bool RenderGraphics();

private:
	// UI Constants and Resolution based constants
	// Standard Button
	int STANDARDBUTTON_WIDTH = 350;
	int STANDARDBUTTON_HEIGHT = 50;

	// Sliders
	const int SLIDER_WIDTH = 16;
	const int SLIDER_HEIGHT = 32;
	const int SLIDER_BAR_WIDTH = 200;
	const int SLIDER_BAR_HEIGHT = 20;

	// Tooltips
	const float MAX_TOOLTIPDELAY = 3000.0f;
	const int TOOLTIPDELAY_SLIDER_STEPS = 30;

	// CombatMap Menubar
	const int COMBAT_MENUBAR_HEIGHT = 100;


	InputClass* m_Input;
	D3DClass* m_D3D;
	CameraClass* m_Camera;

	BitmapClass* m_MainBackground;
	BitmapClass* m_StandardButton;
	BitmapClass* m_StandardSlider;
	BitmapClass* m_StandardSliderBackground;
	BitmapClass* m_MenuBackground;
	BitmapClass* m_Mouse;
	TextureShaderClass* m_TextureShader;

	FontClass* m_StandardFont;
	FontShaderClass* m_FontShader;
	ErrorTextClass* m_ErrorText;
	TextClass* m_TooltipText;

	MainMenuClass* m_MainMenu;
	OptionsMenuClass* m_OptionsMenu;
	GameOptionsMenuClass* m_GameOptionsMenu;
	CombatMenubarClass* m_CombatMenubar;
	CombatMainMenuClass* m_CombatMainMenu;

	TimerClass* m_Timer;
	PositionClass* m_Position;

	CombatMap* m_CombatMap;
	HexMapClass* m_TerrainMap;
	HexMapClass* m_HexHighlight;
	SpriteClass* m_AgentSprites;
	BitmapClass* m_AgentHealthbarBackground;
	BitmapClass* m_AgentHealthbar;

	std::list <MenuClass*> m_CurrentMenus;

	// State Variables
	MainState m_MainState;
	bool m_CommandSelected;
	Command m_SelectedCommand;

	// UI Related Variables
	int m_screenWidth, m_screenHeight;
	bool m_fullscreen;
	int m_combatMapWidth, m_combatMapHeight;

	// Cursor Related Variables
	int m_mouseX, m_mouseY;
	int m_currentTileX, m_currentTileY;
	int m_currentTileIndex;
	bool m_cursorOverTile;

	// Tooltip Variables
	bool m_displayTooltip, m_checkedForTooltip;
	float m_tooltipDelay, m_tooltipSlider;
	float m_cursorIdleTime;
	int m_tooltipX, m_tooltipY;
	int m_tooltipWidth, m_tooltipHeight;

	// CombatMap Variables, Arrays and Queues
	Pathnode* m_MovementMap;
	std::list <Pathnode*> m_MovementQueue;
	std::list <ActiveAgentClass*> m_ActiveAgents;
	std::list <AgentClass*> m_InactiveAgents;
	ActiveAgentClass* m_SelectedAgent;
	ActiveAgentClass* m_HighlightedAgent;
	AbilityClass* m_SelectedAbility;

	D3DXMATRIX m_UIViewMatrix;
};
