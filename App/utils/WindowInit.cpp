#include "WindowInit.h"
#include <memory>

Window* Window::WindowSingleton;


Window::Window(): screenWidth(0),
					screenHeight(0),
					m_window(nullptr),
					active(false), renderer(nullptr)
{
		//defining static pointer here
		WindowSingleton = this;

}

GLFWwindow* Window::getNative()
{
    return m_window;
}

bool Window::createWindow(int width, int height, const char* WindowName)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, WindowName, NULL, NULL);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetCursorPosCallback(m_window, cursor_callback);
    if (!m_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(m_window);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    glClearColor(0.f, 0.f, 0.f, 0.f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    return true;
}
void Window::Clear()
{
      glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);
      glViewport(0, 0, screenWidth, screenHeight);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Renderer* Window::getRenderer()
{
    if (!renderer)
        renderer = std::make_unique<Renderer>("vertex_shader.glsl", "fragment_shader.glsl");
    return renderer.get();

}

void Window::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* mywin = (Window*)glfwGetWindowUserPointer(window);
    mywin->OnKeyboardKey(key, scancode, action, mods);
}

void  Window::cursor_callback(GLFWwindow* window, double pos_x, double pos_y)
{
    Window* mywin = (Window*)glfwGetWindowUserPointer(window);
    mywin->OnMouseMove(pos_x, pos_y);
}

void  Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Window* mywin = (Window*)glfwGetWindowUserPointer(window);
    mywin->OnMouseKey(button, action, mods);
}


