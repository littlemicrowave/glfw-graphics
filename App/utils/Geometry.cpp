#include "Geometry.h"
#include <random>
#include <time.h>

using namespace glm;

void Geometry::GenPyramid()
{
	vec3 p0 = vec3(-1, 0, -1);
	vec3 p1 = vec3(1, 0, -1);
	vec3 p2 = vec3(1, 0, 1);
	vec3 p3 = vec3(-1, 0, 1);
	vec3 top = vec3(0, 1, 0);

	vec3 baseN = vec3(0, -1, 0);

	vec3 side0N = normalize(cross(top - p0, p1 - p0));
	vec3 side1N = normalize(cross(top - p1, p2 - p1));
	vec3 side2N = normalize(cross(top - p2, p3 - p2));
	vec3 side3N = normalize(cross(top - p3, p0 - p3));
	
	vertices = {
		// Base
		Vertex(p0, vec2(0,0), baseN),
		Vertex(p1, vec2(1,0), baseN),
		Vertex(p3, vec2(0,1), baseN),
		Vertex(p1, vec2(1,0), baseN),
		Vertex(p2, vec2(1,1), baseN),
		Vertex(p3, vec2(0,1), baseN),
		// Side 0
		Vertex(p0, vec2(0,0), side0N),
		Vertex(top,vec2(0.5,1), side0N),
		Vertex(p1, vec2(1,0), side0N),
		// Side 1
		Vertex(p1, vec2(0,0), side1N),
		Vertex(top,vec2(0.5,1), side1N),
		Vertex(p2, vec2(1,0), side1N),
		// Side 2
		Vertex(p2, vec2(0,0), side2N),
		Vertex(top,vec2(0.5,1), side2N),
		Vertex(p3, vec2(1,0), side2N),
		// Side 3
		Vertex(p3, vec2(0,0), side3N),
		Vertex(top,vec2(0.5,1), side3N),
		Vertex(p0, vec2(1,0), side3N)
	};

    srand((uint32_t)time(0));
    for (int i = 0; i < vertices.size(); i++)
    {
		for (int j = 0; j < 3; j++)
            vertices[i].color[j] = glm::linearRand<float>(0.f, 1.f);
    }
}

void Geometry::GenSphere(const glm::vec3& radius, const glm::vec3& offset, int rings, int segments)
{
	draw_mode = GL_TRIANGLE_STRIP; //each new vertex is connected with two privous ones, more optimal in comparison to GL_TRIANGLES when you need 3 vertices for each new polygon, saves memory

	const float deltaRingAngle = glm::pi<float>() / rings;
	const float deltaSegAngle = glm::two_pi<float>() / segments;
	for (int ring = 0; ring < rings; ring++)
	{
		const float r0 = sinf((ring + 0) * deltaRingAngle);
		const float r1 = sinf((ring + 1) * deltaRingAngle);
		const float y0 = cosf((ring + 0) * deltaRingAngle);
		const float y1 = cosf((ring + 1) * deltaRingAngle);
		for (int seg = 0; seg < (segments + 1); seg++)
		{
			const float angle = seg * deltaSegAngle;
			const float x0 = r0 * sinf(angle);
			const float z0 = r0 * cosf(angle);
			const float x1 = r1 * sinf(angle);
			const float z1 = r1 * cosf(angle);
			vertices.push_back(Vertex(vec3(radius.x * x0 + offset.x, radius.y * y0 + offset.y, radius.z * z0 + offset.z), vec2((float)seg / segments, (ring) / (float)rings), vec3(x0, y0, z0)));
			vertices.push_back(Vertex(vec3(radius.x * x1 + offset.x, radius.y * y1 + offset.y, radius.z * z1 + offset.z), vec2((float)seg / segments, (ring + 1) / (float)rings), vec3(x1, y1, z1)));
		}
	}

}

void Geometry::GenIcosahedronSphere(uint32_t tesselation_num, const glm::vec3 &base_color)
{
	//icosahedron base
	vertices = {
	Vertex(vec3(-0.262865f, 0.f,  0.425325f), vec2(1.f, 0.f), vec3(-0.262865f, 0.f,  0.425325f)),
	Vertex(vec3(0.262865f, 0.f,  0.425325f), vec2(1.f, 0.f), vec3(0.262865f, 0.f,  0.425325f)),
	Vertex(vec3(-0.262865f, 0.f, -0.425325f), vec2(1.f, 0.f), vec3(-0.262865f, 0.f, -0.425325f)),
	Vertex(vec3(0.262865f, 0.f, -0.425325f), vec2(1.f, 0.f), vec3(0.262865f, 0.f, -0.425325f)),

	Vertex(vec3(0.f,  0.425325f,  0.262865f), vec2(1.f, 0.f), vec3(0.f,  0.425325f,  0.262865f)),
	Vertex(vec3(0.f,  0.425325f, -0.262865f), vec2(1.f, 0.f), vec3(0.f,  0.425325f, -0.262865f)),
	Vertex(vec3(0.f, -0.425325f,  0.262865f), vec2(1.f, 0.f), vec3(0.f, -0.425325f,  0.262865f)),
	Vertex(vec3(0.f, -0.425325f, -0.262865f), vec2(1.f, 0.f), vec3(0.f, -0.425325f, -0.262865f)),

	Vertex(vec3(0.425325f,  0.262865f, 0.f), vec2(1.f, 0.f), vec3(0.425325f,  0.262865f, 0.f)),
	Vertex(vec3(-0.425325f,  0.262865f, 0.f), vec2(1.f, 0.f), vec3(-0.425325f,  0.262865f, 0.f)),
	Vertex(vec3(0.425325f, -0.262865f, 0.f), vec2(1.f, 0.f), vec3(0.425325f, -0.262865f, 0.f)),
	Vertex(vec3(-0.425325f, -0.262865f, 0.f), vec2(1.f, 0.f), vec3(-0.425325f, -0.262865f, 0.f)),
	};

	std::vector<uint32_t> indices = { 0, 6, 1, 0, 11, 6, 1, 4, 0, 1, 8, 4, 1, 10, 8, 2, 5, 3, 2, 9, 5, 2, 11, 9, 3, 7, 2, 3, 10, 7, 4, 8, 5, 4, 9, 0, 5, 8, 3, 5, 9, 4, 6, 10, 1, 6, 11, 7, 7, 10, 6, 7, 11, 2, 8, 10, 3, 9, 11, 0 };

	for (auto &v : vertices)
	{
		v.pos = normalize(v.pos);
		v.normal = normalize(v.normal);
	}
	//split 
	for (int i = 0; i < tesselation_num; i++)
	{
		int triangles_num = indices.size() / 3;
		for (int t = 0; t < triangles_num ; t++)
		{
			int tpos = 3 * t;
			int v0ind = indices[tpos];
			int v1ind = indices[tpos + 1];
			int v2ind = indices[tpos + 2];
			Vertex v0 = vertices[v0ind];
			Vertex v1 = vertices[v1ind];
			Vertex v2 = vertices[v2ind];

			//New vertices
			Vertex v3 = Vertex(normalize(v0.pos + v1.pos), (v0.uv + v1.uv) * 0.5f, normalize(v0.pos + v1.pos));
			Vertex v4 = Vertex(normalize(v1.pos + v2.pos), 0.5f * (v1.uv + v2.uv), normalize(v1.pos + v2.pos));
			Vertex v5 = Vertex(normalize(v2.pos + v0.pos), 0.5f * (v2.uv + v0.uv), normalize(v2.pos + v0.pos));
			uint32_t vertex_array_size = vertices.size();
			uint32_t v3ind = vertex_array_size;
			uint32_t v4ind = v3ind + 1;
			uint32_t v5ind = v4ind + 1;
			vertices.push_back(v3);
			vertices.push_back(v4);
			vertices.push_back(v5);

			//update old indeces
			indices[tpos + 1] = v3ind;
			indices[tpos + 2] = v5ind;

			// add new faces
			uint32_t new_faces[] = { v3ind, v1ind, v4ind, v3ind, v4ind, v5ind, v5ind, v4ind, v2ind };

			for (int j = 0; j < 9; j++)
				indices.push_back(new_faces[j]);
		}
	}

	//create UVs and set base color 
	for (auto &vert : vertices)
	{
		float u = atan2(vert.normal.x, vert.normal.z) / (two_pi<float>()) + 0.5;
		float v = vert.normal.y * 0.5 + 0.5;
		vert.uv = vec2(u, v);
		vert.color = base_color;
	}

	//fixes broken seam (works with only with GL_REPEAT)
	for (int i = 0; i < indices.size(); i += 3)
	{
		int ids[3] = {
			indices[i],
			indices[i + 1],
			indices[i + 2]
		};

		float u0 = vertices[ids[0]].uv.x;
		float u1 = vertices[ids[1]].uv.x;
		float u2 = vertices[ids[2]].uv.x;

		float minU = std::min({ u0, u1, u2 });
		float maxU = std::max({ u0, u1, u2 });

		// here if triangle crosses seam
		if (maxU - minU > 0.5f)
		{
			for (int j = 0; j < 3; j++)
			{
				int oldIndex = ids[j];
				// duplicate low U vertices
				if (vertices[oldIndex].uv.x < 0.5f)
				{
					Vertex copy = vertices[oldIndex];
					copy.uv.x += 1.0f; // works with GL_REPEAT

					vertices.push_back(copy);
					indices[i + j] = vertices.size() - 1;
				}
			}
		}
	}

	index_size = indices.size();
	glGenBuffers(1, &index_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_array);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size * sizeof(int), indices.data(), GL_STATIC_DRAW);
}


void Geometry::GenCubeExercise()
{
	/*
	Texture coordinates with labeled sides      Vertex names for our cube
	#                                                   v8      v7
	#  3/3+---+---+---+         v                         *------*     y
	#     | 1 | 2 | 3 |        ^                         /|     /|     ^
	#  2/3+---+---+---+        |                     v4/ |  v3/ |     |
	#     | 4 | 5 | 6 |        +--->u                 *--*---*--*v6   +--->x
	#  1/3+---+---+---+                               | /v5  | /     /
	#     |   |   |   |                               |/     |/     z
	#    0+---+---+---+                               *------*
	#     0  1/3 2/3 3/3                             v1      v2
	*/	
	vertices = {
		  // Front
		  Vertex({-1,-1, 1}), //v1
		  Vertex({ 1,-1, 1}), //v2
		  Vertex({ 1, 1, 1}), //v3
		  Vertex({ 1, 1, 1}), //v3
		  Vertex({-1, 1, 1}), //v4
		  Vertex({-1,-1, 1}), //v1
		  // Right
		  Vertex({ 1,-1, 1}),
		  Vertex({ 1,-1,-1}),
		  Vertex({ 1, 1,-1}),
		  Vertex({ 1, 1,-1}),
		  Vertex({ 1, 1, 1}),
		  Vertex({ 1,-1, 1}),
		  // Bottom 
		  Vertex({-1,-1, 1}), // v1
		  Vertex({ 1,-1,-1}), // v6
		  Vertex({ 1,-1, 1}), // v2
		  Vertex({ 1,-1,-1}), // v6
		  Vertex({-1,-1, 1}), // v1
		  Vertex({-1,-1,-1}), // v5
		  // Top
		  Vertex({-1, 1, 1}),
		  Vertex({ 1, 1, 1}),
		  Vertex({ 1, 1,-1}),
		  Vertex({ 1, 1,-1}),
		  Vertex({-1, 1,-1}),
		  Vertex({-1, 1, 1}),
		  // Left
		  Vertex({-1,-1,-1}),
		  Vertex({-1,-1, 1}),
		  Vertex({-1, 1, 1}),
		  Vertex({-1, 1, 1}),
		  Vertex({-1, 1,-1}),
		  Vertex({-1,-1,-1}),
		  // Back
		  Vertex({ 1,-1,-1}),
		  Vertex({-1,-1,-1}),
		  Vertex({-1, 1,-1}),
		  Vertex({-1, 1,-1}),
		  Vertex({ 1, 1,-1}),
		  Vertex({ 1,-1,-1}),
	};

	for (int k = 0; k < 2; k++)
		for (int i = 0, j = k * 18; i < 3; i++, j += 6)
		{
			//for the bottom face with wrong order
			if (k == 0 && i == 2)
			{
				vertices[j + 0].uv = vec2((i % 3) / 3.f, (3.f - k) / 3.f); // v1
				vertices[j + 1].uv = vec2(((i % 3) + 1.f) / 3.f, (2.f - k) / 3.f); // v6
				vertices[j + 2].uv = vec2((i % 3) / 3.f, (2.f - k) / 3.f); // v2
				vertices[j + 3].uv = vec2(((i % 3) + 1.f) / 3.f, (2.f - k) / 3.f); // v6
				vertices[j + 4].uv = vec2((i % 3) / 3.f, (3.f - k) / 3.f); // v1
				vertices[j + 5].uv = vec2(((i % 3) + 1.f) / 3.f, (3.f - k) / 3.f); // v5
			}
			else
			{
				//lower triangles
				vertices[j + 0].uv = vec2((i % 3) / 3.f, (2.f - k) / 3.f);
				vertices[j + 1].uv = vec2(((i % 3) + 1.f) / 3.f, (2.f - k) / 3.f);
				vertices[j + 2].uv = vec2(((i % 3) + 1.f) / 3.f, (3.f - k) / 3.f);
				//upper triangles
				vertices[j + 5].uv = vec2((i % 3) / 3.f, (2.f - k) / 3.f);
				vertices[j + 4].uv = vec2((i % 3) / 3.f, (3.f - k) / 3.f);
				vertices[j + 3].uv = vec2(((i % 3) + 1.f) / 3.f, (3.f - k) / 3.f);
			}
		}
}

void Geometry::GenCube(const glm::vec3& size, const glm::vec3& offset, const glm::vec3& color)
{

	// cube size
	const float fW = size.x * 0.5f;
	const float fH = size.y * 0.5f;
	const float fD = size.z * 0.5f;

	const vec3 nor1(1.0f, 0.0f, 0.0f);
	const vec3 nor2(0.0f, 1.0f, 0.0f);
	const vec3 nor3(0.0f, 0.0f, 1.0f);

	// corners
	vec3 p[8];
	p[0] = vec3(-fW, fH, fD); //top left front
	p[1] = vec3(fW, fH, fD);  //top right front
	p[2] = vec3(fW, -fH, fD);  //bottom right front
	p[3] = vec3(-fW, -fH, fD); //bottom left front
	p[4] = vec3(-fW, -fH, -fD); //bottom left back
	p[5] = vec3(fW, -fH, -fD); //right left back
	p[6] = vec3(fW, fH, -fD);  
	p[7] = vec3(-fW, fH, -fD);

	// offset
	for (int32_t i = 0; i < 8; ++i)
	{
		p[i] += offset;
	}

	// front face
	vertices.emplace_back(Vertex(p[0], vec2(0.0f, 0.0f), nor3));
	vertices.emplace_back(Vertex(p[1], vec2(1.0f, 0.0f), nor3));
	vertices.emplace_back(Vertex(p[2], vec2(1.0f, 1.0f), nor3));
	vertices.emplace_back(Vertex(p[3], vec2(0.0f, 1.0f), nor3));
	// right face
	vertices.emplace_back(Vertex(p[1], vec2(0.0f, 0.0f), nor1));
	vertices.emplace_back(Vertex(p[6], vec2(1.0f, 0.0f), nor1));
	vertices.emplace_back(Vertex(p[5], vec2(1.0f, 1.0f), nor1));
	vertices.emplace_back(Vertex(p[2], vec2(0.0f, 1.0f), nor1));
	// back face
	vertices.emplace_back(Vertex(p[6], vec2(0.0f, 0.0f), -nor3));
	vertices.emplace_back(Vertex(p[7], vec2(1.0f, 0.0f), -nor3));
	vertices.emplace_back(Vertex(p[4], vec2(1.0f, 1.0f), -nor3));
	vertices.emplace_back(Vertex(p[5], vec2(0.0f, 1.0f), -nor3));
	// left face
	vertices.emplace_back(Vertex(p[7], vec2(0.0f, 0.0f), -nor1));
	vertices.emplace_back(Vertex(p[0], vec2(1.0f, 0.0f), -nor1));
	vertices.emplace_back(Vertex(p[3], vec2(1.0f, 1.0f), -nor1));
	vertices.emplace_back(Vertex(p[4], vec2(0.0f, 1.0f), -nor1));
	// top face
	vertices.emplace_back(Vertex(p[7], vec2(0.0f, 0.0f), nor2));
	vertices.emplace_back(Vertex(p[6], vec2(1.0f, 0.0f), nor2));
	vertices.emplace_back(Vertex(p[1], vec2(1.0f, 1.0f), nor2));
	vertices.emplace_back(Vertex(p[0], vec2(0.0f, 1.0f), nor2));
	// bottom face
	vertices.emplace_back(Vertex(p[3], vec2(0.0f, 0.0f), -nor2));
	vertices.emplace_back(Vertex(p[2], vec2(1.0f, 0.0f), -nor2));
	vertices.emplace_back(Vertex(p[5], vec2(1.0f, 1.0f), -nor2));
	vertices.emplace_back(Vertex(p[4], vec2(0.0f, 1.0f), -nor2));


	// drawing order
	uint32_t indices[36];

	for (int32_t i = 0, j = 0; i < 21; i += 4, j += 6)
	{
		indices[j] = (i + 2);
		indices[j + 1] = (i + 1);
		indices[j + 2] = (i + 0);
		indices[j + 3] = (i + 3);
		indices[j + 4] = (i + 2);
		indices[j + 5] = (i + 0);
	}

	for (int32_t i = 0; i < vertices.size(); i++)
		vertices[i].color = color;


	index_size = 36;
	glGenBuffers(1, &index_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_array);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * 4, indices, GL_STATIC_DRAW);
}

Geometry::~Geometry()
{
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array);
	if (index_array)
		glDeleteBuffers(1, &index_array);
}

void Geometry::DisableAttribs(GLuint program)
{
	if (vertex_array)
	{
		glBindVertexArray(vertex_array);
		const GLint position = glGetAttribLocation(program, "vertexPosition");
		const GLint color = glGetAttribLocation(program, "vertexColor");
		const GLint uv = glGetAttribLocation(program, "vertexUV");
		const GLint normal = glGetAttribLocation(program, "vertexNormal");
		glDisableVertexAttribArray(position);
		glDisableVertexAttribArray(color);
		glDisableVertexAttribArray(uv);
		glDisableVertexAttribArray(normal);
	}
}

void Geometry::Set(GLuint program)
{
    const GLuint vpos_location = glGetAttribLocation(program, "vertexPosition");
    const GLuint vcol_location = glGetAttribLocation(program, "vertexColor");
	const GLuint uv = glGetAttribLocation(program, "vertexUV");
	const GLuint normal = glGetAttribLocation(program, "vertexNormal");
	Load();

    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray(vcol_location);
	glEnableVertexAttribArray(uv);
	glEnableVertexAttribArray(normal);

	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * 4));
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * 4 * 2));
	glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * 4 * 3));
}


void Geometry::Draw()
{  
	glBindVertexArray(vertex_array);
	if (index_array && index_size)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_array);
		glDrawElements(draw_mode, index_size, GL_UNSIGNED_INT, 0);
	}
	else
		glDrawArrays(draw_mode, 0, (GLsizei)vertices.size());
}

bool Geometry::ReadObj(const char* filename)
{
	OBJContainer obj;
	if (obj.LoadOBJ(filename))
	{
		vertices = obj.to_vertex_array();
		std::cout << "Object Loaded" << "\n";
		return true;
	}
	return false;
}

void Geometry::Load()
{
    glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}





