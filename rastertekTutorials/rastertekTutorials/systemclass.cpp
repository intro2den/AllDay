////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"

// Object pointers initialize to NULL (Important in event of initialization failure)
SystemClass::SystemClass(){
	m_Application = 0;
}

// Empty copy constructor and empty class deconstructor
// It is possible these would be made otherwise, and are not necessarily called, can cause memory leaks if relied upon
SystemClass::SystemClass(const SystemClass& other){
}

SystemClass::~SystemClass(){
}

// Initialize - creates window for application, initializes input and graphics objects
bool SystemClass::Initialize(){
	int screenWidth, screenHeight;
	bool result;

	// Initialize settings from the configuration file
	ReadConfig(screenWidth, screenHeight);

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create new application wrapper object
	m_Application = new ApplicationClass;
	if (!m_Application){
		return false;
	}

	// Initialize the application wrapper object
	result = m_Application->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight, m_fullscreen);
	if (!result){
		MessageBox(m_hwnd, "Could not initialize the input object.", "Error", MB_OK);
		return false;
	}

	return true;
}

// Shutdown function - shuts down and releases everything, shuts down window, cleans up handles
void SystemClass::Shutdown(){
	// Release the graphics object.
	if (m_Application){
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}

// Run function - main loop
// NOTE: Only 1 message is handled per frame (most input is handled in the Frame function)
void SystemClass::Run(){
	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done){
		// Handle the window's messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If the window signals to end the application then exit out.
		if (msg.message == WM_QUIT){
			done = true;
		} else{
			// Do frame processing
			result = Frame();
			if (!result){
				done = true;
			}
		}
	}

	return;
}

// Direct Input handles all input now, this just passes messages back to the default Windows message handler
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void SystemClass::ReadConfig(int& screenWidth, int& screenHeight){
	// Read the configuration file for initial application settings.
	// If settings are missing from the file or the file can't be opened, use
	// default values for those settings.
	ifstream fin;
	char configString[20];

	// Set all values to their default
	screenWidth = 800;
	screenHeight = 600;
	m_fullscreen = false;

	// Open the configuration file
	fin.open("../rastertekTutorials/data/configuration.txt");
	if (fin.fail()){
		// The file couldn't be opened, return with the default settings
		return;
	}

	// Read through the configuration file and adjust settings accordingly
	// NOTE: Currently, for proper parsing of the config file each setting
	//       should occupy a single line, starting with the first line.
	//       Should find better method of parsing files.
	fin.getline(configString, 20, ' ');

	while (!fin.eof()){
		// Screen Width
		if (strncmp(configString, "screenwidth", 11) == 0){
			fin.getline(configString, 20, '\n');
			screenWidth = atoi(configString);
			fin.getline(configString, 20, ' ');
			continue;
		}

		// Screen Height
		if (strncmp(configString, "screenheight", 12) == 0){
			fin.getline(configString, 20, '\n');
			screenHeight = atoi(configString);
			fin.getline(configString, 20, ' ');
			continue;
		}

		// Full Screen
		if (strncmp(configString, "fullscreen", 10) == 0){
			fin.getline(configString, 20, '\n');
			if (strncmp(configString, "true", 4) == 0) m_fullscreen = true;
			fin.getline(configString, 20, ' ');
			continue;
		}

		// Read from the next line.
		fin.getline(configString, 20, '\n');
		fin.getline(configString, 20, ' ');
	}

	// Close the configuration file
	fin.close();
	return;
}


// All processing for application done here
bool SystemClass::Frame(){
	bool result;

	// Do frame processing for the application object
	result = m_Application->Frame();
	if (!result){
		return false;
	}

	return true;
}

// Build the window we will be rendering to, returns screenWidth and screenHeight so we can use them
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight){
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = "AllDay";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (m_fullscreen){
		// Set the screen to the maximum resolution of the user's desktop
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	} else{
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

// Return screen settings to normal, release window and handles associated with it
void SystemClass::ShutdownWindows(){
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (m_fullscreen){
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

// Windows sends its messages here, included in system class file because we can
// tie it directly into the system class, have it send all messages to the MessageHandler
// function that we have defined inside SystemClass, hook messaging functionality straight
// to the class and keep the code clean
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam){
	switch (umessage){
	// Check if the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	// Check if the window is being closed.
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;

	// All other messages pass to the message handler in the system class.
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);

	}
}
