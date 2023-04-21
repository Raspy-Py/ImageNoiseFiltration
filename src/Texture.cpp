#include "Texture.h"
#include "Exception.h"
#include <string>
#include <cstring>

Texture::Texture(int w, int h) 
    : 
    width(w), height(h), nrChannels(0), data(nullptr)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    Unbind();
}

Texture::Texture(const char* filepath) 
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    std::string filepathStr(filepath);
    if (bool isPng = filepathStr.substr(filepathStr.size() - 3, 3) == "png")
    {
        throw EXCEPTION("Error: passed a PNG image: [" + std::string(filepath) + "] while it's not supported.");
    }
    else if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (isPng ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        throw EXCEPTION("Failed to load texture: " + std::string(filepath));
    }        
}

Texture::Texture(const Texture& other)
    :
    width(other.width), height(other.height), nrChannels(other.nrChannels)
{
    this->data = new uint8_t[width * height * nrChannels];
    std::memcpy(this->data, other.data, height * width * nrChannels * sizeof(uint8_t));

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    Unbind();
}

Texture& Texture::operator=(const Texture& other)
{
    this->width       = other.width;
    this->height      = other.height;
    this->nrChannels  = other.nrChannels;
    this->data        = new uint8_t[width * height * nrChannels];
    std::memcpy(this->data, other.data, height * width * nrChannels * sizeof(uint8_t));

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    Unbind();
    return *this;
}

Texture::~Texture() 
{
    glDeleteTextures(1, &m_TextureID);

    delete[] data;
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::BindCompute(unsigned int slot) const
{
    glBindImageTexture(slot, m_TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Update()
{
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::GetID()
{
    return m_TextureID;
}
