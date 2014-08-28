#include "agentclass.h"

AgentClass::AgentClass(){
}

AgentClass::~AgentClass(){
}

bool AgentClass::Initialize(AgentType agentType, int posX, int posY){
	m_agentType = agentType;

	//we will need to set these as parameters later, just leaving it like this to not break the code in the application class
	m_health = 100;
	m_defense = 1;
	m_strength = 10;

	// Set the spriteID based on the agentType
	// NOTE: We may at some point want to have Agents with the same type use different sprites
	//       however for now this allows us to have Agents with different types use the same
	//       sprite.
	switch (m_agentType){
	case AGENTTYPE_INACTIVE1:
		strcpy_s(m_agentName, "Tree");
		m_agentSpriteID = 4;
		break;

	case AGENTTYPE_INACTIVE2:
		strcpy_s(m_agentName, "Tree");
		m_agentSpriteID = 4;
		break;

	case AGENTTYPE_ACTIVE1:
		strcpy_s(m_agentName, "Purple");
		m_agentSpriteID = 1;
		break;

	case AGENTTYPE_ACTIVE2:
		strcpy_s(m_agentName, "Grey");
		m_agentSpriteID = 2;
		break;

	case AGENTTYPE_ACTIVE3:
		strcpy_s(m_agentName, "Orange");
		m_agentSpriteID = 3;
		break;

	case AGENTTYPE_ACTIVE4:
		strcpy_s(m_agentName, "Yellow");
		m_agentSpriteID = 0;
		break;

	default:
		strcpy_s(m_agentName, "Default Name");
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

AgentType AgentClass::GetType(){
	return m_agentType;
}

char* AgentClass::GetName(){
	return m_agentName;
}

int AgentClass::GetSpriteID(){
	return m_agentSpriteID;
}

void AgentClass::GetPosition(int &posX, int &posY){
	posX = m_posX;
	posY = m_posY;

	return;
}

void AgentClass::GetHeading(int &heading){
	heading = m_heading;

	return;
}

void AgentClass::getHealth(int &health){
	health = m_health;
}
void AgentClass::GetDefense(int &defense){
	defense = m_defense;
}

void AgentClass::setHealth(int health){
	m_health = health;
}
void AgentClass::setDefense(int defense){
	m_defense = defense;
}

void AgentClass::takeDamage(int damage){
	if (m_health > damage){
		m_health = m_health - damage;
	}
	else {
		m_health = 0;
	}
}

void AgentClass::getStrength(int &strength){
	strength = m_strength;
}

void AgentClass::setStrength(int strength){
	m_strength = strength;
}
