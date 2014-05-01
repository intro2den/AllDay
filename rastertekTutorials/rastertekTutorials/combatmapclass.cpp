#include "combatmapclass.h"


combatmapclass::combatmapclass()
{
}


combatmapclass::~combatmapclass()
{
}

bool combatmapclass::Initialize(ID3D11Device* device, int width, int height, int map, int screenwidth, int screenheight){
	m_height = height;
	m_width = width;
	
	terrain* grass = new terrain(0, 1, true);
	terrain* water = new terrain(1, 1, false);

	for (int i = 0; i < m_height*m_width; i++){
		if (i % m_width == m_width / 2){
			m_terrains[i] = water;
		}	else {
			m_terrains[i] = grass;
		}
	}

	LRESULT result = m_bitmaps[0]->Initialize(device, screenwidth, screenheight, "../rastertekTutorials/data/hexagons.dds", 50*width, 50*height);
	if (!result){
		return false;
	}

	return true;
}