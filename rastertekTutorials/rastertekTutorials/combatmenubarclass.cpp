////////////////////////////////////////////////////////////////////////////////
// Filename: combatmenubarclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "combatmenubarclass.h"

CombatMenubarClass::CombatMenubarClass(){
}

CombatMenubarClass::CombatMenubarClass(const CombatMenubarClass& other){
}

CombatMenubarClass::~CombatMenubarClass(){
}

bool CombatMenubarClass::Initialize(int screenWidth, int screenHeight, BitmapClass* texture, BitmapClass* standardButtonTexture, TextureShaderClass* textureShader, FontClass* font, FontShaderClass* fontShader, D3DXMATRIX viewMatrix, ID3D11Device* device, ID3D11DeviceContext* deviceContext){
	// Initialize a Combat Menubar
	bool result;
	int buttonWidth, buttonHeight;
	int buttonXOffset, buttonYOffset, buttonSpacing;

	// Do standard menu initialization, create a text object
	result = MenuClass::Initialize(screenWidth, screenHeight, texture, textureShader, font, fontShader, viewMatrix);
	if (!result){
		return false;
	}

	// Initialize the text objects for the currently selected agent and highlighted information
	m_SelectedAgentText = new TextClass;
	if (!m_SelectedAgentText){
		return false;
	}

	m_SelectedAgentText->Initialize(font, fontShader, screenWidth, screenHeight, viewMatrix);

	m_HighlightedText = new TextClass;
	if (!m_HighlightedText){
		return false;
	}

	m_HighlightedText->Initialize(font, fontShader, screenWidth, screenHeight, viewMatrix);

	// Set the size and position of the menu
	PositionMenubar(screenWidth, screenHeight);

	// Set text offsets based on the menu's position and dimensions
	SetTextOffsets();

	// The Combat Menubar is not exclusive and does not close on a miss
	m_exclusive = false;
	m_closeOnMiss = false;

	buttonWidth = 32;
	buttonHeight = 32;
	buttonXOffset = screenWidth - int(2.5f * buttonWidth);
	buttonYOffset = (m_menuHeight - 2 * buttonHeight) / 2;
	buttonSpacing = buttonHeight;

	// Add buttons to the Combat Menubar
	result = NewButton(BUTTON_ORDERMOVE, m_menuX + buttonXOffset, m_menuY + buttonYOffset, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	result = NewButton(BUTTON_ORDERATTACK, m_menuX + buttonXOffset, m_menuY + buttonYOffset + buttonSpacing, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	result = NewButton(BUTTON_ENDTURN, m_menuX + buttonXOffset + buttonWidth, m_menuY + buttonYOffset, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	result = NewButton(BUTTON_OPENCOMBATMENU, m_menuX + buttonXOffset + buttonWidth, m_menuY + buttonYOffset + buttonSpacing, buttonWidth, buttonHeight, standardButtonTexture);
	if (!result){
		return false;
	}

	return true;
}

void CombatMenubarClass::Shutdown(){
	// Shutdown the menu specific text objects and then shutdown as a normal menu
	if (m_HighlightedText){
		m_HighlightedText->Shutdown();
		delete m_HighlightedText;
		m_HighlightedText = 0;
	}

	if (m_SelectedAgentText){
		m_SelectedAgentText->Shutdown();
		delete m_SelectedAgentText;
		m_SelectedAgentText = 0;
	}

	MenuClass::Shutdown();

	return;
}

bool CombatMenubarClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldmatrix, D3DXMATRIX viewmatrix, D3DXMATRIX orthomatrix){
	// Render as a normal menu and also render additional text
	bool result;

	result = MenuClass::Render(deviceContext, worldmatrix, viewmatrix, orthomatrix);
	if (!result){
		return false;
	}

	// Render the selected agent and highlighted agent/tile text
	result = m_SelectedAgentText->Render(deviceContext, worldmatrix, orthomatrix);
	if (!result){
		return false;
	}

	result = m_HighlightedText->Render(deviceContext, worldmatrix, orthomatrix);
	if (!result){
		return false;
	}

	return true;
}

bool CombatMenubarClass::SetSelectedAgent(char* agentName, ID3D11Device* device, ID3D11DeviceContext* deviceContext){
	// Set the selected agent text
	bool result;

	// Remove the previous text
	m_SelectedAgentText->ClearText();

	// If an agent name was provided, add new text
	if (agentName){
		result = m_SelectedAgentText->AddText(m_selectedAgentXOffset, m_textYOffset, agentName, 1.0f, 1.0f, 1.0f, m_highlightedAgentXOffset - m_selectedAgentXOffset, device, deviceContext);
		if (!result){
			return false;
		}
	}

	return true;
}

bool CombatMenubarClass::SetHighlightedTile(int x, int y, char* agentName, ID3D11Device* device, ID3D11DeviceContext* deviceContext){
	// Update the string displaying the grid coordinates under the cursor to
	// the provided coordinates and if an agent is provided, set the highlighted
	// agent text
	bool result;
	char newString[10];
	char tempString[4];

	// Remove the previous highlight text
	m_HighlightedText->ClearText();

	// Create the new string to display the coordinates in the form "(x,y)"
	strcpy_s(newString, "(");

	_itoa_s(x, tempString, 10);
	strcat_s(newString, tempString);
	strcat_s(newString, ",");

	_itoa_s(y, tempString, 10);
	strcat_s(newString, tempString);
	strcat_s(newString, ")");	

	// Add text to display the string
	result = m_HighlightedText->AddText(m_tileCoordinatesXOffset, m_textYOffset, newString, 1.0f, 1.0f, 1.0f, 52, device, deviceContext);
	if (!result){
		return false;
	}

	// If agentName was provided also set the highlighted agent text
	if (agentName){
		result = SetHighlightedAgent(agentName, device, deviceContext);
		if (!result){
			return false;
		}
	}

	return true;
}

bool CombatMenubarClass::GetTooltip(int cursorX, int cursorY, int& tooltipX, int& tooltipY, int& tooltipWidth, int& tooltipHeight, ButtonAction& action){
	// Return false if no button is under the cursor, otherwise set the position
	// and dimensions of the tooltip, set action as the ButtonAction returned by
	// left clicking on the button and return true
	std::list<ButtonClass*>::iterator button;

	// Set action to NOBUTTON before checking for a button
	action = BUTTON_NOBUTTON;

	// Set the default tooltip dimensions
	// NOTE: Currently proof of concept, fixed dimensions for tooltips
	//       At minimum. tooltip height should probably scale as needed to fit
	tooltipWidth = 200;
	tooltipHeight = 48;

	// Set the menubar tooltip position
	// NOTE: Menubar tooltips appear above the right end of the menubar
	tooltipX = m_menuWidth - tooltipWidth;
	tooltipY = m_menuY - tooltipHeight;

	// If a button is under the cursor, set action and the position of tooltip
	// and return true
	// NOTE: Default tooltip location is to the right of the button it describes
	for (button = m_Buttons.begin(); button != m_Buttons.end(); ++button){
		if ((*button)->HitCheck(cursorX, cursorY)){
			action = (*button)->Click();
			return true;
		}
	}

	// If no button was under the cursor return false
	return false;
}

void CombatMenubarClass::SetTextOffsets(){
	// Set the text offsets based on the menubar's position and dimensions
	// NOTE: This will be built into a more general function that deals with all
	//       information displayed on the menubar
	
	// Small gap between the top of the menu and the top text
	m_textYOffset = m_menuY + 14;

	// Small gap between the left side of the menu and the leftmost text
	m_selectedAgentXOffset = m_menuX + 20;

	// Highlighted agent text should start about halfway between the selected
	// agent text and the menubar's buttons
	m_highlightedAgentXOffset = m_selectedAgentXOffset + (m_menuWidth - 100) / 2;

	// Highlighted tile coordinates should appear just before the menubar's
	// buttons
	m_tileCoordinatesXOffset = m_menuX + m_menuWidth - 150;
	
	return;
}

bool CombatMenubarClass::SetHighlightedAgent(char* agentName, ID3D11Device* device, ID3D11DeviceContext* deviceContext){
	// Set the highlight agent text
	bool result;

	// Add the highlighted Agent text
	result = m_HighlightedText->AddText(m_highlightedAgentXOffset, m_textYOffset, agentName, 1.0f, 1.0f, 1.0f, m_tileCoordinatesXOffset - m_highlightedAgentXOffset, device, deviceContext);
	if (!result){
		return false;
	}
	

	return true;
}
