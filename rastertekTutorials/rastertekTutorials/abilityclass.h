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

class abilityclass
{
public:
	abilityclass();
	~abilityclass();

	//this is the main function, it takes the agent using the ability, a list
	//of all possible agents it can attack, and x and y location of that attack
	virtual int execute(AgentClass*, std::list <AgentClass*>, int, int) = 0;

	//every ability should have its own findTarget, it is called within the
	//execute function. It handles finding all possible targets for the attack
	virtual std::list <AgentClass*> findTargets(int, int, std::list <AgentClass*>) = 0;

private:
	AbilityType type;
};

