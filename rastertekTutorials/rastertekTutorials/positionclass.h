////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include <math.h>

/////////////
// GLOBALS //
/////////////
const float POSITION_MAXSPEED = 0.5f;
const float POSITION_ACCELLERATION = 0.05f;
const float POSITION_DECELLERATION = 0.035f;


////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass
////////////////////////////////////////////////////////////////////////////////
class PositionClass{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(float, float, float);

	void GetPosition(float&, float&, float&);

	void SetBounds(float, float, float);

	void SetFrameTime(float);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveLeft(bool);
	void MoveRight(bool);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_boundX, m_boundY, m_boundZ;

	float m_frameTime;

	float m_forwardSpeed, m_backwardSpeed;
	float m_leftSpeed, m_rightSpeed;
};