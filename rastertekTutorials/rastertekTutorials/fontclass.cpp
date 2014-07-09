///////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "fontclass.h"

FontClass::FontClass(){
	m_Font = 0;
	m_Texture = 0;
}

FontClass::FontClass(const FontClass& other){
}

FontClass::~FontClass(){
}

bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, CHAR* textureFilename){
	bool result;

	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if (!result){
		return false;
	}

	// Load the texture that has the font characters on it.
	result = LoadTexture(device, textureFilename);
	if (!result){
		return false;
	}

	return true;
}

void FontClass::Shutdown(){
	// Release the font texture.
	ReleaseTexture();

	// Release the font data.
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(char* filename){
	ifstream fin;
	int i;
	char temp;

	// Create the font spacing buffer.
	m_Font = new FontType[95]; // 95 characters in the texture, indicies in fontdata.txt
	if (!m_Font){
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if (fin.fail()){
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i<95; i++){
		fin.get(temp);
		while (temp != ' '){
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' '){
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

void FontClass::ReleaseFontData(){
	// Release the font data array.
	if (m_Font){
		delete[] m_Font;
		m_Font = 0;
	}

	return;
}

bool FontClass::LoadTexture(ID3D11Device* device, CHAR* filename){
	bool result;

	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture){
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result){
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture(){
	// Release the texture object.
	if (m_Texture){
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* FontClass::GetTexture(){
	return m_Texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY, int maxWidth){
	VertexType* vertexPtr;
	int numLetters, index, i, letter;
	int xOffset;

	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	// Initialize the current xOffset
	xOffset = 0;

	// Draw each letter onto a quad.
	for (i = 0; i<numLetters; i++){
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if (letter == 0){
			drawX += 3.0f;
			xOffset += 3;
		} else{
			// First triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX += m_Font[letter].size + 1.0f;
			xOffset += m_Font[letter].size + 1;
		}

		// If the next character will exceed the maximum width of the text being drawn, continue drawing on the next line.
		// NOTE: This does not yet account for words, only single characters exceeding the maximum width.
		// TODO: Ensure that words are displayed on single lines and that new lines are not offset by spaces.
		if ((maxWidth > 0 && i < numLetters - 1) && (xOffset + m_Font[(int)sentence[i + 1] - 32].size + 2) > maxWidth){
			drawX -= (float)xOffset;
			drawY -= 13;
			xOffset = 0;
		}
	}

	return;
}
