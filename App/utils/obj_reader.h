#include "glm-master/glm/glm.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>


typedef struct Vertex
{
    glm::vec3 pos = glm::vec3(0.f);
    glm::vec3 color = glm::vec3(0.f);
    glm::vec3 normal = glm::vec3(0.f);
    glm::vec2 uv = glm::vec2(0.f);
    Vertex() {};
    Vertex(glm::vec3 pos) : pos(pos) {}
    Vertex(glm::vec3 pos, glm::vec2 uv) : pos(pos), uv(uv) {}
    Vertex(glm::vec3 pos, glm::vec3 color) : pos(pos), color(color) {}
    Vertex(glm::vec3 pos, glm::vec3 color, glm::vec2 uv) : pos(pos), color(color), uv(uv) {}
    Vertex(glm::vec3 pos, glm::vec3 color, glm::vec3 normal) : pos(pos), color(color), normal(normal) {}
    Vertex(glm::vec3 pos, glm::vec3 color, glm::vec3 normal, glm::vec2 uv) : pos(pos), color(color), normal(normal), uv(uv) {}
    Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 normal): pos(pos), uv(uv), normal(normal) {}
} Vertex;

class OBJContainer
{
    struct OBJData
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::ivec3> indices;
        std::string smooth;
        std::string materialLib;
        std::string material;
    }data;
    public:
        bool LoadOBJ(const std::string& filename);
        std::vector<Vertex> to_vertex_array();
};