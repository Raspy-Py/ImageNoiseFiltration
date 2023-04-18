#pragma once
#include "Texture.h"

class Filter
{
public:
	Filter() = default;
	virtual ~Filter() = default;

	virtual void Transform(Texture* image) = 0;
};
