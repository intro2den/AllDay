////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"

// Object pointers initialize to NULL (Important in event of initialization failure)
ApplicationClass::ApplicationClass(){
	m_Input = 0;
	m_D3D = 0;
	m_Camera = 0;

	m_MainBackground = 0;
	m_StandardButton = 0;
	m_StandardSlider = 0;
	m_StandardSliderBackground = 0;
	m_MenuBackground = 0;
	m_Mouse = 0;
	m_TextureShader = 0;

	m_StandardFont = 0;
	m_FontShader = 0;
	m_Text = 0;

	m_MainMenu = 0;
	m_OptionsMenu = 0;
	m_GameOptionsMenu = 0;
	m_CombatMenubar = 0;
	m_CombatMainMenu = 0;

	m_Timer = 0;
	m_Position = 0;

	m_CombatMap = 0;
	m_TerrainMap = 0;
	m_HexHighlight = 0;
	m_AgentSprites = 0;

	// Initialize variables that do not depend on input from the config file
	m_MainState = MAINSTATE_MAINMENU;

	m_mouseX = 0;
	m_mouseY = 0;

	m_currentTileX = -1;
	m_currentTileY = -1;
	m_currentTileIndex = -1;
	m_cursorOverTile = false;

	m_SelectedAgent = 0;
	m_CommandSelected = false;
	m_SelectedCommand = COMMAND_DEFAULT;

	m_displayTooltip = false;
	m_cursorIdleTime = 0.0f;
	m_tooltipX = 0;
	m_tooltipY = 0;
	m_tooltipWidth = 200;
	m_tooltipHeight = 48;
}

// Empty copy constructor and empty class deconstructor
// It is possible these would be made otherwise, and are not necessarily called, can cause memory leaks if relied upon
ApplicationClass::ApplicationClass(const ApplicationClass& other){
}

ApplicationClass::~ApplicationClass(){
}

bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, bool fullscreen){
	bool result;
	float cameraX, cameraY, cameraZ;
	D3DXMATRIX baseViewMatrix;

	// Keep track of the screen width and height for bounding the camera, dynamic bitmap initialization and/or scaling(?)
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_fullscreen = fullscreen;

	// Read the config file to load any other saved settings
	ReadConfig();

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
	result = m_D3D->Initialize(m_screenWidth, m_screenHeight, VSYNC_ENABLED, hwnd, m_fullscreen, SCREEN_DEPTH, SCREEN_NEAR);
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
		MessageBox(hwnd, "Could not initialize the background bitmap object.", "Error", MB_OK);
		return false;
	}

	// Initialize the StandardButton bitmap object
	m_StandardButton = new BitmapClass;
	if (!m_StandardButton){
		return false;
	}

	result = m_StandardButton->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/ui_button.png", STANDARDBUTTON_WIDTH, STANDARDBUTTON_HEIGHT);
	if (!result){
		MessageBox(hwnd, "Could not initialize the standard button bitmap object.", "Error", MB_OK);
		return false;
	}

	// Initialize the StandardSlider bitmap object
	m_StandardSlider = new BitmapClass;
	if (!m_StandardSlider){
		return false;
	}

	result = m_StandardSlider->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/ui_slider.png", SLIDER_WIDTH, SLIDER_HEIGHT);
	if (!result){
		MessageBox(hwnd, "Could not initialize the standard slider bitmap object.", "Error", MB_OK);
		return false;
	}

	// Initialize the StandardSliderBackground bitmap object
	m_StandardSliderBackground = new BitmapClass;
	if (!m_StandardSliderBackground){
		return false;
	}

	result = m_StandardSliderBackground->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/highlight_black.png", SLIDER_BAR_WIDTH, SLIDER_BAR_HEIGHT);
	if (!result){
		MessageBox(hwnd, "Could not initialize the standard slider background bitmap object.", "Error", MB_OK);
		return false;
	}

	// Initialize the MenuBackground bitmap object
	m_MenuBackground = new BitmapClass();
	if (!m_MenuBackground){
		return false;
	}

	result = m_MenuBackground->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/ui_menubarbackground.png", m_screenWidth, COMBAT_MENUBAR_HEIGHT);
	if (!result){
		MessageBox(hwnd, "Could not initialize the menu background bitmap object.", "Error", MB_OK);
		return false;
	}

	// Initialize the cursor bitmap object
	m_Mouse = new BitmapClass;
	if (!m_Mouse){
		return false;
	}

	result = m_Mouse->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/cursor.png", 32, 32);
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

	// Create and initialize the standard font and font shader objects
	m_StandardFont = new FontClass;
	if (!m_StandardFont){
		return false;
	}

	result = m_StandardFont->Initialize(m_D3D->GetDevice(), "../rastertekTutorials/data/fontdata.txt", "../rastertekTutorials/data/font.png");
	if (!result){
		MessageBox(hwnd, "Could not initialize the standard font object.", "Error", MB_OK);
		return false;
	}

	m_FontShader = new FontShaderClass;
	if (!m_FontShader){
		return false;
	}

	result = m_FontShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result){
		MessageBox(hwnd, "Could not initialize the font shader object.", "Error", MB_OK);
		return false;
	}

	// Create and Initialize the text object.
	m_Text = new TextClass;
	if (!m_Text){
		return false;
	}

	result = m_Text->Initialize(m_StandardFont, m_FontShader, m_D3D->GetDevice(), m_D3D->GetDeviceContext(), m_screenWidth, m_screenHeight, baseViewMatrix);
	if (!result){
		MessageBox(hwnd, "Could not initialize the text object.", "Error", MB_OK);
		return false;
	}

	// Create and initialize the menu objects
	result = InitializeMenus();
	if (!result){
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

	// Release the Combat Main Menu object
	if (m_CombatMainMenu){
		m_CombatMainMenu->Shutdown();
		delete m_CombatMainMenu;
		m_CombatMainMenu = 0;
	}

	// Release the Combat Menubar object
	if (m_CombatMenubar){
		m_CombatMenubar->Shutdown();
		delete m_CombatMenubar;
		m_CombatMenubar = 0;
	}

	// Release the Game Options Menu object
	if (m_GameOptionsMenu){
		m_GameOptionsMenu->Shutdown();
		delete m_GameOptionsMenu;
		m_GameOptionsMenu = 0;
	}

	// Release the Options Menu object
	if (m_OptionsMenu){
		m_OptionsMenu->Shutdown();
		delete m_OptionsMenu;
		m_OptionsMenu = 0;
	}

	// Release the Main Menu object
	if (m_MainMenu){
		m_MainMenu->Shutdown();
		delete m_MainMenu;
		m_MainMenu = 0;
	}

	// Release the text object.
	if (m_Text){
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the font shader object
	if (m_FontShader){
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the standard font object
	if (m_StandardFont){
		m_StandardFont->Shutdown();
		delete m_StandardFont;
		m_StandardFont = 0;
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

	// Release the MenuBackground Bitmap object
	if (m_MenuBackground){
		m_MenuBackground->Shutdown();
		delete m_MenuBackground;
		m_MenuBackground = 0;
	}

	// Release the StandardSliderBackground bitmap object
	if (m_StandardSliderBackground){
		m_StandardSliderBackground->Shutdown();
		delete m_StandardSliderBackground;
		m_StandardSliderBackground = 0;
	}

	// Release the StandardSlider bitmap object
	if (m_StandardSlider){
		m_StandardSlider->Shutdown();
		delete m_StandardSlider;
		m_StandardSlider = 0;
	}

	// Release the StandardButton bitmap object
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
// NOTE: Some parsing of the configuration file is done in the SystemClass
//       before the ApplicationClass is instantiated. As such, not all of the
//       parsed data is valid in this function. If possible, all configuration
//       data should be parsed in the SystemClass and passed into the
//       instantiation of the ApplicationClass.
void ApplicationClass::ReadConfig(){
	ifstream fin;
	char configString[20];

	// Set default values for configurable variables
	m_tooltipDelay = 500.0f;
	m_tooltipSlider = m_tooltipDelay / MAX_TOOLTIPDELAY;

	// Proof of Concept for reading Configuration File
	fin.open("../rastertekTutorials/data/configuration.txt");
	if (fin.fail()){
		// If the configuration file can't be opened return
		return;
	}

	// Read through the Configuration File and set relevant variables accordingly
	fin.getline(configString, 20, ' ');

	while (!fin.eof()){
		if (strncmp(configString, "tooltipdelay", 12) == 0){
			fin.getline(configString, 20, '\n');
			m_tooltipDelay = min(float(atoi(configString)), MAX_TOOLTIPDELAY);
			m_tooltipSlider = m_tooltipDelay / MAX_TOOLTIPDELAY;
			fin.getline(configString, 20, ' ');
			continue;
		}

		fin.getline(configString, 20, '\n');
		fin.getline(configString, 20, ' ');
	}

	// Close the configuration file.
	fin.close();
	return;
}

void ApplicationClass::WriteConfig(){
	// Write current configuration settings to a file to save them
	ofstream fout;

	fout.open("../rastertekTutorials/data/configuration.txt");
	if (fout.fail()){
		// If a configuration file can't be opened return
		return;
	}

	// Store screen resolution
	fout << "screenwidth " << m_screenWidth << '\n';
	fout << "screenheight " << m_screenHeight << '\n';

	// Store fullscreen setting
	fout << "fullscreen ";
	if (m_fullscreen) fout << "true";
	else fout << "false";
	fout << '\n';
	fout << "tooltipdelay " << m_tooltipDelay << '\n';

	// Close the configuration file.
	fout.close();
	return;
}

bool ApplicationClass::InitializeMenus(){
	// Initialize all menu objects
	bool result;

	// Create the Main Menu and add it to the list of current menus
	m_MainMenu = new MainMenuClass;
	if (!m_MainMenu){
		return false;
	}

	result = m_MainMenu->Initialize(m_screenWidth, m_screenHeight, m_MenuBackground, m_StandardButton, m_TextureShader, m_StandardFont, m_FontShader, m_UIViewMatrix, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!result){
		return false;
	}

	m_CurrentMenus.push_front(m_MainMenu);

	// Create the Options Menu
	m_OptionsMenu = new OptionsMenuClass;
	if (!m_OptionsMenu){
		return false;
	}

	result = m_OptionsMenu->Initialize(m_screenWidth, m_screenHeight, m_MenuBackground, m_StandardButton, m_TextureShader, m_StandardFont, m_FontShader, m_UIViewMatrix, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!result){
		return false;
	}

	// Create the Game Options Menu
	m_GameOptionsMenu = new GameOptionsMenuClass;
	if (!m_GameOptionsMenu){
		return false;
	}

	result = m_GameOptionsMenu->Initialize(m_screenWidth, m_screenHeight, m_MenuBackground, m_StandardButton, m_StandardSlider, m_StandardSliderBackground, m_TextureShader, m_StandardFont, m_FontShader, m_UIViewMatrix, m_D3D->GetDevice(), m_D3D->GetDeviceContext(), m_tooltipDelay, &m_tooltipSlider);
	if (!result){
		return false;
	}

	// Create the Combat Menubar
	m_CombatMenubar = new CombatMenubarClass;
	if (!m_CombatMenubar){
		return false;
	}

	result = m_CombatMenubar->Initialize(m_screenWidth, m_screenHeight, m_MenuBackground, m_StandardButton, m_TextureShader, m_StandardFont, m_FontShader, m_UIViewMatrix, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!result){
		return false;
	}

	// Create the Combat Main Menu
	m_CombatMainMenu = new CombatMainMenuClass;
	if (!m_CombatMainMenu){
		return false;
	}

	result = m_CombatMainMenu->Initialize(m_screenWidth, m_screenHeight, m_MenuBackground, m_StandardButton, m_TextureShader, m_StandardFont, m_FontShader, m_UIViewMatrix, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!result){
		return false;
	}

	return true;
}

bool ApplicationClass::EnterMainMenu(){
	// Shutdown the CombatMap if it is open, close all open menus and open the
	// Main Menu
	bool result;

	// Deselect any selected Commands and Agents
	DeselectCommand();
	result = SetSelectedAgent(NULL);
	if (!result){
		return false;
	}

	// Set the cursor over tile flag to false
	m_cursorOverTile = false;

	// Set the position of the camera back to the origin
	m_Position->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Exit the CombatMap and Enter the Main Menu
	ShutdownCombatMap();
	m_MainState = MAINSTATE_MAINMENU;
	m_CurrentMenus.clear();
	m_CurrentMenus.push_front(m_MainMenu);
	result = StateChanged();
	if (!result){
		return false;
	}


	return true;
}

// Handle all user input
bool ApplicationClass::HandleInput(float frameTime){
	bool result;
	bool cursorInBounds, scrolling;
	float posX, posY, posZ;
	float cursorX, cursorY, normalizedCursorX, normalizedCursorY;
	std::list<MenuClass*>::iterator menu;
	ButtonAction buttonAction;
	bool cursorOverMenu;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Update the frame time for any existing error messages
	// NOTE: This must happen before new errors are created in this frame - not in the Update function.
	result = m_Text->Frame(frameTime, m_D3D->GetDeviceContext());
	if (!result){
		return false;
	}

	// Determine whether the cursor is within window bounds or not
	cursorInBounds = (m_mouseX >= 0 && m_mouseX < m_screenWidth && m_mouseY >= 0 && m_mouseY < m_screenHeight);

	// Determine whether the cursor is over (or locked to an exclusive) a menu
	cursorOverMenu = false;
	menu = m_CurrentMenus.begin();
	while (menu != m_CurrentMenus.end()){
		if ((*menu)->HitCheck(m_mouseX, m_mouseY) || (*menu)->GetExclusive()){
			cursorOverMenu = true;
			break;
		}

		++menu;
	}

	// By default, buttonAction is set to NOBUTTON
	buttonAction = BUTTON_NOBUTTON;

	switch (m_MainState){
	case MAINSTATE_MAINMENU:
		// Main Menu Processing
		// Process mouse input, left mouse button
		if (m_Input->WasLeftMouseClicked() == true){
			// Check if a button was clicked on one of the current menus
			menu = m_CurrentMenus.begin();
			while (menu != m_CurrentMenus.end()){
				if ((*menu)->HitCheck(m_mouseX, m_mouseY)){
					buttonAction = (*menu)->LeftMouseClick(m_mouseX, m_mouseY);
					break;
				}

				// If this menu is exclusive don't check any menus below it
				if ((*menu)->GetExclusive()) break;

				// If this menu closes when the mouse is clicked somewhere else
				// close the menu and check the next menu below
				if ((*menu)->GetCloseOnMiss()){
					menu = m_CurrentMenus.erase(menu);
					result = StateChanged();
					if (!result){
						return false;
					}

					continue;
				}

				// Check the next open menu
				++menu;
			}

			switch (buttonAction){
			case BUTTON_NOBUTTON:
				// No button was clicked, do nothing
				break;

			case BUTTON_CLOSEMENU:
				// Close the menu the close button was clicked from
				m_CurrentMenus.erase(menu);
				result = StateChanged();
				if (!result){
					return false;
				}

				break;

			case BUTTON_EXITAPPLICATION:
				return false;

			case BUTTON_ENTERCOMBAT:
				result = EnterCombatMap();
				if (!result){
					return false;
				}

				break;

			case BUTTON_OPENOPTIONSMENU:
				m_CurrentMenus.push_front(m_OptionsMenu);
				result = StateChanged();
				if (!result){
					return false;
				}

				break;

			case BUTTON_OPENGAMEOPTIONS:
				m_CurrentMenus.push_front(m_GameOptionsMenu);
				result = StateChanged();
				if (!result){
					return false;
				}

				break;

			case BUTTON_SAVESETTINGS:
				// Save current settings to a configuration file and close the menu
				WriteConfig();

				m_CurrentMenus.erase(menu);
				result = StateChanged();
				if (!result){
					return false;
				}

				break;

			default:
				// Do nothing if an unexpected action is specified
				break;
			}
		}

		if (m_Input->IsLeftMousePressed()){
			// If the cursor is over one of the current, interactive menus
			// while the left mouse button is pressed down, adjust any sliders
			// under the cursor that are part of that menu
			menu = m_CurrentMenus.begin();
			while (menu != m_CurrentMenus.end()){
				if ((*menu)->HitCheck(m_mouseX, m_mouseY)){
					result = (*menu)->LeftMousePressed(m_mouseX, m_mouseY);

					// If a slider was adjusted, update slider variables
					if (result){
						result = UpdateSliderVariables();
						if (!result){
							return false;
						}
					}

					break;
				}

				// If this menu is exclusive don't check any menus below it
				if ((*menu)->GetExclusive()) break;

				// Check the next open menu
				++menu;
			}
		}

		break;

	case MAINSTATE_COMBATMAP:
		// Combat Map Processing

		// If no menu is open, check for camera movement
		if (m_CurrentMenus.size() == 1){
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
		}

		// Find the coordinates of the hex that the cursor is overtop of this frame - if the cursor is over the map
		m_currentTileX = -1;
		m_currentTileY = -1;
		m_currentTileIndex = -1;
		m_cursorOverTile = false;

		if (cursorInBounds && !cursorOverMenu){
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

			// Check if a button was clicked on one of the current menus
			menu = m_CurrentMenus.begin();
			while (menu != m_CurrentMenus.end()){
				if ((*menu)->HitCheck(m_mouseX, m_mouseY)){
					buttonAction = (*menu)->LeftMouseClick(m_mouseX, m_mouseY);
					break;
				}

				// If this menu is exclusive don't check any menus below it
				if ((*menu)->GetExclusive()) break;

				// If this menu closes when the mouse is clicked somewhere else
				// close the menu and check the next menu below
				if ((*menu)->GetCloseOnMiss()){
					menu = m_CurrentMenus.erase(menu);
					result = StateChanged();
					if (!result){
						return false;
					}

					continue;
				}

				// Check the next open menu
				++menu;
			}

			switch (buttonAction){
			case BUTTON_NOBUTTON:
				// No button was clicked, do nothing
				break;

			case BUTTON_CLOSEMENU:
				// Close the menu the close button was clicked from
				m_CurrentMenus.erase(menu);
				result = StateChanged();
				if (!result){
					return false;
				}

				break;

			case BUTTON_EXITAPPLICATION:
				return false;

			case BUTTON_ORDERMOVE:
				// Select the Move Command
				m_CommandSelected = true;
				m_SelectedCommand = COMMAND_MOVE;
				break;

			case BUTTON_ORDERATTACK:
				m_CommandSelected = true;
				m_SelectedCommand = COMMAND_ATTACK;
				break;

			case BUTTON_ENDTURN:
				// End Turn
				DeselectCommand();
				EndTurn();
				break;

			case BUTTON_OPENCOMBATMENU:
				DeselectCommand();
				m_CurrentMenus.push_front(m_CombatMainMenu);
				result = StateChanged();
				if (!result){
					return false;
				}

				break;

			case BUTTON_EXITCOMBAT:
				// Shutdown the CombatMap and return to the Main Menu
				result = EnterMainMenu();
				if (!result){
					return false;
				}

				break;

			default:
				// Do nothing if an unexpected action is specified
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
					DeselectCommand();

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
					DeselectCommand();

					break;
				}

				break;
			}
		}

		if (m_Input->WasRightMouseClicked() == true){
			// Right Click - Cancel Command Selection, Default mouse behaviour
			if (m_CommandSelected){
				// If a command is selected, deselect it
				DeselectCommand();
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

bool ApplicationClass::UpdateSliderVariables(){
	// Round all slider variables to the nearest step for their corresponding
	// controlled variable and update the controlled variable to the appropriate
	// value
	// NOTE: In some cases we may want to wait until the user confirms their
	//       changes before adjusting the actual variable
	bool result;

	// Update the tooltip delay slider variable and the tooltip delay
	m_tooltipSlider = round(m_tooltipSlider * TOOLTIPDELAY_SLIDER_STEPS) / TOOLTIPDELAY_SLIDER_STEPS;
	m_tooltipDelay = m_tooltipSlider * MAX_TOOLTIPDELAY;
	result = m_GameOptionsMenu->UpdateTooltipDelayLabel(m_tooltipDelay, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!result){
		return false;
	}

	return true;
}

bool ApplicationClass::StateChanged(){
	// Set stateChanged to true, reset tooltips and clear error messages
	bool result;

	ResetTooltip();

	// Clear all error messages on state change
	result = m_Text->ClearErrors(m_D3D->GetDeviceContext());
	if (!result){
		return false;
	}

	return true;
}

void ApplicationClass::DeselectCommand(){
	// Reset the selected command and associated flag
	m_CommandSelected = false;
	m_SelectedCommand = COMMAND_DEFAULT;
	return;
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
	// Process up to processLimit pathnodes in the MovementQueue and update the
	// MovementMap. Nodes are processed in batches of equal cost nodes. If
	// processing all minimum cost nodes exceeds the processLimit, all nodes in
	// the batch will be processed before returning.
	int minCost;
	std::list<Pathnode*>::iterator pathnode;
	int currX, currY;
	int currIndex, neighbourIndex;
	int nodesProcessed = 0;

	// Process Loop - Find the minimum cost of all pathnodes in the
	// MovementQueue then process all pathnodes with the minimum cost.
	while (!m_MovementQueue.empty() && nodesProcessed < processLimit){
		// Initialize minCost to -1
		minCost = -1;

		// Find the minimum cost of all pathnodes in the MovementQueue
		for (pathnode = m_MovementQueue.begin(); pathnode != m_MovementQueue.end(); ++pathnode){
			if ((*pathnode)->cost < minCost || (minCost = -1 && (*pathnode)->cost >= 0)) minCost = (*pathnode)->cost;
		}

		// Process all minimum cost pathnodes in the queue.
		pathnode = m_MovementQueue.begin();
		while (pathnode != m_MovementQueue.end()){
			if ((*pathnode)->cost == minCost){
				// Process this node

				// The path found to the current pathnode is guaranteed to be the
				// shortest path from the current Agent's location.
				(*pathnode)->optimal = true;

				// For the current pathnode, find and update any adjacent pathnodes
				// that have not yet been visited.
				currX = (*pathnode)->tileX;
				currY = (*pathnode)->tileY;
				currIndex = currX * m_combatMapHeight + currY;

				// Visit neighbours of the current tile
				// NOTE: The coordinates of some adjacent pathnodes/tiles are dependant on
				//       the x-coordinate of the current tile

				// Check if there is a tile above the current tile
				if (currY > 0){
					// There is a tile above the current tile
					neighbourIndex = currIndex - 1;
					VisitPathnode(currIndex, neighbourIndex);
				}

				// Check if there is a tile below the current tile
				if (currY < m_combatMapHeight - 1){
					// There is a tile below the current tile
					neighbourIndex = currIndex + 1;
					VisitPathnode(currIndex, neighbourIndex);
				}

				if (currX % 2 == 0){
					// Even Column hex

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

				// Remove the processed node from the Queue and move the iterator to the next node
				pathnode = m_MovementQueue.erase(pathnode);
				++nodesProcessed;
			} else {
				++pathnode;
			}
		}
	}

	return;
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

bool ApplicationClass::EnterCombatMap(){
	// Set the Mainstate to CombatMap, close all open menus and open the Combat Menubar
	bool result;

	m_MainState = MAINSTATE_COMBATMAP;
	m_CurrentMenus.clear();
	m_CurrentMenus.push_front(m_CombatMenubar);
	result = StateChanged();
	if (!result){
		return false;
	}

	// Initialize a new CombatMap and scenario and start combat
	// NOTE: This should be done separately from Entering the CombatMap
	if (!m_CombatMap){
		result = InitializeCombatMap((MapType)(rand() % 2), 32, 32);
		if (!result){
			return false;
		}
	}

	// Begin the first round of Combat
	NextTurn();

	return true;
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

	result = m_TerrainMap->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/combatmap_terrain.png", m_combatMapWidth, m_combatMapHeight);
	if (!result){
		return false;
	}

	// Initialize a HexMap to highlight the tile that the user has the cursor over
	m_HexHighlight = new HexMapClass();
	if (!m_HexHighlight){
		return false;
	}

	result = m_HexHighlight->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/highlight_white.png", 1, 1);
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
	boundY = min(0, -1.0f * HEX_HEIGHT*(1.5f + (float)mapHeight) + (float)m_screenHeight - float(COMBAT_MENUBAR_HEIGHT));
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

	result = m_AgentSprites->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/agentsprites.png", (int)HEX_SIZE, (int)HEX_HEIGHT);
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
		// The cursor moved, reset
		ResetTooltip();
	} else if (cursorIdle && m_cursorIdleTime < m_tooltipDelay){
		m_cursorIdleTime += frameTime;
	}

	// If the cursor has been idle for long enough and a tooltip hasn't been
	// checked for, check if there is a tooltip to display
	if (m_cursorIdleTime >= m_tooltipDelay && !m_checkedForTooltip){
		m_checkedForTooltip = true;

		// Update the tooltip to be displayed
		result = UpdateTooltip();
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

bool ApplicationClass::UpdateTooltip(){
	// Set the tooltip position and dimensions and set the text based on the
	// UI element that is currently under the cursor.
	bool result;
	std::list<MenuClass*>::iterator menu;
	ButtonAction action;

	// NOTE: Tooltips should appear close to the UI element that they apply to
	//       without obscuring it or any related elements.
	//       For example: Tooltips for UI elements on the CombatMap menu bar
	//       should appear immediately above the menu bar, close to the element
	//       being described but not obscuring any part of the menu bar.
	// NOTE2: This is partially proof of concept - all tooltip labels and
	//        descriptions should be stored as constants (or variables set
	//        from data files) or in some cases stored with the object that the
	//        tooltip will describe.
	// NOTE3: This code should be cleaned up, split into helper functions to
	//        reduce duplicate code (same tooltips with different positions).

	// If the cursor is over a menu element with a tooltip, display that tooltip
	// at the location returned by the menu
	action = BUTTON_NOBUTTON;

	menu = m_CurrentMenus.begin();
	while (menu != m_CurrentMenus.end()){
		if ((*menu)->HitCheck(m_mouseX, m_mouseY)){
			result = (*menu)->GetTooltip(m_mouseX, m_mouseY, m_tooltipX, m_tooltipY, m_tooltipWidth, m_tooltipHeight, action);

			// If a tooltip should be displayed set displayTooltip to true
			if (result){
				m_displayTooltip = true;
			}

			break;
		}

		// If this menu is exclusive don't check any menus below it
		if ((*menu)->GetExclusive()) break;

		// Check the next open menu
		++menu;
	}

	switch (action){
	case BUTTON_NOBUTTON:
		// False positive, set displayTooltip to false
		m_displayTooltip = false;
		break;

	case BUTTON_CLOSEMENU:
		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, "Close Menu", "Close this menu", m_tooltipWidth, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}

		break;

	case BUTTON_EXITAPPLICATION:
		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, "Exit Application", "Shutdown the game and return to desktop", m_tooltipWidth, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}

		break;

	case BUTTON_ENTERCOMBAT:
		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, "Enter Combat", "Generates and starts a combat scenario", m_tooltipWidth, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}

		break;

	case BUTTON_OPENOPTIONSMENU:
		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, "Options Menu", "Configure audio, game and graphics settings", m_tooltipWidth, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}

		break;

	case BUTTON_OPENGAMEOPTIONS:
		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, "Game Options", "Configure game settings", m_tooltipWidth, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}

		break;

	case BUTTON_SAVESETTINGS:
		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, "Save and Close", "Save changes to the settings and close this menu", m_tooltipWidth, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}

		break;

	case BUTTON_ORDERMOVE:
		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, "Move", "Command the selected Agent to move to a specific location", m_tooltipWidth, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}

		break;

	case BUTTON_ORDERATTACK:
		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, "Attack", "Command the selected Agent to attack a specific target", m_tooltipWidth, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}

		break;

	case BUTTON_ENDTURN:
		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, "End Turn", "End the turn of all currently active Agents", m_tooltipWidth, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}

		break;

	case BUTTON_OPENCOMBATMENU:
		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, "Menu", "Open the Main Menu", m_tooltipWidth, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}

		break;

	case BUTTON_EXITCOMBAT:
		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, "Exit to Main Menu", "Exit from combat and return to the Main Menu", m_tooltipWidth, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}

		break;

	default:
		// If a tooltip has not yet been implemented display a tooltip saying so
		result = m_Text->SetTooltipText(m_tooltipX, m_tooltipY, "Tooltip Missing", "Tooltip not yet implemented for this button", m_tooltipWidth, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}

		break;
	}

	return true;
}

void ApplicationClass::ResetTooltip(){
	// Reset cursor idle time and set displayTooltip and checkedForTooltip to false
	m_cursorIdleTime = 0.0f;
	m_displayTooltip = false;
	m_checkedForTooltip = false;
	return;
}

bool ApplicationClass::RenderGraphics(){
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;
	int* terrain;
	int highlightX, highlightY;
	int tileIndex;
	std::list<AgentClass*>::iterator inactiveAgent;
	std::list<ActiveAgentClass*>::iterator activeAgent;
	std::list<MenuClass*>::reverse_iterator menu;
	int agentX, agentY;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

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
		// All MainMenu rendering is now handled by the currently open menus
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

		// Release the array created for terrain rendering
		if (terrain){
			delete terrain;
			terrain = 0;
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

		break;
	}

	// Render any text that is above the background/CombatMap but below any open
	// menus
	// Ex. Floating Text on the map
	m_D3D->TurnOnAlphaBlending();

	// Render all open menus
	for (menu = m_CurrentMenus.rbegin(); menu != m_CurrentMenus.rend(); ++menu){
		result = (*menu)->Render(m_D3D->GetDeviceContext(), worldMatrix, m_UIViewMatrix, orthoMatrix);
		if (!result){
			return false;
		}
	}

	// The Font Engine also renders in 2D

	// Render Any text that isn't part of a menu
	// NOTE: There shouldn't be any more text until after the tooltip background is rendered
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	result = m_Text->RenderStaticText(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	// If a tooltip should be displayed, render it
	if (m_displayTooltip){
		// Alpha blending is not required for rendering the tooltip background
		m_D3D->TurnOffAlphaBlending();

		// Set the dimensions of the MenuBackground bitmap to render the tooltip background
		m_MenuBackground->SetDimensions(m_tooltipWidth, m_tooltipHeight);

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

	// Render error text
	result = m_Text->RenderErrorText(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result){
		return false;
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
