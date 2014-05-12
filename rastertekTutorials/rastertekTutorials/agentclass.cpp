#include "agentclass.h"

AgentClass::AgentClass(){
	m_heading = 0;
}

AgentClass::~AgentClass(){
}

bool AgentClass::Initialize(AgentType agentType, int posX, int posY){
	m_agentType = agentType;
	m_posX = posX;
	m_posY = posY;

	return true;
}

void AgentClass::setPosition(int x, int y){
	m_posX = x;
	m_posY = y;
}

void AgentClass::setHeading(int heading){
	m_heading = heading;
}

AgentType AgentClass::getType(){
	return m_agentType;
}

void AgentClass::getPosition(int &posX, int &posY){
	posX = m_posX;
	posY = m_posY;

	return;
}

void AgentClass::getHeading(int &heading){
	heading = m_heading;

	return;
}