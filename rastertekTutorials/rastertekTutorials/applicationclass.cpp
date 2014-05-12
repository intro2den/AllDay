////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"

// Object pointers initialize to NULL (Important in event of initialization failure)
ApplicationClass::ApplicationClass(){
	m_Input = 0;
	m_D3D = 0;
	m_Camera = 0;
	m_Background = 0;
	m_MainMenuButton = 0;
	m_Mouse = 0;
	m_CombatMap = 0;
	m_TerrainMap = 0;
	m_HexHighlight = 0;
	m_TextureShader = 0;
	m_Text = 0;
	m_FontShader = 0;
	m_Timer = 0;
	m_Position = 0;
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

	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if (!m_Input){
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, m_screenWidth, m_screenHeight);
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

	// Set the initial position of the camera.
	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = -10.0f;

	m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	// Initialize the Background bitmap object
	m_Background = new BitmapClass;
	if (!m_Background){
		return false;
	}

	result = m_Background->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/seafloor.dds", m_screenWidth, m_screenHeight);
	if (!result){
		MessageBox(hwnd, "Could not initialize the background object.", "Error", MB_OK);
		return false;
	}

	// Initialize the Main Menu bitmap object
	m_MainMenuButton = new BitmapClass;
	if (!m_MainMenuButton){
		return false;
	}

	result = m_MainMenuButton->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/hexagons.dds", m_screenWidth - 100, 50);
	if (!result){
		MessageBox(hwnd, "Could not initialize the main menu bitmap object.", "Error", MB_OK);
		return false;
	}

	// Initialize the cursor bitmap object
	m_Mouse = new BitmapClass;
	if (!m_Mouse){
		return false;
	}

	result = m_Mouse->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/cursor.dds", 50, 50);
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
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, m_screenWidth, m_screenHeight, baseViewMatrix);
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

	// Set initial MainState
	m_MainState = MAINSTATE_MAINMENU;

	// Initialize the mouse (cursor) position (this will be overwritten in the first frame)
	m_mouseX = 0;
	m_mouseY = 0;

	return true;
}

// Shutdown function - shuts down and releases everything, shuts down window, cleans up handles
void ApplicationClass::Shutdown(){
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

	// Release the Highlight HexMap object
	if (m_HexHighlight){
		m_HexHighlight->Shutdown();
		delete m_HexHighlight;
		m_HexHighlight = 0;
	}

	// Release the terrain HexMap object
	if (m_TerrainMap){
		m_TerrainMap->Shutdown();
		delete m_TerrainMap;
		m_TerrainMap = 0;
	}

	// Release the Combat Map
	if (m_CombatMap){
		m_CombatMap->Shutdown();
		delete m_CombatMap;
		m_CombatMap = 0;
	}

	// Release the Cursor bitmap object
	if (m_Mouse){
		m_Mouse->Shutdown();
		delete m_Mouse;
		m_Mouse = 0;
	}

	// Release the Main Menu bitmap object
	if (m_MainMenuButton){
		m_MainMenuButton->Shutdown();
		delete m_MainMenuButton;
		m_MainMenuButton = 0;
	}


	// Release the Background bitmap object
	if (m_Background){
		m_Background->Shutdown();
		delete m_Background;
		m_Background = 0;
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

	// Set the location of the mouse
	result = m_Text->SetMousePosition(m_mouseX, m_mouseY, m_D3D->GetDeviceContext());
	if (!result){
		return false;
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
	bool result, scrolling;
	float posX, posY, posZ;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// NOTE: Will also be using MainState/other state checks for rendering
	// NOTE2: Processing of mouse input occurs while it is pressed down, and
	//        should instead occur when the button is released (or initially pressed)

	switch (m_MainState){
	case MAINSTATE_MAINMENU:
		// Main Menu Processing
		// Process mouse input, left mouse button
		if (m_Input->IsLeftMousePressed() == true){
			// First option changes MainState to CombatMap
			if (m_mouseX > 50 && m_mouseX < 750 && m_mouseY > 125 && m_mouseY < 175){
				m_MainState = MAINSTATE_COMBATMAP;
				if (!m_CombatMap){
					result = InitializeCombatMap((MapType)(rand() % 2), 32, 32);
					if (!result){
						return false;
					}
				}
			}
			// Second option exits the application
			if (m_mouseX > 50 && m_mouseX < 750 && m_mouseY > 200 && m_mouseY < 250){
				return false;
			}
		}

		break;

	case MAINSTATE_COMBATMAP:
		// Combat Map Processing
		// Get the currect camera position
		m_Position->GetPosition(posX, posY, posZ);

		// Scroll the camera if the cursor is near the edge of the window
		// or if the user is pressing the corresponding arrow keys
		
		// TODO: Implement scrolling by cursor position
		//       Implement bounds on the camera
		//       Fix the rendered position of any UI elements regardless of camera position

		// Scroll Up/Forward
		scrolling = (m_Input->IsUpPressed() || (m_mouseY >= 0 && m_mouseY < 20));
		m_Position->MoveForward(scrolling);

		// Scroll Down/Backward
		scrolling = (m_Input->IsDownPressed() || (m_mouseY > m_screenHeight - 20 && m_mouseY <= m_screenHeight));
		m_Position->MoveBackward(scrolling);

		// Scroll Left
		scrolling = (m_Input->IsLeftPressed() || (m_mouseX >= 0 && m_mouseX < 20));
		m_Position->MoveLeft(scrolling);

		// Scroll Right
		scrolling = (m_Input->IsRightPressed() || (m_mouseX > m_screenWidth - 20 && m_mouseX <= m_screenWidth));
		m_Position->MoveRight(scrolling);

		// Update the position of the camera
		m_Position->GetPosition(posX, posY, posZ);
		m_Camera->SetPosition(posX, posY, posZ);

		if (m_Input->IsLeftMousePressed() == true){
			// If the mouse is in the top left corner, return to the main menu
			if (m_mouseX >= 0 && m_mouseX < 50 && m_mouseY >= 0 && m_mouseY < 50){
				m_MainState = MAINSTATE_MAINMENU;
				ShutdownCombatMap();

				// Set the position of the camera back to the origin
				m_Position->SetPosition(0.0f, 0.0f, -10.0f);
				m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
			}
		}

		break;
	}

	return true;
}

// Initialize a new Combat Map - this should happen when entering the CombatMap MainState (entering the Combat Map)
bool ApplicationClass::InitializeCombatMap(MapType mapType, int mapWidth, int mapHeight){
	bool result;
	float boundX, boundY, boundZ;

	m_combatMapWidth = mapWidth;
	m_combatMapHeight = mapHeight;

	m_CombatMap = new CombatMap();
	if (!m_CombatMap){
		return false;
	}

	result = m_CombatMap->Initialize(mapType, m_combatMapWidth, m_combatMapHeight, m_screenWidth, m_screenHeight);
	if (!result){
		return false;
	}

	// Initialize the corresponding HexMap for rendering the CombatMap
	m_TerrainMap = new HexMapClass();
	if (!m_TerrainMap){
		return false;
	}

	result = m_TerrainMap->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/terrain.dds", m_combatMapWidth, m_combatMapHeight);
	if (!result){
		return false;
	}

	// Initialize a HexMap to highlight the tile that the user has the cursor over
	m_HexHighlight = new HexMapClass();
	if (!m_HexHighlight){
		return false;
	}

	result = m_HexHighlight->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, "../rastertekTutorials/data/white.dds", 1, 1);
	if (!result){
		return false;
	}

	// Adjust the bounds on the camera position to allow for proper scrolling
	boundX = max(0, HEX_SIZE*(1.5f + 1.5f*(float)mapWidth) - (float)m_screenWidth);
	boundY = min(0, -1.0f * HEX_HEIGHT*(1.5f + (float)mapHeight) + (float)m_screenHeight);
	boundZ = -10.0f;

	m_Position->SetBounds(boundX, boundY, boundZ);

	return true;
}

// Shutdown the Combat Map specifically - this should happen when exiting from the CombatMap but not the application
void ApplicationClass::ShutdownCombatMap(){
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
	float cursorX, cursorY, normalizedCursorX, normalizedCursorY;
	int hexX, hexY;
	int highlightX, highlightY;

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

	// Render the Background first
	result = m_Background->Render(m_D3D->GetDeviceContext(), (int)cameraX, -(int)cameraY);
	if (!result){
		return false;
	}

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Background->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Background->GetTexture(), PSTYPE_NORMAL);
	if (!result){
		return false;
	}

	// Render based on the MainState
	// NOTE: Will probably nest additional switch/if statements for substates (submenus/different display modes etc) later on
	switch (m_MainState){
	case MAINSTATE_MAINMENU:
		// Render the Main Menu buttons
		// NOTE: Will also want to render text on these buttons
		for (i = 0; i < 2; i++){
			// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
			result = m_MainMenuButton->Render(m_D3D->GetDeviceContext(), 50, 125 + 75 * i);
			if (!result){
				return false;
			}

			// Render the bitmap with the texture shader.
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_MainMenuButton->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_MainMenuButton->GetTexture(), PSTYPE_NORMAL);
			if (!result){
				return false;
			}
		}

		break;

	case MAINSTATE_COMBATMAP:
		// Render the Combat Map and associated UI/appropriate overlays

		// Render the terrain first
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

		// Find the hex that the cursor is overtop of this frame
		// NOTE: Because the result of this calculation will need to be used elsewhere as well
		//       it will be moved and the results stored in the class

		// Calculate the actual cursor position relative to the map
		cursorX = (float)m_mouseX + cameraX - MAP_HORIZONTALOFFSET;
		cursorY = (float)m_mouseY - cameraY - MAP_VERTICALOFFSET;

		if (cursorX >= 0){
			// Calculate the grid coordinates for the hex the cursor is over
			hexX = (int)(cursorX / (1.5f * HEX_SIZE));

			// Use the X-coordinate of the cursor to find the column
			if (fmod(cursorX, 1.5f * HEX_SIZE) >= 0.5f * HEX_SIZE){
				// The X-position of the Hex that the cursor is over can be identified by the X-coordinate alone
				hexX = (int)(cursorX / (1.5f * HEX_SIZE));
			} else{
				// The cursor could be over a hex in either of 2 adjacent columns, will need to use both
				// coordinates to determine the column

				// Normalize the coordinates to be relative the origin
				normalizedCursorX = abs(fmod(cursorX, 1.5f * HEX_SIZE)) / (0.5f * HEX_SIZE);
				normalizedCursorY = abs((fmod(cursorY + 0.5f * HEX_HEIGHT * (float)(abs(hexX % 2)), HEX_HEIGHT)) - 0.5f * HEX_HEIGHT) / (0.5f * HEX_HEIGHT);

				if (normalizedCursorX >= abs(normalizedCursorY)){
					hexX = (int)(cursorX / (1.5f * HEX_SIZE));
				} else{
					hexX = (int)(cursorX / (1.5f * HEX_SIZE)) - 1;
				}
			}

			// Now that we know the column, we can identify the row using the Y-coordinate
			hexY = (int)((cursorY - 0.5f * HEX_HEIGHT * abs(hexX % 2)) / HEX_HEIGHT);

			// Integer division rounds towards 0, make sure that we catch all negative Y-coordinates as negative
			if (cursorY - 0.5f * HEX_HEIGHT * abs(hexX % 2) < 0){
				hexY = -1;
			}

			// Highlight only hexes that are actually on the map (non-negative coordinates, within bounds)
			if (hexX >= 0 && hexX < m_combatMapWidth && hexY >= 0 && hexY < m_combatMapHeight){

				// From the grid coordinates, calculate the absolute pixel coordinates to render the highlight to
				highlightX = (int)(1.5f * HEX_SIZE * hexX);
				highlightY = (int)(HEX_HEIGHT * (hexY + 0.5 * abs(hexX % 2)));

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
		}

		break;
	}

	// Sanity Check for rendering text

	// The Font Engine also renders in 2D
	// NOTE: 3D Rendering should generally happen AFTER 2D Rendering, but Font Rendering should probably happen AFTER 3D Rendering
	//       so this isn't the exact ordering/structure that will or should be used.

	// Turn on the alpha blending before rendering the text and cursor.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	// Turn off alpha blending after rendering the text and cursor
	m_D3D->TurnOffAlphaBlending();

	// Render the cursor
	result = m_Mouse->Render(m_D3D->GetDeviceContext(), m_mouseX + (int)cameraX, m_mouseY - (int)cameraY);
	if (!result){
		return false;
	}

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Mouse->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Mouse->GetTexture(), PSTYPE_NORMAL);
	if (!result){
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}
