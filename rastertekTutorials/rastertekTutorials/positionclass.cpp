////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "positionclass.h"

PositionClass::PositionClass(){
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_boundX = 0.0f;
	m_boundY = 0.0f;
	m_boundZ = -10.0f;

	m_frameTime = 0.0f;

	m_forwardSpeed = 0.0f;
	m_backwardSpeed = 0.0f;
	m_leftSpeed = 0.0f;
	m_rightSpeed = 0.0f;
}

PositionClass::PositionClass(const PositionClass& other){
}

PositionClass::~PositionClass(){
}

void PositionClass::SetPosition(float x, float y, float z){
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void PositionClass::GetPosition(float& x, float& y, float& z){
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}

void PositionClass::SetBounds(float boundX, float boundY, float boundZ){
	// NOTE: The bounds are not all the same sign
	//       0 <= positionX <= boundX
	//       boundY <= positionY <= 0
	//       boundZ <= positionZ <= 0

	// Set new bounds for the camera
	m_boundX = boundX;
	m_boundY = boundY;
	m_boundZ = boundZ;
}

void PositionClass::SetFrameTime(float time){
	m_frameTime = time;
	return;
}

void PositionClass::MoveForward(bool moving){
	// Update the forward movement speed based on the frame time and whether the user is attempting to scroll the camera or not.
	if (moving){
		m_forwardSpeed += m_frameTime * POSITION_ACCELERATION;

		if (m_forwardSpeed > (m_frameTime * POSITION_MAXSPEED)){
			m_forwardSpeed = m_frameTime * POSITION_MAXSPEED;
		}
	} else{ // Slow down
		m_forwardSpeed -= m_frameTime * POSITION_DECELERATION;

		if (m_forwardSpeed < 0.0f){
			m_forwardSpeed = 0.0f;
		}
	}

	// Update the position.
	m_positionY += m_forwardSpeed;

	// If out of bounds, reposition to the boundary and reset speed to 0
	if (m_positionY > 0.0f){
		m_positionY = 0.0f;
		m_forwardSpeed = 0.0f;
	}

	return;
}

void PositionClass::MoveBackward(bool moving){
	// Update the backward movement speed based on the frame time and whether the user is attempting to scroll the camera or not.
	if (moving){
		m_backwardSpeed += m_frameTime * POSITION_ACCELERATION;

		if (m_backwardSpeed > (m_frameTime * POSITION_MAXSPEED)){
			m_backwardSpeed = m_frameTime * POSITION_MAXSPEED;
		}
	} else{ // Slow down
		m_backwardSpeed -= m_frameTime * POSITION_DECELERATION;

		if (m_backwardSpeed < 0.0f){
			m_backwardSpeed = 0.0f;
		}
	}

	// Update the position.
	m_positionY -= m_backwardSpeed;

	// If out of bounds, reposition to the boundary and reset speed to 0
	if (m_positionY < m_boundY){
		m_positionY = m_boundY;
		m_backwardSpeed = 0.0f;
	}

	return;
}

void PositionClass::MoveLeft(bool moving){
	// Update the left movement speed based on the frame time and whether the user is attempting to scroll the camera or not.
	if (moving){
		m_leftSpeed += m_frameTime * POSITION_ACCELERATION;

		if (m_leftSpeed > (m_frameTime * POSITION_MAXSPEED)){
			m_leftSpeed = m_frameTime * POSITION_MAXSPEED;
		}
	} else{ // Slow down
		m_leftSpeed -= m_frameTime * POSITION_DECELERATION;

		if (m_leftSpeed < 0.0f){
			m_leftSpeed = 0.0f;
		}
	}

	// Update the position.
	m_positionX -= m_leftSpeed;

	// If out of bounds, reposition to the boundary and reset speed to 0
	if (m_positionX < 0.0f){
		m_positionX = 0.0f;
		m_leftSpeed = 0.0f;
	}

	return;
}

void PositionClass::MoveRight(bool moving){
	// Update the right movement speed based on the frame time and whether the user is attempting to scroll the camera or not.
	if (moving){
		m_rightSpeed += m_frameTime * POSITION_ACCELERATION;

		if (m_rightSpeed > (m_frameTime * POSITION_MAXSPEED)){
			m_rightSpeed = m_frameTime * POSITION_MAXSPEED;
		}
	} else{ // Slow down
		m_rightSpeed -= m_frameTime * POSITION_DECELERATION;

		if (m_rightSpeed < 0.0f){
			m_rightSpeed = 0.0f;
		}
	}

	// Update the position.
	m_positionX += m_rightSpeed;

	// If out of bounds, reposition to the boundary and reset speed to 0
	if (m_positionX > m_boundX){
		m_positionX = m_boundX;
		m_rightSpeed = 0.0f;
	}

	return;
}
