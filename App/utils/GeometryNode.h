#include "Geometry.h"

class GeometryNode : public Geometry
{
	GeometryNode* parent;
	std::vector<std::shared_ptr<GeometryNode>> children;
	std::string name;
	glm::mat4 world_matrix; //accumulated shared base transforms
	glm::mat4 local_matrix; //non shared pre transformations
	glm::mat4 post_matrix; //shared secondary matrix

	public:
		GeometryNode(const std::string& name) :
			parent(nullptr),
			name(name),
			world_matrix(glm::mat4(1.f)),
			local_matrix(glm::mat4(1.f)),
			post_matrix(glm::mat4(1.f)) {}
		GeometryNode(const std::string& name, GLuint texture_handle) :
			Geometry(texture_handle),
			parent(nullptr), name(name),
			world_matrix(glm::mat4(1.f)),
			local_matrix(glm::mat4(1.f)),
			post_matrix(glm::mat4(1.f)) {}

		std::shared_ptr<GeometryNode> SeekChild(const std::string& name);
		GeometryNode* GetRoot();

		void AddChild(std::shared_ptr<GeometryNode> child);
		std::vector<std::shared_ptr<GeometryNode>> GetChildren() { return children; }

		void RotateLocal(glm::vec3 axis, float angle, float frametime);
		void SetPost(const glm::mat4& post_transform) { post_matrix = post_transform; }
		void SetLocal(const glm::mat4& local_transform) { local_matrix = local_transform; }
		const std::string& GetName() { return name; }

		void SetToProgram(GLuint program_handle);
		void DrawGraph(GLuint program_handle, const char* model_matrix_uniform, const char* texture_uniform);
		const glm::mat4& GetWorld() { return world_matrix; } //returns current state of world matrix
		const glm::mat4& GetPost() { return post_matrix; }
		const glm::mat4& GetLocal() { return local_matrix; }
};