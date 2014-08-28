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

	// Begin and End turn
	void BeginTurn();
	void EndTurn();
	void EndRound();

	// Actions
	void Move(int, int, int);

	// Variable requests
	int GetOwner();
	int GetInitiative();
	bool StartedTurn();
	bool EndedTurn();
	
	std::list<abilityclass*> m_abilities;

private:
	int m_Owner;
	int m_Initiative;
	bool m_beganTurn, m_endedTurn;
};
