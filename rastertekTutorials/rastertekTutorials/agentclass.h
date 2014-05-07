////////////////////////////////////////////////////////////////////////////////
// Filename: agentclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

class AgentClass{
public:
	AgentClass();
	~AgentClass();

	bool Initialize(int, int, int);

	void setPosition(int, int);
	void setHeading(int);

	int getType();
	void getPosition(int&, int&);
	void getHeading(int&);

private:
	int m_agentType;
	int m_agentSize;
	int m_posX, m_posY;
	int m_heading;

};

