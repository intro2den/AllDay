#include "terrainclass.h"


terrain::terrain(int type, int cost, bool passable)
{
	m_type = type;
	m_cost = cost;
	m_passable = passable;
}


terrain::~terrain()
{

}

int terrain::movement_cost()
{
	return m_cost;
}
bool terrain::check_passable()
{
	return m_passable;
}
int terrain::get_type(){
	return m_type;
}
