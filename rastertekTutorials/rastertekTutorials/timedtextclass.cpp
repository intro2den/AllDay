///////////////////////////////////////////////////////////////////////////////
// Filename: timedtextclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "timedtextclass.h"

TimedTextClass::TimedTextClass(){
}

TimedTextClass::TimedTextClass(const TimedTextClass& other){
}

TimedTextClass::~TimedTextClass(){
}

void TimedTextClass::Initialize(FontClass* font, FontShaderClass* fontShader, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix){
	TextClass::Initialize(font, fontShader, screenWidth, screenHeight, baseViewMatrix);

	m_timeLimit = 5000.0f;

	return;
}

void TimedTextClass::Shutdown(){
	// Release all sentences
	ClearText();

	// Shutdown as a normal text object
	TextClass::Shutdown();
	
	return;
}

void TimedTextClass::Frame(float frameTime){
	// Update the time that each sentence has been displayed
	std::list<TimedSentenceType*>::iterator sentence;

	for (sentence = m_TimedSentences.begin(); sentence != m_TimedSentences.end(); ++sentence){
		(*sentence)->time += frameTime;
	}
}

bool TimedTextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix){
	// Draw all timed and static text
	bool result;
	std::list<TimedSentenceType*>::iterator sentence;

	result = TextClass::Render(deviceContext, worldMatrix, orthoMatrix);
	if (!result){
		return false;
	}

	for (sentence = m_TimedSentences.begin(); sentence != m_TimedSentences.end(); ++sentence){
		result = RenderSentence(deviceContext, (*sentence)->Sentence, worldMatrix, orthoMatrix);
		if (!result){
			return false;
		}
	}

	return true;
}

void TimedTextClass::ClearText(){
	// Delete all existing text

	// Delete all timed sentences
	while (!m_TimedSentences.empty()){
		ReleaseTimedSentence(&m_TimedSentences.front());
		m_TimedSentences.pop_front();
	}

	TextClass::ClearText();

	return;
}

bool TimedTextClass::AddTimedText(int positionX, int positionY, char* text, float red, float green, float blue, int maxWidth, ID3D11Device* device, ID3D11DeviceContext* deviceContext){
	// Create a new timed sentence and add it to the list of timed sentences
	bool result;
	int stringLength;
	TimedSentenceType* newSentence;

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

	result = UpdateTimedSentence(newSentence, positionX, positionY, red, green, blue, maxWidth, deviceContext);
	if (!result){
		return false;
	}

	m_TimedSentences.push_back(newSentence);

	return true;
}

bool TimedTextClass::UpdateTimedSentence(TimedSentenceType* sentence, int positionX, int positionY, float red, float green, float blue, int maxWidth, ID3D11DeviceContext* deviceContext){
	// Move the provided sentence to the provided location
	bool result;

	result = UpdateSentence(sentence->Sentence, sentence->sentenceText, positionX, positionY, red, green, blue, maxWidth, deviceContext);
	if (!result){
		return false;
	}

	return true;
}

void TimedTextClass::ReleaseTimedSentence(TimedSentenceType** sentence){
	// Release the timed sentence
	if (*sentence){
		// If the timed sentence has a sentence release it
		if ((*sentence)->Sentence){
			ReleaseSentence(&(*sentence)->Sentence);
		}

		// Delete any stored text
		if ((*sentence)->sentenceText){
			delete (*sentence)->sentenceText;
			(*sentence)->sentenceText = 0;
		}

		// Release the timed sentence
		delete *sentence;
		*sentence = 0;
	}

	return;
}
