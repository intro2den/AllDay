////////////////////////////////////////////////////////////////////////////////
// Filename: combatmapclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "combatmapclass.h"

CombatMap::CombatMap(){
	grassTerrain = 0;
	waterTerrain = 0;
	m_terrain = 0;
}

CombatMap::~CombatMap(){
}

bool CombatMap::Initialize(MapType mapType, int mapWidth, int mapHeight){
	bool result;
	int i;

	// Sanity check, map must have no more than MAX_MAPSIZE tiles
	if (mapWidth * mapHeight > MAX_MAPSIZE){
		return false;
	}

	// Store the map dimensions
	m_mapWidth = mapWidth;
	m_mapHeight = mapHeight;

	// Initialize an array of Terrain pointers
	m_terrain = new Terrain*[m_mapWidth*m_mapHeight];
	if (!m_terrain){
		return false;
	}

	// Create Terrain objects for each type of Terrain that may appear on the combat map.
	grassTerrain = new Terrain();
	if (!grassTerrain){
		return false;
	}

	result = grassTerrain->Initialize(0, 1, true);
	if (!result){
		return false;
	}

	waterTerrain = new Terrain();
	if (!waterTerrain){
		return false;
	}

	result = waterTerrain->Initialize(1, 1, false);
	if (!result){
		return false;
	}

	// NOTE: Will need to find a way to more conveniently create/store fixed maps - ie. Read from file
	// NOTE2: Randomized maps should be reasonable, ie. No agents permanently isolated by terrain, etc.

	// Generate Terrain based on mapType
	switch (mapType){
	case MAPTYPE_ALLGRASS:
		// A map that is all grass tiles
		for (i = 0; i < m_mapHeight*m_mapWidth; i++){
			m_terrain[i] = grassTerrain;
		}

		break;

	case MAPTYPE_RIVERSPLIT:
		// All Grass split by a North-South river
		for (int i = 0; i < m_mapHeight*m_mapWidth; i++){
			if (i / m_mapHeight == m_mapWidth / 2){
				m_terrain[i] = waterTerrain;
			} else{
				m_terrain[i] = grassTerrain;
			}
		}

		break;
	}

	return true;
}

void CombatMap::Shutdown(){
	if (grassTerrain){
		grassTerrain->Shutdown();
		delete grassTerrain;
		grassTerrain = 0;
	}

	if (waterTerrain){
		waterTerrain->Shutdown();
		delete waterTerrain;
		waterTerrain = 0;
	}

	return;
}

Terrain CombatMap::GetTileTerrain(int index){
	return GetTile(index);
}

bool CombatMap::GetTerrainArray(int *terrain){
	// Set the values in the array terrain to the TerrainType of corresponding tiles
	int i;

	for (i = 0; i < m_mapWidth * m_mapHeight; i++){
		terrain[i] = GetTile(i).GetTerrainType();
	}

	return true;
}

Terrain& CombatMap::GetTile(int index){
	return *m_terrain[index];
}
