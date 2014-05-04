////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

// NOTE: The GraphicsClass has been entirely replaced by the ApplicationClass

GraphicsClass::GraphicsClass(){
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	//m_ColorShader = 0;
	m_TextureShader = 0;
	m_Mouse = 0; // 2D Rendering
	m_Text = 0; // Font Engine
}

GraphicsClass::GraphicsClass(const GraphicsClass& other){
}

GraphicsClass::~GraphicsClass(){
}

// Create the D3DClass object and initialize it 
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd){
	bool result;
	D3DXMATRIX baseViewMatrix; // Font Engine

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D){
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result){
		MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera){
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model){
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "../rastertekTutorials/data/seafloor.dds");
	if (!result){
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}
	
	/*
	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader){
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result){
		MessageBox(hwnd, "Could not initialize the color shader object.", "Error", MB_OK);
		return false;
	}
	*/

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

	// Create the bitmap object.
	m_Mouse = new BitmapClass;
	if (!m_Mouse){
		return false;
	}

	// Initialize the bitmap object.
	result = m_Mouse->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, "../rastertekTutorials/data/hexagons.dds", 50, 50);
	if (!result){
		MessageBox(hwnd, "Could not initialize the bitmap object.", "Error", MB_OK);
		return false;
	}

	// Initialize the Main Menu bitmaps
	InitializeMainMenu(screenWidth, screenHeight, hwnd);

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text){
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result){
		MessageBox(hwnd, "Could not initialize the text object.", "Error", MB_OK);
		return false;
	}

	return true;
}

// Shutdown all of the graphics objects
void GraphicsClass::Shutdown(){
	// Release the Main Menu objects
	ShutdownMainMenu();

	// Release the text object.
	if (m_Text){
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the bitmap object.
	if (m_Mouse){
		m_Mouse->Shutdown();
		delete m_Mouse;
		m_Mouse = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader){
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	/*
	// Release the color shader object.
	if (m_ColorShader){
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}
	*/

	
	// Release the model object.
	if (m_Model){
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
	

	// Release the camera object.
	if (m_Camera){
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_D3D){
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::InitializeMainMenu(int screenWidth, int screenHeight, HWND hwnd){
	HRESULT result;

	// Create MainMenu Bitmaps
	m_MainMenu[0] = new BitmapClass;
	if (!m_MainMenu[0]){
		return false;
	}

	result = m_MainMenu[0]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, "../rastertekTutorials/data/seafloor.dds", screenWidth, screenHeight);
	if (!result){
		MessageBox(hwnd, "Could not initialize the bitmap object.", "Error", MB_OK);
		return false;
	}

	m_MainMenu[1] = new BitmapClass;
	if (!m_MainMenu[1]){
		return false;
	}

	result = m_MainMenu[1]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, "../rastertekTutorials/data/hexagons.dds", screenWidth - 100, 50);
	if (!result){
		MessageBox(hwnd, "Could not initialize the bitmap object.", "Error", MB_OK);
		return false;
	}

	m_MainMenu[2] = new BitmapClass;
	if (!m_MainMenu[2]){
		return false;
	}

	result = m_MainMenu[2]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, "../rastertekTutorials/data/hexagons.dds", screenWidth - 100, 50);
	if (!result){
		MessageBox(hwnd, "Could not initialize the bitmap object.", "Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::ShutdownMainMenu(){
	for (int i = 0; i < sizeof(m_MainMenu) / sizeof(BitmapClass*); i++){
		if (&m_MainMenu[i]){
			m_MainMenu[i]->Shutdown();
		}
	}
}


// Render the scene each frame
bool GraphicsClass::Frame(int mouseX, int mouseY){
	bool result;

	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result){
		return false;
	}

	// Set the position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	return true;
}

// D3D object clears the scene, renders
bool GraphicsClass::Render(int mouseX, int mouseY, int state){
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;

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

	switch (state){
	case 0:

		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_MainMenu[0]->Render(m_D3D->GetDeviceContext(), 0, 0);
		if (!result){
			return false;
		}

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_MainMenu[0]->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_MainMenu[0]->GetTexture());
		if (!result){
			return false;
		}

		for (int i = 1; i < 3; i++){ // Do not use 3
			// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
			result = m_MainMenu[1]->Render(m_D3D->GetDeviceContext(), 50, 50 + 75 * i);
			if (!result){
				return false;
			}

			// Render the bitmap with the texture shader.
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_MainMenu[i]->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_MainMenu[i]->GetTexture());
			if (!result){
				return false;
			}
		}

		break;

	case 1:
		break;
	}

	// The Font Engine also renders in 2D
	// NOTE: 3D Rendering should generally happen AFTER 2D Rendering, but Font Rendering should probably happen AFTER 3D Rendering
	//       so this isn't the exact ordering/structure that will or should be used.

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Render the pointer
	result = m_Mouse->Render(m_D3D->GetDeviceContext(), mouseX, mouseY);
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
