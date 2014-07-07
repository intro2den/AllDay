////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


bool compare2(Pathnode* x, Pathnode* y){
	return x->cost < y->cost;
}

// Object pointers initialize to NULL (Important in event of initialization failure)
ApplicationClass::ApplicationClass(){
	m_Input = 0;
	m_D3D = 0;
	m_Camera = 0;
	m_MainBackground = 0;
	m_StandardButton = 0;
	m_Mouse = 0;
	m_MenuBackground = 0;
	m_CombatMap = 0;
	m_TerrainMap = 0;
	m_HexHighlight = 0;
	m_TextureShader = 0;
	m_Text = 0;
	m_FontShader = 0;
	m_Timer = 0;
	m_Position = 0;
	m_AgentSprites = 0;
}

// Empty copy constructor and empty class deconstructor
// It is possible these would be made otherwise, and are not necessarily called, can cause memory leaks if relied upon
ApplicationClass::ApplicationClass(const ApplicationClass& other){
}

ApplicationClass::~ApplicationClass(){
}

// Initialize - creates window for application, initializes input and graphics objects
bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight){
	bool result;
	float cameraX, cameraY, cameraZ;
	D3DXMATRIX baseViewMatrix;
	
	// Keep track of the screen width and height for bounding the camera, dynamic bitmap initialization and/or scaling(?)
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Set initial MainState, MenuState and CommandState
	m_MainState = MAINSTATE_MAINMENU;
	m_MenuState = MENUSTATE_MAINMENU;
	m_CommandSelected = false;
	m_SelectedCommand = COMMAND_DEFAULT;

	// Initialize the mouse (cursor) position (this will be overwritten in the first frame)
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the flag indicating a state change
	m_stateChanged = true;

	// Initialize the currently highlighted UI menu and element to none, no UI elements are highlighted on initialization
	m_currentUIMenu = UIMENU_NOMENU;
	m_currentUIElement = MAINMENUBUTTON_NOBUTTON;

	// Initialize the coordinates and index for the currently highlighted tile (for the CombatMap - initially invalid coordinates)
	m_currentTileX = -1;
	m_currentTileY = -1;
	m_currentTileIndex = -1;
	m_cursorOverTile = false;

	// Initialize the index of the currently selected Agent to null, no agent is initially selected
	m_SelectedAgent = 0;

	// Initialize tooltip specific variables
	m_displayTooltip = false;
	m_tooltipDelay = 500.0f;
	m_cursorIdleTime = 0.0f;
	m_tooltipX = 0;
	m_tooltipY = 0;
	m_tooltipWidth = 0;
	m_tooltipHeight = 0;

	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if (!m_Input){
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd);
	if (!result){
		MessageBox(hwnd, "Could not initialize the input object.", "Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D){
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(m_screenWidth, m_screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result){
		MessageBox(hwnd, "Could not initialize DirectX 11.", "Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera){
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	m_UIViewMatrix = baseViewMatrix;

	// Set the initial position of the camera.
	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = -10.0f;

	m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	// Initialize the Background bitmap object
	m_MainBackground = new BitmapClass;
	if (!m_MainBackground){
		return false;
	}

	result = m_MainBackground->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/seafloor.dds", m_screenWidth, m_screenHeight);
	if (!result){
		MessageBox(hwnd, "Could not initialize the background object.", "Error", MB_OK);
		return false;
	}

	// Initialize the Main Menu bitmap object
	m_StandardButton = new BitmapClass;
	if (!m_StandardButton){
		return false;
	}

	result = m_StandardButton->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/ui_button.dds", MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT);
	if (!result){
		MessageBox(hwnd, "Could not initialize the main menu bitmap object.", "Error", MB_OK);
		return false;
	}

	// Initialize the MenuBackground bitmap object
	m_MenuBackground = new BitmapClass();
	if (!m_MenuBackground){
		return false;
	}

	result = m_MenuBackground->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/ui_menubarbackground.dds", m_screenWidth, COMBAT_MENU_HEIGHT);
	if (!result){
		return false;
	}

	// Initialize the cursor bitmap object
	m_Mouse = new BitmapClass;
	if (!m_Mouse){
		return false;
	}

	result = m_Mouse->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/cursor.dds", 32, 32);
	if (!result){
		MessageBox(hwnd, "Could not initialize the bitmap object.", "Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader){
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result){
		MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text){
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, m_screenWidth, m_screenHeight, baseViewMatrix, MAIN_MENU_BUTTON_HORIZONTAL_OFFSET, MAIN_MENU_BUTTON_VERTICAL_OFFSET, MAIN_MENU_BUTTON_HEIGHT, MAIN_MENU_BUTTON_SPACING);
	if (!result){
		MessageBox(hwnd, "Could not initialize the text object.", "Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader){
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result){
		MessageBox(hwnd, "Could not initialize the font shader object.", "Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer){
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result){
		MessageBox(hwnd, "Could not initialize the Timer object.", "Error", MB_OK);
		return false;
	}

	// Create the position object
	m_Position = new PositionClass;
	if (!m_Position){
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(cameraX, cameraY, cameraZ);

	// Proof of Concept, Read Configuration file
	result = ReadConfig();
	if (!result){
		return false;
	}

	return true;
}

// Shutdown function - shuts down and releases everything, shuts down window, cleans up handles
void ApplicationClass::Shutdown(){
	// Shutdown the CombatMap and release all associated objects
	ShutdownCombatMap();

	// Release the position object
	if (m_Position){
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object
	if (m_Timer){
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the font shader object.
	if (m_FontShader){
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the text object.
	if (m_Text){
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the texture shader object
	if (m_TextureShader){
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the Cursor bitmap object
	if (m_Mouse){
		m_Mouse->Shutdown();
		delete m_Mouse;
		m_Mouse = 0;
	}

	// Release the MenuBarBackground Bitmap object
	if (m_MenuBackground){
		m_MenuBackground->Shutdown();
		delete m_MenuBackground;
		m_MenuBackground = 0;
	}

	// Release the Main Menu bitmap object
	if (m_StandardButton){
		m_StandardButton->Shutdown();
		delete m_StandardButton;
		m_StandardButton = 0;
	}


	// Release the Background bitmap object
	if (m_MainBackground){
		m_MainBackground->Shutdown();
		delete m_MainBackground;
		m_MainBackground = 0;
	}

	// Release the camera object.
	if (m_Camera){
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_D3D){
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the input object.
	if (m_Input){
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}

// All processing for application done here - input, processing and graphics
bool ApplicationClass::Frame(){
	bool result;
	float frameTime;
	int prevMouseX, prevMouseY;

	// Update system stats
	m_Timer->Frame();

	// Do the input frame processing.
	result = m_Input->Frame();
	if (!result){
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsEscapePressed() == true){
		return false;
	}

	// Store the previous cursor coordinates
	prevMouseX = m_mouseX;
	prevMouseY = m_mouseY;

	// Get the location of the mouse from the input object,
	m_Input->GetMouseLocation(m_mouseX, m_mouseY);

	// If the mouse is over the window, update the text strings displaying the cursor coordinates
	if (m_mouseX >= 0 && m_mouseX < m_screenWidth && m_mouseY >= 0 && m_mouseY < m_screenHeight){
		result = m_Text->SetMousePosition(m_mouseX, m_mouseY, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}
	}

	// If the cursor has moved, set stateChanged to true
	m_stateChanged = (m_stateChanged || (m_mouseX != prevMouseX || m_mouseY != prevMouseY));
	
	// Find which UI element is under the cursor
	FindCurrentUIElement();

	frameTime = m_Timer->GetTime();

	result = HandleInput(frameTime);
	if (!result){
		return false;
	}

	result = Update(frameTime, (m_mouseX == prevMouseX && m_mouseY == prevMouseY));
	if (!result){
		return false;
	}

	// Render the graphics
	result = RenderGraphics();
	if (!result){
		return false;
	}
	
	return true;
}

// Read Configuration File to configure settings
// NOTE: This should probably be done in the SystemClass before the
//       ApplicationClass is instantiated, specifically for setting the
//       resolution of the application window.
bool ApplicationClass::ReadConfig(){
	bool result;
	ifstream fin;
	char configString[20];

	// Proof of Concept for reading Configuration File
	fin.open("../rastertekTutorials/data/configuration.txt");
	if (fin.fail()){
		return false;
	}

	// Read through the Configuration File and set relevant variables accordingly
	fin.getline(configString, 20, ' ');

	while (!fin.eof()){
		// This is probably not a good way to parse a text file
		if (strncmp(configString, "mainstate", 8) == 0){
			fin.getline(configString, 20, ' ');
			fin.getline(configString, 20, ' ');

			if (strncmp(configString, "combat", 6) == 0){
				// Change the MainState to CombatMap, create a new CombatMap and begin the first round
				m_MainState = MAINSTATE_COMBATMAP;
				m_MenuState = MENUSTATE_NOMENU;

				// Clear all error messages on state change
				result = m_Text->ClearErrors(m_D3D->GetDeviceContext());
				if (!result){
					return false;
				}

				if (!m_CombatMap){
					result = InitializeCombatMap((MapType)(rand() % 2), 32, 32);
					if (!result){
						return false;
					}
				}

				// Set appropriate Menu Text
				result = m_Text->SetCombatMapText(m_D3D->GetDeviceContext());
				if (!result){
					return false;
				}

				// Begin the first round of Combat
				NextTurn();
			}

		}

		fin.getline(configString, 20, ' ');
	}

	// Close the configuration file.
	fin.close();
	return true;
}

void ApplicationClass::FindCurrentUIElement(){
	// If the state has changed (state change or cursor movement), find the UI
	// Element that the cursor is currently over.
	int cursorX, cursorY;

	// If the cursor is still over the same UI element, return
	if (!m_stateChanged){
		return;
	}

	// Reset the current menu and UI element to none, the previous element should not be remembered.
	m_currentUIMenu = UIMENU_NOMENU;
	m_currentUIElement = MAINMENUBUTTON_NOBUTTON;

	// The current MainState determines the placement of open menus and other UI Elements
	switch (m_MainState){
	case MAINSTATE_MAINMENU:
		// Main Menu - caluclate the cursor position relative to the menu
		// buttons
		cursorX = m_mouseX - MAIN_MENU_BUTTON_HORIZONTAL_OFFSET;
		cursorY = m_mouseY - MAIN_MENU_BUTTON_VERTICAL_OFFSET;

		// Determine which menu is currently active and which UI element in that menu is under the cursor (if any)
		// NOTE: Currently assuming that the Main Menu MainState has at most a
		//       single UI Menu with no additional menus on top.
		switch (m_MenuState){
		case MENUSTATE_MAINMENU:
			// The Main Menu is currently open
			m_currentUIMenu = UIMENU_MAINMENU;

			// Check for a button under the cursor
			if (cursorX > 0 && cursorX < MAIN_MENU_BUTTON_WIDTH && cursorY > 0 && cursorY < MAIN_MENU_BUTTON_COUNT * (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING) - MAIN_MENU_BUTTON_SPACING){
				// Cursor is within the bounds of the menu buttons, determine which button (if any) was clicked
				if (cursorY % (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING) < MAIN_MENU_BUTTON_HEIGHT){
					m_currentUIElement = cursorY / (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING);
				}
			}

			break;

		case MENUSTATE_OPTIONMENU:
			// The Option Menu is currently open
			m_currentUIMenu = UIMENU_OPTIONSMENU;

			// Check for a button under the cursor
			if (cursorX > 0 && cursorX < MAIN_MENU_BUTTON_WIDTH && cursorY > 0 && cursorY < OPTIONS_MENU_BUTTON_COUNT * (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING) - MAIN_MENU_BUTTON_SPACING){
				// Cursor is within the bounds of the menu buttons, determine which button (if any) was clicked
				if (cursorY % (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING) < MAIN_MENU_BUTTON_HEIGHT){
					m_currentUIElement = cursorY / (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING);
				}
			}

			break;
		}

		break;

	case MAINSTATE_COMBATMAP:
		// Combat Map - check for open Menus, otherwise default to the menu bar
		// NOTE: Some MenuStates allow the user to interact with the menu bar, other MenuStates such
		//       as the MAINMENU and related submenus do not allow interaction with the menu bar.
		switch (m_MenuState){
		case MENUSTATE_MAINMENU:
			// The Combat Map Main Menu is open

			break;

		case MENUSTATE_OPTIONMENU:
			// The Combat Map Options Menu is open

			break;

		default:
			// Calculate the cursor position relative to the menu bar
			cursorX = m_mouseX;
			cursorY = m_mouseY - (m_screenHeight - COMBAT_MENU_HEIGHT);

			// If the cursor is over the menu bar set the current UI menu and check for buttons
			if (cursorX >= 0 && cursorX < m_screenWidth && cursorY >= 0 && cursorY < COMBAT_MENU_HEIGHT){
				m_currentUIMenu = UIMENU_COMBATMENUBAR;

				// Calculate the cursor position relative to the menu bar buttons
				cursorX = m_mouseX - (m_screenWidth + COMBAT_MENU_BUTTON_HORIZONTAL_OFFSET);
				cursorY = m_mouseY - (m_screenHeight - COMBAT_MENU_HEIGHT) - COMBAT_MENU_BUTTON_VERTICAL_OFFSET;

				if (cursorX > 0 && cursorX < COMBAT_MENU_BUTTON_WIDTH * (COMBAT_MENU_BUTTON_COUNT / 2) && cursorY > 0 && cursorY < COMBAT_MENU_BUTTON_HEIGHT * 2){
					m_currentUIElement = 2 * (cursorX / COMBAT_MENU_BUTTON_WIDTH) + (cursorY / COMBAT_MENU_BUTTON_HEIGHT);
				}
			}

			break;
		}

		break;
	}

	// Set stateChanged to false so these checks don't occur unless necessary
	m_stateChanged = false;

	return;
}

// Handle all user input
bool ApplicationClass::HandleInput(float frameTime){
	bool result;
	bool cursorInBounds, scrolling;
	float posX, posY, posZ;
	float cursorX, cursorY, normalizedCursorX, normalizedCursorY;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Update the frame time for any existing error messages
	// NOTE: This must happen before new errors are created in this frame - not in the Update function.
	result = m_Text->Frame(frameTime, m_D3D->GetDeviceContext());
	if (!result){
		return false;
	}

	// Determine weather the cursor is within window bounds or not
	cursorInBounds = (m_mouseX >= 0 && m_mouseX < m_screenWidth && m_mouseY >= 0 && m_mouseY < m_screenHeight);

	switch (m_MainState){
	case MAINSTATE_MAINMENU:
		// Main Menu Processing
		// Process mouse input, left mouse button
		if (m_Input->WasLeftMouseClicked() == true){
			// Which button was pressed, if any, depends on the current menu
			switch (m_currentUIMenu){
			case UIMENU_MAINMENU:
				switch (m_currentUIElement){
				case MAINMENUBUTTON_ENTERCOMBATMAP:
					// Change the MainState to CombatMap, create a new CombatMap and begin the first round
					m_MainState = MAINSTATE_COMBATMAP;
					m_MenuState = MENUSTATE_NOMENU;
					m_stateChanged = true;

					// Clear all error messages on state change
					result = m_Text->ClearErrors(m_D3D->GetDeviceContext());
					if (!result){
						return false;
					}

					if (!m_CombatMap){
						result = InitializeCombatMap((MapType)(rand() % 2), 32, 32);
						if (!result){
							return false;
						}
					}

					// Set appropriate Menu Text
					result = m_Text->SetCombatMapText(m_D3D->GetDeviceContext());
					if (!result){
						return false;
					}

					// Begin the first round of Combat
					NextTurn();

					break;

				case MAINMENUBUTTON_OPTIONS:
					// Change the MenuState to OptionsMenu
					m_MenuState = MENUSTATE_OPTIONMENU;
					m_stateChanged = true;

					// Clear all error messages on state change
					result = m_Text->ClearErrors(m_D3D->GetDeviceContext());
					if (!result){
						return false;
					}

					// Set appropriate Menu Text
					result = m_Text->SetOptionsMenuText(MAIN_MENU_BUTTON_HORIZONTAL_OFFSET, MAIN_MENU_BUTTON_VERTICAL_OFFSET, MAIN_MENU_BUTTON_HEIGHT, MAIN_MENU_BUTTON_SPACING, m_D3D->GetDeviceContext());
					if (!result){
						return false;
					}

					break;

				case MAINMENUBUTTON_EXIT:
					// Exit the application
					return false;

				default:
					// Do nothing if no button was pressed
					break;
				}

				break;

			case UIMENU_OPTIONSMENU:
				switch (m_currentUIElement){
				case OPTIONSMENUBUTTON_BACK:
					// Change the MenuState to MainMenu
					m_MenuState = MENUSTATE_MAINMENU;
					m_stateChanged = true;

					// Clear all error messages on state change
					result = m_Text->ClearErrors(m_D3D->GetDeviceContext());
					if (!result){
						return false;
					}

					// Set appropriate Menu Text
					result = m_Text->SetMainMenuText(MAIN_MENU_BUTTON_HORIZONTAL_OFFSET, MAIN_MENU_BUTTON_VERTICAL_OFFSET, MAIN_MENU_BUTTON_HEIGHT, MAIN_MENU_BUTTON_SPACING, m_D3D->GetDeviceContext());
					if (!result){
						return false;
					}

					break;

				default:
					// Do nothing if no button was pressed
					break;
				}

				break;
			}
		}

		break;

	case MAINSTATE_COMBATMAP:
		// Combat Map Processing
		// Get the currect camera position
		m_Position->GetPosition(posX, posY, posZ);

		// Scroll the camera if the cursor is near the edge of the window
		// or if the user is pressing the corresponding arrow keys
		// Scroll Up/Forward
		scrolling = (m_Input->IsUpPressed() || (cursorInBounds && m_mouseY < 20));
		m_Position->MoveForward(scrolling);

		// Scroll Down/Backward
		scrolling = (m_Input->IsDownPressed() || (m_mouseY > m_screenHeight - 20 && cursorInBounds));
		m_Position->MoveBackward(scrolling);

		// Scroll Left
		scrolling = (m_Input->IsLeftPressed() || (cursorInBounds && m_mouseX < 20));
		m_Position->MoveLeft(scrolling);

		// Scroll Right
		scrolling = (m_Input->IsRightPressed() || (m_mouseX > m_screenWidth - 20 && cursorInBounds));
		m_Position->MoveRight(scrolling);

		// Update the position of the camera
		m_Position->GetPosition(posX, posY, posZ);
		m_Camera->SetPosition(posX, posY, posZ);

		// Find the coordinates of the hex that the cursor is overtop of this frame - if the cursor is over the map
		m_currentTileX = -1;
		m_currentTileY = -1;
		m_currentTileIndex = -1;
		m_cursorOverTile = false;

		if (cursorInBounds && m_currentUIMenu == UIMENU_NOMENU){
			// Calculate the actual cursor position relative to the map
			cursorX = (float)m_mouseX + posX - MAP_HORIZONTALOFFSET;
			cursorY = (float)m_mouseY - posY - MAP_VERTICALOFFSET;

			// Calculate the grid coordinates for the hex the cursor is over
			m_currentTileX = (int)(cursorX / (1.5f * HEX_SIZE));

			// Use the X-coordinate of the cursor to find the column
			if (fmod(cursorX, 1.5f * HEX_SIZE) >= 0.5f * HEX_SIZE){
				// The X-position of the Hex that the cursor is over can be identified by the X-coordinate alone
				m_currentTileX = (int)(cursorX / (1.5f * HEX_SIZE));
			} else{
				// The cursor could be over a hex in either of 2 adjacent columns, will need to use both
				// coordinates to determine the column

				// Normalize the coordinates to be relative the origin
				normalizedCursorX = abs(fmod(cursorX, 1.5f * HEX_SIZE)) / (0.5f * HEX_SIZE);
				normalizedCursorY = abs((fmod(cursorY + 0.5f * HEX_HEIGHT * (float)(abs(m_currentTileX % 2)), HEX_HEIGHT)) - 0.5f * HEX_HEIGHT) / (0.5f * HEX_HEIGHT);

				if (normalizedCursorX >= abs(normalizedCursorY)){
					m_currentTileX = (int)(cursorX / (1.5f * HEX_SIZE));
				} else{
					m_currentTileX = (int)(cursorX / (1.5f * HEX_SIZE)) - 1;
				}
			}

			// Now that we know the column, we can identify the row using the Y-coordinate
			m_currentTileY = (int)((cursorY - 0.5f * HEX_HEIGHT * abs(m_currentTileX % 2)) / HEX_HEIGHT);

			// Integer division rounds towards 0, make sure that we catch all negative X- and Y-coordinates as negative
			if (cursorX < 0){
				m_currentTileX = -1;
			}

			if (cursorY - 0.5f * HEX_HEIGHT * abs(m_currentTileX % 2) < 0){
				m_currentTileY = -1;
			}

			// Set the currentTileIndex and the cursorOverTile flag to true if the cursor is over a hex on the map
			if (m_currentTileX >= 0 && m_currentTileX < m_combatMapWidth && m_currentTileY >= 0 && m_currentTileY < m_combatMapHeight){
				m_currentTileIndex = m_currentTileX * m_combatMapHeight + m_currentTileY;
				m_cursorOverTile = true;
			}
		}

		// Handle user input (aside from Camera movement) relevant to the CombatMap
		if (m_Input->WasLeftMouseClicked() == true){
			// Left Click - Check for clicking on Menu Buttons, map tiles

			// If the cursor is over a menu and a button was clicked respond accordingly
			switch (m_currentUIMenu){
			case UIMENU_MAINMENU:
				break;

			case UIMENU_OPTIONSMENU:
				break;

			case UIMENU_COMBATMENUBAR:
				switch (m_currentUIElement){
				case COMBATMENUBUTTON_MOVE:
					// Select the Move Command
					m_CommandSelected = true;
					m_SelectedCommand = COMMAND_MOVE;
					break;

				case COMBATMENUBUTTON_ATTACK:
					// Select the (Basic) Attack Command
					m_CommandSelected = true;
					m_SelectedCommand = COMMAND_ATTACK;
					break;

				case COMBATMENUBUTTON_ENDTURN:
					// End Turn
					EndTurn();
					break;

				case COMBATMENUBUTTON_MENU:
					// Return to the Main Menu
					// NOTE: This will be replaced by a popup menu with in-game options (including returning to the Main Menu)
					m_MainState = MAINSTATE_MAINMENU;
					m_MenuState = MENUSTATE_MAINMENU;
					m_CommandSelected = false;
					m_SelectedCommand = COMMAND_DEFAULT;
					m_stateChanged = true;

					// Clear all error messages on state change
					result = m_Text->ClearErrors(m_D3D->GetDeviceContext());
					if (!result){
						return false;
					}

					// Set appropriate Menu Text
					result = m_Text->SetMainMenuText(MAIN_MENU_BUTTON_HORIZONTAL_OFFSET, MAIN_MENU_BUTTON_VERTICAL_OFFSET, MAIN_MENU_BUTTON_HEIGHT, MAIN_MENU_BUTTON_SPACING, m_D3D->GetDeviceContext());
					if (!result){
						return false;
					}

					// Set the cursorOverTile flag to false
					m_cursorOverTile = false;

					// Deselect any selected Agent and update the associated sentence
					result = SetSelectedAgent(NULL);
					if (!result){
						return false;
					}

					// Shutdown the Combat Map and all associated data structures
					ShutdownCombatMap();

					// Set the position of the camera back to the origin
					m_Position->SetPosition(0.0f, 0.0f, -10.0f);
					m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
					break;

				default: 
					// Do nothing if no button was pressed (this should only happen with an odd number of buttons)
					break;
				}

				break;
			}

			// If the left mouse button was clicked over a tile, different behaviour can occur.
			if (m_cursorOverTile){
				switch (m_SelectedCommand){
				case COMMAND_DEFAULT:
					// Select an Agent in the highlighted tile
					result = SelectAgent();
					if (!result){
						return false;
					}

					break;

				case COMMAND_MOVE:
					// Command the selected Agent to move to the highlighted tile

					// Ensure that an Agent is selected
					if (m_SelectedAgent){
						result = OrderMove();
						if (!result){
							return false;
						}
					} else{
						result = m_Text->NewErrorMessage("No Agent Selected", m_D3D->GetDeviceContext());
						if (!result){
							return false;
						}
					}

					// Return to default behaviour, deselect the current command
					m_CommandSelected = false;
					m_SelectedCommand = COMMAND_DEFAULT;

					break;

				case COMMAND_ATTACK:
					// Command the selected Agent to use their Basic Attack against
					// the highlighted tile
					
					// Ensure that an Agent is selected
					if (m_SelectedAgent){
						result = OrderAttack();
						if (!result){
							return false;
						}
					} else{
						result = m_Text->NewErrorMessage("No Agent Selected", m_D3D->GetDeviceContext());
						if (!result){
							return false;
						}
					}

					// Return to default behaviour, deselect the current command
					m_CommandSelected = false;
					m_SelectedCommand = COMMAND_DEFAULT;

					break;
				}

				break;
			}
		}

		if (m_Input->WasRightMouseClicked() == true){
			// Right Click - Cancel Command Selection, Default mouse behaviour
			if (m_CommandSelected){
				// If a command is selected, deselect it
				m_CommandSelected = false;
				m_SelectedCommand = COMMAND_DEFAULT;
			} else if (m_cursorOverTile && m_SelectedAgent){
				// Move the selected Agent to the currently highlighted tile
				// NOTE: Will also need behaviour to Order an Attack if an Enemy Agent is
				//       present in the highlighted tile.
				result = OrderMove();
				if (!result){
					return false;
				}
			}

			break;
		}

		break;
	}

	return true;
}

bool ApplicationClass::SelectAgent(){
	// Find and select an Active Agent in the currently highlighted tile. If
	// there are no Active Agents in the currently highlighted tile, deslect
	// any currently selected Agent.
	bool result;
	std::list<ActiveAgentClass*>::iterator agent;
	bool agentFound;
	int agentX, agentY;

	// NOTE: This is primarily proof of concept, additional work will need to be done if multiple
	//       agents are present in the hex, and for any other necessary checks related to Agent
	//       selection.
	agentFound = false;

	// Check if an agent is in the tile that was just selected
	for (agent = m_ActiveAgents.begin(); agent != m_ActiveAgents.end(); ++agent){
		(*agent)->GetPosition(agentX, agentY);
		if (agentX == m_currentTileX && agentY == m_currentTileY){
			// An agent is in the highlighted tile
			agentFound = true;

			// Update the ID of the selected Agent
			result = SetSelectedAgent(*agent);
			if (!result){
				return false;
			}

			// For now we only care about the first Agent found, break
			break;
		}
	}

	// If no agent was found in the tile, unselect any selected Agent
	if (!agentFound){
		result = SetSelectedAgent(NULL);
		if (!result){
			return false;
		}
	}

	return true;
}

bool ApplicationClass::SetSelectedAgent(ActiveAgentClass* agent){
	// Set m_SelectedAgent, update the selectedAgent string in the Text object and start
	// pathfinding for the newly selected Agent.
	bool result;

	// If the newly selected agent is already the currently selected agent, do nothing
	if (agent == m_SelectedAgent){
		return true;
	}

	// Update selectedAgent, the associated string displaying which Agent is selected
	m_SelectedAgent = agent;

	if (m_SelectedAgent){
		result = m_Text->SetSelectedAgent(m_SelectedAgent->GetName(), m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}
	} else{
		result = m_Text->SetSelectedAgent(NULL, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}
	}

	// If an Agent was selected, start building a new MovementMap starting at the Agent's current location,
	// if an Inactive Agent or no Agent was selected, clear the MovementQueue and MovementMap
	if (m_SelectedAgent){
		BuildMovementMap();
	} else{
		ClearMovementMap();
	}

	return true;
}

bool ApplicationClass::OrderMove(){
	// Command the currently selected Agent to move to the currently highlighted tile
	bool result;

	// NOTE: Other checks for invalid move commands and display of appropriate error messages
	//       should occur in this function.

	// Only move the selected Agent if it is that Agent's turn, otherwise display an appropriate error message
	if (m_SelectedAgent->StartedTurn() && !m_SelectedAgent->EndedTurn()){
		// Move the Agent and start building a new MovementMap from the Agent's new position
		m_SelectedAgent->Move(m_currentTileX, m_currentTileY, m_MovementMap[m_currentTileIndex].cost);
		BuildMovementMap();
	} else{
		result = m_Text->NewErrorMessage("The selected Agent isn't active.", m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}
	}

	return true;
}

bool ApplicationClass::OrderAttack(){
	// Command the currently selected Agent to move within range of the currently highlighted tile
	// and use their Basic Attack ability against any Agents in the highlighted tile.
	bool result;

	// NOTE: When the Ability Class and an OrderAbility or equivalent function are implemented,
	//       this behaviour of this function should be executed by that function. In effect this
	//       function is purely for proof of concept.

	// Only order the selected Agent to attack if it is that Agent's turn, otherwise display an
	// approproate error message
	if (m_SelectedAgent->StartedTurn() && !m_SelectedAgent->EndedTurn()){
		// TODO: Implement functionality for a Basic Attack
		//       If the target is not within range of the currently selected Agent's attack
		//       the Agent should move only as far as necessary (minimum cost) to get within
		//       range before attacking.

	} else{
		result = m_Text->NewErrorMessage("The selected Agent isn't active.", m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}
	}

	return true;
}

void ApplicationClass::BuildMovementMap(){
	// Begin building a MovementMap for the currently selected Agent
	Pathnode* firstNode;
	int startX, startY;
	int startIndex;

	// Empty the MovementQueue and clear the MovementMap of any previous data
	ClearMovementMap();

	// Get the currently selected Agent's position as an index location on the map
	m_SelectedAgent->GetPosition(startX, startY);
	startIndex = startX * m_combatMapHeight + startY;
	
	// Add the first node in the queue - the selected Agent's current location and
	// set the cost to 0 (not moving) and 'visit' the node so that it is not added
	// to the queue again.
	firstNode = &m_MovementMap[startIndex];
	firstNode->cost = 0;
	firstNode->visited = true;

	// Push the first node into the MovementQueue
	m_MovementQueue.push_back(firstNode);

	// Process a few nodes in the Queue, the remainder of the MovementMap will
	// be generated over time or upon demand for a specific path
	ProcessPathnodes(7);	

	return;
}

void ApplicationClass::ClearMovementMap(){
	// Empty the MovementQueue and (Re)initialize the Pathnodes for the MovementMap
	int i;

	m_MovementQueue.clear();

	if (m_MovementMap){
		for (i = 0; i < m_combatMapWidth * m_combatMapHeight; i++){
			m_MovementMap[i].tileX = i / m_combatMapHeight;
			m_MovementMap[i].tileY = i % m_combatMapHeight;
			m_MovementMap[i].cost = -1;
			m_MovementMap[i].visited = false;
			m_MovementMap[i].optimal = false;
			m_MovementMap[i].prev = 0;
		}
	}

	return;
}

void ApplicationClass::ProcessPathnodes(int processLimit){
	// Process up to processLimit pathnodes in the MovementQueue and update
	// the MovementMap
	int currX, currY;
	int currIndex, neighbourIndex;
	int nodesProcessed = 0;

	// Search Loop - update the Pathnodes that correspond to all the tiles that
	// the currently selected Agent can path to.
	while (!m_MovementQueue.empty() && nodesProcessed < processLimit){
		// Process the next node in the queue

		// The path found to the current pathnode is guaranteed to be the
		// shortest path from the current Agent's location.
		m_MovementQueue.front()->optimal = true;

		// For the current pathnode, find and update any adjacent pathnodes
		// that have not yet been visited.
		currX = m_MovementQueue.front()->tileX;
		currY = m_MovementQueue.front()->tileY;
		currIndex = currX * m_combatMapHeight + currY;

		// Check neighbours of the current tile in a clockwise direction starting
		// with the tile directly above the current tile
		// NOTE: The coordinates of adjacent pathnodes/tiles is dependant on
		//       the x-coordinate of the current tile
		if (currX % 2 == 0){
			// Even Column hex

			// Check if there is a tile above the current tile
			if (currY > 0){
				// There is a tile above the current tile
				neighbourIndex = currIndex - 1;
				VisitPathnode(currIndex, neighbourIndex);
			}

			// Check if there are any tiles to the right (Agent not on the rightmost edge of the map)
			if (currX < m_combatMapWidth - 1){
				// Check if there is a tile above and to the right
				if (currY > 0){
					// There is a tile above and to the right of the current tile
					neighbourIndex = currIndex + m_combatMapHeight - 1;
					VisitPathnode(currIndex, neighbourIndex);
				}

				// There is a tile below and to the right of the current tile
				// NOTE: This is because the current tile is an even column tile with tiles to the right of it
				neighbourIndex = currIndex + m_combatMapHeight;
				VisitPathnode(currIndex, neighbourIndex);
			}

			// Check if there is a tile below the current tile
			if (currY < m_combatMapHeight - 1){
				// There is a tile below the current tile
				neighbourIndex = currIndex + 1;
				VisitPathnode(currIndex, neighbourIndex);
			}

			// Check if there are any tiles to the left (Agent not on the rightmost edge of the map)
			if (currX > 0){
				// There is a tile below and to the left of the current tile
				// NOTE: This is because the current tile is an even column tile with tiles to the left of it
				neighbourIndex = currIndex - m_combatMapHeight;
				VisitPathnode(currIndex, neighbourIndex);

				// Check if there is a tile above and to the left
				if (currY > 0){
					// There is a tile above and to the left of the current tile
					neighbourIndex = currIndex - m_combatMapHeight - 1;
					VisitPathnode(currIndex, neighbourIndex);
				}
			}
		} else{
			// Odd Column hex

			// Check if there is a tile above the current tile
			if (currY > 0){
				// There is a tile above the current tile
				neighbourIndex = currIndex - 1;
				VisitPathnode(currIndex, neighbourIndex);
			}

			// Check if there are any tiles to the right (Agent not on the rightmost edge of the map)
			if (currX < m_combatMapWidth - 1){
				// There is a tile above and to the right of the current tile
				// NOTE: This is because the current tile is an odd column tile with tiles to the right of it
				neighbourIndex = currIndex + m_combatMapHeight;
				VisitPathnode(currIndex, neighbourIndex);

				// Check if there is a tile below and to the right
				if (currY < m_combatMapHeight - 1){
					// There is a tile below and to the right of the current tile
					neighbourIndex = currIndex + m_combatMapHeight + 1;
					VisitPathnode(currIndex, neighbourIndex);
				}
			}

			// Check if there is a tile below the current tile
			if (currY < m_combatMapHeight - 1){
				// There is a tile below the current tile
				neighbourIndex = currIndex + 1;
				VisitPathnode(currIndex, neighbourIndex);
			}

			// Check if there are any tiles to the left (Agent not on the rightmost edge of the map)
			if (currX > 0){
				// Check if there is a tile below and to the left
				if (currY < m_combatMapHeight - 1){
					// There is a tile below and to the left of the current tile
					neighbourIndex = currIndex - m_combatMapHeight + 1;
					VisitPathnode(currIndex, neighbourIndex);
				}

				// There is a tile above and to the left of the current tile
				// NOTE: This is because the current tile is an odd column tile with tiles to the left of it
				neighbourIndex = currIndex - m_combatMapHeight;
				VisitPathnode(currIndex, neighbourIndex);
			}
		}

		// Pop the front of the queue and sort the remaining pathnodes
		m_MovementQueue.pop_front();
		m_MovementQueue.sort(compare2);

		// Increase nodesProcessed
		nodesProcessed++;
	}
}

void ApplicationClass::VisitPathnode(int currIndex, int neighbourIndex){
	// Visit the provided neighbouring tile to the current tile, update and/or
	// add a corresponding pathnode in the queue if movement into the tile
	// is both possible and the shortest path from the current Agent's location
	// to the tile.
	Pathnode* newNode;
	int moveCost;

	newNode = &m_MovementMap[neighbourIndex];

	// Find the cost to the neighbouring tile from the current tile
	moveCost = FindCost(currIndex, neighbourIndex);

	// If the shortest path to this new tile from the current Agent's location
	// includes the current tile (and the Agent can actually move there), update
	// the cost and previous node for the corresponding pathnode.
	if (moveCost > -1 && (newNode->cost == -1 || moveCost + m_MovementQueue.front()->cost < newNode->cost)){
		newNode->cost = FindCost(currIndex, neighbourIndex) + m_MovementQueue.front()->cost;
		newNode->prev = m_MovementQueue.front();
	}

	// If the Agent can get to this new tile and it hasn't already been visited, add it to the queue.
	if (newNode->cost >= 0 && !newNode->visited){
		m_MovementQueue.push_back(newNode);
		newNode->visited = true;
	}
}

int ApplicationClass::FindCost(int start, int end){
	// Return the cost of moving between the two adjacent tiles provided (by index).
	Terrain endTerrain;
	int cost;

	// Get an instance of the Terrain Type of the ending tile
	// NOTE: If we intend to also take into account the Terrain Type an Agent is moving
	//       from we will need to get an instance of that as well
	endTerrain = m_CombatMap->GetTileTerrain(end);

	// If the end terrain is impassable, return a cost of -1 indicating that the Agent
	// can not move to that tile.
	if (!endTerrain.IsPassable()){
		return -1;
	}

	// Calculate the cost of moving between the two tiles
	// NOTE: Currently only using the base movement cost of the end terrain
	//       however we may end up modifying the final cost based on various
	//       conditions including Effects on the moving Agent and the Terrain
	//       type.
	cost = endTerrain.GetMovementCost();

	return cost;
}

int ApplicationClass::FindDistance(int startX, int startY, int endX, int endY){
	// Return the number of tiles in the shortest path from the tile at the
	// start coordinates to the end coordinates (including the ending tile but
	// not the starting tile).
	int diffX, diffY;
	int distance;

	// The number of tiles in the shortest path between 2 tiles can be determined
	// as follows:
	//  abs(diffX) + max(0, abs(diffY) - (abs(diffX) + abs(diffX % 2) * sign(diffY)) / 2)   if startX is odd
	//  abs(diffX) + max(0, abs(diffY) - (abs(diffX) + abs(diffX % 2) * -sign(diffY)) / 2)  if startX is even
	diffX = endX - startX;
	diffY = endY - startY;
	distance = abs(diffX) + max(0, abs(diffY) - (abs(diffX) + abs(diffX % 2) * ((diffY * 2 + 1) % 2) * ((startX % 2) * 2 - 1)) / 2);

	return distance;
}

void ApplicationClass::NextTurn(){
	// Determine which player, and which of their Agents are able to act for the
	// next turn of Combat.
	int highestInitiative, highestOpposingInitiative;
	int nextActingPlayer;
	std::list<ActiveAgentClass*>::iterator agent;
	int agentOwner, agentInitiative;

	// Initialize the highest initiative to be lower than all valid initiative values
	highestInitiative = -1;
	highestOpposingInitiative = -1;

	// Initialize the nextActing player to an invalid ID
	nextActingPlayer = -1;

	// Determine which player's Agents will have their turn next and the minimum initiative
	// required for them to have that turn. In the event of a tie, a lower agentOwner value
	// takes precedence.
	for (agent = m_ActiveAgents.begin(); agent != m_ActiveAgents.end(); ++agent){
		// Get the Agent's owner and initiative
		agentOwner = (*agent)->GetOwner();
		agentInitiative = (*agent)->GetInitiative();

		if (!(*agent)->StartedTurn() && !(*agent)->EndedTurn() && agentOwner >= 0){
			// Check agentOwner to determine tiebreaking behaviour
			if (agentOwner < nextActingPlayer){
				// Compare initiative, this agent wins ties
				if (agentInitiative >= highestInitiative){
					// New fastest agent, the minimum initiative to act is the previous
					// highestInitiative
					highestOpposingInitiative = highestInitiative;
					highestInitiative = agentInitiative;
					nextActingPlayer = agentOwner;
				} else if (agentInitiative > highestOpposingInitiative){
					// This agent is the fastest owned by a different player, the minimum
					// initiative to act is this agent's initiative + 1
					highestOpposingInitiative = agentInitiative + 1;
				}

			} else if (agentOwner == nextActingPlayer && agentInitiative > highestInitiative){
				// New fastest agent is owned by the same player - update highestInitiative
				highestInitiative = agentInitiative;

			} else if (agentOwner > nextActingPlayer){
				// Compare initiative, this agent loses ties
				if (agentInitiative > highestInitiative){
					// New fastest agent, the minimum initiative to act is the previous
					// highestInitiative + 1
					highestOpposingInitiative = highestInitiative + 1;
					highestInitiative = agentInitiative;
					nextActingPlayer = agentOwner;
				} else if (agentInitiative > highestOpposingInitiative){
					// This agent is the fastest owned by a different player, the minimum
					// initiative to act is this agent's initiative
					highestOpposingInitiative = agentInitiative;
				}
			}
		}
	}

	// Begin the turn of all Agents that will be able to act for the next turn of Combat
	for (agent = m_ActiveAgents.begin(); agent != m_ActiveAgents.end(); ++agent){
		// To act, the Agent must not have already acted this round, meet the minimum
		// initiative requirement and be owned by the next acting player.
		if (!(*agent)->StartedTurn() && (*agent)->GetInitiative() >= highestOpposingInitiative && (*agent)->GetOwner() == nextActingPlayer){
			(*agent)->BeginTurn();
		}
	}

	return;
}

void ApplicationClass::EndTurn(){
	// End the turn of all Agents which have started but not yet ended their turn
	// If all Agents have ended their turn at this point, begin a new round of turns.
	std::list<ActiveAgentClass*>::iterator agent;
	bool endOfRound;

	// Assume the round is ending, if an Agent has not started their turn yet, then
	// the round is not yet ending
	endOfRound = true;

	// End the turn of all Agents which have started but not ended their turn and determine
	// if the round is ending or not.
	for (agent = m_ActiveAgents.begin(); agent != m_ActiveAgents.end(); ++agent){
		if (!(*agent)->StartedTurn() && (*agent)->GetInitiative() >= 0){
			endOfRound = false;
		}

		if ((*agent)->StartedTurn() && !(*agent)->EndedTurn()){
			// End this agent's turn
			(*agent)->EndTurn();
		}
	}

	// If the round is over, begin a new round
	if (endOfRound){
		for (agent = m_ActiveAgents.begin(); agent != m_ActiveAgents.end(); ++agent){
			(*agent)->EndRound();
		}
	}

	// Start the next turn
	NextTurn();

	return;
}

// Initialize a new Combat Map - this should happen when entering the CombatMap MainState (entering the Combat Map)
bool ApplicationClass::InitializeCombatMap(MapType mapType, int mapWidth, int mapHeight){
	bool result;
	float boundX, boundY, boundZ;

	m_combatMapWidth = mapWidth;
	m_combatMapHeight = mapHeight;

	// Initialize the new Combat Map
	m_CombatMap = new CombatMap();
	if (!m_CombatMap){
		return false;
	}

	result = m_CombatMap->Initialize(mapType, m_combatMapWidth, m_combatMapHeight);
	if (!result){
		return false;
	}

	// Initialize the corresponding HexMap for rendering the CombatMap
	m_TerrainMap = new HexMapClass();
	if (!m_TerrainMap){
		return false;
	}

	result = m_TerrainMap->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/combatmap_terrain.dds", m_combatMapWidth, m_combatMapHeight);
	if (!result){
		return false;
	}

	// Initialize a HexMap to highlight the tile that the user has the cursor over
	m_HexHighlight = new HexMapClass();
	if (!m_HexHighlight){
		return false;
	}

	result = m_HexHighlight->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/highlight_white.dds", 1, 1);
	if (!result){
		return false;
	}

	// Initialize an array of Pathnodes for the creation of a MovementMap
	m_MovementMap = new Pathnode[m_combatMapWidth * m_combatMapHeight];
	if (!m_MovementMap){
		return false;
	}

	// Clear the MovementQueue and initialize the Pathnodes for the MovementMap
	ClearMovementMap();

	// Adjust the bounds on the camera position to allow for proper scrolling
	boundX = max(0, HEX_SIZE*(1.5f + 1.5f*(float)mapWidth) - (float)m_screenWidth);
	boundY = min(0, -1.0f * HEX_HEIGHT*(1.5f + (float)mapHeight) + (float)m_screenHeight - float(COMBAT_MENU_HEIGHT));
	boundZ = -10.0f;

	m_Position->SetBounds(boundX, boundY, boundZ);

	// Initialize Agents
	// NOTE: This process will be subject to considerable change, currently used for Proof of Concept
	result = CreateActiveAgent(AGENTTYPE_ACTIVE1, 0, 0, 0, 10);
	if (!result){
		return false;
	}

	result = CreateActiveAgent(AGENTTYPE_ACTIVE2, 0, 2, 0, 7);
	if (!result){
		return false;
	}

	result = CreateInactiveAgent(AGENTTYPE_INACTIVE1, 1, 16);
	if (!result){
		return false;
	}

	result = CreateInactiveAgent(AGENTTYPE_INACTIVE2, 3, 5);
	if (!result){
		return false;
	}

	result = CreateActiveAgent(AGENTTYPE_ACTIVE2, 0, 5, 7, 5);
	if (!result){
		return false;
	}

	result = CreateActiveAgent(AGENTTYPE_ACTIVE3, 1, 7, 3, 7);
	if (!result){
		return false;
	}

	result = CreateActiveAgent(AGENTTYPE_ACTIVE4, 1, 7, 5, 5);
	if (!result){
		return false;
	}

	result = CreateActiveAgent(AGENTTYPE_ACTIVE4, 1, 9, 3, 4);
	if (!result){
		return false;
	}

	// Initialize the bitmap for Agent Sprites
	m_AgentSprites = new SpriteClass();
	if (!m_AgentSprites){
		return false;
	}

	result = m_AgentSprites->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/agentsprites.dds", (int)HEX_SIZE, (int)HEX_HEIGHT);
	if (!result){
		return false;
	}

	return true;
}

// Shutdown the Combat Map specifically - this should happen when exiting from the CombatMap but not the application
void ApplicationClass::ShutdownCombatMap(){
	// Release the AgentSprites bitmap
	if (m_AgentSprites){
		m_AgentSprites->Shutdown();
		delete m_AgentSprites;
		m_AgentSprites = 0;
	}

	// Release the Agents
	ClearAgents();
	
	// Clear the MovementQueue
	ClearMovementMap();
	if (m_MovementMap){
		delete m_MovementMap;
		m_MovementMap = 0;
	}

	// Release the Highlight HexMap object
	if (m_HexHighlight){
		m_HexHighlight->Shutdown();
		delete m_HexHighlight;
		m_HexHighlight = 0;
	}

	// Shutdown the HexMap corresponding to the Combat Map
	if (m_TerrainMap){
		m_TerrainMap->Shutdown();
		delete m_TerrainMap;
		m_TerrainMap = 0;
	}
	
	// Shutdown the CombatMap
	if (m_CombatMap){
		m_CombatMap->Shutdown();
		delete m_CombatMap;
		m_CombatMap = 0;
	}
}

bool ApplicationClass::CreateActiveAgent(AgentType agentType, int playerID, int agentX, int agentY, int initiative){
	// Create a new Active Agent and add it to the list of Active Agents
	bool result;
	ActiveAgentClass* newAgent;

	newAgent = new ActiveAgentClass();
	if (!newAgent){
		return false;
	}

	result = newAgent->Initialize(agentType, playerID, agentX, agentY, initiative);
	if (!result){
		return false;
	}

	m_ActiveAgents.push_back(newAgent);

	return true;
}

bool ApplicationClass::CreateInactiveAgent(AgentType agentType, int agentX, int agentY){
	// Create a new Inactive Agent and add it to the list of Inactive Agents
	bool result;
	AgentClass* newAgent;

	newAgent = new AgentClass();
	if (!newAgent){
		return false;
	}

	result = newAgent->Initialize(agentType, agentX, agentY);
	if (!result){
		return false;
	}

	m_InactiveAgents.push_back(newAgent);

	return true;
}

void ApplicationClass::ClearAgents(){
	// Delete all existing agents
	while (!m_ActiveAgents.empty()){
		delete m_ActiveAgents.front();
		m_ActiveAgents.pop_front();
	}

	while (!m_InactiveAgents.empty()){
		delete m_InactiveAgents.front();
		m_InactiveAgents.pop_front();
	}
}

bool ApplicationClass::Update(float frameTime, bool cursorIdle){
	bool result;

	// If the cursor was not moved and a tooltip was not previously displayed,
	// update cursorIdleTime and check if any appropriate tooltip should be
	// displayed
	if (!cursorIdle){
		// The cursor moved
		m_cursorIdleTime = 0.0f;
		m_displayTooltip = false;
	} else if (cursorIdle && m_cursorIdleTime < m_tooltipDelay){
		m_cursorIdleTime += frameTime;
	}

	// If a tooltip should be displayed and is not currently displayed (either
	// because the cursor moved or wasn't idle for long enough) determine which
	// tooltip should be displayed and set the appropriate variables and text
	// to display the tooltip.
	if (m_cursorIdleTime >= m_tooltipDelay && !m_displayTooltip){
		// If the UI element under the cursor has a tooltip, it should be
		// displayed
		m_displayTooltip = true;

		// Determine which UI element is under the cursor and update the
		// tooltip to be displayed

		// Proof of Concept
		m_tooltipX = m_screenWidth - 150;
		m_tooltipY = 50;
		m_tooltipWidth = 100;
		m_tooltipHeight = 40;

		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}
	}

	// If currently on the CombatMap with an Agent selected, process a few
	// additional pathnodes from the MovementQueue.
	ProcessPathnodes(MAX_PATHNODES_PER_FRAME);

	// TODO: Update the visual location of any moving Agents depending on how
	//       much time has passed since the last frame.
	// NOTE: Consider using fixed step updates instead.

	return true;
}


bool ApplicationClass::RenderGraphics(){
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	int i;
	int* terrain;
	float cameraX, cameraY, cameraZ;
	int highlightX, highlightY;
	int tileIndex;
	std::list<AgentClass*>::iterator inactiveAgent;
	std::list<ActiveAgentClass*>::iterator activeAgent;
	int agentX, agentY;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Get the offset for any static UI elements
	m_Position->GetPosition(cameraX, cameraY, cameraZ);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// NOTE: Most of what is rendered - particularly UI elements should probably scale/adjust to the screen resolution
	//       This should probably be done once most of the UI elements are implemented (to avoid continual restructuring)
	//       however some initial work should be done before the UI gets complicated so there is something to build off of.

	// Render the Background first
	result = m_MainBackground->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result){
		return false;
	}

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_MainBackground->GetIndexCount(), worldMatrix, m_UIViewMatrix, orthoMatrix, m_MainBackground->GetTexture(), PSTYPE_NORMAL);
	if (!result){
		return false;
	}

	// Render based on the MainState
	// NOTE: Will probably nest additional switch/if statements for substates (submenus/different display modes etc) later on
	switch (m_MainState){
	case MAINSTATE_MAINMENU:
		// Render the Main Menu buttons
		
		// First ensure the buttons have the proper dimensions
		result = m_StandardButton->SetDimensions(MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT);
		if (!result){
			return false;
		}

		// Render buttons based on the MenuState
		switch (m_MenuState){
		case MENUSTATE_MAINMENU:
			// Render all the buttons on the Main Menu
			for (i = 0; i < MAIN_MENU_BUTTON_COUNT; i++){
				// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
				result = m_StandardButton->Render(m_D3D->GetDeviceContext(), MAIN_MENU_BUTTON_HORIZONTAL_OFFSET, MAIN_MENU_BUTTON_VERTICAL_OFFSET + i * (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING));
				if (!result){
					return false;
				}

				// Render the bitmap with the texture shader.
				result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_StandardButton->GetIndexCount(), worldMatrix, m_UIViewMatrix, orthoMatrix, m_StandardButton->GetTexture(), PSTYPE_NORMAL);
				if (!result){
					return false;
				}
			}

			break;

		case MENUSTATE_OPTIONMENU:
			// Render all the buttons on the Options Menu
			for (i = 0; i < OPTIONS_MENU_BUTTON_COUNT; i++){
				result = m_StandardButton->Render(m_D3D->GetDeviceContext(), MAIN_MENU_BUTTON_HORIZONTAL_OFFSET, MAIN_MENU_BUTTON_VERTICAL_OFFSET + i * (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING));
				if (!result){
					return false;
				}

				// Render the bitmap with the texture shader.
				result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_StandardButton->GetIndexCount(), worldMatrix, m_UIViewMatrix, orthoMatrix, m_StandardButton->GetTexture(), PSTYPE_NORMAL);
				if (!result){
					return false;
				}
			}
			
			break;

		default:
			// No Menu, don't render any buttons
			break;
		}

		break;

	case MAINSTATE_COMBATMAP:
		// Render the Combat Map and associated UI/appropriate overlays

		// Render the terrain first - create an array
		terrain = new int[m_combatMapWidth*m_combatMapHeight];
		if (!terrain){
			return false;
		}

		result = m_CombatMap->GetTerrainArray(terrain);
		if (!result){
			return false;
		}

		result = m_TerrainMap->Render(m_D3D->GetDeviceContext(), 0, 0, terrain);
		if (!result){
			return false;
		}

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_TerrainMap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_TerrainMap->GetTexture(), PSTYPE_NORMAL);
		if (!result){
			return false;
		}

		// Turn on alpha blending while rendering tile highlights and Agent sprites
		m_D3D->TurnOnAlphaBlending();

		// If the cursor is currently over a tile on the map render highlights over map tiles
		if (m_cursorOverTile){
			// Render highlights over tiles depending on the currently selected command
			switch (m_SelectedCommand){
			case COMMAND_MOVE:
				// When moving, highlight the path that the selected Agent will take from their current
				// location to the tile under the cursor and display the cost of the move.
				tileIndex = m_currentTileIndex;

				// TODO: Display the cost of moving the Agent to the highlighted tile
				// TODO2: If the Agent can not move to the highlighted tile, highlight the tile
				//        with a different colour and do not display a cost.
				
				while (m_MovementMap[tileIndex].prev){
					// From the grid coordinates, calculate the absolute pixel coordinates to render the highlight to
					highlightX = (int)(1.5f * HEX_SIZE * m_MovementMap[tileIndex].tileX);
					highlightY = (int)(HEX_HEIGHT * (m_MovementMap[tileIndex].tileY + 0.5 * abs(m_MovementMap[tileIndex].tileX % 2)));

					// Render the highlight overtop of whatever hex the cursor is over
					result = m_HexHighlight->Render(m_D3D->GetDeviceContext(), highlightX, highlightY, terrain);
					if (!result){
						return false;
					}

					result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_HexHighlight->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_HexHighlight->GetTexture(), PSTYPE_LOWALPHA);
					if (!result){
						return false;
					}

					tileIndex = m_MovementMap[tileIndex].prev->tileX * m_combatMapHeight + m_MovementMap[tileIndex].prev->tileY;
				}

				break;

			default:
				// By default just highlight the tile under the cursor
				// From the grid coordinates, calculate the absolute pixel coordinates to render the highlight to
				highlightX = (int)(1.5f * HEX_SIZE * m_currentTileX);
				highlightY = (int)(HEX_HEIGHT * (m_currentTileY + 0.5 * abs(m_currentTileX % 2)));

				// Render the highlight overtop of whatever hex the cursor is over
				result = m_HexHighlight->Render(m_D3D->GetDeviceContext(), highlightX, highlightY, terrain);
				if (!result){
					return false;
				}

				result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_HexHighlight->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_HexHighlight->GetTexture(), PSTYPE_LOWALPHA);
				if (!result){
					return false;
				}

				break;
			}
		}

		// Render Agent Sprites
		for (inactiveAgent = m_InactiveAgents.begin(); inactiveAgent != m_InactiveAgents.end(); ++inactiveAgent){
			// Get the position of the agent
			(*inactiveAgent)->GetPosition(agentX, agentY);

			// Calculate the pixel coordinates to render the agent to
			agentY = (int)(MAP_VERTICALOFFSET + HEX_HEIGHT*((float)agentY + 0.5f * fmod((float)agentX, 2.0f)));
			agentX = (int)(MAP_HORIZONTALOFFSET + (HEX_SIZE / 2.0f) + (1.5f * HEX_SIZE * agentX));

			// Render the agent using its SpriteID
			result = m_AgentSprites->Render(m_D3D->GetDeviceContext(), agentX, agentY, (*inactiveAgent)->GetSpriteID());
			if (!result){
				return false;
			}

			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_AgentSprites->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_AgentSprites->GetTexture(), PSTYPE_SPRITE);
			if (!result){
				return false;
			}
		}

		for (activeAgent = m_ActiveAgents.begin(); activeAgent != m_ActiveAgents.end(); ++activeAgent){
			// Get the position of the agent
			(*activeAgent)->GetPosition(agentX, agentY);
			
			// Calculate the pixel coordinates to render the agent to
			agentY = (int)(MAP_VERTICALOFFSET + HEX_HEIGHT*((float)agentY + 0.5f * fmod((float)agentX, 2.0f)));
			agentX = (int)(MAP_HORIZONTALOFFSET + (HEX_SIZE / 2.0f) + (1.5f * HEX_SIZE * agentX));

			// Render the agent using its SpriteID
			result = m_AgentSprites->Render(m_D3D->GetDeviceContext(), agentX, agentY, (*activeAgent)->GetSpriteID());
			if (!result){
				return false;
			}

			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_AgentSprites->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_AgentSprites->GetTexture(), PSTYPE_SPRITE);
			if (!result){
				return false;
			}
		}

		// Turn off alpha blending after rendering the sprites.
		m_D3D->TurnOffAlphaBlending();

		// Render relevant UI elements (overlays, menus, etc.) that should currently be visible.
		// NOTE: The elements involved are subject to change in the future

		// Render the CombatMap menubar
		// First ensure the menuBackground has the proper dimensions
		result = m_MenuBackground->SetDimensions(m_screenWidth, COMBAT_MENU_HEIGHT);
		if (!result){
			return false;
		}

		result = m_MenuBackground->Render(m_D3D->GetDeviceContext(), 0, m_screenHeight - COMBAT_MENU_HEIGHT);
		if (!result){
			return false;
		}

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_MenuBackground->GetIndexCount(), worldMatrix, m_UIViewMatrix, orthoMatrix, m_MenuBackground->GetTexture(), PSTYPE_NORMAL);
		if (!result){
			return false;
		}

		// Render the buttons on the menubar
		
		// First ensure the buttons have the proper dimensions
		result = m_StandardButton->SetDimensions(COMBAT_MENU_BUTTON_WIDTH, COMBAT_MENU_BUTTON_HEIGHT);
		if (!result){
			return false;
		}

		for (i = 0; i < COMBAT_MENU_BUTTON_COUNT; i++){
			result = m_StandardButton->Render(m_D3D->GetDeviceContext(), m_screenWidth + COMBAT_MENU_BUTTON_HORIZONTAL_OFFSET + COMBAT_MENU_BUTTON_WIDTH * (i / 2), m_screenHeight - COMBAT_MENU_HEIGHT + COMBAT_MENU_BUTTON_VERTICAL_OFFSET + COMBAT_MENU_BUTTON_HEIGHT * (i % 2));
			if (!result){
				return false;
			}

			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_StandardButton->GetIndexCount(), worldMatrix, m_UIViewMatrix, orthoMatrix, m_StandardButton->GetTexture(), PSTYPE_NORMAL);
			if (!result){
				return false;
			}
		}

		// Release the array created for terrain rendering
		if (terrain){
			delete terrain;
			terrain = 0;
		}

		break;
	}

	// The Font Engine also renders in 2D

	// Turn on the alpha blending before rendering the text and cursor.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	// If a tooltip should be displayed, render it
	if (m_displayTooltip){
		// Alpha blending is not required for rendering the tooltip background
		m_D3D->TurnOffAlphaBlending();

		// Set the dimensions of the MenuBackground bitmap to render the tooltip background
		result = m_MenuBackground->SetDimensions(m_tooltipWidth, m_tooltipHeight);
		if (!result){
			return false;
		}

		// Render the tooltip background
		result = m_MenuBackground->Render(m_D3D->GetDeviceContext(), m_tooltipX, m_tooltipY);
		if (!result){
			return false;
		}

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_MenuBackground->GetIndexCount(), worldMatrix, m_UIViewMatrix, orthoMatrix, m_MenuBackground->GetTexture(), PSTYPE_NORMAL);
		if (!result){
			return false;
		}

		// Render the tooltip text
		m_D3D->TurnOnAlphaBlending();
		result = m_Text->RenderTooltip(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
		if (!result){
			return false;
		}
	}

	// Render the cursor
	result = m_Mouse->Render(m_D3D->GetDeviceContext(), m_mouseX, m_mouseY);
	if (!result){
		return false;
	}

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Mouse->GetIndexCount(), worldMatrix, m_UIViewMatrix, orthoMatrix, m_Mouse->GetTexture(), PSTYPE_SPRITE);
	if (!result){
		return false;
	}

	// Turn off alpha blending after rendering the text and cursor
	m_D3D->TurnOffAlphaBlending();
	
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}
