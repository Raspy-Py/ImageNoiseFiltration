#include <iostream>
#include "glad/glad.h"
#include "stb_image/stb_image.h"
#include "glm/glm.hpp"

class Texture 
{
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



public:
    uint8_t* data;
    int width;
    int height;
    int nrChannels;

private:
    GLuint m_TextureID;
};
