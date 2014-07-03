#include "activeagentclass.h"

ActiveAgentClass::ActiveAgentClass(){
}

ActiveAgentClass::ActiveAgentClass(const ActiveAgentClass& other){
}

ActiveAgentClass::~ActiveAgentClass(){
}

bool ActiveAgentClass::Initialize(AgentType agentType, int playerID, int agentX, int agentY, int initiative){
	// Initialize the Active Agent
	bool result;

	// Basic Agent Initialization
	result = AgentClass::Initialize(agentType, agentX, agentY);
	if (!result){
		return false;
	}

	m_Owner = playerID;
	m_Initiative = initiative;
	m_beganTurn = false;
	m_endedTurn = false;

	return true;
}

void ActiveAgentClass::BeginTurn(){
	// Begin the Agent's turn
	m_beganTurn = true;
}

void ActiveAgentClass::EndTurn(){
	// End the Agent's turn
	m_endedTurn = true;
}

void ActiveAgentClass::EndRound(){
	// The round of combat has ended, reset beganTurn and endedTurn flags
	m_beganTurn = false;
	m_endedTurn = false;
}

void ActiveAgentClass::Move(int targetX, int targetY, int cost){
	// Move the Agent to the provided coordinates
	// NOTE: To properly render the Agent travelling between tiles, the series
	//       of tiles along the Agent's path will need to be provided aswell.
	setPosition(targetX, targetY);
}

int ActiveAgentClass::GetOwner(){
	// Return the playerID of the player that controls this Agent
	return m_Owner;
}

int ActiveAgentClass::GetInitiative(){
	// Return the Agent's initiative
	return m_Initiative;
}

bool ActiveAgentClass::StartedTurn(){
	return m_beganTurn;
}

bool ActiveAgentClass::EndedTurn(){
	return m_endedTurn;
}
