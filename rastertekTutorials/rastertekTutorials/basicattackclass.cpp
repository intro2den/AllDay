#include "basicattackclass.h"


basicattackclass::basicattackclass()
{
}


basicattackclass::~basicattackclass()
{
}

int basicattackclass::execute(AgentClass* source, std::list <AgentClass*> enemies, int targetx, int targety){
	//first calcuate attack strength based on source stats
	int attack;
	int defense;
	source->getStrength(attack);

	//second find all targets in range of attack
	std::list <AgentClass*> target = findTargets(targetx, targety, enemies);

	//calcuate defense strength of each target
	for (std::list<AgentClass*>::iterator it = target.begin(); it != target.end(); it++){
		(*it)->GetDefense(defense);
		//call targets takeDamage(attack-defense) method
		if (defense < attack){
			(*it)->takeDamage(attack - defense);
		}
		else {
			(*it)->takeDamage(1);
		}
	}
	
	
	return attack;
}

std::list <AgentClass*> basicattackclass::findTargets(int targetx, int targety, std::list <AgentClass*> targets){
	//only need to check if there is a target at that X and Y
	std::list <AgentClass*> target;
	int x;
	int y;
	for (std::list<AgentClass*>::iterator it = targets.begin(); it != targets.end(); it++){
		(*it)->GetPosition(x, y);
		if (targetx == x && targety == y){
			target.push_front(*it);
		}
	}
	return target;
}