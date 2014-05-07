////////////////////////////////////////////////////////////////////////////////
// Filename: hexmapclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "hexmapclass.h"

HexMapClass::HexMapClass(){
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
}

HexMapClass::HexMapClass(const HexMapClass& other){
}

HexMapClass::~HexMapClass(){
}

bool HexMapClass::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, CHAR* textureFilename, int mapWidth, int mapHeight){
	bool result;

	// Store the screen size.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the dimensions of the map.
	m_mapWidth = mapWidth;
	m_mapHeight = mapHeight;
	m_mapTiles = m_mapWidth*m_mapHeight;

	// Initialize the previous rendering position to negative one.
	m_previousPosX = -1;
	m_previousPosY = -1;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result){
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result){
		return false;
	}

	return true;
}

void HexMapClass::Shutdown(){
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}

bool HexMapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY, int* terrain){
	bool result;

	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	result = UpdateBuffers(deviceContext, positionX, positionY, terrain);
	if (!result){
		return false;
	}

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}

int HexMapClass::GetIndexCount(){
	return m_indexCount;
}

ID3D11ShaderResourceView* HexMapClass::GetTexture(){
	return m_Texture->GetTexture();
}

bool HexMapClass::InitializeBuffers(ID3D11Device* device){
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Initialize vertex and index buffers for rendering the map

	// NOTE: This creates a buffer large enough for ALL tiles, regardless of if they will actually be visible
	//		 It is possible to save on buffer size and scale with resolution/window size so some recoding may be
	//		 required later.

	// Set the number of vertices in the vertex array.
	// There are 6 vertices + central point for each hexagonal tile
	m_vertexCount = m_mapTiles*7;

	// Set the number of indices in the index array.
	// Each hexagonal tile is rendered as 6 triangles over 7 vertices
	m_indexCount = m_mapTiles*18;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices){
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices){
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType)* m_vertexCount));

	// Load the index array with data.
	for (i = 0; i < m_mapTiles; i++){
		// Set the indicies for each hex

		// First triangle
		indices[18 * i] = 7 * i;
		indices[18 * i + 1] = 7 * i + 1;
		indices[18 * i + 2] = 7 * i + 6;

		// Second triangle
		indices[18 * i + 3] = 7 * i + 1;
		indices[18 * i + 4] = 7 * i + 2;
		indices[18 * i + 5] = 7 * i + 6;

		// Third triangle
		indices[18 * i + 6] = 7 * i + 2;
		indices[18 * i + 7] = 7 * i + 3;
		indices[18 * i + 8] = 7 * i + 6;

		// Fourth triangle
		indices[18 * i + 9] = 7 * i + 3;
		indices[18 * i + 10] = 7 * i + 4;
		indices[18 * i + 11] = 7 * i + 6;

		// Fifth triangle
		indices[18 * i + 12] = 7 * i + 4;
		indices[18 * i + 13] = 7 * i + 5;
		indices[18 * i + 14] = 7 * i + 6;

		// Sixth triangle
		indices[18 * i + 15] = 7 * i + 5;
		indices[18 * i + 16] = 7 * i;
		indices[18 * i + 17] = 7 * i + 6;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result)){
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result)){
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void HexMapClass::ShutdownBuffers(){
	// Release the index buffer.
	if (m_indexBuffer){
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer){
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

bool HexMapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY, int* terrain){
	float left, top;
	int i;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	// If the position we are rendering this HexMap to has not changed then don't update the vertex buffer since it
	// currently has the correct parameters.
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY)){
		return true;
	}

	// If it has changed then update the position it is being rendered to.
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	// Calculate the screen coordinates of the left side of the map.
	left = (float)((m_screenWidth / 2) * -1) + MAP_HORIZONTALOFFSET + (float)positionX;

	// Calculate the screen coordinates of the top of the map.
	top = (float)(m_screenHeight / 2) - MAP_VERTICALOFFSET - (float)positionY;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices){
		return false;
	}

	// NOTE: Currently loads the vertex array with data for ALL tiles, regardless of if they will actually be visible
	//		 It is possible to instead only load data for tiles that will be visible in the given frame.

	// Load the vertex array with data.
	for (i = 0; i < m_mapTiles; i++){
		// Set the position of the verticies for the ith tile - column by column

		// Top left vertex
		vertices[7 * i].position = D3DXVECTOR3(
			left + (HEX_SIZE / 2.0f) + (1.5f * HEX_SIZE * (i / m_mapHeight)),
			top - HEX_HEIGHT*(fmod((float)i, (float)m_mapHeight) + 0.5f * fmod((float)(i / m_mapHeight), 2.0f)),
			0.0f);

		// Top right vertex
		vertices[7 * i + 1].position = D3DXVECTOR3(
			left + 1.5f * HEX_SIZE + (1.5f * HEX_SIZE * (i / m_mapHeight)),
			top - HEX_HEIGHT*(fmod((float)i, (float)m_mapHeight) + 0.5f * fmod((float)(i / m_mapHeight), 2.0f)),
			0.0f);

		// Right vertex
		vertices[7 * i + 2].position = D3DXVECTOR3(
			left + (2.0f * HEX_SIZE) + (1.5f * HEX_SIZE * (i / m_mapHeight)),
			top - HEX_HEIGHT*(0.5f + fmod((float)i, (float)m_mapHeight) + 0.5f * fmod((float)(i / m_mapHeight), 2.0f)),
			0.0f);

		// Bottom right vertex
		vertices[7 * i + 3].position = D3DXVECTOR3(
			left + (1.5f * HEX_SIZE) + (1.5f * HEX_SIZE * (i / m_mapHeight)),
			top - HEX_HEIGHT*(1.0f + fmod((float)i, (float)m_mapHeight) + 0.5f * fmod((float)(i / m_mapHeight), 2.0f)),
			0.0f);

		// Bottom left vertex
		vertices[7 * i + 4].position = D3DXVECTOR3(
			left + (HEX_SIZE / 2.0f) + (1.5f * HEX_SIZE * (i / m_mapHeight)),
			top - HEX_HEIGHT*(1.0f + fmod((float)i, (float)m_mapHeight) + 0.5f * fmod((float)(i / m_mapHeight), 2.0f)),
			0.0f);

		// Left vertex
		vertices[7 * i + 5].position = D3DXVECTOR3(
			left + (1.5f * HEX_SIZE * (i / m_mapHeight)),
			top - HEX_HEIGHT*(0.5f + fmod((float)i, (float)m_mapHeight) + 0.5f * fmod((float)(i / m_mapHeight), 2.0f)),
			0.0f);

		// Center vertex
		vertices[7 * i + 6].position = D3DXVECTOR3(
			left + HEX_SIZE + (1.5f * HEX_SIZE * (i / m_mapHeight)),
			top - HEX_HEIGHT*(0.5f + fmod((float)i, (float)m_mapHeight) + 0.5f * fmod((float)(i / m_mapHeight), 2.0f)),
			0.0f);

		// Set the texture position for the verticies based on the terrain type.
		// NOTE: Will need to know the number of distinct terrain types to properly access the texture.
		//       Currently using 2 - if a constant is set, replace denominators with constant
		
		// Top left vertex
		vertices[7 * i].texture = D3DXVECTOR2(float(terrain[i]) / 2.0f, 0.0f);

		// Top right vertex
		vertices[7 * i + 1].texture = D3DXVECTOR2(float(terrain[i]) / 2.0f + (1.0f / 2.0f), 0.0f);

		// Right vertex
		vertices[7 * i + 2].texture = D3DXVECTOR2(float(terrain[i]) / 2.0f + (1.0f / 2.0f), 0.5f);

		// Bottom right vertex
		vertices[7 * i + 3].texture = D3DXVECTOR2(float(terrain[i]) / 2.0f + (1.0f / 2.0f), 1.0f);

		// Bottom left vertex
		vertices[7 * i + 4].texture = D3DXVECTOR2(float(terrain[i]) / 2.0f, 1.0f);

		// Left vertex
		vertices[7 * i + 5].texture = D3DXVECTOR2(float(terrain[i]) / 2.0f, 0.5f);

		// Center vertex
		vertices[7 * i + 6].texture = D3DXVECTOR2(float(terrain[i]) / 2.0f + (0.5f / 2.0f), 0.5f);

	}

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)){
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)* m_vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

void HexMapClass::RenderBuffers(ID3D11DeviceContext* deviceContext){
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool HexMapClass::LoadTexture(ID3D11Device* device, CHAR* filename){
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

void HexMapClass::ReleaseTexture(){
	// Release the texture object.
	if (m_Texture){
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}
