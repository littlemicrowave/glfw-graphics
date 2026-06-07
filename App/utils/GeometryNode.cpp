#include "GeometryNode.h"

std::shared_ptr<GeometryNode> GeometryNode::SeekChild(const std::string &name)
{
    for (auto node : children)
    {
        if (node->name == name)
            return node;

       auto found = node->SeekChild(name);
        if (found != nullptr)
            return found;
    }
    return nullptr;
}

GeometryNode* GeometryNode::GetRoot()
{
	if (parent == nullptr)
		return this;
	else
		return parent->GetRoot();
}

void GeometryNode::AddChild(std::shared_ptr<GeometryNode> child)
{
	child->parent = this;
	children.push_back(child);
}

void GeometryNode::RotateLocal(glm::vec3 axis, float angluar_speed, float frametime)
{
    local_matrix = glm::rotate(glm::mat4(1.f), glm::radians(angluar_speed) * frametime, axis) * local_matrix;
}

void GeometryNode::SetToProgram(GLuint program_handle)
{
    this->Set(program_handle);
    for (auto child : children)
        child->SetToProgram(program_handle);
}


void GeometryNode::DrawGraph(GLuint program_handle, const char* model_matrix_uniform, const char* texture_uniform)
{
    //base transform
    glm::mat4 base = this->GetBase();

    //transformations relative to parent
    if (parent == nullptr)
        world_matrix = base * post_matrix;
    else
        world_matrix = parent->world_matrix * base * post_matrix;

    //texture stuff
    Renderer::SetTexture(program_handle, this->texture_handle, 0, texture_uniform);

    //light vector control
    if(this->name == "sun")
        Renderer::SetUniformFloat(program_handle, "lightBool", -1.f);
    else
        Renderer::SetUniformFloat(program_handle, "lightBool", 1.f);

    Renderer::SetUniformMatrix4(program_handle, model_matrix_uniform, world_matrix * local_matrix);
    this->Draw();
    for (auto child : children)
        child -> DrawGraph(program_handle, model_matrix_uniform, texture_uniform);
}

