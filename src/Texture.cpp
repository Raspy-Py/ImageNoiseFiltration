#include "Texture.h"
#include "Exception.h"
#include <string>
#include <cstring>

Texture::Texture(int w, int h) 
    : 
    m_Width(w), m_Height(h), m_NrChannels(0), m_Data(nullptr)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Data);

    Unbind();
}

Texture::Texture(const char* filepath) 
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_Data = stbi_load(filepath, &m_Width, &m_Height, &m_NrChannels, 0);
    std::string filepathStr(filepath);
    if (bool isPng = filepathStr.substr(filepathStr.size() - 3, 3) == "png")
    {
        throw EXCEPTION("Error: passed a PNG image: [" + std::string(filepath) + "] while it's not supported.");
    }
    else if (m_Data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, (isPng ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, m_Data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        throw EXCEPTION("Failed to load texture: " + std::string(filepath));
    }        
}

Texture::Texture(const Texture& other)
    :
    m_Width(other.m_Width), m_Height(other.m_Height), m_NrChannels(other.m_NrChannels)
{
    this->m_Data = new uint8_t[m_Width * m_Height * 3];
    std::memcpy(this->m_Data, other.m_Data, m_Height * m_Width * sizeof(other.m_Data[0]) * 3);

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Data);

    Unbind();
}

Texture& Texture::operator=(const Texture& other)
{
    this->m_Width       = other.m_Width;
    this->m_Height      = other.m_Height;
    this->m_NrChannels  = other.m_NrChannels;
    this->m_Data        = new uint8_t[m_Width * m_Height * 3];
    std::memcpy(this->m_Data, other.m_Data, m_Height * m_Width * sizeof(other.m_Data[0]) * 3);

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Data);

    Unbind();
    return *this;
}

Texture::~Texture() 
{
    glDeleteTextures(1, &m_TextureID);

    delete[] m_Data;
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
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, m_Data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::GetID()
{
    return m_TextureID;
}
