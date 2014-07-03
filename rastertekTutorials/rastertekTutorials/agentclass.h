////////////////////////////////////////////////////////////////////////////////
// Filename: agentclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


#include <string.h>

///////////////
// CONSTANTS //
///////////////
const int MAX_AGENT_NAME_LENGTH = 16;

//////////
// ENUM //
//////////
enum AgentType{
	AGENTTYPE_INACTIVE1,
	AGENTTYPE_INACTIVE2,

	AGENTTYPE_ACTIVEINACTIVESPLIT, // All AgentTypes enumerated after this point are ActiveAgents, all before are InactiveAgents

	AGENTTYPE_ACTIVE1,
	AGENTTYPE_ACTIVE2,
	AGENTTYPE_ACTIVE3,
	AGENTTYPE_ACTIVE4
};

class AgentClass{
public:
	AgentClass();
	~AgentClass();

	bool Initialize(AgentType, int, int);

	void setPosition(int, int);
	void setHeading(int);

	AgentType GetType();
	char* GetName();
	int GetSpriteID();
	void GetPosition(int&, int&);
	void GetHeading(int&);

private:
	AgentType m_agentType;
	char m_agentName[MAX_AGENT_NAME_LENGTH];
	int m_agentSpriteID;
	int m_agentSize;
	int m_posX, m_posY;
	int m_heading;

};

