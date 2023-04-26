#include "GaussianNoiser.h"
#include "Texture.h"
#include <cmath>

GaussianNoiser::GaussianNoiser(float mean, float standartDeviation)
	:
	m_Mean(mean), m_StandartDeviation(standartDeviation)
{}

GaussianNoiser::~GaussianNoiser()
{}

void GaussianNoiser::Transform(Texture* image)
{
    srand(time(NULL));
	size_t nSamples = image->width * image->height * image->nrChannels;
	for (size_t i = 0; i < nSamples; i+=image->nrChannels)
	{
        float noise = RandBias();
        image->data[i    ] = std::min(std::max(int(image->data[i    ] + noise), 0), 255);
        image->data[i + 1] = std::min(std::max(int(image->data[i + 1] + noise), 0), 255);
        image->data[i + 2] = std::min(std::max(int(image->data[i + 2] + noise), 0), 255);
	}
}

void GaussianNoiser::TransformFrom(Texture* resultImage, Texture* sourceImage)
{
    srand(time(NULL));
    size_t nSamples = resultImage->width * resultImage->height * resultImage->nrChannels;

    if (resultImage->width != sourceImage->width ||
        resultImage->height != sourceImage->height ||
        resultImage->nrChannels != sourceImage->nrChannels)
    {
        resultImage->width = sourceImage->width;
        resultImage->height = sourceImage->height;
        resultImage->nrChannels = sourceImage->nrChannels;

        if (resultImage->data != nullptr)
        {
            delete[] resultImage->data;
        }

        resultImage->data = new uint8_t[sourceImage->width * sourceImage->height * sourceImage->nrChannels];
    }

    for (size_t i = 0; i < nSamples; i += resultImage->nrChannels)
    {
        float noise = RandBias();
        resultImage->data[i    ] = std::min(std::max(int(sourceImage->data[i    ] + noise), 0), 255);
        resultImage->data[i + 1] = std::min(std::max(int(sourceImage->data[i + 1] + noise), 0), 255);
        resultImage->data[i + 2] = std::min(std::max(int(sourceImage->data[i + 2] + noise), 0), 255);
    }
}

void GaussianNoiser::SetParam(const char* name, float value)
{
    if (name == "standartDeviation")
    {
        m_StandartDeviation = value;
    }
}

float GaussianNoiser::RandBias() 
{
    static float z0, z1;
    static bool generate;
    generate = !generate;
    if (!generate)
    {
        return z1 * m_StandartDeviation + m_Mean;
    }
    float u1, u2;
    do 
    {
        u1 = rand() * (1.0f / RAND_MAX);
        u2 = rand() * (1.0f / RAND_MAX);
    } while (u1 <= 1e-7f);
    float mag = std::sqrt(-2.0f * std::log(u1));
    z0 = mag * std::cos(6.283185307179586476925286766559f * u2);
    z1 = mag * std::sin(6.283185307179586476925286766559f * u2);
    return z0 * m_StandartDeviation + m_Mean;
}
