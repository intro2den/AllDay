#pragma once

#include "agentclass.h"
#include "basicattackclass.h"
#include <list>

class ActiveAgentClass : public AgentClass{
public:
	ActiveAgentClass();
	ActiveAgentClass(const ActiveAgentClass&);
	~ActiveAgentClass();

	bool Initialize(AgentType, int, int, int, int);

	// NOTE: Shutdown currently to eliminate memory leak caused by instantiation
	//       of AbilityClasses on a per-agent basis.
	void Shutdown();

	// Addition and Initialization of Abilities
	// NOTE: Not yet implemented
	void AddAbility(AbilityClass*);
	void SetStandardAttack(AbilityClass*);

	// Begin and End turn
	void BeginTurn();
	void EndTurn();
	void EndRound();

	// Actions
	void Move(int, int, int);

	// Variable requests
	int GetOwner();
	int GetInitiative();
	int GetCurrentHealth();
	int GetMaxHealth();
	int GetStrength();
	bool StartedTurn();
	bool EndedTurn();
	AbilityClass* GetStandardAttack();

	void TakeDamage(int);

private:
	void SetHealth(int);
	void SetStrength(int);
	void SetDefense(int);

private:
	int m_Owner;
	int m_Initiative;
	int m_currentHealth, m_maxHealth;
	int m_strength;
	int m_defense;
	bool m_beganTurn, m_endedTurn;

	std::list <AbilityClass*> m_abilities;
	AbilityClass* m_standardAttack;
};
