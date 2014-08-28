#pragma once
#include "abilityclass.h"
class basicattackclass :
	public abilityclass
{
public:
	basicattackclass();
	~basicattackclass();

	virtual int execute(AgentClass*, std::list <AgentClass*>, int, int);
	virtual std::list <AgentClass*> findTargets(int, int, std::list <AgentClass*>);
};

