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

	// Set 'Vital Statistics' for this Active Agent
	// NOTE: Currently Proof of Concept, fixed values for ALL Active Agents
	m_maxHealth = 100;
	m_currentHealth = m_maxHealth - 50;

	m_strength = 10;
	m_defense = 1;

	// Create and configure the standard attack for this Active Agent
	// NOTE: Currently Proof of Concept
	//for now everyone has basicattack
	m_abilities.push_back(new BasicAttackClass());
	m_standardAttack = m_abilities.back();

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

int ActiveAgentClass::GetCurrentHealth(){
	// Return the Agent's current health
	return m_currentHealth;
}

int ActiveAgentClass::GetMaxHealth(){
	// Return the Agent's maximum health
	return m_maxHealth;
}

int ActiveAgentClass::GetStrength(){
	// Return the Agent's strength
	return m_strength;
}

bool ActiveAgentClass::StartedTurn(){
	return m_beganTurn;
}

bool ActiveAgentClass::EndedTurn(){
	return m_endedTurn;
}

void ActiveAgentClass::TakeDamage(int baseDamage){
	// Reduce the Agent's health by up to baseDamage
	// NOTE: Work in Progress, currently Proof of Concept
	//       Agent takes baseDamage - defense damage
	//       If defense >= baseDamage, the Agent takes 1 damage
	int damageTaken;

	if (baseDamage > m_defense){
		damageTaken = baseDamage - m_defense;
	} else{
		damageTaken = 1;
	}

	SetHealth(m_currentHealth - damageTaken);
}

void ActiveAgentClass::SetHealth(int health){
	// Set the Agent's current health
	m_currentHealth = health;
}

void ActiveAgentClass::SetStrength(int strength){
	// Set the Agent's strength
	m_strength = strength;
}

void ActiveAgentClass::SetDefense(int defense){
	// Set the Agent's defense
	m_defense = defense;
}