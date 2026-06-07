#pragma once
#include "utils/WindowInit.h"
#include "utils/Camera.h"
#include "utils/GeometryNode.h"

class App : public Window
{
	glm::mat4 projection_matrix;
	std::vector<std::shared_ptr<GeometryNode>> roots;
	GLuint program;
	Camera* camera;
	float frametime;
	uint32_t scene_n = 1;

	void OnCreate() override;
	void OnDraw() override;
	void OnUpdate() override;
	void PollKeys();
	void OnMouseKey(int button, int action, int mods) override;
	void OnMouseMove(double pos_x, double pos_y) override;
	void OnKeyboardKey(int key, int scancode, int action, int mods) override;

	public:
		App() {
			program = 0;
			camera = nullptr;
			projection_matrix = glm::mat4(1.f);
			frametime = (float)glfwGetTime();
		};
		~App();
		void Run();
		Camera* getCamera() { return camera;  }
};