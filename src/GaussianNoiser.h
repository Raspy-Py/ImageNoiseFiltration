#pragma once
#include "Filter.h"

class GaussianNoiser : public Filter
{
public:
	GaussianNoiser(float mean, float standartDeviation);
	~GaussianNoiser() override;

	void Transform(Texture* image) override;
	void TransformFrom(Texture* resultImage, Texture* sourceImage) override;
	void SetParam(const char* name, float value) override;

private:
	float RandBias();

private:
	float m_Mean;
	float m_StandartDeviation;
};

