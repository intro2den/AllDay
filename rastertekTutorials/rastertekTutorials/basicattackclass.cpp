#include "basicattackclass.h"

BasicAttackClass::BasicAttackClass(){
}

BasicAttackClass::BasicAttackClass(const BasicAttackClass& other){
}

BasicAttackClass::~BasicAttackClass(){
}

void BasicAttackClass::Execute(AgentClass* source, std::list <AgentClass*> enemies, int targetx, int targety){
	// Execute this ability from the source/executing Agent, affecting all
	// legitimate targets of those provided when targeting the provided
	// coordinates
	// NOTE: Proof of Concept
	//first calcuate attack strength based on source stats
	std::list<AgentClass*> targets;
	std::list<AgentClass*>::iterator target;
	int baseDamage;

	// Proof of Concept, Base Damage is Attacking Agent's 'Strength'
	baseDamage = source->GetStrength();

	//second find all targets in range of attack
	targets = FindTargets(targetx, targety, enemies);

	// Attack all valid targets
	for (target = targets.begin(); target != targets.end(); ++target){
		(*target)->TakeDamage(baseDamage);
	}
}

std::list <AgentClass*> BasicAttackClass::FindTargets(int targetx, int targety, std::list <AgentClass*> potentialTargets){
	// Return a list of Active Agents that may be affected by this ability
	// NOTE: Proof of Concept - need to handle cases other than single target,
	//       terrain/Line of Sight etc.
	//only need to check if there is a target at that X and Y
	std::list<AgentClass*> targets;
	std::list<AgentClass*>::iterator target;
	int x, y;

	for (target = potentialTargets.begin(); target != potentialTargets.end(); ++target){
		(*target)->GetPosition(x, y);
		if (targetx == x && targety == y){
			targets.push_front(*target);
		}
	}
	return targets;
}