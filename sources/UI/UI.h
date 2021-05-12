#ifndef UI_H
#define UI_H


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class UI {
public:
    UI(GLFWwindow *window_in);
    void render();

    void addCheckButton();

    private:
    GLFWwindow *window;
};

#endif