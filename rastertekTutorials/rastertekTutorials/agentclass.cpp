#include "agentclass.h"

AgentClass::AgentClass(){
}

AgentClass::~AgentClass(){
}

bool AgentClass::Initialize(AgentType agentType, int posX, int posY){
	m_agentType = agentType;

	// Set the spriteID based on the agentType
	// NOTE: We may at some point want to have Agents with the same type use different sprites
	//       however for now this allows us to have Agents with different types use the same
	//       sprite.
	switch (m_agentType){
	case AGENTTYPE_INACTIVE1:
		m_agentSpriteID = 4;
		break;

	case AGENTTYPE_INACTIVE2:
		m_agentSpriteID = 4;
		break;

	case AGENTTYPE_ACTIVE1:
		m_agentSpriteID = 1;
		break;

	case AGENTTYPE_ACTIVE2:
		m_agentSpriteID = 2;
		break;

	case AGENTTYPE_ACTIVE3:
		m_agentSpriteID = 3;
		break;

	case AGENTTYPE_ACTIVE4:
		m_agentSpriteID = 0;
		break;

	default:
		m_agentSpriteID = 4;
		break;
	}

	// Set the Agent's position as provided and set its heading to 0
	m_posX = posX;
	m_posY = posY;
	m_heading = 0;

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

int AgentClass::getSpriteID(){
	return m_agentSpriteID;
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