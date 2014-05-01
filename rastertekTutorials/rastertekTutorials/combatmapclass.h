#pragma once
#include "terrainclass.h"
#include "bitmapclass.h"

class combatmapclass
{
public:
	combatmapclass();
	~combatmapclass();

	bool Initialize(ID3D11Device*, int width, int height, int map, int screenWidth, int screenHeight);
	void Shutdown();
	bool Frame();
	bool Render();

private:
	terrain* m_terrains[1024];
	int m_width;
	int m_height;
	BitmapClass* m_bitmaps[5];
};

