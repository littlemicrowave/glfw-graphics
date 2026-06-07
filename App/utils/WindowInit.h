#pragma once
#include "OpenGLRenderer.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>

class Window
{
	bool active;
	static Window* WindowSingleton;
	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double pos_x, double pos_y);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	public:
		Window();
		~Window() { WindowSingleton = nullptr; };
		GLFWwindow* getNative();
		Window* getWindow() { return WindowSingleton; };
		bool createWindow(int width, int height, const char* WindowName);
		virtual void OnCreate() = 0;
		virtual void OnDraw() = 0;
		virtual void Clear();
		virtual void OnUpdate() = 0;
		virtual void OnMouseKey(int button, int action, int mods) = 0;
		virtual void OnMouseMove(double pos_x, double pos_y) = 0;
		virtual void OnKeyboardKey(int key, int scancode, int action, int mods) = 0;
		Renderer* getRenderer();
	protected:
		GLFWwindow* m_window;
		int screenWidth, screenHeight;
		std::unique_ptr<Renderer> renderer;
};

