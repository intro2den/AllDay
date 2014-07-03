////////////////////////////////////////////////////////////////////////////////
// Filename: combatmapclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

/////////////
// GLOBALS //
/////////////
const int MAX_MAPSIZE = 1024;

//////////////
// INCLUDES //
//////////////
#include "terrainclass.h"

///////////
// enum  //
///////////
enum MapType{
	MAPTYPE_ALLGRASS,
	MAPTYPE_RIVERSPLIT
};

////////////////////////////////////////////////////////////////////////////////
// Class name: CombatMap
////////////////////////////////////////////////////////////////////////////////
class CombatMap{
public:
	CombatMap();
	~CombatMap();

	bool Initialize(MapType mapType, int mapWidth, int mapHeight);
	void Shutdown();

	Terrain GetTileTerrain(int);
	bool GetTerrainArray(int*);

private:
	Terrain& GetTile(int index);

private:
	Terrain* grassTerrain;
	Terrain* waterTerrain;

	int m_mapWidth, m_mapHeight;
	Terrain** m_terrain;
};
