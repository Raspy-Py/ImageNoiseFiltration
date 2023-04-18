#pragma once
class Texture;

class Filter
{
public:
	Filter() = default;
	virtual ~Filter() = default;

	virtual void Transform(Texture* image) = 0;
	virtual void TransformFrom(Texture* resultImage, Texture* sourceImage) = 0;
};
