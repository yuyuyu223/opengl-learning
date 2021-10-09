#include <menu.h>
#include <iostream>

Menu::Menu(bool showMenu)
{
    this->showMenu = showMenu;
    
}

void Menu::DrawMenu()
{
    if (!showMenu)
    {
       return;
    }
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O"))
            { /* Do stuff */
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            { /* Do stuff */
            }
            if (ImGui::MenuItem("Close", "Ctrl+W"))
            {
                // my_tool_active = false;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    else
    {
        std::cout<<"load menu error!"<<std::endl;
    }
}
