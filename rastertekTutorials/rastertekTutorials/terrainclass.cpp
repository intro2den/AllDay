////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "terrainclass.h"


Terrain::Terrain(){
}

Terrain::~Terrain(){
}

bool Terrain::Initialize(int terrainType, int movementCost, bool passable){
	m_terrainType = terrainType;
	m_movementCost = movementCost;
	m_isPassable = passable;

	return true;
}

void Terrain::Shutdown(){
	// Nothing should need to be released
}

int Terrain::GetMovementCost(){
	return m_movementCost;
}

bool Terrain::IsPassable(){
	return m_isPassable;
}

int Terrain::GetTerrainType(){
	return m_terrainType;
}
