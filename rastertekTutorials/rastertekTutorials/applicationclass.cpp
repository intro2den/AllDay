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
	m_MenuBarBackground = 0;
	m_CombatMap = 0;
	m_TerrainMap = 0;
	m_HexHighlight = 0;
	m_TextureShader = 0;
	m_Text = 0;
	m_FontShader = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Agents = 0;
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
	int i;
	
	// Keep track of the screen width and height for bounding the camera, dynamic bitmap initialization and/or scaling(?)
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Set initial MainState
	m_MainState = MAINSTATE_MAINMENU;

	// Initialize the mouse (cursor) position (this will be overwritten in the first frame)
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the coordinates for the currently highlighted tile (for the CombatMap - initially invalid coordinates)
	m_currentTileX = -1;
	m_currentTileY = -1;
	m_cursorOverTile = false;

	// Initialize the index of the currently selected Agent to -1, there are no Agents to be selected on initialization
	m_selectedAgent = -1;

	// Set the current number of Agents to 0
	m_numAgents = 0;

	// Initialize the CombatMap specific arrays: agentOwner agentInitiative, agentBeganTurn, agentEndedTurn
	for (i = 0; i < MAX_AGENTS; i++){
		m_agentOwner[i] = -1;
		m_agentInitiative[i] = -1;
		m_agentBeganTurn[i] = false;
		m_agentEndedTurn[i] = false;
	}

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

	// Get the location of the mouse from the input object,
	m_Input->GetMouseLocation(m_mouseX, m_mouseY);

	// If the mouse is over the window, update the text strings displaying the cursor coordinates
	if (m_mouseX >= 0 && m_mouseX < m_screenWidth && m_mouseY >= 0 && m_mouseY < m_screenHeight){
		result = m_Text->SetMousePosition(m_mouseX, m_mouseY, m_D3D->GetDeviceContext());
		if (!result){
			return false;
		}
	}

	result = HandleInput(m_Timer->GetTime());
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

// Handle all user input
bool ApplicationClass::HandleInput(float frameTime){
	bool result;
	bool cursorInBounds, scrolling;
	float posX, posY, posZ;
	float cursorX, cursorY, normalizedCursorX, normalizedCursorY;
	int buttonClicked;
	bool agentFound;
	int agentX, agentY;
	int agentIndex;
	int i;

	// Initialize buttonClicked to an invalid value
	buttonClicked = -1;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Update the frame time for any existing error messages
	result = m_Text->Frame(frameTime, m_D3D->GetDeviceContext());
	if (!result){
		return false;
	}

	// Determine weather the cursor is within window bounds or not
	cursorInBounds = (m_mouseX >= 0 && m_mouseX < m_screenWidth && m_mouseY >= 0 && m_mouseY < m_screenHeight);

	// NOTE: Will also be using MainState/other state checks for rendering

	switch (m_MainState){
	case MAINSTATE_MAINMENU:
		// Main Menu Processing
		// Process mouse input, left mouse button
		if (m_Input->WasLeftMouseClicked() == true){
			// Calculate the cursor position relative to the menu buttons
			cursorX = (float)(m_mouseX - MAIN_MENU_BUTTON_HORIZONTAL_OFFSET);
			cursorY = (float)(m_mouseY - MAIN_MENU_BUTTON_VERTICAL_OFFSET);

			// Which button was pressed, if any, depends on the current MenuState
			switch (m_MenuState){
			case MENUSTATE_MAINMENU:
				// Check for a button was pressed if the cursor is potentially over a button on the Main Menu
				if (cursorX > 0 && cursorX < MAIN_MENU_BUTTON_WIDTH && cursorY > 0 && cursorY < MAIN_MENU_BUTTON_COUNT * (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING) - MAIN_MENU_BUTTON_SPACING){
					// Cursor is within the bounds of the menu buttons, determine which button (if any) was clicked
					if ((int)cursorY % (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING) < MAIN_MENU_BUTTON_HEIGHT){
						buttonClicked = (int)(cursorY / (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING));
					}

					switch (buttonClicked){
					case MAINMENUBUTTON_ENTERCOMBATMAP:
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

						break;

					case MAINMENUBUTTON_OPTIONS:
						// Change the MenuState to OptionsMenu
						m_MenuState = MENUSTATE_OPTIONMENU;

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
				}
				break;

			case MENUSTATE_OPTIONMENU:
				// Check for a button was pressed if the cursor is potentially over a button on the Options Menu
				if (cursorX > 0 && cursorX < MAIN_MENU_BUTTON_WIDTH && cursorY > 0 && cursorY < OPTIONS_MENU_BUTTON_COUNT * (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING) - MAIN_MENU_BUTTON_SPACING){
					// Cursor is within the bounds of the menu buttons, determine which button (if any) was clicked
					if ((int)cursorY % (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING) < MAIN_MENU_BUTTON_HEIGHT){
						buttonClicked = (int)(cursorY / (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACING));
					}

					switch (buttonClicked){
					case OPTIONSMENUBUTTON_BACK:
						// Change the MenuState to MainMenu
						m_MenuState = MENUSTATE_MAINMENU;

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
		m_cursorOverTile = false;

		if (cursorInBounds && m_mouseY < m_screenHeight - COMBAT_MENU_HEIGHT){
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

			// Set the cursorOverTile flag to true if the cursor is over a hex on the map
			if (m_currentTileX >= 0 && m_currentTileX < m_combatMapWidth && m_currentTileY >= 0 && m_currentTileY < m_combatMapHeight){
				m_cursorOverTile = true;
			}
		}

		// Handle user input (aside from Camera movement) relevant to the CombatMap
		if (m_Input->WasLeftMouseClicked() == true){
			// Left Click - Check for clicking on Menu Buttons, map tiles
			
			// NOTE: Additional state checks may need to be done here to check menus that may be open
			//       Currently only check the CombatMap menu bar

			// Calculate the cursor position relative to the menu bar buttons
			cursorX = (float)(m_mouseX - (m_screenWidth + COMBAT_MENU_BUTTON_HORIZONTAL_OFFSET));
			cursorY = (float)(m_mouseY - (m_screenHeight - COMBAT_MENU_HEIGHT) - COMBAT_MENU_BUTTON_VERTICAL_OFFSET);

			// If the cursor is over a button, determine which one and act accordingly
			if (cursorX > 0 && cursorX < COMBAT_MENU_BUTTON_WIDTH * (COMBAT_MENU_BUTTON_COUNT / 2) && cursorY > 0 && cursorY < COMBAT_MENU_BUTTON_HEIGHT * 2){
				buttonClicked = 2 * (int)((cursorX / COMBAT_MENU_BUTTON_WIDTH)) + (int)((cursorY / COMBAT_MENU_BUTTON_HEIGHT));

				switch (buttonClicked){
				case COMBATMENUBUTTON_ENDTURN:
					// End Turn
					EndTurn();
					break;

				case COMBATMENUBUTTON_MENU:
					// Return to the Main Menu
					// NOTE: This will be replaced by a popup menu with in-game options (including returning to the Main Menu)
					m_MainState = MAINSTATE_MAINMENU;
					m_MenuState = MENUSTATE_MAINMENU;

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

					// Set numAgents to 0
					m_numAgents = 0;

					// Reset the CombatMap specific arrays
					for (i = 0; i < MAX_AGENTS; i++){
						m_agentOwner[i] = -1;
						m_agentInitiative[i] = -1;
						m_agentBeganTurn[i] = false;
						m_agentEndedTurn[i] = false;
					}

					// Deselect any selected Agent and update the associated sentence
					result = SetSelectedAgent(-1);
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
					// Do nothing if no button was pressed (this will only happen with an odd number of buttons)
					break;
				}

				// A button was clicked, no other checks should be made for the input
				break;
			}

			// If the left mouse button was clicked over a tile, different behaviour can occur.
			// NOTE: There will be probably be some state handling here if the player has an action selected
			//       or if they are simply selecting an Agent. Currently we just select the Agent that is in
			//       the highlighted Hex if one exists at those coordinates
			if (m_cursorOverTile){
				// NOTE: This is primarily proof of concept, additional work will need to be done if multiple
				//       agents are present in the hex, and for any other necessary checks related to Agent
				//       selection.
				agentFound = false;

				// Check if an agent is in the hex that was just selected
				for (agentIndex = 0; agentIndex < m_numAgents; agentIndex++){
					m_Agents[agentIndex]->getPosition(agentX, agentY);
					if (agentX == m_currentTileX && agentY == m_currentTileY){
						// An agent is in the selected hex
						agentFound = true;

						// Update the ID of the selected Agent
						result = SetSelectedAgent(agentIndex);
						if (!result){
							return false;
						}

						// For now we only care about the first Agent found, break
						break;
					}
				}

				// If no agent was found in the tile, unselect any selected Agent
				if (!agentFound){
					result = SetSelectedAgent(-1);
					if (!result){
						return false;
					}
				}

				break;
			}
		}

		if (m_Input->WasRightMouseClicked() == true){
			// Move the first agent to whatever hex is currently highlighted
			// NOTE: Should check to ensure the cursor is not overtop of a menu/submenu when considering interaction with the map
			//       Do not interact with a hex if the mouse is clicked and there is the background of a menu between the cursor and
			//       the hex.
			if (m_cursorOverTile && m_selectedAgent >= 0){
				// Only move the selected Agent if it is that Agent's turn, otherwise display an appropriate error message
				if (m_agentBeganTurn[m_selectedAgent] && !m_agentEndedTurn[m_selectedAgent]){
					m_Agents[m_selectedAgent]->setPosition(m_currentTileX, m_currentTileY);
				} else{
					result = m_Text->NewErrorMessage("The selected Agent isn't active.", m_D3D->GetDeviceContext());
					if (!result){
						return false;
					}

				}
			}
		}

		break;
	}

	return true;
}

bool ApplicationClass::SetSelectedAgent(int agentID){
	// Set m_selectedAgent, update the selectedAgent string in the Text object and do all pathfinding for the newly selected
	// Agent.
	// NOTE: Currently not checking if the newly selected Agent is already selected - it may be worth checking.
	bool result;

	m_selectedAgent = agentID;
	result = m_Text->SetSelectedAgent(agentID, m_D3D->GetDeviceContext());
	if (!result){
		return false;
	}

	//////////////////////////////////////////////
	// TODO: Do pathfinding for the selected Agent from its current position on the map to all tiles on the map.
	//       Unreachable tiles should be specially denoted in some way that is easy to check without adding unnessesary
	//       bulk to the data structure used, perhaps a cost of -1 or similar. Create an ENUM/Constant if necessary.
	//       If an Inactive Agent or No Agent is selected, the data structure storing the results should be updated
	//       in such a way as to reflect that no movement is possible to any hex.
	//////////////////////////////////////////////


	return true;
}

void ApplicationClass::NextTurn(){
	// Determine which player, and which of their Agents are able to act for the
	// next turn of Combat.
	int highestInitiative, highestOpposingInitiative;
	int nextActingPlayer;
	int i;

	// Initialize the highest initiative to be lower than all valid initiative values
	highestInitiative = -1;
	highestOpposingInitiative = -1;

	// Initialize the nextActing player to an invalid ID
	nextActingPlayer = -1;

	// Determine which player's Agents will have their turn next and the minimum initiative
	// required for them to have that turn. In the event of a tie, a lower agentOwner value
	// takes precedence.
	for (i = 0; i < m_numAgents; i++){
		if (!m_agentBeganTurn[i] && !m_agentEndedTurn[i] && m_agentOwner[i] >= 0){
			// Check agentOwner to determine tiebreaking behaviour
			if (m_agentOwner[i] < nextActingPlayer){
				// Compare initiative, this agent wins ties
				if (m_agentInitiative[i] >= highestInitiative){
					// New fastest agent, the minimum initiative to act is the previous
					// highestInitiative
					highestOpposingInitiative = highestInitiative;
					highestInitiative = m_agentInitiative[i];
					nextActingPlayer = m_agentOwner[i];
				} else if (m_agentInitiative[i] > highestOpposingInitiative){
					// This agent is the fastest owned by a different player, the minimum
					// initiative to act is this agent's initiative + 1
					highestOpposingInitiative = m_agentInitiative[i] + 1;
				}

			} else if (m_agentOwner[i] == nextActingPlayer && m_agentInitiative[i] > highestInitiative){
				// New fastest agent is owned by the same player - update highestInitiative
				highestInitiative = m_agentInitiative[i];

			} else if (m_agentOwner[i] > nextActingPlayer){
				// Compare initiative, this agent loses ties
				if (m_agentInitiative[i] > highestInitiative){
					// New fastest agent, the minimum initiative to act is the previous
					// highestInitiative + 1
					highestOpposingInitiative = highestInitiative + 1;
					highestInitiative = m_agentInitiative[i];
					nextActingPlayer = m_agentOwner[i];
				} else if (m_agentInitiative[i] > highestOpposingInitiative){
					// This agent is the fastest owned by a different player, the minimum
					// initiative to act is this agent's initiative
					highestOpposingInitiative = m_agentInitiative[i];
				}
			}
		}
	}

	// Begin the turn of all Agents that will be able to act for the next turn of Combat
	for (i = 0; i < m_numAgents; i++){
		// To act, the Agent must not have already acted this round, meet the minimum
		// initiative requirement and be owned by the next acting player.
		if (!m_agentBeganTurn[i] && m_agentInitiative[i] >= highestOpposingInitiative && m_agentOwner[i] == nextActingPlayer){
			m_agentBeganTurn[i] = true;
		}
	}

	return;
}

void ApplicationClass::EndTurn(){
	// End the turn of all Agents which have started but not yet ended their turn
	// If all Agents have ended their turn at this point, begin a new round of turns.
	int i;
	bool endOfRound;

	// Assume the round is ending, if an Agent has not started their turn yet, then
	// the round is not yet ending
	endOfRound = true;

	// End the turn of all Agents which have started but not ended their turn and determine
	// if the round is ending or not.
	for (i = 0; i < m_numAgents; i++){
		if (!m_agentBeganTurn[i] && m_agentInitiative[i] >= 0){
			endOfRound = false;
		}

		if (m_agentBeganTurn[i] && !m_agentEndedTurn[i]){
			// End this agent's turn
			m_agentEndedTurn[i] = true;
		}
	}

	// If the round is over, begin a new round
	// NOTE: May want to move this behaviour into its own function - be able to call it at the start of combat as well
	if (endOfRound){
		for (i = 0; i < m_numAgents; i++){
			m_agentBeganTurn[i] = false;
			m_agentEndedTurn[i] = false;
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

	// Adjust the bounds on the camera position to allow for proper scrolling
	boundX = max(0, HEX_SIZE*(1.5f + 1.5f*(float)mapWidth) - (float)m_screenWidth);
	boundY = min(0, -1.0f * HEX_HEIGHT*(1.5f + (float)mapHeight) + (float)m_screenHeight - float(COMBAT_MENU_HEIGHT));
	boundZ = -10.0f;

	m_Position->SetBounds(boundX, boundY, boundZ);

	// Initialize the CombatMap UI elements
	m_MenuBarBackground = new BitmapClass();
	if (!m_MenuBarBackground){
		return false;
	}

	result = m_MenuBarBackground->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/ui_menubarbackground.dds", m_screenWidth, COMBAT_MENU_HEIGHT);
	if (!result){
		return false;
	}

	// Initialize Agents
	// NOTE: This process will be subject to considerable change, currently used for Proof of Concept
	// NOTE2: Currently sequentially initializing Agents in an array based on the current number of Agents
	//        May reimplement Agents using a list (or two).
	m_Agents = new AgentClass*[MAX_AGENTS];

	// Initialize an Agent
	m_Agents[m_numAgents] = new AgentClass();
	if (!m_Agents[m_numAgents]){
		return false;
	}

	result = m_Agents[m_numAgents]->Initialize(AGENTTYPE_ACTIVE1, 0, 0);
	if (!result){
		return false;
	}

	m_numAgents++;

	// Initialize a 2nd Agent
	m_Agents[m_numAgents] = new AgentClass();
	if (!m_Agents[m_numAgents]){
		return false;
	}

	result = m_Agents[m_numAgents]->Initialize(AGENTTYPE_ACTIVE2, 2, 0);
	if (!result){
		return false;
	}

	m_numAgents++;

	// Initialize a 3rd Agent
	m_Agents[m_numAgents] = new AgentClass();
	if (!m_Agents[m_numAgents]){
		return false;
	}

	result = m_Agents[m_numAgents]->Initialize(AGENTTYPE_INACTIVE1, 1, 16);
	if (!result){
		return false;
	}

	m_numAgents++;

	// Initialize a 4th Agent
	m_Agents[m_numAgents] = new AgentClass();
	if (!m_Agents[m_numAgents]){
		return false;
	}

	result = m_Agents[m_numAgents]->Initialize(AGENTTYPE_INACTIVE2, 3, 5);
	if (!result){
		return false;
	}

	m_numAgents++;

	// Initialize a 5th Agent
	m_Agents[m_numAgents] = new AgentClass();
	if (!m_Agents[m_numAgents]){
		return false;
	}

	result = m_Agents[m_numAgents]->Initialize(AGENTTYPE_ACTIVE2, 5, 7);
	if (!result){
		return false;
	}

	m_numAgents++;

	// Initialize a 6th Agent
	m_Agents[m_numAgents] = new AgentClass();
	if (!m_Agents[m_numAgents]){
		return false;
	}

	result = m_Agents[m_numAgents]->Initialize(AGENTTYPE_ACTIVE3, 7, 3);
	if (!result){
		return false;
	}

	m_numAgents++;

	// Initialize a 7th Agent
	m_Agents[m_numAgents] = new AgentClass();
	if (!m_Agents[m_numAgents]){
		return false;
	}

	result = m_Agents[m_numAgents]->Initialize(AGENTTYPE_ACTIVE4, 7, 5);
	if (!result){
		return false;
	}

	m_numAgents++;

	// Initialize an 8th Agent
	m_Agents[m_numAgents] = new AgentClass();
	if (!m_Agents[m_numAgents]){
		return false;
	}

	result = m_Agents[m_numAgents]->Initialize(AGENTTYPE_ACTIVE4, 9, 3);
	if (!result){
		return false;
	}

	m_numAgents++;

	// Set the values in the agentOwner array to the ID of the player which owns the corresponding agent for each index
	// NOTE: This is purely for proof of concept - checking Agent ownership will be done more directly in a proper
	//       implementation.
	m_agentOwner[0] = 0;
	m_agentOwner[1] = 0;
	m_agentOwner[2] = -1;
	m_agentOwner[3] = -1;
	m_agentOwner[4] = 0;
	m_agentOwner[5] = 1;
	m_agentOwner[6] = 1;
	m_agentOwner[7] = 1;

	// Set the values in the agentInitiative array to the initiative of the corresponding agent for each index
	// NOTE: This is purely for proof of concept - checking the initiative of each Active Agent will be done more
	//       directly in a proper implementation.
	m_agentInitiative[0] = 10;
	m_agentInitiative[1] = 7;
	m_agentInitiative[2] = -1;
	m_agentInitiative[3] = -1;
	m_agentInitiative[4] = 5;
	m_agentInitiative[5] = 7;
	m_agentInitiative[6] = 5;
	m_agentInitiative[7] = 4;

	// Initialize a HexMap to highlight the tile that the user has the cursor over
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
	int i;

	// Release the AgentSprites bitmap
	if (m_AgentSprites){
		m_AgentSprites->Shutdown();
		delete m_AgentSprites;
		m_AgentSprites = 0;
	}

	// Release the Agents
	if (m_Agents){
		for (i = 0; i < m_numAgents; i++){
			delete m_Agents[i];
			m_Agents[i] = 0;
		}

		delete m_Agents;
		m_Agents = 0;
	}

	// Release the MenuBarBackground Bitmap object
	if (m_MenuBarBackground){
		m_MenuBarBackground->Shutdown();
		delete m_MenuBarBackground;
		m_MenuBarBackground = 0;
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

bool ApplicationClass::RenderGraphics(){
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	int i;
	int* terrain;
	float cameraX, cameraY, cameraZ;
	int highlightX, highlightY;
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

		// Highlight only hexes that are actually on the map (non-negative coordinates, within bounds)
		if (m_cursorOverTile){
			// From the grid coordinates, calculate the absolute pixel coordinates to render the highlight to
			highlightX = (int)(1.5f * HEX_SIZE * m_currentTileX);
			highlightY = (int)(HEX_HEIGHT * (m_currentTileY + 0.5 * abs(m_currentTileX % 2)));

			// Turn on alpha blending to highlight
			m_D3D->TurnOnAlphaBlending();

			// Render the highlight overtop of whatever hex the cursor is over
			result = m_HexHighlight->Render(m_D3D->GetDeviceContext(), highlightX, highlightY, terrain);
			if (!result){
				return false;
			}

			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_HexHighlight->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_HexHighlight->GetTexture(), PSTYPE_LOWALPHA);
			if (!result){
				return false;
			}

			// Turn off alpha blending
			m_D3D->TurnOffAlphaBlending();
		}

		// Render Agents
		// Turn on alpha blending while rendering sprites
		m_D3D->TurnOnAlphaBlending();

		for (i = 0; i < m_numAgents; i++){
			// Get the position of the agent
			m_Agents[i]->getPosition(agentX, agentY);
			
			// Calculate the pixel coordinates to render the agent to
			agentY = (int)(MAP_VERTICALOFFSET + HEX_HEIGHT*((float)agentY + 0.5f * fmod((float)agentX, 2.0f)));
			agentX = (int)(MAP_HORIZONTALOFFSET + (HEX_SIZE / 2.0f) + (1.5f * HEX_SIZE * agentX));

			// Render the agent using its SpriteID
			result = m_AgentSprites->Render(m_D3D->GetDeviceContext(), agentX, agentY, m_Agents[i]->getSpriteID());
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
		result = m_MenuBarBackground->Render(m_D3D->GetDeviceContext(), 0, m_screenHeight - COMBAT_MENU_HEIGHT);
		if (!result){
			return false;
		}

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_MenuBarBackground->GetIndexCount(), worldMatrix, m_UIViewMatrix, orthoMatrix, m_MenuBarBackground->GetTexture(), PSTYPE_NORMAL);
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
bool ApplicationClass::Search(){
	std::list <Pathnode*> queue;

	int x = 0;
	int y = 0;
	m_Agents[m_selectedAgent]->getPosition(x, y);
	bool success = false;

	//Create first node of agents current location
	Pathnode* temp = new Pathnode;
	temp->tileX = x;
	temp->tileY = y;
	temp->cost = 0;
	temp->in = false;
	temp->prev = NULL;

	queue.push_back(temp);

	//search loop
	while (!queue.empty()){
		int index = queue.front()->tileX*m_combatMapHeight + queue.front()->tileY;
		//if tile in even column
		if (queue.front()->tileX % 2 == 0){
			//add the neighbor six hexs to the queue
			//Note need to add if statements to determine if hex is a valid neighbor
			
			//Check if there is a hex above 
			if (queue.front()->tileY > 0 && m_Path[index-1] == NULL){
				Pathnode* temp = new Pathnode;
				temp->cost = FindCost(queue.front()->tileX, queue.front()->tileY) + queue.front()->cost;
				temp->tileX = queue.front()->tileX;
				temp->tileY = queue.front()->tileY - 1;
				temp->prev = queue.front();
				temp->in = false;
				queue.push_back(temp);
				m_Path[index - 1] = temp;
			}

			//Check if currently on last column
			if (queue.front()->tileX < m_combatMapWidth-1){
				//Check if there is a above to the right
				if (queue.front()->tileY > 0 && m_Path[index + m_combatMapHeight - 1] == NULL){
					temp = new Pathnode;
					temp->tileX = queue.front()->tileX + 1;
					temp->tileY = queue.front()->tileY - 1;
					temp->cost = FindCost(temp->tileX, temp->tileY) + queue.front()->cost;
					temp->prev = queue.front();
					temp->in = false;
					queue.push_back(temp);
					m_Path[index + m_combatMapHeight - 1] = temp;
				}

				//there is always a neighbor to the right
				if (m_Path[index + m_combatMapHeight] == NULL){
					temp = new Pathnode;
					temp->tileX = queue.front()->tileX + 1;
					temp->tileY = queue.front()->tileY;
					temp->cost = FindCost(temp->tileX, temp->tileY) + queue.front()->cost;
					temp->prev = queue.front();
					temp->in = false;
					queue.push_back(temp);
					m_Path[index + m_combatMapHeight] = temp;
				}
			}

			//Check if there is a hex below
			if (queue.front()->tileY > m_combatMapHeight-1 && m_Path[index + 1] == NULL){
				temp = new Pathnode;
				temp->tileX = queue.front()->tileX;
				temp->tileY = queue.front()->tileY + 1;
				temp->cost = FindCost(temp->tileX, temp->tileY) + queue.front()->cost;
				temp->prev = queue.front();
				temp->in = false;
				queue.push_back(temp);
				m_Path[index + 1] = temp;
			}

			//Check if on the first colomn
			if (queue.front()->tileX > 0){
				//there is always a hex to the left
				if (m_Path[index - m_combatMapHeight] == NULL){
					temp = new Pathnode;
					temp->tileX = queue.front()->tileX - 1;
					temp->tileY = queue.front()->tileY;
					temp->cost = FindCost(temp->tileX, temp->tileY) + queue.front()->cost;
					temp->prev = queue.front();
					temp->in = false;
					queue.push_back(temp);
					m_Path[index - m_combatMapHeight] = temp;
				}
				//Check if neighbor above to the left
				if (queue.front()->tileY > 0 && m_Path[index - m_combatMapHeight - 1] == NULL){
					temp = new Pathnode;
					temp->tileX = queue.front()->tileX - 1;
					temp->tileY = queue.front()->tileY - 1;
					temp->prev = queue.front();
					temp->in = false;
					queue.push_back(temp);
					temp->cost = FindCost(temp->tileX, temp->tileY) + queue.front()->cost;
					m_Path[index - m_combatMapHeight - 1] = temp;
				}
			}
		}
		//tile is in odd column
		else {
			//add the neighbor six hexs to the queue
			//Note need to add if statements to determine if hex is a valid neighbor
			//Check if there is a hex above 
			if (queue.front()->tileY > 0 && m_Path[index - 1] == NULL){
				temp = new Pathnode;
				temp->tileX = queue.front()->tileX;
				temp->tileY = queue.front()->tileY - 1;
				temp->cost = FindCost(temp->tileX, temp->tileY) + queue.front()->cost;
				temp->prev = queue.front();
				temp->in = false;
				queue.push_back(temp);
				m_Path[index - 1] = temp;
			}
			//Check if currently on last column
			if (queue.front()->tileX < m_combatMapWidth - 1){
				//there is always a neighbor to the right
				if (m_Path[index + m_combatMapHeight] == NULL){
					temp = new Pathnode;
					temp->tileX = queue.front()->tileX + 1;
					temp->tileY = queue.front()->tileY;
					temp->cost = FindCost(temp->tileX, temp->tileY) + queue.front()->cost;
					temp->prev = queue.front();
					temp->in = false;
					queue.push_back(temp);
					m_Path[index + m_combatMapHeight] = temp;
				}

				//check if there is a neighbor to bottom right
				if (queue.front()->tileY < m_combatMapHeight-1 && m_Path[index + m_combatMapHeight + 1] == NULL){
					temp = new Pathnode;
					temp->tileX = queue.front()->tileX + 1;
					temp->tileY = queue.front()->tileY + 1;
					temp->cost = FindCost(temp->tileX, temp->tileY) + queue.front()->cost;
					temp->prev = queue.front();
					temp->in = false;
					queue.push_back(temp);
					m_Path[index + m_combatMapHeight + 1] =  temp;
				}
			}

			//Check if there is a hex below
			if (queue.front()->tileY < m_combatMapHeight - 1 && m_Path[index + 1] == NULL){
				temp = new Pathnode;
				temp->tileX = queue.front()->tileX;
				temp->tileY = queue.front()->tileY + 1;
				temp->cost = FindCost(temp->tileX, temp->tileY) + queue.front()->cost;
				temp->prev = queue.front();
				temp->in = false;
				queue.push_back(temp);
				m_Path[index + 1] = temp;
			}
			
			//check for a bottom left neighbor
			if (queue.front()->tileY < m_combatMapHeight - 1 && m_Path[index + 1 - m_combatMapHeight] == NULL){
				temp = new Pathnode;
				temp->tileX = queue.front()->tileX - 1;
				temp->tileY = queue.front()->tileY + 1;
				temp->cost = FindCost(temp->tileX, temp->tileY) + queue.front()->cost;
				temp->prev = queue.front();
				temp->in = false;
				queue.push_back(temp);
				m_Path[index + 1 - m_combatMapHeight] = temp;
			}

			//there is always a neight to the left
			if(m_Path[index - m_combatMapHeight] == NULL){
				temp = new Pathnode;
				temp->tileX = queue.front()->tileX - 1;
				temp->tileY = queue.front()->tileY;
				temp->prev = queue.front();
				temp->cost = FindCost(temp->tileX, temp->tileY) + queue.front()->cost;
				temp->in = false;
				queue.push_back(temp);
				m_Path[index - m_combatMapHeight] = temp;
			}
		}
		//sort the queue
		queue.pop_front();
		queue.sort(compare2);
	}

	return success;

}

int ApplicationClass::FindCost(int x, int y){
	return 1;
}
/*if (tileY > 0) : the hex at (tileX, tileY - 1) is a valid neighbour

if (tileX < mapWidth - 1):
  // Odd tileX == Odd column
  if (tileX % 2 == 1) :
    // Checks for both neighbours to the right

if (tileX % 2 == 1):
  if (tileY > 0): the hex at (tileX + 1, tileY - 1) is a valid neighbour

  the hex at (tileX + 1, tileY) is a valid neighbour (no check for this)

elseif (tileX % 2 == 0) :
  the hex at (tileX + 1, tileY) is a valid neighbour

  if (tileY < mapHeight -1) : the hex at (tileX + 1, tileY + 1) is a valid neighbour

end if (tileX < mapWidth - 1)

if (tileY < mapHeight - 1) : the hex at (tileX, tileY + 1) is a valid neighbour

if (tileX > 0) : 
  // Odd tileX == Odd column
  if (tileX % 2 == 1) :
    // Checks for both neighbours to the left

if (tileX % 2 == 1):
  the hex at (tileX - 1, tileY) is a valid neighbour

  if (tileY > 0) : the hex at (tileX - 1, tileY - 1) is a valid neighbour

elseif (tileX % 2 == 0) :
  if (tileY < mapHeight - 1) : the hex at (tileX - 1, tileY + 1) is a valid neighbour

  the hex at (tileX - 1, tileY) is a valid neighbour

end if (tileX > 0)

and then you've checked all 6 neighbours

You could also do it the way you have it coded now - you just need to add bounds checks, ie. If the neighbour you're checking has a different X or Y coordinate, check to make sure that that coordinate is within the map's bounds*/