///////////////////////////////////////////////////////////////////////////////
// Filename: errortextclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "errortextclass.h"

ErrorTextClass::ErrorTextClass(){
}

ErrorTextClass::ErrorTextClass(const ErrorTextClass& other){
}

ErrorTextClass::~ErrorTextClass(){
}

void ErrorTextClass::Initialize(FontClass* font, FontShaderClass* fontShader, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix){
	TextClass::Initialize(font, fontShader, screenWidth, screenHeight, baseViewMatrix);

	// Error messages should persist for about 5 seconds after being created if the aren't replaced
	m_timeLimit = 5000.0f;

	return;
}

bool ErrorTextClass::Frame(float frameTime, ID3D11DeviceContext* deviceContext){
	// Update the time that each sentence has been displayed and delete any expired sentences
	bool result;
	bool repositionRequired;

	// Update the time each sentence has been displayed as normal
	TimedTextClass::Frame(frameTime);

	// By default no repositioning of the error messages is required
	repositionRequired = false;

	// Delete any expired error messages, if any messages have expired
	// reposition the remaining error message
	while (!m_TimedSentences.empty()){
		if (m_TimedSentences.front()->time < m_timeLimit){
			break;
		}

		ReleaseTimedSentence(&m_TimedSentences.front());
		m_TimedSentences.pop_front();
		repositionRequired = true;
	}

	if (repositionRequired){
		result = PositionErrorText(deviceContext);
		if (!result){
			return false;
		}
	}

	return true;
}

bool ErrorTextClass::NewError(char* text, ID3D11Device* device, ID3D11DeviceContext* deviceContext){
	// Create a new error message and add it to the list of error messages.
	// If this results in too many error messages, delete the first error
	// message in the list.
	bool result;
	int stringLength;
	TimedSentenceType* newSentence;

	// If the list of error messages is already full, delete the first error
	if (m_TimedSentences.size() == MAX_ERRORS){
		ReleaseTimedSentence(&m_TimedSentences.front());
		m_TimedSentences.pop_front();
	}

	// Create the new error message
	newSentence = new TimedSentenceType;
	if (!newSentence){
		return false;
	}

	// Get the the length of the string to allocate only as much memory as required
	stringLength = strlen(text);

	// Set initial data for the new timed sentence
	newSentence->time = 0.0f;

	newSentence->sentenceText = new char[stringLength + 1];
	strcpy_s(newSentence->sentenceText, stringLength + 1, text);

	// Initialize and update the sentence in the timed sentence
	result = InitializeSentence(&newSentence->Sentence, stringLength, device);
	if (!result){
		return false;
	}

	m_TimedSentences.push_back(newSentence);

	// Position the error messages
	PositionErrorText(deviceContext);

	return true;
}

bool ErrorTextClass::PositionErrorText(ID3D11DeviceContext* deviceContext){
	// Position the error messages so that they appear in the correct positions
	bool result;
	int i;
	std::list<TimedSentenceType*>::iterator sentence;

	i = 0;
	for (sentence = m_TimedSentences.begin(); sentence != m_TimedSentences.end(); ++sentence){
		result = UpdateTimedSentence(*sentence, m_screenWidth / 2 - 80, 80 + 16 * i, 1.0f, 0.0f, 0.0f, NO_LENGTH_LIMIT, deviceContext);
		if (!result){
			return false;
		}

		++i;
	}

	return true;
}
