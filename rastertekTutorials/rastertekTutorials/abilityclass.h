#pragma once

#include "agentclass.h"
#include <list>

enum AbilityType{
	WATER,
	EARTH,
	FIRE, 
	WIND,
	ELECTRIC,
	HEAL,
	NORMAL
};

class AbilityClass{
public:
	AbilityClass();
	AbilityClass(const AbilityClass&);
	~AbilityClass();

	//this is the main function, it takes the agent using the ability, a list
	//of all possible agents it can attack, and x and y location of that attack
	virtual void Execute(AgentClass*, std::list <AgentClass*>, int, int) = 0;

private:
	//every ability should have its own findTarget, it is called within the
	//Execute function. It handles finding all possible targets for the attack
	virtual std::list <AgentClass*> FindTargets(int, int, std::list <AgentClass*>) = 0;

private:
	AbilityType type;
};

