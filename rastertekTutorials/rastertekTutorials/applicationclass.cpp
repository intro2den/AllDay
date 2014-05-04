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
	m_TextureShader = 0;
	m_Text = 0;
	m_FontShader = 0;
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

	// Set initial MainState
	m_MainState = MAINSTATE_MAINMENU;

	// Initialize the mouse (cursor) position (this will be overwritten in the first frame)
	m_mouseX = 0;
	m_mouseY = 0;

	return true;
}

// Shutdown function - shuts down and releases everything, shuts down window, cleans up handles
void ApplicationClass::Shutdown(){
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

	// NOTE: Will also be using MainState/other state checks for rendering
	// NOTE2: Processing of input will likely be handed off to the HandleInput() function

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
		if (m_Input->IsLeftMousePressed() == true){
			// If the mouse is in the top left corner, return to the main menu
			if (m_mouseX >= 0 && m_mouseX < 50 && m_mouseY >= 0 && m_mouseY < 50){
				m_MainState = MAINSTATE_MAINMENU;
				ShutdownCombatMap();
			}
		}

		break;
	}

	// Render the graphics
	result = RenderGraphics();
	if (!result){
		return false;
	}
	
	return true;
}

// Initialize a new Combat Map - this should happen when entering the CombatMap MainState (entering the Combat Map)
bool ApplicationClass::InitializeCombatMap(MapType mapType, int mapWidth, int mapHeight){
	bool result;

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

	return true;
}

// Shutdown the Combat Map specifically - this should happen when exiting from the CombatMap but not the application
void ApplicationClass::ShutdownCombatMap(){
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

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Render the Background first
	result = m_Background->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result){
		return false;
	}

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Background->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Background->GetTexture());
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
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_MainMenuButton->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_MainMenuButton->GetTexture());
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

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_TerrainMap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_TerrainMap->GetTexture());
		if (!result){
			return false;
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
	result = m_Mouse->Render(m_D3D->GetDeviceContext(), m_mouseX, m_mouseY);
	if (!result){
		return false;
	}

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Mouse->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Mouse->GetTexture());
	if (!result){
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}
