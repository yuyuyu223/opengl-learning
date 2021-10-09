#include "ImGUI/imgui.h"
#include "ImGUI/backends/imgui_impl_glfw.h"
#include "ImGUI/backends/imgui_impl_opengl3.h"

class Menu
{
private:
    bool showMenu;
public:
    Menu(bool showMenu);
    void DrawMenu();
};