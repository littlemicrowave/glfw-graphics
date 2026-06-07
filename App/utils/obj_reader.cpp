#include "obj_reader.h"

bool OBJContainer::LoadOBJ(const std::string& filename)
{

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Failed to open OBJ file: " << filename << std::endl;
        return false;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v")
        {
            float x, y, z;
            iss >> x >> y >> z;
            data.vertices.push_back(glm::vec3(x, y, z));
        }
        else if (type == "vn")
        {
            float x, y, z;
            iss >> x >> y >> z;
            data.normals.push_back(glm::vec3(x, y, z));
        }
        else if (type == "vt")
        {
            float u, v;
            iss >> u >> v;
            data.texcoords.push_back(glm::vec2(u, v));
        }
        else if (type == "f")
        {
            std::string v1, v2, v3;
            iss >> v1 >> v2 >> v3;

            glm::ivec3 (*parseFace)(const std::string&) = [](const std::string& vert)
                {
                    std::istringstream viss(vert);
                    std::string part;
                    glm::ivec3 inds(-1); // vertex index/u_v index/ normal index
                    int i = 0;
                    while (std::getline(viss, part, '/'))
                    {
                        if (!part.empty())
                            inds[i] = std::stoi(part) - 1; //shift to zero index
                        i++; 
                    }
                    return inds;
                };

            data.indices.push_back(parseFace(v1));
            data.indices.push_back(parseFace(v2));
            data.indices.push_back(parseFace(v3));
        }
        else if (type == "s")
            iss >> data.smooth;
        else if (type == "mtllib")
            iss >> data.materialLib;
        else if (type == "usemtl")
            iss >> data.material;
        else
            std::cout << "skip unknown line: " << line << std::endl;
    }
    return true;
}

std::vector<Vertex> OBJContainer::to_vertex_array()
{
    std::vector<Vertex> vertices;
    for (auto f : data.indices)
    {
        Vertex v;
        if (f[0] >= 0)
            v.pos = data.vertices[f[0]];
        if (f[1] >= 0)
            v.uv = data.texcoords[f[1]];
        if (f[2] >= 0)
            v.normal = data.normals[f[2]];
        vertices.push_back(v);
    }
    return vertices;
}
