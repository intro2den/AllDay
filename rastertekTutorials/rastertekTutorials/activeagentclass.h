#pragma once

#include "agentclass.h"
#include <list>
////////////
// STRUCT //
////////////
struct Pathnode{
	int tileX;
	int tileY;
	int cost;
	bool in;
	Pathnode *prev;
};

class ActiveAgentClass : public AgentClass{
public:
	ActiveAgentClass();
	~ActiveAgentClass();

	void Move(int, int, int*, int, int);

private:
	virtual bool Search(int, int, int*, int, int, Pathnode*);

};
