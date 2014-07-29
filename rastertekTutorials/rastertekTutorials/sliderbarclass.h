////////////////////////////////////////////////////////////////////////////////
// Filename: sliderbarclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

////////////////////
// CLASS INCLUDES //
////////////////////
#include "bitmapclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SliderbarClass
////////////////////////////////////////////////////////////////////////////////
class SliderbarClass{
public:
	SliderbarClass();
	SliderbarClass(const SliderbarClass&);
	~SliderbarClass();

	void Initialize(float*, int, int, int, int, int, int, BitmapClass*, BitmapClass*);
	void SetPosition(int, int);
	void SetSliderbarDimensions(int, int);
	void SetSliderDimensions(int, int);
	bool HitCheck(int, int);
	void Click(int);

	bool RenderSliderbar(ID3D11DeviceContext*);
	bool RenderSlider(ID3D11DeviceContext*);

	int GetSliderbarIndexCount();
	ID3D11ShaderResourceView* GetSliderbarTexture();
	int GetSliderIndexCount();
	ID3D11ShaderResourceView* GetSliderTexture();

private:
	BitmapClass* m_SliderTexture;
	BitmapClass* m_SliderbarTexture;

	float* m_variable;
	int m_sliderbarX, m_sliderbarY;
	int m_sliderbarWidth, m_sliderbarHeight;
	int m_sliderWidth, m_sliderHeight;
};
