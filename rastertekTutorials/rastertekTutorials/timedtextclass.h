////////////////////////////////////////////////////////////////////////////////
// Filename: timedtextclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

////////////////////
// CLASS INCLUDES //
////////////////////
#include "textclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TimedTextClass
////////////////////////////////////////////////////////////////////////////////
class TimedTextClass : public TextClass{
protected:
	/////////////
	// STRUCTS //
	/////////////
	struct TimedSentenceType{
		SentenceType* Sentence;
		float time;
		char* sentenceText;
	};

public:
	TimedTextClass();
	TimedTextClass(const TimedTextClass&);
	~TimedTextClass();

	void Initialize(FontClass*, FontShaderClass*, int, int, D3DXMATRIX);
	void Shutdown();
	void Frame(float);
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	void ClearText();
	bool AddTimedText(int, int, char*, float, float, float, int, ID3D11Device*, ID3D11DeviceContext*);

protected:
	bool UpdateTimedSentence(TimedSentenceType*, int, int, float, float, float, int, ID3D11DeviceContext*);
	void ReleaseTimedSentence(TimedSentenceType**);

protected:
	float m_timeLimit;

	std::list <TimedSentenceType*> m_TimedSentences;
};
