////////////////////////////////////////////////////////////////////////////////
// Filename: agentclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////
// ENUM //
//////////
enum AgentType{
	AGENTTYPE_INACTIVE1,
	AGENTTYPE_INACTIVE2,
	AGENTTYPE_ACTIVE1 = 1000,
	AGENTTYPE_ACTIVE2
};

class AgentClass{
public:
	AgentClass();
	~AgentClass();

	bool Initialize(AgentType, int, int);

	void setPosition(int, int);
	void setHeading(int);

	AgentType getType();
	void getPosition(int&, int&);
	void getHeading(int&);

private:
	AgentType m_agentType;
	int m_agentSize;
	int m_posX, m_posY;
	int m_heading;

};

