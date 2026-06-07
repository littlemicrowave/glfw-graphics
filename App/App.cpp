#include "App.h"

App::~App()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void App::OnCreate()
{
    glfwSetWindowUserPointer(m_window, this);
    program = getRenderer()->getProgram();
    camera = new Camera();

    GLuint sun_texture = Renderer::CreateTexture("resources/2k_sun.jpg");
    GLuint earth_texture = Renderer::CreateTexture("resources/2k_earth_daymap.jpg");
    GLuint moon_texture = Renderer::CreateTexture("resources/2k_moon.jpg");
    GLuint box_texture = Renderer::CreateTexture("resources/box.png");
    GLuint box_texture2 = Renderer::CreateTexture("resources/object/uvmap.png");
    //scene 1
    auto sun = std::make_shared<GeometryNode>("sun", sun_texture);
    auto earth = std::make_shared<GeometryNode>("earth", earth_texture);
    auto moon = std::make_shared<GeometryNode>("moon", moon_texture);

    auto earth_model = earth->GetBase();
    auto moon_model = moon->GetBase();

    earth_model = glm::scale(earth_model, glm::vec3(0.1f));
    earth_model = glm::translate(earth_model, glm::vec3(-20.f, 0.f, 3.f));
    earth->SetBase(earth_model);

    moon_model = glm::scale(moon_model, glm::vec3(0.1f));           // relative moon size
    moon_model = glm::translate(moon_model, glm::vec3(-20.f, 0.f, 3.f)); // translation relative to earth (also affected by scaling of both)
    moon->SetBase(moon_model);

    sun->GenIcosahedronSphere(4);
    earth->GenIcosahedronSphere(4);
    moon->GenIcosahedronSphere(4);

    sun->AddChild(earth);
    earth->AddChild(moon);
    roots.push_back(sun);

    //scene 2 
    auto dummy_root_node = std::make_shared<GeometryNode>("dummy_root");
    auto earth2 = std::make_shared<GeometryNode>("earth", earth_texture);
    earth2->GenIcosahedronSphere(4);

    auto pyr0 = std::make_shared<GeometryNode>("pyr0");
    pyr0->GenPyramid();;
    auto pyr1 = std::make_shared<GeometryNode>("pyr1");
    pyr1->GenPyramid();
    auto pyr2 = std::make_shared<GeometryNode>("pyr2");
    pyr2->GenPyramid();

    auto box0 = std::make_shared<GeometryNode>("box", box_texture);
    box0->GenCube(glm::vec3(1.f), glm::vec3(0.f));
    auto box1 = std::make_shared<GeometryNode>("box1", box_texture2);
    box1->ReadObj("resources/object/cube.obj");
    auto box2 = std::make_shared<GeometryNode>("box2", box_texture);
    box2-> GenCube(glm::vec3(1.f), glm::vec3(0.f));

    dummy_root_node->AddChild(pyr0);
    dummy_root_node->AddChild(box0);
    box0->AddChild(box1);            // box1 orbits box0
    box0->AddChild(box2);            // box2 orbits box0

    pyr0->AddChild(pyr1);
    pyr1->AddChild(pyr2);
    pyr2->AddChild(earth2);

    //non-relative scales
    earth2->SetLocal(glm::scale(glm::mat4(1.f), glm::vec3(0.5f)));
    pyr2->SetLocal(glm::scale(glm::mat4(1.f), glm::vec3(1/3.f)));
    pyr1->SetLocal(glm::scale(glm::mat4(1.f), glm::vec3(0.5f)));

    //relative translations
    pyr1->SetBase(glm::translate(glm::mat4(1.f), glm::vec3(0, 0.9, 0)));
    pyr2->SetBase(glm::translate(glm::mat4(1.f), glm::vec3(0, 0.5, 0)));
    earth2->SetBase(glm::translate(glm::mat4(1.f), glm::vec3(0, 0.5 + 1/3.f, 0)));

    box0->SetBase(glm::translate(glm::mat4(1.f), glm::vec3(4, 0, 0)));

    // diagonal around box0
    box1->SetBase(glm::translate(glm::mat4(1.f), glm::vec3(2, 2, 0)));
    box2->SetBase(glm::translate(glm::mat4(1.f), glm::vec3(-2, 2, 0)));

    box1->SetLocal(glm::scale(glm::mat4(1.f), glm::vec3(0.5f)));

    roots.push_back(dummy_root_node);
    for (auto& root : roots)
        root->SetToProgram(program);
}

void App::OnDraw()
{
    Clear();
    glUseProgram(program);
    const float ratio = screenWidth / (float)screenHeight;
    projection_matrix = glm::perspective<float>(glm::radians<float>(90.0f), ratio, 0.1f, 1000);
    Renderer::SetUniformMatrix4(program, "projection", projection_matrix);
    Renderer::SetUniformMatrix4(program, "view", camera->GetViewMatrix());

    if (scene_n == 0)
    {
        glm::mat4 sun_transform = roots[0].get()->GetBase();
        Renderer::SetUniformVec4(program, "lightPosition", sun_transform * glm::vec4(0.f, 0.f, 0.f, 1));
    }
    else
        Renderer::SetUniformVec4(program, "lightPosition", glm::vec4(5.f, 5.f, 5.f, 1));

    roots[scene_n]->DrawGraph(program, "model", "texture_sampler");
}

void App::OnUpdate()
{
    //scene 0
    const static auto sun = roots[0].get();
    const static auto identity = glm::mat4(1.f);

    auto earth = sun->SeekChild("earth");
    auto moon = earth->SeekChild("moon");
    const static glm::vec3 y_axis = glm::vec3(0, 1, 0);
    const static glm::vec3 x_axis = glm::vec3(1, 0, 0);
    const static glm::vec3 z_axis = glm::vec3(0, 0, 1);

    sun->RotateLocal(y_axis, 5, frametime);  //sun 5 degrees per second (ccw)
    earth->RotateLocal(y_axis, -10, frametime);  //sun -10 degrees per second (cw)
    moon->RotateLocal(y_axis, 8, frametime);  //sun 15 degrees per second (ccw)

    auto moon_orbit = moon->GetBase();
    auto earth_orbit = earth->GetBase();

    auto rotate = glm::rotate(identity, glm::radians(5 * frametime), y_axis);

    earth->SetBase(rotate * earth_orbit);
    moon->SetBase(rotate * moon_orbit);

    //scene 1
    double current_time = glfwGetTime();
    const static auto root = roots[1].get();
    const static auto pyramid0 = root->GetChildren().at(0);
    auto pyr_children = pyramid0->GetChildren(); //get children of first pyramid

    while (pyr_children.size() != 0)
    {
        for (auto &child : pyr_children)
        {
            //post sway wrt to parent
            glm::mat4 sway = glm::rotate(identity, glm::radians((float)(sin(current_time) * 20)), x_axis);
            sway = glm::rotate(sway, glm::radians((float)(cos(current_time) * 20)), z_axis);
            child->SetPost(sway);

            //just local rotaion around y
            if (child->GetName() != "earth")
                child->RotateLocal(y_axis, 30 * sin(current_time / 5), frametime);
            else
                child->RotateLocal(y_axis, -180, frametime);

            for (auto &next: child->GetChildren())
                pyr_children.push_back(next);
            pyr_children.erase(pyr_children.cbegin());
        }
    }

    pyramid0->RotateLocal(y_axis, 30 * sin(current_time / 5), frametime);

    auto box0 = root->SeekChild("box");
    auto box1 = root->SeekChild("box1");
    auto box2 = root->SeekChild("box2");

    glm::mat4 orbitRoot = box0->GetBase();
    glm::mat4 orbitChild1 = box1->GetBase();
    glm::mat4 orbitChild2 = box2->GetBase();

    glm::mat4 rootOrbit = glm::rotate(identity, glm::radians(20.0f * frametime), y_axis);
    glm::mat4 diagOrbit1 = glm::rotate(identity, glm::radians(60.0f * frametime), glm::normalize(glm::vec3(1, 1, 0)));
    glm::mat4 diagOrbit2 = glm::rotate(identity, glm::radians(-45.0f * frametime), glm::normalize(glm::vec3(1, 0, 1)));

    box0->SetBase(rootOrbit * orbitRoot);
    box1->SetBase(diagOrbit1 * orbitChild1);
    box2->SetBase(diagOrbit2 * orbitChild2);

    //keeps track of rotations
    static glm::mat4 rot0(1.0f);
    static glm::mat4 rot1(1.0f);
    static glm::mat4 rot2(1.0f);

    static glm::vec3 axis0 = glm::sphericalRand(1.0f);
    static glm::vec3 axis1 = glm::sphericalRand(1.0f);
    static glm::vec3 axis2 = glm::sphericalRand(1.0f);

    rot0 = glm::rotate(rot0, glm::radians(-60.0f * frametime), axis0);
    rot1 = glm::rotate(rot1, glm::radians(45.0f * frametime), axis1);
    rot2 = glm::rotate(rot2, glm::radians(-35.0f * frametime), axis2);

    //combines with current scale
    box0->SetLocal(rot0 * glm::scale(identity, glm::vec3(sin(current_time) * 0.5f + 0.5f)));
    box1->SetLocal(rot1 * glm::scale(identity, glm::vec3(cos(current_time + glm::pi<float>() * 2.5f) * 0.5f + 0.5f)));
    box2->SetLocal(rot2 * glm::scale(identity, glm::vec3(cos(current_time) * 0.5f + 0.5f)));

    PollKeys();

    camera->Move(frametime);
}


void App::Run()
{
    OnCreate();
    const static double two_pi = glm::two_pi<double>();

    while (!glfwWindowShouldClose(m_window))
    {
        double time_in = glfwGetTime();
        OnUpdate();
        OnDraw();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
        frametime = (float)(glfwGetTime() - time_in);
    }
}


void App::OnMouseKey(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1)
        if (action == GLFW_PRESS)
        {
            glfwGetCursorPos(m_window, &camera->cursor_x, &camera->cursor_y);
            camera->active = true;
        }
        else
            camera->active = false;
}

void App::OnKeyboardKey(int key, int scancode, int action, int mods)
{
    
    switch (action)
    {
        case GLFW_PRESS:
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
            if (key == GLFW_KEY_TAB)
            {
                scene_n += 1;
                if (scene_n > roots.size() - 1)
                    scene_n = 0;
            }
    }

}

void App::OnMouseMove(double xpos, double ypos)
{
    if (camera->active)
        camera->UpdateDirection(xpos, ypos, frametime);
}

void App::PollKeys()
{
    camera->keys.A = glfwGetKey(m_window, GLFW_KEY_A);
    camera->keys.D = glfwGetKey(m_window, GLFW_KEY_D);
    camera->keys.S = glfwGetKey(m_window, GLFW_KEY_S);
    camera->keys.W = glfwGetKey(m_window, GLFW_KEY_W);
    camera->keys.E = glfwGetKey(m_window, GLFW_KEY_E);
    camera->keys.R = glfwGetKey(m_window, GLFW_KEY_R);
}
