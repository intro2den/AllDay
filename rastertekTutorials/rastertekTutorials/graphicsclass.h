////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <windows.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h" // Not used for 2D Rendering
//#include "colorshaderclass.h" // Not used for texturing
#include "textureshaderclass.h" // Used for texturing
#include "bitmapclass.h" // Used for 2D Rendering
#include "textclass.h" // Used for the Font Engine

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool InitializeMainMenu(int, int, HWND);
	void ShutdownMainMenu();
	bool Frame(int, int);
	bool Render(int, int, int);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	//ColorShaderClass* m_ColorShader;
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Mouse; // 2D Rendering
	TextClass* m_Text; // Font Engine
	BitmapClass* m_MainMenu[3]; // Main Menu Bitmaps
};

#endif
