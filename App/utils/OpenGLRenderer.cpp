#include "OpenGLRenderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <iostream>
#include <fstream>
using namespace std;


const GLuint Renderer::CreateTexture(const char* filename)
{
    GLuint textureHandle = 0;
    int32_t textureWidth = 0;
    int32_t textureHeight = 0;
    int32_t bitsPerPixel = 0;
    stbi_set_flip_vertically_on_load(true);
    uint8_t* imgdata = stbi_load(filename, &textureWidth, &textureHeight, &bitsPerPixel, STBI_rgb_alpha);
    if (!imgdata || !textureWidth || !textureHeight || !bitsPerPixel)
    {
        cout << "Failed to load texture!" << endl;
        cout << filename << endl;
        return 0;
    }

    GLint internalFormat = GL_RGBA;
    GLenum format = GL_RGBA;

    glGenTextures(1, &textureHandle); //Create handle
    glBindTexture(GL_TEXTURE_2D, textureHandle); //Associates the texture object (textureHandle) with the 2D texture target
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, imgdata); //loading texture data to video memory
   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//magnification filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //minifaction filter texture filtering
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] imgdata;
    return textureHandle;
}

bool Renderer::SetTexture(GLuint program, GLuint texture, unsigned char slot, const char* uniformName)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);
    const GLint location = glGetUniformLocation(program, uniformName);
    if (location >= 0)
    {
        glUniform1i(location, slot);
        return true;
    }
    return false;
}


string* Renderer::ReadShaderFromFile(const char* filename)
{
    cout << filename << "\n";
    ifstream file(filename, ios::binary);
    if (!file.is_open())
        std::cout << "Failed to open file\n";
    istreambuf_iterator<char> start(file), end;
    string* text = new string(start, end);
    text->push_back('\0');
    return text;
}

bool Renderer::BuildFragmentShader(const char* filename)
{
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    fragment_shader_text = ReadShaderFromFile(filename);
    cout << *fragment_shader_text << endl;
    const char* src_f = fragment_shader_text->c_str();
    glShaderSource(fragment_shader, 1, &src_f, NULL);
    glCompileShader(fragment_shader);

    GLint status = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char log[1024];
        glGetShaderInfoLog(fragment_shader, 1024, NULL, log);
        std::cout << "Fragment shader compile error : \n" << log << "\n";
        return false;
    }
    return true;
}

bool Renderer::BuildVertexShader(const char* filename)
{
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    vertex_shader_text = ReadShaderFromFile(filename);
    cout << *vertex_shader_text << endl;;
    const char* src_v = vertex_shader_text->c_str();
    glShaderSource(vertex_shader, 1, &src_v, NULL);
    glCompileShader(vertex_shader);
    GLint status = 0;

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char log[1024];
        glGetShaderInfoLog(vertex_shader, 1024, NULL, log);
        cout << "Vertex shader compile error:\n" << log << "\n";
        return false;
    }
    return true;
}

bool Renderer::CompileProgram()
{
    program_handle = glCreateProgram();
    glAttachShader(program_handle, vertex_shader);
    glAttachShader(program_handle, fragment_shader);
    glLinkProgram(program_handle);
    GLint status = 0;
    glGetProgramiv(program_handle, GL_LINK_STATUS, &status);
    if (!status)
    {
        char log[1024];
        glGetProgramInfoLog(program_handle, 1024, NULL, log);
        cout << "Program link error:\n" << log << "\n";
        return false;
    }
    return true;
}
