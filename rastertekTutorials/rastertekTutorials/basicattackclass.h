#pragma once

#include "abilityclass.h"

class BasicAttackClass : public AbilityClass{
public:
	BasicAttackClass();
	BasicAttackClass(const BasicAttackClass&);
	~BasicAttackClass();

	virtual void Execute(AgentClass*, std::list <AgentClass*>, int, int);

private:
	virtual std::list <AgentClass*> FindTargets(int, int, std::list <AgentClass*>);
};
