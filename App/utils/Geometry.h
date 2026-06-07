#pragma once
#include "OpenGLRenderer.h"
#include "obj_reader.h"
#include "glm-master/glm/gtc/matrix_transform.hpp"
#include "glm-master/glm/gtc/random.hpp"
#include "glm-master/glm/geometric.hpp"


class Geometry {
    std::vector<Vertex> vertices;
    glm::mat4 transform;
    GLuint vertex_buffer;
    GLuint vertex_array;
    GLuint index_array;
    GLsizei index_size;
    GLuint draw_mode;

    void Load();
public:
    GLuint texture_handle = 0;
    Geometry(): transform(glm::mat4(1.0f)) ,vertex_buffer(0), vertex_array(0), index_array(0), index_size(0), texture_handle(0), draw_mode(GL_TRIANGLES) {}
    Geometry(GLuint texture): Geometry() { texture_handle = texture; }
    ~Geometry();

    void GenPyramid();
    void GenSphere(const glm::vec3& radius, const glm::vec3& offset, int rings, int segments);
    void GenIcosahedronSphere(uint32_t tesselation_num, const glm::vec3 &base_color = glm::vec3(0.f, 0.f, 0.f));
    void GenCubeExercise();
    void GenCube(const glm::vec3& size, const glm::vec3& offset, const glm::vec3& color = glm::vec3(0.f));

    void DisableAttribs(GLuint program);
    void Set(GLuint program);
    void Draw();
    bool ReadObj(const char* filename);
    void SetDrawMode(GLuint mode) { draw_mode = mode; }

    void SetBase(const glm::mat4& m) { transform = m; }
    const glm::mat4& GetBase() { return transform; }
};