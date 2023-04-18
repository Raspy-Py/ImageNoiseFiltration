#include <iostream>
#include "glad/glad.h"
#include "stb_image/stb_image.h"
#include "glm/glm.hpp"

class Texture 
{
    friend class Filter;
public:
    Texture(int w, int h);
    Texture(const char* filepath);
    Texture(const Texture& other);
    Texture& operator=(const Texture& other);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void BindCompute(unsigned int slot = 0) const;
    void Unbind() const;
    void Update();

    GLuint GetID();

private:
    GLuint m_TextureID;
    uint8_t* m_Data;
    int m_Width;
    int m_Height;
    int m_NrChannels;
};
